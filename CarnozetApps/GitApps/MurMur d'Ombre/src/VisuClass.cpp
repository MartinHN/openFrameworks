//
//  VisuClass.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 14/01/14.
//  Copyright 2014 __MyCompanyName__. All rights reserved.
//

#include "VisuClass.h"




VisuClass::VisuClass(VisuHandler * vh){

    dad=vh;
    MYPARAM(screenN,0,0,199);
    isHighFPS = false;
    MYPARAM(isMapping,true,false,true);
}



