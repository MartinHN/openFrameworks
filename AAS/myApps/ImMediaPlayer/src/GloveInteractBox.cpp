//
//  Draggable.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "GloveInteractBox.h"
#include "Screens.h"


extern ofEvent<ofEventArgs> drawSyphonEvent;

GloveInteractBox * GloveInteractBox::dragged=NULL;
GloveInteractBox * GloveInteractBox::zoomed=NULL;
GloveInteractBox * GloveInteractBox::selected=NULL;



vector<GloveInteractBox*> GloveInteractBox::allElements;

GloveInteractBox::GloveInteractBox():GloveInteract(){
    
    
    backColor = ofColor::blueSteel;
    
    
    ofAddListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP);
    
    box.set(1+ofRandom(1000),1+ofRandom(1000),400,400);// = Screens::instance()->walls[0];
    
    
            allElements.push_back(this);
}



GloveInteractBox::~GloveInteractBox(){

    ofRemoveListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP);
    
    
    
    vector<GloveInteractBox*>::iterator it =find(allElements.begin(),allElements.end(),this);
    if(it!=allElements.end())allElements.erase(it);
}

void GloveInteractBox::cursor2DMoved(ofVec2f & pos){
    
    bool _isHovered = this->isHit(pos);
    if(_isHovered && !isHovered){this->entered();}
    if(!_isHovered && isHovered){this->exited();}
    
    isHovered = _isHovered;
    if(isHovered){
        ofVec2f newPos=pos-ofVec2f(box.x,box.y);
        this->hover(newPos);
    }
    
    updateDrag(pos);
    
}

void GloveInteractBox::relativeMoved(ofVec3f & pos){
    float ofZoom = 1.0+pos.z;
    updateZoom(ofZoom);
    
}

void GloveInteractBox::touch(touchEventArgs & a){

    if(a.state == GLOVE_UP){
        dragged=NULL;
        zoomed == NULL;
        selected = NULL;
    }

    if(isHovered){
        if(a.touchId == GLOVE_CLICK  ){
        this->clicked(a.state);
            selected = this;
        }
        else if(isDraggable && a.touchId == GLOVE_DRAG){
            if(a.state == GLOVE_DOWN && dragged == NULL){
                dragged = this;
                dragOffset = curGlove->cursor2D-box.getCenter();
            }

        }
        else if(isZoomable && a.touchId == GLOVE_ZOOM){
            if(a.state == GLOVE_DOWN && zoomed == NULL){
                zoomed = selected;
            }

        }
    }
    
    


}




void GloveInteractBox::draw(ofEventArgs &e){
    ofPushStyle();

    ofSetColor(backColor);
    ofRect(box);

    ofPopStyle();
    
}

void GloveInteractBox::update(ofEventArgs &e){
    
}

void GloveInteractBox::updateDrag(ofVec2f & v){
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

void GloveInteractBox::updateZoom(float & z){
    
    if(zoomed==this && box.width*z>100 && box.height*z>100 && box.height*z<Screens::instance()->resolution.y){
        ofRectangle newR (box);
        newR.setFromCenter(box.getCenter(),box.width* z,box.height* z);
        
        isValid(newR);
        box.set(newR);
        
    }
}

bool GloveInteractBox::isValid(ofRectangle & newR){
    ofRectangle rr = (*Screens::instance()->full);
    if(!(*Screens::instance()->full).inside(newR)){
        newR.x = MAX(rr.getMinX()+1,newR.x);
        newR.width = MIN(rr.getMaxX()-1,newR.getMaxX()) -newR.getMinX();
        newR.y = MAX(rr.getMinY()+1,newR.y);
        newR.height = MIN(rr.getMaxY()-1,newR.getMaxY()) -newR.getMinY();
        
        return false;
    }
    
    bool res = true;
    for(vector<GloveInteractBox*>::iterator it = allElements.begin() ; it!=allElements.end() ; ++it){
        if((*it)->isCollider && (*it)->box.intersects(newR)){
            res = false;
            break;
        }
        
    }
    return res;
    
}

