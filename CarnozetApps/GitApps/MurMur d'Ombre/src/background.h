//
//  background.h
//  MurMur d'Ombre
//
//  Created by Tinmar on 04/02/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "VisuHandler.h"




class background:public VisuClass{
public:
    
    background(VisuHandler * v);
    ~background(){};
    
    ofParameter<ofVec2f> center;
    
    ofParameter<int> type;
    ofParameter<float> scale;
    ofParameter<float> ratio;
    ofParameter<ofVec4f> color;
    
    void draw(int w, int h);
    void update(int w, int h);
    
    
};




