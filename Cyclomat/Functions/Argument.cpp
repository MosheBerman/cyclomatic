//
//  Argument.cpp
//  Cyclomat
//
//  Created by Moshe Berman on 11/30/12.
//  Copyright (c) 2012 Moshe Berman. All rights reserved.
//

#include "Argument.h"

/* Constructors */

Argument::Argument():datatype("void"), name("new argument"){};
Argument::Argument(std::string _datatype, std::string _name):datatype(_datatype), name(_name){};

/* Getters and Setters */

std::string Argument::getDatatype(){
    return datatype;
};

std::string Argument::getName(){
    return name;
};

void Argument::setDataType(std::string _datatype){
    datatype = _datatype;
}

void Argument::setName(std::string _name){
    name = _name;
};