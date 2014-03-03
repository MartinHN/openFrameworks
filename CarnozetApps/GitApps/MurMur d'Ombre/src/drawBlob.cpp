//
//  drawBlob.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 02/03/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "drawBlob.h"


drawBlob::drawBlob(VisuHandler * v):VisuClass(v){

    MYPARAM(fill,true,false,true);

    MYPARAM(pos,ofVec3f(0.5),ofVec3f(0),ofVec3f(1));
    MYPARAM(scale,ofVec2f(1),ofVec2f(0),ofVec2f(2));
   
    

    
    settings.setName("drawBlob");
}


void drawBlob::update(int w, int h){
    
    
}

void drawBlob::draw(int w, int h){
    ofSetColor(255);

    vector<ofPath> paths = dad->bH->getPaths(w*scale->x , h*scale->y);
    ofPushMatrix();
    if(fill)ofFill();
    else ofNoFill();
    
    ofTranslate(w*(pos->x-scale->x/2.),h*(pos->y-scale->y/2.), (pos->z-0.5)*dad->zdepth);
    for(int i  = 0 ; i< paths.size();i++){
        paths[i].draw();
    }
    
    
    ofPopMatrix();
    
 }

