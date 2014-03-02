//
//  drawBlob.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 02/03/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "drawBlob.h"


drawBlob::drawBlob(VisuHandler * v):VisuClass(v){
   
    MYPARAM(smooth,0.f,0.f,10.f)
    
    MYPARAM(screen1,0,-1,15);
    MYPARAM(pos1,ofVec3f(0.5),ofVec3f(0),ofVec3f(1));
    MYPARAM(scale1,ofVec2f(1),ofVec2f(0),ofVec2f(2));
   
    
    
    MYPARAM(screen2,-1,-1,15);
    MYPARAM(pos2,ofVec3f(0.5,0.5),ofVec3f(0),ofVec3f(1));
    MYPARAM(scale2,ofVec2f(1),ofVec2f(0),ofVec2f(2));

    
    settings.setName("drawBlob");
}


void drawBlob::update(int w, int h){
    
    
}

void drawBlob::draw(int w, int h){
    
    dad->pipeFbo.begin();
        ofPushMatrix();
        ofTranslate(w*scale1->x /2.0, h*scale1->y/2.0, pos1->z*dad->zdepth);
    
        vector<ofPolyline> paths = dad->bH->getPaths(w*scale1->x , h*scale1->y) ;
        
        for(int i  = 0 ; i< polys.size();i++){
            paths[i].draw();
            
        }
        
        ofPopMatrix();
    }
    
 }
