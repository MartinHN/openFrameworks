//
//  background.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 04/02/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "background.h"



background::background(VisuHandler * v):VisuClass(v){
    MYPARAM(type,0,0,10);
    MYPARAM(center, ofVec2f(0.5), ofVec2f(0), ofVec2f(1));
    MYPARAM(color, ofVec4f(255), ofVec4f(0), ofVec4f(255));
    MYPARAM(scale, 1.f,0.f,5.f);
    MYPARAM(ratio, 1.f,0.f,5.f);
    settings.setName("background");
}


void background::update(int w, int h){

    
}

void background::draw(int w, int h){
    ofSetColor(color.get().x,color.get().y,color.get().z,color.get().w);
    ofRectangle rect;
    rect.setFromCenter(center.get()*ofVec2f(w,h), w*scale, h*scale*ratio);
    if(type>0)
        dad->sharedImg["circlegrad"].draw(rect);
    else
        ofRectangle(rect);
}
