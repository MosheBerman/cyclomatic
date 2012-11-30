//
//  Argument.h
//  Cyclomat
//
//  Created by Moshe Berman on 11/30/12.
//  Copyright (c) 2012 Moshe Berman. All rights reserved.
//

#ifndef __Cyclomat__Argument__
#define __Cyclomat__Argument__

#include <iostream>

class Argument {
    std::string datatype;
    std::string name;
    
public:

    /* Constructors */
    
    Argument();
    Argument(std::string, std::string);

    /* Getters and Setters */
    
    std::string getDatatype();
    std::string getName();
    
    void setDataType(std::string);
    void setName(std::string);
};

#endif /* defined(__Cyclomat__Argument__) */
