//
//  Draggable.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "GloveInteract.h"
#include "Screens.h"


extern ofEvent<ofEventArgs> drawSyphonEvent;
vector<GloveInteract*> GloveInteract::allElements;
GloveInteract * GloveInteract::dragged=NULL;

GloveInteract::GloveInteract(){
    backColor = ofColor::blueSteel;
    
    for(vector<Cursor*>::iterator it =Cursor::cursors.begin() ; it!= Cursor::cursors.end() ; ++it){
        ofAddListener((*it)->cursor2DEvent,this,&GloveInteract::cursor2DMoved);
        ofAddListener((*it)->touchEvent,this,&GloveInteract::click);

    }
    
    ofAddListener(ofEvents().update, this, &GloveInteract::update);
    ofAddListener(drawSyphonEvent, this, &GloveInteract::draw);
    
    box = Screens::instance()->walls[0];
    
    allElements.push_back(this);
}



GloveInteract::~GloveInteract(){
    for(vector<Cursor*>::iterator it =Cursor::cursors.begin() ; it!= Cursor::cursors.end() ; ++it){
        ofRemoveListener((*it)->cursor2DEvent,this,&GloveInteract::cursor2DMoved);
        ofRemoveListener((*it)->touchEvent,this,&GloveInteract::click);
        
    }
    
    ofRemoveListener(ofEvents().update, this, &GloveInteract::update);
    ofRemoveListener(drawSyphonEvent, this, &GloveInteract::draw);
    allElements.erase(find(allElements.begin(),allElements.end(),this));
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
    
    updateDrag(arg.second);
    
}

void GloveInteract::click(touchEventArgs & a){
    if(isHovered){
        if(a.touchId == A_CLICK ){
        this->clicked(a.gloveId,a.type);
        }
        else if(isDraggable && a.touchId == A_DRAG){
            if(a.type == T_DOWN && dragged == NULL){
                dragged = this;
            }

        }
        
    }
    
    
    if(a.type == T_UP && dragged!=NULL){
        dragged=NULL;
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

void GloveInteract::updateDrag(ofVec2f & v){
//    cout << "up" << endl;
    if(dragged==this){
        ofVec2f cen =box.getCenter();
        float alpha =ofClamp(0.7 - (cen-v).length()/700,0,1);
        dragged->box.setFromCenter(alpha*cen+v*(1-alpha), dragged->box.width, dragged->box.height);
    }
    if(dragged !=NULL){
        int tttt=0;
    }
}

