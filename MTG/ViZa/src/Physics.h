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

class Container;

class Physics{
    
public:
   
    
    static ofVec3f* vs;
    static ofFloatColor *cols;
    static unsigned int* idxs;
    static ofVbo vbo;
    
    static void updateVBO();
    static void freeVbo();
    static void updateOneColor(int idx,ofColor col);
    static void orderBy(string attr,int axe,bool norm,bool stdnorm);
    static Container * Cast(ofCamera cam, ofVec2f mouse);
    static float distanceVanish(ofCamera cam);
    vector<float> ks;
    
    
    
    
    ofFbo pos;
    ofFbo vel;
    
    
    string curAttribute;
    
    Physics * instance(){if(inst == NULL){inst = this;}    }
    
    void buildNetwork();
    void updatePhy(float time);

    
private:
     static Physics* inst;
    
    
};
#endif /* defined(__ViZa__Physics__) */
