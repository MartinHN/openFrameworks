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



class Meta : public ofVec3f{
public:
    Meta(ofPoint p, float r, int lifeTime):ofVec3f(p),rayon(r),lifetime(lifeTime){};
    Meta(ofPoint p, float r, int lifeTime,ofVec3f vin):ofVec3f(p),rayon(r),lifetime(lifeTime),v(vin){};
    float rayon;
    int lifetime;
    ofVec3f v;

    
    
    
    
};


class metaBalls:public VisuClass{
public:
    
    metaBalls(VisuHandler * v);
    ~metaBalls(){};
    
    void draw(int w, int h);
    void update(int w, int h);

    
    
    ofParameter<float> rayon,alpha,theta;
    ofParameter<int> curFamilly,mode;
    ofParameter<ofVec4f> color;
    vector<ofVec3f>  dpoints;
    vector<Meta>  points;
    ofParameter<float> lifeTime,emitrate,noise,angle,mass;
    ofParameter<bool> drawLines;
    ofParameter<float> maxv;
    
    ofImage metaBall;
    
    unsigned long lastT;

    
};







