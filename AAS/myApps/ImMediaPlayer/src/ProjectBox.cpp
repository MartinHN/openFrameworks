//
//  ProjectBox.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "ProjectBox.h"



void ProjectBox::draw(ofEventArgs & e){
    ofxUIToggle::draw();
//    ofPushStyle();
//    
//    if(isHovered){
//        backColor = ofColor::seaShell;
//    }
//    else{
//        backColor = ofColor::blueSteel;
//    }
//    
//    GloveInteract::draw(e);
//    
//    ofSetColor(0);
//    
//    ofDrawBitmapString(name, box.x,box.y+PROJECTBHEIGHT/2);
//    
//    ofPopStyle();
}
//TODO : Implement recursion
void ProjectBox::clicked(Cursor *gId, touchType & s){
    if(s==T_UP){
        if(!MediaPool::instance()->loadMedia(PROJECTPATH+name)){
            if(ofFile(PROJECTPATH+name).isDirectory()){
//                GUI
            }
            
        }
    
    }
}

void ProjectBox::parentHasMoved(GloveInteract & p){
    box.set((p).box.x,(p).box.y,box.width,box.height);
    int aaa=0;
}