//
//  AttrCtl.h
//  MurMur d'Ombre
//
//  Created by Tinmar on 23/03/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "ofMain.h"
#define MAXATTRACTORS 16

class AttrCtl{
    
    public :
    AttrCtl();
    
    float* attr;

    
    float attrotate;

    int surech;

    float echcoef[9];//[7]; 
    int curech;
    int numattr;
    bool attrmirorx[3];
    bool attrmirory[3];
    
    vector<ofPoint> lastp;
    vector<ofPoint> destp;
    vector<ofPoint> curp;
    vector<int> lastfamilly;
    vector<int> familly;
    vector<int> newfamilly;
    vector<ofPoint> addpoint;
    vector<int> addfamilly;
    
    
    void update(vector<ofPoint> curpin,vector<int> famillyin);
    void smooth();
    void addpoints();
    void setattr();
    vector<ofPoint> getFamilly(int f);
    
    ofParameterGroup settings;
    ofParameter<float> zoffset;
    
};