//
//  drawBlob.h
//  MurMur d'Ombre
//
//  Created by Tinmar on 02/03/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "VisuHandler.h"




class drawBlob:public VisuClass{
public:
    
    drawBlob(VisuHandler * v);
    ~drawBlob(){};
    
    
    ofParameter<int> screen1;
    ofParameter<ofVec3f> pos1;
    ofParameter<ofVec2f> scale1;
    
    ofParameter<int> screen2;
    ofParameter<ofVec3f> pos2;
    ofParameter<ofVec2f> scale2;
    
    ofParameter<float> smooth;
    
    void draw(int w, int h);
    void update(int w, int h);
    
    
};







