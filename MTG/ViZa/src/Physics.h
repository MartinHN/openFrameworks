//
//  Physics.h
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#ifndef __ViZa__Physics__
#define __ViZa__Physics__


#include "ofMain.h"

#include <iostream>

class phyElement{
public:
    Container * c;
    int level;
    
    
    
};

class Physics{
    
public:
    Physics instance;
    
    Vector<float> ks;
    
    Vector<phyElement> elements;
    
    
    ofFbo pos;
    ofFbo vel;
    
    
    Physics(){if(instance == NULL){instance = this;}    }
    
    void addElement(Container * c,int level);
    void updatePhy(float time);
    
    
    
    
    
};
#endif /* defined(__ViZa__Physics__) */
