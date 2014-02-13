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
    MYPARAM(screenN,0,-1,199);
    MYPARAM(recopy,true,false,true);
    isHighFPS = false;
    MYPARAM(isMapping,true,false,true);
    
    
}



