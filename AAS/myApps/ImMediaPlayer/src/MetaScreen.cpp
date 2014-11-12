//
//  MetaScreen.cpp
//  ImMedia
//
//  Created by martin hermant on 11/11/14.
//
//

#include "MetaScreen.h"


MetaScreen::MetaScreen(ofRectangle r,bool anchorUp,const string & _name ,ofVec2f offset){
    set(r);
    ofAddListener(ofEvents().update, this, &MetaScreen::update);
    ofAddListener(drawSyphonEvent, this, &MetaScreen::draw,OF_EVENT_ORDER_AFTER_APP);
    fullScreened=NULL;
    isHovering =false;
    
    image.loadImage("UI/fullScreenAnchor.png");
    image.resize(ANCHOR_WIDTH,ANCHOR_WIDTH*image.height*1.0/image.width);
    image.setAnchorPercent(.5,.5);
    if(anchorUp)image.mirror(true, false);
    
    fullScreenAnchor.setFromCenter(r.x + r.width/2.0, anchorUp?(ANCHOR_WIDTH*image.height*1.0/image.width)/2.0:r.y + r.height - (ANCHOR_WIDTH*image.height*1.0/image.width)/2.0, ANCHOR_WIDTH, (ANCHOR_WIDTH*image.height*1.0/image.width));
    
    fullScreenAnchor.x+=offset.x;
    fullScreenAnchor.y+=offset.y;
}


MetaScreen::~MetaScreen(){
    image.clear();
    ofRemoveListener(ofEvents().update, this, &MetaScreen::update);
    ofRemoveListener(drawSyphonEvent, this, &MetaScreen::draw,OF_EVENT_ORDER_AFTER_APP);
}


void MetaScreen::update(ofEventArgs &a){
    
    if(GloveInteractBox::dragged!=NULL){
    if(fullScreenAnchor.inside(GloveInteractBox::dragged->box.getCenter())){
            isHovering = true;
            fullScreened =GloveInteractBox::dragged;
    }
    else{
        isHovering = false;
        fullScreened =NULL;
        }
    }
    
    else{
        if(fullScreened!=NULL && isHovering==true){
            fullScreened->drawLayer = defaultLayer;
            fullScreened->targetBox = *this;
            isHovering = false;
        }
    }
    
    
}



void MetaScreen::draw(ofEventArgs &a){
    ofPushStyle();
    
    image.draw(fullScreenAnchor.getCenter());
    if(isHovering){
        ofSetColor(ofColor::white, 30);
        ofRect(*this);
        float lineW = 10;
        ofSetColor(ofColor::red);
        ofRect(getMinX(),getMinY(),width,lineW);
        ofRect(getMaxX()-lineW,getMinY()+lineW,lineW,height-2*lineW);
        ofRect(getMinX(),getMaxY()-lineW,width,lineW);
        ofRect(getMinX(),getMinY()+lineW,lineW,height-2*lineW);
    }
    
    
    ofPopStyle();
    
}