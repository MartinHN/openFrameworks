//
//  ProjectBox.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "ProjectBox.h"


void ProjectBox::draw(ofEventArgs & e){
    ofPushStyle();
    
    if(isHovered){
        backColor = ofColor::seaShell;
    }
    else{
        backColor = ofColor::blueSteel;
    }
    
    GloveInteract::draw(e);
    
    ofSetColor(0);
    
    ofDrawBitmapString(name, box.x,box.y+PROJECTBHEIGHT/2);
    
    ofPopStyle();
}

void ProjectBox::clicked(Cursor *gId, touchType & s){
    if(s==T_UP){
        MediaPool::instance()->loadMedia(PROJECTPATH+name);}
}