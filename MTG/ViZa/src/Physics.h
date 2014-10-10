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
#include "Container.h"


#include <iostream>



class Physics{
    
public:
    static Physics* instance;
    
    vector<float> ks;
    
    
    
    
    ofFbo pos;
    ofFbo vel;
    
    
    Physics(){if(instance == NULL){instance = this;}    }
    
    void addElement(ofVec3f c,int level);
    void updatePhy(float time);
    
    
    
    
    
};
#endif /* defined(__ViZa__Physics__) */
