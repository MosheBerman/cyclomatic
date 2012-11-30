//
//  Function.h
//  Cyclomat
//
//  Created by Moshe Berman on 11/30/12.
//  Copyright (c) 2012 Moshe Berman. All rights reserved.
//

#ifndef __Cyclomat__Function__
#define __Cyclomat__Function__

#include <iostream>
#include <vector>

#include "Argument.h"

/*
 
 This class allows for the tracking of 
 functions and their complexity count.
 
 */

class Function {
    std::vector<Argument> arguments;
    std::string name;
    int numberOfDecisions;
    std::string returnType;
public:
    
    /* Constructors */
    
    Function();
    Function(std::string);                              //  Takes a name for the function
    Function(std::string, std::vector<Argument>);                   //  Takes a name for the function, and its arguments
    Function(std::string, std::vector<Argument>, std::string);       //  Takes a name for the function, arguments, and a return type
    
    /* Complexity Count */
    
    void increaseComplexity();
    void decreaseComplexity();
    int complexity();
    
    /* Arguments */
    
    void addArgument(Argument);
    void removeArgumentNamed(std::string);
    
    Argument argumentNamed(std::string);
    std::vector<Argument> argumentsOfType(std::string);
};

#endif /* defined(__Cyclomat__Function__) */

