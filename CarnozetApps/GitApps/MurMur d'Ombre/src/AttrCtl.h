//
//  AttrCtl.h
//  MurMur d'Ombre
//
//  Created by Tinmar on 23/03/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "Constants.h"
#define MAXATTRACTORS 16

class AttrStruct{
public:
    AttrStruct(ofPoint pin ,int fin):p(pin),f(fin){};
   
    ofPoint p;
    int f;
};

class AttrCtl{
    
    public :
    AttrCtl();
    
    void registerParam();

    
    float attrotate;

    bool attrmirorx[3];
    bool attrmirory[3];
    
    vector<AttrStruct> lastA;
    vector<AttrStruct> destA;

    vector<AttrStruct> smoothedA;
    vector<AttrStruct> staticA;

    
    
    void updatePoints(vector<ofPoint> curpin);
    void update();
    void smooth();
    void staticpoints();
    void timedPoints();
    vector<ofPoint> getFamilly(int f);
    vector<ofPoint> getAll();
    
    ofParameterGroup settings;
    ofParameter<float> zoffset;
    ofParameter<ofVec4f> zonefamilly3;
    ofParameter<ofVec4f> zonefamilly1;
    ofParameter<ofVec4f> zonefamilly2;
    ofParameter<int> attrmirrorx;
    ofParameter<int> attrmirrory;
    ofParameter<int> attrmirrorz;
};