/* get_src.h */

#ifndef GET_SRC_INCLUDED
#define GET_SRC_INCLUDED

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
    
    /* This is the size of the largest token we'll attempt to deal with.  If
     * you want to deal with bigger tokens, change this, and recompile
     * get_src.c.  Note that an entire comment is treated as a single token,
     * so long comments could overflow this.  In case of an overflow, the
     * entire comment will be read as a single token, but the part larger
     * than this will not be stored.
     */
#define MAX_TOKEN_SIZE 8192
    
    /* `last_token' will contain the text of the most recently read token.
     */
    extern char last_token[];
    
    /* This is the maximum number of characters that can be put back into a
     * file opened with parse_fopen or parse_fdopen.
     */
#define MAX_UNGETS 5
    
#include <limits.h>
#include <stdio.h>
    
    typedef struct {
        FILE *file;
        char peeks[MAX_UNGETS];
        int last_peek;
    } PFILE;
    
    /* Some codes we return to indicate having found various items in the
     * source code.  ERROR is returned to indicate a newline found in the
     * middle of a character or string literal or if a file ends inside a
     * comment, or if a character literal contains more than two characters.
     *
     * Note that this starts at INT_MIN, the most negative number available
     * in an int.  This keeps these symbols from conflicting with any
     * characters read from the file.  However, one of these could
     * theoretically conflict with EOF.  EOF usually -1, and these are far
     * more negative than that.  However, officially EOF can be any value
     * less than 0...
     */
    enum {
        ERROR = INT_MIN,
        COMMENT,
        CHAR_LIT,
        STR_LIT,
        IDENT,  /* This is for any word except those below...       */
        CASE,
        DEFAULT,
        DO,
        ELSE,
        IF,
        SWITCH,
        WHILE,
        INCLUDE,
        DEFINE
    };
    
    /* Opens a file for parsing and returns a pointer to a structure which
     * can be passed to the other functions in the parser/lexer to identify
     * the file being worked with.
     */
    PFILE *parse_fopen(char const *name);
    
    /* This corresponds closely to fdopen - it takes a FILE * as its
     * only parameter, creates a PFILE structure identifying that file, and
     * returns a pointer to that structure.
     */
    PFILE *parse_fdopen(FILE *stream);
    
    /* Corresponds to fclose.
     */
    int parse_fclose(PFILE *stream);
    
    /* returns characters from `stream' read as C source code.  String
     * literals, characters literals and comments are each returned as a
     * single code from those above.  All strings of any kind of whitespace
     * are returned as a single space character.
     */
    int get_source(PFILE *stream);
    
    /* As above, but adds classification of some C keywords as well.  The
     * keywords recognized are mostly for flow control and are those listed
     * in the enumeration above, following IDENT.  All identifiers and
     * unrecognized keywords are returned as IDENT.
     */
    int get_token(PFILE *stream);
    
    /* If called with a value of 0, turns off recognition of C++ digraphs
     * and single line comments.  If called with a non-zero value, turns
     * on recognition of same.  Default is 1.
     */
    //void read_CPP(int status);
    
    /* Basically, these two work just like the normal versions of the same,
     * with the minor exception that unget_character can unget more than one
     * character.
     */
    int get_character(PFILE *stream);
    void unget_character(int ch, PFILE *stream);
    
#ifdef __cplusplus
}
#endif

#endif
