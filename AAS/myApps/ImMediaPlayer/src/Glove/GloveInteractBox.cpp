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

bool GloveInteractBox::isCollision=true;

vector<GloveInteractBox*> GloveInteractBox::allElements;

GloveInteractBox::GloveInteractBox():GloveInteract(){
 
    backColor = ofColor::blueSteel;
    ofAddListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+drawLayer);
    drawLayer.addListener(this, &GloveInteractBox::setDrawLayer);
    box.set(1+ofRandom(1000),1+ofRandom(1000),400,400);// = Screens::instance()->walls[0];
    allElements.push_back(this);
}



GloveInteractBox::~GloveInteractBox(){
    
    ofRemoveListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+drawLayer);
    vector<GloveInteractBox*>::iterator it =find(allElements.begin(),allElements.end(),this);
    if(it!=allElements.end())allElements.erase(it);
}

void GloveInteractBox::cursor2DMoved(ofVec2f  pos){
    
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

void GloveInteractBox::relativeMoved(ofVec3f  pos){
    float ofZoom = 1.0+ZOOM_FACTOR*pos.z;
    updateZoom(ofZoom);
    
}

void GloveInteractBox::touch(TouchType touchId,TouchAction state){
    
    if(state == GLOVE_UP){
        if(dragged!=NULL)dragged->drawLayer = 0;
        dragged=NULL;
        zoomed == NULL;
        
    }
    
    if(isHovered){
        if(touchId == GLOVE_CLICK  ){
            this->clicked(state);
            selected = this;
        }
        else if(isDraggable && touchId == GLOVE_DRAG){
            if(state == GLOVE_DOWN && dragged == NULL){
                dragged = this;
                dragged->drawLayer = 1;
                
                targetBox = box;
                dragOffset = curGlove->cursor2D-box.getCenter();
            }
            
        }
        else if(isZoomable && touchId == GLOVE_ZOOM){
            if(state == GLOVE_DOWN ){
                zoomed = selected;
            }
            
        }
    }
    
    
    
    
}




void GloveInteractBox::drawFrontMask(){
    ofPushStyle();
    
    ofSetColor(backColor,30);
    if(isHovered){
        ofRectangle frame;
        frame.setFromCenter(box.getCenter(), box.width, box.height);
        ofRect(frame);
    }
    if(this == selected){
        ofRectangle frame;
        ofNoFill();
        ofSetLineWidth(10);
        ofSetColor(255,0,0);
        frame.setFromCenter(box.getCenter(), box.width+5, box.height+5);
        ofRect(frame);
    }
    
    
    ofPopStyle();
    
}



void GloveInteractBox::updateDrag(ofVec2f & v){
    
    if(dragged==this){
        
        ofRectangle newR (box);
        ofVec2f cen =v-dragOffset ;
        float alpha =1;
        newR.setFromCenter(alpha*cen+v*(1-alpha), box.width, box.height);
        makeValid(newR);
        targetBox.set(newR);
        
        
    }
}

void GloveInteractBox::updateZoom(float & z){
    
    if(zoomed==this ){
        ofRectangle newR;
        newR.setFromCenter(box.getCenter(),box.width*z,box.height* z);
        
        makeValid(newR);
        targetBox.set(newR);
        this->resize(box.width,box.height);
    }
}

void GloveInteractBox::makeValid(ofRectangle & newR){
    ofRectangle fullScreen = (*Screens::instance()->full);
    
    
    
    
    // lock to minimal size (format is checked at the end)
    if(newR.width<MIN_BOX_WIDTH){
        newR.setFromCenter(box.getCenter(), MIN_BOX_WIDTH, newR.height);
    }
    if(newR.height<MIN_BOX_HEIGHT){
        newR.setFromCenter(box.getCenter(), newR.width,MIN_BOX_HEIGHT);
    }
    
    // keep format
    newR.setFromCenter(newR.getCenter(),newR.width , newR.width*1.0/format);
    
    // check that box is inside full screen
    if(!fullScreen.inside(newR)){
        newR.x = MAX(fullScreen.getMinX()+1,newR.x);
        newR.x = MIN(fullScreen.getMaxX()-1,newR.getMaxX()) -newR.width;
        
        newR.y = MAX(fullScreen.getMinY()+1,newR.y);
        newR.y = MIN(fullScreen.getMaxY()-1,newR.getMaxY()) -newR.height;
        
        
    }
    
    
    
    
    
    
    
    
}
void GloveInteractBox::smooth(){
    ofRectangle newBox = box;
    resolveCollision(box);
    
    
    //    if(targetBox!=box){
    
    
    newBox.x = ofLerp(box.x, targetBox.x + targetMagnet.x, alphaTarget);
    newBox.y = ofLerp(box.y, targetBox.y +targetMagnet.y, alphaTarget);
    newBox.width = ofLerp(box.width, targetBox.width, alphaTarget);
    newBox.height = ofLerp(box.height, targetBox.height, alphaTarget);
    
    //    }
    
    
    box = newBox;
    
    
    
}


// find new place for colliding object

void GloveInteractBox::resolveCollision(ofRectangle & newR){
    // check 2d collision
    if(isCollision){
    bool isColliding = false;
    targetMagnet.set(0);
    
    if(!dragged){
        for(vector<GloveInteractBox*>::iterator it = allElements.begin() ; it!=allElements.end() ; ++it){
            if((*it)->isCollider && *it!=this && (*it)->box.intersects(newR) ){
                
                
                
                ofVec2f distance = newR.getCenter() - (*it)->box.getCenter();
                // vector representing overlapping of boxes
                ofVec2f delta(0);
                if(newR.getMinX() < (*it)->box.getMaxX() && newR.getMaxX() > (*it)->box.getMinX()){
                    delta.x =(distance.x>0?1:-1)*( abs(distance.x) - (newR.width + (*it)->box.width)/2.0);
                }
                if(newR.getMinY() < (*it)->box.getMaxY() && newR.getMaxY() > (*it)->box.getMinY()){
                    delta.y = (distance.y>0?1:-1 )* ( abs(distance.y) - (newR.height + (*it)->box.height)/2.0);
                }
                
                //                if(delta.x>delta.y)delta.y=0;
                //                else delta.x=0;
                targetMagnet-=delta;
                isColliding = true;
                break ;
                
            }
            
        }
    }
    }
    
    
    
}

void GloveInteractBox::setDrawLayer(int &l){
    ofRemoveListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+drawLayer.getLast());
    ofAddListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+drawLayer);
    
    
}



