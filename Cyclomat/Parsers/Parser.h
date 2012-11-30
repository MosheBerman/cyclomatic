//
//  Parser.h
//  Cyclomat
//
//  Created by Moshe Berman on 11/30/12.
//  Copyright (c) 2012 Moshe Berman. All rights reserved.
//

#ifndef __Cyclomat__Parser__
#define __Cyclomat__Parser__

#include <iostream>
#include "Function.h"
#include <vector>
#include <fstream>

class Parser {
    std::string fileName;
    std::vector<Function> functions;
    std::vector<std::fstream> files;
    
    //
    //  Parsing state
    //
    
    bool isInsideOfAFunction;
    bool isInsideOfAComment;
    bool isInsideOfAString;
    
    int scopeDepthLevel;
    
    bool didfinishParsing;
    bool errors;
    
    std::vector<std::string> errorMessages;
public:
    
    /* Constructor */
    
    Parser();
    Parser(const std::string &, bool);  //  Takes a fileName, and a flag indicating if parsing should start automatically
    
    /* Triggers the parsing action. */
    
    bool parse();
    
    /* Function accessors */
    
    int functionCount();
    Function functionAtIndex(int);
    
private:
    
    /* User interface methods */
    void promptForFileName();
    
    /*
     Parser helper methods - override
     these in a subclass to support
     different programming languages.
     */
    
    std::vector<std::string> argumentsFromLine(std::string);
    int numberOfDecisionsPerLine(std::string);
    std::string functionNameFromLine(std::string);
    std::string returnTypeFromLine(std::string);
    
    
};
#endif /* defined(__Cyclomat__Parser__) */
