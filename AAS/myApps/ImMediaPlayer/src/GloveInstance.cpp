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



GloveInstance::GloveInstance(string _gloveID):gloveID(_gloveID){
    
    touchs = vector<bool>(NUMTOUCH,false);
    
    flex = vector<float>(NUMFLEX,0);
    
    cursorImg =  ofImage();
    cursorImg.loadImage(ofToDataPath("UI/cursor.png"));
    cursorImg.resize(40,40);
    cursorImg.setAnchorPercent(.5,.5);
    ofAddListener(ofEvents().update, this, &GloveInstance::update);
    ofAddListener(drawSyphonEvent, this, &GloveInstance::draw,OF_EVENT_ORDER_AFTER_APP);
    
}


GloveInstance::~GloveInstance(){
    
    ofRemoveListener(ofEvents().update, this, &GloveInstance::update);
    ofRemoveListener(drawSyphonEvent, this, &GloveInstance::draw,OF_EVENT_ORDER_AFTER_APP);
}


void GloveInstance::update(ofEventArgs & a){

    
}


void GloveInstance::draw(ofEventArgs & a){
    cursorImg.draw(cursor2D.x,cursor2D.y);
}








