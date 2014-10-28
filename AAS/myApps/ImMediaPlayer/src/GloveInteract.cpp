//
//  Draggable.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "GloveInteract.h"



GloveInteract::GloveInteract(){
    backColor = ofColor::blueSteel;
    
    for(vector<Cursor*>::iterator it =Cursor::cursors.begin() ; it!= Cursor::cursors.end() ; ++it){
        ofAddListener((*it)->cursor2DEvent,this,&GloveInteract::cursor2DMoved);
        ofAddListener((*it)->touchEvent,this,&GloveInteract::click);

    }
    
}



GloveInteract::~GloveInteract(){

}

void GloveInteract::cursor2DMoved(pair<Cursor*,ofVec2f> & arg){
    ofVec2f pos = arg.second;
    bool _isHovered = this->isHit(pos);
    if(_isHovered && !isHovered){this->entered();}
    if(!_isHovered && isHovered){this->exited();}
    
    isHovered = _isHovered;
    if(isHovered){
        ofVec2f newPos=pos-ofVec2f(box.x,box.y);
        this->hover(newPos);
    }
    
    
    
}

void GloveInteract::click(touchEventArgs & a){
    if(isHovered){
        this->clicked(a.gloveId,a.touchId,a.type);
    }
}




void GloveInteract::draw(ofEventArgs &e){
    ofPushStyle();

    ofSetColor(backColor);
    ofRect(box);

    ofPopStyle();
    
}

void GloveInteract::update(ofEventArgs &e){
    
}


void GloveInteract::draw(){
    ofEventArgs a;
    draw(a);
    
}