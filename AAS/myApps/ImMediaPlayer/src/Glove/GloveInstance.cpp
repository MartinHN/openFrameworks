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

ofEvent<ofVec3f > GloveInstance::orientationEvent;
ofEvent<ofVec3f > GloveInstance::relativeOrientationEvent;
ofEvent<touchEventArgs > GloveInstance::touchEvent;
ofEvent< vector < float> > GloveInstance::flexEvent;
ofEvent<ofVec2f > GloveInstance::cursor2DEvent;


GloveInstance::GloveInstance(string _gloveID):gloveID(_gloveID){
    
    touchs = vector<bool>(NUMTOUCH,false);
    flex = vector<float>(NUMFLEX,0);
    
    cursorImg =  ofImage();
    cursorImg.load(ofToDataPath("UI/cursor1.png"));
    cursorImg.resize(40,40);
    cursorImg.setAnchorPercent(0,0);
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




void GloveInstance::setOrientation(ofVec3f _orientation){
    orientationVel = _orientation - orientation;
    orientation = _orientation;
    ofNotifyEvent(orientationEvent,orientation ,this);
}

void GloveInstance::setRelativeOrientation(ofVec3f _r){
    float alphaVel = 0.3;
    relativeOrientationVel = (1-alphaVel) * relativeOrientationVel + alphaVel*(_r - relativeOrientation);
    relativeOrientation = _r;
    ofNotifyEvent(relativeOrientationEvent, relativeOrientation,this);
}

void GloveInstance::setTouch(TouchButton tid,TouchAction state){
    if(state == GLOVE_ACTION_DOWN)touchs[tid] = true;
    else touchs[tid] = false;
    touchEventArgs a;
    a.touchId = (TouchButton)tid;
    a.state = state;
    ofNotifyEvent(touchEvent,a,this);
}

void GloveInstance::setFlex(int n,float f){
    flex[n] = f;
    ofNotifyEvent(flexEvent, flex,this);
}

void GloveInstance::setCursor2D(ofVec2f c){
// freeze cursor during clicks

    ofRectangle full = *Screens::instance()->full;
    cursor2DVel = c*ofVec2f(full.x,full.y)-cursor2D;
    cursor2D.x = c.x*full.width;
    cursor2D.y = c.y*full.height;

    ofNotifyEvent(cursor2DEvent, cursor2D,this);
    
    
}






