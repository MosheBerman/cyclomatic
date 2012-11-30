/* get_src.c */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define GET_SOURCE
#include "get_src.h"

#if 1
/* These are the keywords we recognize - those that involve flow control.
 * recognition of all keywords simply involves adding them to this list,
 * and adding matching identifiers to the enumeration in get_src.h.  The
 * matching enumerators for these keywords start after IDENT in the
 * enumeration, but MUST be maintained in the same order as the keywords
 * appear here.  E.g. if "case" remains the first keyword here, `CASE'
 * must follow immediately after IDENT in the enumeration.  Any types
 * added to the enumeration that do not have matching keywords should
 * precede `IDENT'.
 */
static char *keys[] = {
    "case",
    "default",
    "do",
    "else",
    "if",
    "switch",
    "while",
    "#include",
    "#define"
};

#define elems(x) (sizeof(x) / sizeof(x[0]))
#endif

static size_t current = 0;

char last_token[MAX_TOKEN_SIZE];

PFILE *parse_fopen(char const *name) {
    
    PFILE *temp = malloc(sizeof(PFILE));
    
    if ( NULL != temp ) {
        temp->file = fopen(name, "r");
        memset(temp->peeks, 0, sizeof(temp->peeks));
        temp->last_peek = 0;
    }
    return temp;
}

PFILE *parse_fdopen(FILE *file) {
    
    PFILE *temp = malloc(sizeof(PFILE));
    
    if ( NULL != temp) {
        temp->file = file;
        memset(temp->peeks, 0, sizeof(temp->peeks));
        temp->last_peek = 0;
    }
    return temp;
}

int parse_fclose(PFILE *stream) {
    
    int retval = fclose(stream->file);
    
    free(stream);
    return retval;
}

static void addchar(int ch) {
    /* adds the passed character to the end of `last_token' */
    
    if ( current < sizeof(last_token) -1 )
        last_token[current++] = (char)ch;
    
    if ( current == sizeof(last_token)-1 )
        last_token[current] = '\0';
}

static void clear(void) {
    /* clears the previous token and starts building a new one. */
    current = 0;
}

static int read_char(PFILE *stream) {
    if ( stream->last_peek > 0 )
        return stream->peeks[--stream->last_peek];
    return fgetc(stream->file);
}

void unget_character(int ch, PFILE * stream) {
    if ( stream->last_peek < sizeof(stream->peeks) )
        stream->peeks[stream->last_peek++] = ch;
}

/* Here's where we start getting into sort of sophisticated stuff.
 */

static int check_trigraph(PFILE *stream) {
    /* Checks for trigraphs and returns the equivalant character if there
     * is one.  Expects that the leading '?' of the trigraph has already
     * been read before this is called.
     */
    
    int ch;
    
    if ( '?' != (ch=read_char(stream))) {
        unget_character(ch, stream);
        return '?';
    }
    
    ch = read_char(stream);
    
    switch( ch ) {
        case '(':       return '[';
        case ')':   return ']';
        case '/':   return '\\';
        case '\'':  return '^';
        case '<':   return '{';
        case '>':   return '}';
        case '!':   return '|';
        case '-':   return '~';
        case '=':   return '#';
        default:
            unget_character('?', stream);
            unget_character(ch, stream);
            return '?';
    }
}

static int check_digraph(PFILE *stream, int first) {
    /* Checks for a digraph.  The first character of the digraph is
     * transmitted as the second parameter, as there are several possible
     * first characters of a digraph.
     */
    
    int ch = read_char(stream);
    
    switch(first) {
        case '<':
            if ( '%' == ch )
                return '{';
            if ( ':' == ch )
                return '[';
            break;
        case ':':
            if ( '>' == ch )
                return ']';
            break;
        case '%':
            if ( '>' == ch )
                return '}';
            if ( ':' == ch )
                return '#';
            break;
    }
    
    /* If it's not one of the specific combos above, return the characters
     * separately and unchanged by putting the second one back into the
     * stream, and returning the first one as-is.
     */
    unget_character(ch, stream);
    return first;
}

static int get_char(PFILE *stream) {
    /* Gets a single character from the stream with any trigraphs ( and if
     * C++ support is turned on, digraphs ) converted to the single character
     * represented.
     */
    int ch = read_char(stream);
    
    if ( ch == '?' )
        return check_trigraph(stream);
    
    if (( ch == '<' || ch == ':' || ch == '%' ))
        return check_digraph(stream, ch);
    
    return ch;
}

int get_character(PFILE *stream) {
    /* gets a character from `stream'.  Any amount of any kind of whitespace
     * is returned as a single space. Escaped new-lines are "eaten" here as well.
     */
    int ch;
    
    if ( !isspace(ch=get_char(stream)) && ch != '\\')
        return ch;
    
    // handle line-slicing
    if (ch == '\\') {
        ch = get_char(stream);
        if (ch == '\n')
            ch = get_char(stream);
        else {
            unget_character(ch, stream);
            return ch;
        }
    }
    
    /* If it's a space, skip over consecutive white-space */
    while (isspace(ch) && ('\n' != ch))
        ch = get_char(stream);
    
    if ('\n' == ch)
        return ch;
    
    /* Then put the non-ws character back */
    unget_character(ch, stream);
    
    /* and return a single space character... */
    return ' ';
}

static int read_char_lit(PFILE *stream) {
    /* This is used internally by `get_source' (below) - it expects the
     * opening quote of a character literal to have already been read and
     * returns CHAR_LIT or ERROR if there's a newline before a close
     * quote is found, or if the character literal contains more than two
     * characters after escapes are taken into account.
     */
    
    int ch;
    int i;
    
    
    clear();
    addchar('\'');
    
    for (i=0; i<2 && ('\'' != ( ch = read_char(stream))); i++) {
        
        addchar(ch);
        
        if ( ch == '\n' )
            return ERROR;
        
        if (ch == '\\' ) {
            ch = get_char(stream);
            addchar(ch);
        }
    }
    addchar('\'');
    addchar('\0');
    
    if ( i > 2 )
        return ERROR;
    
    return CHAR_LIT;
}

static int read_str_lit(PFILE *stream) {
    /* Used internally by get_source.  Expects the opening quote of a string
     * literal to have already been read.  Returns STR_LIT, or ERROR if a
     * un-escaped newline is found before the close quote.
     */
    
    int ch;
    
    clear();
    addchar('"');
    
    while ( '"' != ( ch = get_char(stream))) {
        
        if ( '\n' == ch || EOF == ch )
            return ERROR;
        
        addchar(ch);
        
        if( ch == '\\' ) {
            ch = read_char(stream);
            addchar(ch);
        }
        
    }
    
    addchar('"');
    addchar('\0');
    
    return STR_LIT;
}

static int read_comment(PFILE *stream) {
    /* Skips over a comment in stream.  Assumes the leading '/' has already
     * been read and skips over the body.  If we're reading C++ source, skips
     * C++ single line comments as well as normal C comments.
     */
    int ch;
    
    clear();
    
    ch = get_char(stream);
    
    /* Handle a single line comment.
     */
    if ('/' == ch) {
        addchar('/');
        addchar('/');
        
        while ( '\n' != ( ch = get_char(stream)))
            addchar(ch);
        
        addchar('\0');
        return COMMENT;
    }
    
    if ('*' != ch ) {
        unget_character(ch, stream);
        return '/';
    }
    
    addchar('/');
    
    do {
        addchar(ch);
        while ('*' !=(ch = get_char(stream)))
            if (EOF == ch)
                return ERROR;
            else
                addchar(ch);
        addchar(ch);
    } while ( '/' != (ch=get_char(stream)));
    
    addchar('/');
    addchar('\0');
    
    return COMMENT;
}

int get_source(PFILE *stream) {
    /* reads and returns a single "item" from the stream.  An "item" is a
     * comment, a literal or a single character after trigraph and possible
     * digraph substitution has taken place.
     */
    
    int ch = get_character(stream);
    
    switch(ch) {
        case '\'':
            return read_char_lit(stream);
        case '"':
            return read_str_lit(stream);
        case '/':
            return read_comment(stream);
        default:
            return ch;
    }
}
#if TOKEN
int get_token(PFILE *stream) {
    /* This gets a single token from the input stream and places the text
     * of the token in last_token, and returns an identifier of the type of
     * the token.  Only flow control keywords are recognized individually.
     * All other keywords are simply returned as IDENT's, just like other
     * identifiers.
     */
    
    int ch;
    int i;
    
    ch = get_source(stream);
    
    /* If we've got an identifier, read as many characters as can
     * possibly constitute the identifier ( maximal munch ) and build
     * up the complete identifier in `last_token'
     */
    if ( ch > 0 && ('_' == ch || isalpha(ch))) {
        clear();
        while(ch > 0 && (isalpha(ch) || isdigit(ch) || '_' == ch )) {
            addchar(ch);
            ch = get_source(stream);
        }
        unget_character(ch,stream);
        
        addchar('\0');
        
        /* Now we look in our table to see if we've got a keyword
         * we recognize, or some random identifier.
         */
        for (i=0;i<elems(keys);i++) {
            if ( 0 == strcmp(last_token, keys[i]))
                return IDENT+i+1;
        }
        
        /* we didn't recognize it - it must be a normal identifier. */
        return IDENT;
    }
    
    /* it's not an identifier - just return it as a character. */
    return ch;
}
#endif
#ifdef TEST

int main(int argc, char **argv)  {
    PFILE *f;
    int ch;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: get_src <filename>\n");
        return EXIT_FAILURE;
    }
    
    if (NULL==(f= parse_fopen(argv[1]))) {
        fprintf(stderr, "Unable to open: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    
    while (EOF!=(ch=get_source(f)))
        if (ch < 0)
            printf("\n%s\n", last_token);
        else
            printf("%c", ch);
    parse_fclose(f);
    return 0;               
}

#endif
