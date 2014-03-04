//
//  drawBlob.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 02/03/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "drawBlob.h"


drawBlob::drawBlob(VisuHandler * v):VisuClass(v){

    MYPARAM(drawZones,false,false,true);
    MYPARAM(invertx,false,false,true);
    MYPARAM(inverty,false,false,true);

    MYPARAM(pos,ofVec3f(0.5),ofVec3f(0),ofVec3f(1));
    MYPARAM(scale,ofVec2f(1),ofVec2f(0),ofVec2f(2));
    MYPARAM(color,ofVec3f(255),ofVec3f(0),ofVec3f(255));
    MYPARAM(alpha,255.f,0.f,255.f);
    
    
    
    settings.setName("drawBlob");
}


void drawBlob::update(int w, int h){
    
    
}

void drawBlob::draw(int w, int h){
    
    if(drawZones){

        for(int i = 0 ; i < dad->attr->zones.size() ; i++){
            ofNoFill();
            ofSetColor(i==0?255:0,i==1?255:0,i==2?255:0);
            ofRectangle rr =dad->attr->zones[i];
            rr.x*=w;
            rr.y*=h;
            rr.scale(w,h);
            ofRect(rr);
        }
    }
   
    vector<ofPath> paths = dad->bH->getPaths(w*scale->x , h*scale->y,invertx,inverty);
    ofPushMatrix();
    
    ofTranslate(w*(pos->x-scale->x/2.),h*(pos->y-scale->y/2.), (pos->z-0.5)*dad->zdepth);
    for(int i  = 0 ; i< paths.size();i++){
        paths[i].setFillColor(ofColor(color->x,color->y,color->z,alpha));
        paths[i].draw();
    }
    
    
    ofPopMatrix();
    
 }

