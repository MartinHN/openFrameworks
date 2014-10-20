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
    
    
    static ofParameter<ofVec3f> mins;
    static ofParameter<ofVec3f> maxs;
    
    static void updateVBO();
    static void freeVbo();
    static void updateAllColors();
    static void updateOneColor(int idx,ofColor col);
    static void updateOnePos(int idx,ofVec3f & pos);
    static void orderBy(string attr,int axe,int type);
    static Container * Cast(ofEasyCam cam, ofVec2f mouse,float sphereMult=1,bool nearest=true);
    static float distanceVanish(ofCamera cam);
    
    static Container* dragged;
    static float originDrag;
    static void draw();
    vector<float> ks;
    
    static void setSelected(int s,int end);
    static int startLines;
    static int amountLines;
    static bool linksongs;

//    
//    ofFbo pos;
//    ofFbo vel;
    
    
    string curAttribute;
    
    Physics * instance(){if(inst == NULL){inst = this;}    }
    
    void buildNetwork();
    void updatePhy(float time);
    static void updateDrag(ofVec2f mouse);
    
private:
     static Physics* inst;
    
    
};
#endif /* defined(__ViZa__Physics__) */
