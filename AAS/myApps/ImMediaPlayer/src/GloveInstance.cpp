//
//  GloveInstance.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "GloveInstance.h"
#include "Screens.h"





extern ofEvent<ofEventArgs> drawSyphonEvent;



GloveInstance::GloveInstance(){

    
    touchs.reserve(NUMTOUCH);
    flex.reserve(NUMFLEX);
    
    cursorImg = new ofImage();
    cursorImg->loadImage(ofToDataPath("UI/cursor.png"));
    
    ofAddListener(ofEvents().update, this, &GloveInstance::update);
    ofAddListener(drawSyphonEvent, this, &GloveInstance::draw,OF_EVENT_ORDER_AFTER_APP);
    
    
    
    
    
    
}


GloveInstance::~GloveInstance(){

}


void GloveInstance::update(ofEventArgs & a){

    
}


void GloveInstance::draw(ofEventArgs & a){
    cursorImg->draw(cursor2D.x,cursor2D.y);
}








