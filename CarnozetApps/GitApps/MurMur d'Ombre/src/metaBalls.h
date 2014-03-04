//
//  metaBalls.h
//  MurMur d'Ombre
//
//  Created by martin hermant on 14/02/14.
//
//

#pragma once

#include "ofMain.h"
#include "VisuHandler.h"




class metaBalls:public VisuClass{
public:
    
    metaBalls(VisuHandler * v);
    ~metaBalls(){};
    
    void draw(int w, int h);
    void update(int w, int h);

    
    
    ofParameter<float> rayon,alpha;
    ofParameter<int> curFamilly;
    ofParameter<ofVec4f> color;
    vector<ofVec3f>  dpoints;
    vector<ofVec3f>  points;
    
    
    ofImage metaBall;
    

    
};




