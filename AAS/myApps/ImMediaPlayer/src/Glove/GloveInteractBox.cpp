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
GloveInteractBox * GloveInteractBox::lastDragged=NULL;
GloveInteractBox * GloveInteractBox::zoomed=NULL;
GloveInteractBox * GloveInteractBox::selected=NULL;

ofColor GloveInteractBox::hoverColor(100,100,100,30);
ofColor GloveInteractBox::selectedColor(255,0,0,100);

bool GloveInteractBox::isCollision=true;

vector<GloveInteractBox*> GloveInteractBox::allElements;





GloveInteractBox::GloveInteractBox():GloveInteract(){
    ofAddListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+drawLayer);
    drawLayer.addListener(this, &GloveInteractBox::setDrawLayer);
    box.set(1+ofRandom(1000),1+ofRandom(1000),400,400);
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
        if(dragged!=NULL){
            lastDragged = dragged;
         dragged->drawLayer = 0;
        }
        dragged=NULL;
        zoomed == NULL;
        
    }
    
    if(isHovered){
        if(touchId == GLOVE_CLICK  ){
            this->clicked(state);
            selected = this;
        }
        else if(isDraggable && touchId == GLOVE_DRAG){
            if(state == GLOVE_DOWN && (dragged == NULL )){//|| (curGlove->cursor2D-box.getCenter()).lengthSquared()< (curGlove->cursor2D-dragged->box.getCenter()).lengthSquared())){
                dragged = this;
                dragged->drawLayer = 1;
                targetBox = box;
                targetMagnet.set(0);
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

    
    ofFill();
    if(isHovered){
        ofRectangle frame;
        ofSetColor(hoverColor);
        frame.setFromCenter(box.getCenter(), box.width, box.height);
        ofRect(frame);
    }
    if(this == selected){
        
        
        float lineW = 10;
        ofSetColor(selectedColor);
        ofRect(box.getMinX(),box.getMinY(),box.width,lineW);
        ofRect(box.getMaxX()-lineW,box.getMinY()+lineW,lineW,box.height-2*lineW);
        ofRect(box.getMinX(),box.getMaxY()-lineW,box.width,lineW);
        ofRect(box.getMinX(),box.getMinY()+lineW,lineW,box.height-2*lineW);
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
        newR.setFromCenter(box.getCenter(),box.width*z,box.width* z*1.0/format);
        
        makeValid(newR);
        targetBox.set(newR);
        
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
    
    

    
    // check that box is inside full screen
    if(!fullScreen.inside(newR)){
        ofVec2f center;
        center.x = ofClamp(newR.getCenter().x,fullScreen.getMinX() + box.width/2.0,fullScreen.getMaxX()-box.width/2.0);
        center.y = ofClamp(newR.getCenter().y,fullScreen.getMinY() + box.height/2.0,fullScreen.getMaxY()-box.height/2.0);
        newR.setFromCenter(center,box.width,box.height);
        
    }
    
    // keep format
    newR.setFromCenter(newR.getCenter(),newR.width , newR.width*1.0/format);

    
    
    
    
    
}
void GloveInteractBox::smooth(){
    
    if(targetBox+targetMagnet!=box){

        resolveCollision(targetBox);
        box.setFromCenter(      ofLerp(box.getCenter().x,   targetBox.getCenter().x +targetMagnet.x,   alphaTarget),
                                ofLerp(box.getCenter().y,   targetBox.getCenter().y+targetMagnet.y,   alphaTarget),
                                ofLerp(box.width,           targetBox.width,                            alphaTarget),
                                ofLerp(box.height,          targetBox.height,                           alphaTarget));
        this->resize(box.width,box.height);
        
    }

   
}


// find new place for colliding object

void GloveInteractBox::resolveCollision(ofRectangle  newR){
    // check 2d collision
    
    ofVec2f _targetMagnet;
    if(isCollision){
        bool isColliding = false;
        
        
        if(!dragged && lastDragged == this){
            _targetMagnet.set(0);
            // vector representing the sum of overlapping of boxes
            ofVec2f delta(0);
            ofRectangle insideBox;
            insideBox.setFromCenter(box.getCenter(),box.width-2, box.height-2);
            for(vector<GloveInteractBox*>::iterator it = allElements.begin() ; it!=allElements.end() ; ++it){
                if((*it)->isCollider && *it!=this && (*it)->box.intersects(insideBox) ){

                    ofVec2f distance = newR.getCenter() - (*it)->box.getCenter();
                   
                    float xoverlap = ((newR.width + (*it)->box.width)/2.0  - abs(distance.x));
                    if(xoverlap>0){
                        delta.x += (distance.x>0?1:-1)*xoverlap;
                    }
                    
                    float yoverlap = (newR.height + (*it)->box.height)/2.0 - abs(distance.y);
                    if(yoverlap>0){
                        delta.y += (distance.y>0?1:-1 )* ( yoverlap );
                    }
                    
                    // keep only main displacement to avoid diagonal displacement
                    if(abs(delta.x)>abs(delta.y) || (isColliding && _targetMagnet.x>0))delta.x=0;
                    else delta.y=0;
                    _targetMagnet= delta ;
                    isColliding = true;
                    
                    
                    
                }
                
            }
            if(isColliding){
                targetMagnet =_targetMagnet;
            }
            else{
                
            }
            

        }
    }
    
    
    
}

void GloveInteractBox::setDrawLayer(int &l){
    ofRemoveListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+drawLayer.getLast());
    ofAddListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+drawLayer);
    
    
}



