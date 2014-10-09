//
//  Container.h
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#ifndef __ViZa__Container__
#define __ViZa__Container__

#include <iostream>
#include "ofMain.h"


class Container{
public:
    
    static vector<Container> containers;
    static ofVbo vbo;
    
    static ofVec3f* vs;
    static ofFloatColor *cols;
    static unsigned int* idxs;
    
    static float radius;
    
    static void updateOneColor(int idx,ofColor col);
    static void updateVBO();
    static void Cast(ofCamera cam,ofVec2f mouse);
    static void freeVbo();
    static float distanceVanish(ofCamera c);
    
    static ofFloatColor stateColor [];
    
    
    static void registerListener();
    
    
    Container(string path,float begin,float end,int idx,int level=0): path(path),begin(begin),end(end),level(level),index(idx){
        pos=ofVec3f(ofRandom(0, 1),ofRandom(0,1),ofRandom(0,1));
        state = 0;
        
    };
    
    
    ofVec3f pos;
    string path;
    float begin;
    float end;
    int level;
    int index;
    ofParameter<float> state;
    void setState(float & a);
    
    
};



#endif /* defined(__ViZa__Container__) */
