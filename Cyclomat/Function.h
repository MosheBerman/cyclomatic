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
    int numberOfDecisions;
    std::string name;
    std::vector<Argument> arguments;
public:
    
    /* Constructors */
    
    Function();                                         //  Default Constructor
    Function(std::string);                              //  Takes a name for the function
    Function(std::string, std::vector<Argument>);       //  Takes a name for the function, and its arguments
    
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

