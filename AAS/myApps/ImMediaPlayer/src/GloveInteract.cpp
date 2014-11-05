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

GloveInteract * GloveInteract::dragged=NULL;
GloveInteract * GloveInteract::zoomed=NULL;
GloveInteract * GloveInteract::selected=NULL;



vector<GloveInteract*> GloveInteract::allElements;

GloveInteract::GloveInteract(){
    backColor = ofColor::blueSteel;
    
    for(vector<Cursor*>::iterator it =Cursor::cursors.begin() ; it!= Cursor::cursors.end() ; ++it){
        ofAddListener((*it)->cursor2DEvent,this,&GloveInteract::cursor2DMoved);
        ofAddListener((*it)->touchEvent,this,&GloveInteract::click);
        ofAddListener((*it)->relativeOrientationEvent,this,&GloveInteract::relativeMoved);
    }
    
    ofAddListener(ofEvents().update, this, &GloveInteract::update);
    ofAddListener(drawSyphonEvent, this, &GloveInteract::draw,OF_EVENT_ORDER_BEFORE_APP);
    
    box.set(1+ofRandom(1000),1+ofRandom(1000),400,400);// = Screens::instance()->walls[0];
    
    
            allElements.push_back(this);
}



GloveInteract::~GloveInteract(){
    for(vector<Cursor*>::iterator it =Cursor::cursors.begin() ; it!= Cursor::cursors.end() ; ++it){
        ofRemoveListener((*it)->cursor2DEvent,this,&GloveInteract::cursor2DMoved);
        ofRemoveListener((*it)->touchEvent,this,&GloveInteract::click);
        ofRemoveListener((*it)->relativeOrientationEvent,this,&GloveInteract::relativeMoved);
        
    }
    
    ofRemoveListener(ofEvents().update, this, &GloveInteract::update);
    ofRemoveListener(drawSyphonEvent, this, &GloveInteract::draw,OF_EVENT_ORDER_BEFORE_APP);
    
    
    
    vector<GloveInteract*>::iterator it =find(allElements.begin(),allElements.end(),this);
    if(it!=allElements.end())allElements.erase(it);
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

void GloveInteract::relativeMoved(pair<Cursor*,ofVec3f> & pos){
    float ofZoom = 1.0+pos.second.z;
    updateZoom(ofZoom);
    
}

void GloveInteract::click(touchEventArgs & a){
    if(a.type == T_UP){
        dragged=NULL;
        zoomed == NULL;
        selected = NULL;
    }

    if(isHovered){
        if(a.touchId == A_CLICK ){
        this->clicked(a.gloveId,a.type);
            selected = this;
        }
        else if(isDraggable && a.touchId == A_DRAG){
            if(a.type == T_DOWN && dragged == NULL){
                dragged = this;
                dragOffset = a.gloveId->cursor2D-box.getCenter();
            }

        }
        else if(isZoomable && a.touchId == A_ZOOM){
            if(a.type == T_DOWN && zoomed == NULL){
                zoomed = selected;
            }

        }
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
        ofRectangle newR (box);
        ofVec2f cen =box.getCenter()-dragOffset ;
        float alpha =ofClamp(0.7 - (cen-v).length()/700,0,1);
        newR.setFromCenter(alpha*cen+v*(1-alpha), box.width, box.height);
        isValid(newR);
        box.set(newR);
        
    }
}

void GloveInteract::updateZoom(float & z){
    
    if(zoomed==this && box.width*z>100 && box.height*z>100 && box.height*z<Screens::instance()->resolution.y){
        ofRectangle newR (box);
        newR.setFromCenter(box.getCenter(),box.width* z,box.height* z);
        
        isValid(newR);
        box.set(newR);
        
    }
}

bool GloveInteract::isValid(ofRectangle & newR){
    ofRectangle rr = (*Screens::instance()->full);
    if(!(*Screens::instance()->full).inside(newR)){
        newR.x = MAX(rr.getMinX()+1,newR.x);
        newR.width = MIN(rr.getMaxX()-1,newR.getMaxX()) -newR.getMinX();
        newR.y = MAX(rr.getMinY()+1,newR.y);
        newR.height = MIN(rr.getMaxY()-1,newR.getMaxY()) -newR.getMinY();
        
        return false;
    }
    
    bool res = true;
    for(vector<GloveInteract*>::iterator it = allElements.begin() ; it!=allElements.end() ; ++it){
        if((*it)->isCollider && (*it)->box.intersects(newR)){
            res = false;
            break;
        }
        
    }
    return res;
    
}

