//
//  Function.cpp
//  Cyclomat
//
//  Created by Moshe Berman on 11/30/12.
//  Copyright (c) 2012 Moshe Berman. All rights reserved.
//

#include "Function.h"

//
//  Constructors
//

Function::Function():name("Unnamed Function"), numberOfDecisions(0){ };
Function::Function(std::string _name):name(_name), numberOfDecisions(0){ };

//
//  Methods related to complexity
//

void Function::increaseComplexity(){
    numberOfDecisions++;
};

void Function::decreaseComplexity(){
    numberOfDecisions--;
};

int Function::complexity(){
    return numberOfDecisions+1;
};