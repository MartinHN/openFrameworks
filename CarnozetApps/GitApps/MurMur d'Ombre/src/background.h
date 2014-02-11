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
    
    ofParameter<int> rayon;
    ofParameter<ofVec2f> center;
    
    void draw(int w, int h);
    void update(int w, int h);
    
    
};




