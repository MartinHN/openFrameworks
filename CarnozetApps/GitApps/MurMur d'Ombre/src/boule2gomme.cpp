//
//  boule2gomme.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 04/02/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "boule2gomme.h"



boule2gomme::boule2gomme(VisuHandler * v):VisuClass(v){
    MYPARAM(rayon,2,0,100);
    MYPARAM(center,ofVec2f(0.5,0.5),ofVec2f(0),ofVec2f(1));
    settings.setName("boule2gomme");
}


void boule2gomme::update(int w, int h){

    
}

void boule2gomme::draw(int w, int h){
    
    ofVec2f scale(w,h);
    vector<ofVec3f> * points =  &dad->attr->curp;
    ofSetColor(255,0,0);
    
    ofRect(center.get().x*w,center.get().y*h,scale.x/2,scale.y/2);
    for(int i = 0 ; i<points->size();i++)    
        ofEllipse(scale*points->at(i),rayon,rayon);
    
}
