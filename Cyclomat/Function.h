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

/*
 
 This class allows for the tracking of 
 functions and their complexity count.
 
 */

class Function {
private:
    int numberOfDecisions;
    std::string name;

public:
    
    /* Constructors */
    
    Function();                 //  Default Constructor
    Function(std::string);      //  Takes a name for the function
    
    /* Complexity Count */
    
    void increaseComplexity();
    void decreaseComplexity();
    int complexity();
};

#endif /* defined(__Cyclomat__Function__) */

