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
   
    
    _anchorUp = anchorUp;
    _anchorOffset = offset;
    

}


MetaScreen::~MetaScreen(){
    image.clear();
    ofRemoveListener(ofEvents().update, this, &MetaScreen::update);
    ofRemoveListener(drawSyphonEvent, this, &MetaScreen::draw,OF_EVENT_ORDER_AFTER_APP);
}


void MetaScreen::update(ofEventArgs &a){
    init();
    for(map<GloveInstance*,GloveInteractBox*>::iterator it = GloveInteractBox::dragged.begin() ; it!= GloveInteractBox::dragged.end();++it){
        if(it->second!=NULL){
            if(fullScreenAnchor.inside(it->second->box.getCenter())){
                isHovering[it->first] = true;
                fullScreened =it->second;
            }
            else{
                isHovering[it->first] = false;
                if(fullScreened!=NULL){
                    
                    if(fullScreened->drawLayer==defaultLayer)fullScreened->sendToLayer(fullScreened->drawLayer.getLast());
                    fullScreened =NULL;
                }
            }
        }
        
        else{
            if(fullScreened!=NULL && isHovering[it->first]==true){
                fullScreened->sendToLayer(defaultLayer);
                fullScreened->targetBox = *this;
                isHovering[it->first] = false;
            }
        }
        
    }
}



void MetaScreen::draw(ofEventArgs &a){
    ofPushStyle();
    ofPushMatrix();
    
    image.draw(fullScreenAnchor.getCenter());
    for(map<GloveInstance*,bool>::iterator it = isHovering.begin() ; it!=isHovering.end();++it){
    if(it->second){
        ofSetColor(ofColor::white, 30);
        ofDrawRectangle(*this);
        float lineW = 10;
        ofSetColor(ofColor::red);
        ofDrawRectangle(getMinX(),getMinY(),width,lineW);
        ofDrawRectangle(getMaxX()-lineW,getMinY()+lineW,lineW,height-2*lineW);
        ofDrawRectangle(getMinX(),getMaxY()-lineW,width,lineW);
        ofDrawRectangle(getMinX(),getMinY()+lineW,lineW,height-2*lineW);
        break;
    }
    }
    ofPopMatrix();
    ofPopStyle();
    
}


void MetaScreen::init(){
    if(!image.isAllocated()){
        image.load("UI/fullScreenAnchor.png");
        image.resize(ANCHOR_WIDTH,ANCHOR_WIDTH*image.height*1.0/image.width);
        image.setAnchorPercent(.5,.5);
        if(_anchorUp)image.mirror(true, false);
        fullScreenAnchor.setFromCenter(x + width/2.0, _anchorUp?(ANCHOR_WIDTH*image.height*1.0/image.width)/2.0:y + height - (ANCHOR_WIDTH*image.height*1.0/image.width)/2.0, ANCHOR_WIDTH, (ANCHOR_WIDTH*image.height*1.0/image.width));
        fullScreenAnchor.x+=_anchorOffset.x;
        fullScreenAnchor.y+=_anchorOffset.y;
    }
}