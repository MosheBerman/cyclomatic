//
//  Function.cpp
//  Cyclomat
//
//  Created by Moshe Berman on 11/30/12.
//  Copyright (c) 2012 Moshe Berman. All rights reserved.
//

#include "Function.h"

/*  Constructors    */

Function::Function():name("Unnamed Function"), numberOfDecisions(0){

}

Function::Function(std::string _name){
    name = _name;
    numberOfDecisions = 0;
    returnType = "void";
}

Function::Function(std::string _name, std::vector<Argument> _arguments){
    name = _name;
    numberOfDecisions = 0;
    arguments = _arguments;
    returnType = "void";
}

Function::Function(std::string _name, std::vector<Argument> _arguments, std::string _returnType){
    name = _name;
    numberOfDecisions = 0;
    arguments = _arguments;
    returnType = _returnType;
}


/*  Complexity Count */

void Function::increaseComplexity(){
    numberOfDecisions++;
}

void Function::decreaseComplexity(){
    numberOfDecisions--;
}

int Function::complexity(){
    return numberOfDecisions+1;
};

/* Arguments */

void Function::addArgument(Argument argument){
    
}

void Function::removeArgumentNamed(std::string _name){
    
}

Argument Function::argumentNamed(std::string _name){
    
}

std::vector<Argument> Function::argumentsOfType(std::string _type){
    
}