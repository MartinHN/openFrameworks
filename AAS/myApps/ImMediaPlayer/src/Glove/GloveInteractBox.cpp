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
//GloveInteractBox * GloveInteractBox::zoomed=NULL;
GloveInteractBox * GloveInteractBox::selected=NULL;

ofColor GloveInteractBox::hoverColor(100,100,100,30);
ofColor GloveInteractBox::selectedColor(255,0,0,100);

bool GloveInteractBox::isCollision=true;

int GloveInteractBox::frontDragLayer=0;

vector<GloveInteractBox*> GloveInteractBox::allElements;





GloveInteractBox::GloveInteractBox():GloveInteract(){
    ofAddListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+drawLayer);
    drawLayer.addListener(this, &GloveInteractBox::setDrawLayer);
    box.set(1+ofRandom(1000),1+ofRandom(1000),400,400);
    
    drawLayer = allElements.size();
    allElements.push_back(this);
    isSelected = false;
    isHovered = false;
    
    targetMagnet.set(0);
    
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

void GloveInteractBox::touch(TouchButton touchId,TouchAction state){
    
    if(state == GLOVE_ACTION_UP){
        if(dragged!=NULL){
            lastDragged = dragged;
            //            dragged->drawLayer = dragged->drawLayer.getLast();
        }
        dragged=NULL;
        //        zoomed == NULL;
        
    }
    
    // intern handling function
    // prefer using GLOVE_ACTION_DOWN for intern state update and GLOVE_ACTION_xxxPRESS for callbacks
    
    if(isHovered){
        cout << "hovered = " << name << endl;
        if( state == GLOVE_ACTION_DOWN && touchId == GLOVE_BUTTON_CLICK ){
            
            
            if(selected!=NULL && drawLayer>selected->drawLayer){
                selected->isSelected = false;
                selected->sendBack();
                selected = NULL;
            }
            if(selected == NULL){//
                selected = this;
                selected->isSelected = true;
                selected->sendForeground();
                
            }
            
            
        }
        else if( isDraggable && touchId == GLOVE_BUTTON_DRAG && state == GLOVE_ACTION_DOWN){
            
            
            //            if( amIFirstLayer(curGlove->cursor2D) ){
            if(dragged == NULL){
                
                dragged = this;
                dragged->sendForeground();
                // underlying evets will be produced if box actually move see boxMoved() function called in updateBox()
                targetBox = box;
                targetMagnet.set(0);
                dragOffset = curGlove->cursor2D-box.getCenter();
            }
            
            
        }
        //        else if(isZoomable && touchId == GLOVE_BUTTON_ZOOM){
        //            if(state == GLOVE_ACTION_DOWN ){
        //                                zoomed = selected;
        //            }
        
        
        
        
        // callbacks
        if(state == GLOVE_ACTION_SHORTPRESS){
            this->clicked(touchId);
        }
    }
    else if(state == GLOVE_ACTION_DOWN && touchId == GLOVE_BUTTON_CLICK && selected==this){
        cout << "unselect = " << name << endl;
        this->isSelected = false;
        selected = NULL;
    }
    
    
    
    
    
    
}


void GloveInteractBox::sendForeground(){
    vector<GloveInteractBox*> vec = GloveInteractBox::allElements;
    if(drawLayer != vec.size()){
        
        for(vector<GloveInteractBox*>::iterator it = vec.begin() ; it!= vec.end() ; it ++){
            if((*it)->drawLayer>drawLayer) (*it)->drawLayer = (*it)->drawLayer  - 1;
        }
        
        drawLayer = vec.size();
    }
}

void GloveInteractBox::sendBackground(){
    if(drawLayer != 0){
        vector<GloveInteractBox*> vec = GloveInteractBox::allElements;
        for(vector<GloveInteractBox*>::iterator it = vec.begin() ; it!= vec.end() ; it ++){
            (*it)->drawLayer = (*it)->drawLayer  +1;
        }
        drawLayer = 0;
    }
}

void GloveInteractBox::sendBack(){
    vector<GloveInteractBox*> vec = GloveInteractBox::allElements;
    for(vector<GloveInteractBox*>::iterator it = vec.begin() ; it!= vec.end() ; it ++){
        if((*it)->drawLayer==drawLayer-1) (*it)->drawLayer = (*it)->drawLayer  +1;
    }
    drawLayer = drawLayer-1;
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
    
    if(this == dragged){
        ofSetColor(255,0,0);
        ofNoFill();
        ofSetLineWidth(box.width/400.0);
        ofCircle(box.getCenter(), box.width/40.0);
        ofLine(box.getCenter() - ofVec2f(box.width/30.0,0), box.getCenter() + ofVec2f(box.width/30.0,0));
        ofLine(box.getCenter() - ofVec2f(0,box.width/30.0), box.getCenter() + ofVec2f(0,box.width/30.0));
    }
    
    
    ofPopStyle();
    
}



void GloveInteractBox::updateDrag(ofVec2f & v){
    
    if(dragged==this){
        
        ofRectangle newR;
        newR.setFromCenter(v-dragOffset, box.width, box.height);
        makeValid(newR);
        targetBox.set(newR);
        
        
    }
}

void GloveInteractBox::updateZoom(float & z){
    
    if(selected==this ){
        ofRectangle newR;
        newR.setFromCenter(box.getCenter(),box.width*z,box.height* z);
        makeValid(newR);
        targetBox.set(newR);
        
        
    }
}

void GloveInteractBox::makeValid(ofRectangle & newR){
    ofRectangle fullScreen = (*Screens::instance()->full);
    
    // lock to minimal size
    if(newR.width<MIN_BOX_WIDTH){
        newR.setFromCenter(box.getCenter(), MIN_BOX_WIDTH, newR.height);
    }
    if(newR.height<MIN_BOX_HEIGHT){
        newR.setFromCenter(box.getCenter(), newR.width,MIN_BOX_HEIGHT);
    }
    
    // check that box center is inside full screen
    if(!fullScreen.inside(newR.getCenter())){
        ofVec2f center;
        center.x = ofClamp(newR.getCenter().x,fullScreen.getMinX(),fullScreen.getMaxX());
        center.y = ofClamp(newR.getCenter().y,fullScreen.getMinY(),fullScreen.getMaxY());
        newR.setFromCenter(center,box.width,box.height);
        
    }
    

    
}
void GloveInteractBox::updateBox(){
    
    if(targetBox+targetMagnet!=box){
        
        //        resolveCollision(targetBox);
        bool isMoving = (targetBox.x+targetMagnet.x != box.x || targetBox.y+targetMagnet.y != box.y);
        
        bool isResizing = (targetBox.width != box.width || targetBox.height != box.height);
        
        
        
        box.setFromCenter(      ofLerp(box.getCenter().x,   targetBox.getCenter().x +targetMagnet.x,   alphaTarget),
                          ofLerp(box.getCenter().y,   targetBox.getCenter().y+targetMagnet.y,   alphaTarget),
                          ofLerp(box.width,           targetBox.width,                            alphaTarget),
                          ofLerp(box.height,          targetBox.height,                           alphaTarget));
        
        
        // avoid infinitesimal changes
        if(abs(targetBox.x+targetMagnet.x - box.x)<.1 &&
           abs(targetBox.y+targetMagnet.y - box.y)<.1 &&
           abs(targetBox.width - box.width)<.05 &&
           abs(targetBox.height - box.height)<.05){
            box.setFromCenter(targetBox.getCenter() + targetMagnet, targetBox.width, targetBox.height);
            
        }
        

        
        // Callbacks
        
        if(isResizing)this->boxResized();
        if(isMoving)this->boxMoved();
    }
    
    
}


// find new place for colliding object

void GloveInteractBox::resolveCollision(ofRectangle  newR){
    // check 2d collision
    
    
    if(isCollision && isCollider){
        bool isColliding = false;
        
        
        if(!dragged && lastDragged == this){
            ofVec2f _targetMagnet;
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
    gloveEventsPriority = -drawLayer.get();
    
}

//bool GloveInteractBox::amIFirstLayer(ofVec2f & p){
//    vector<GloveInteractBox*> res;
//    for(vector<GloveInteractBox*>::iterator it = allElements.begin() ; it != allElements.end() ; ++it){
//        if((*it)!=this &&(*it)->isHovered && this->drawLayer<(*it)->drawLayer)return false;
//    }
//    
//    return true;
//}


void GloveInteractBox::draw(ofEventArgs & e){
    ofPushStyle();
    ofSetColor(ofColor::black, 100);
    ofRect(box+ofVec2f(-5,5));
    ofSetColor(0);
    ofRect(box);
    ofSetColor(255);
    this->draw();
    drawFrontMask();
    ofPopStyle();
};


void GloveInteractBox::update(ofEventArgs & e){
    updateBox();
    this->update();
}





bool GloveInteractBox::layerCompare(GloveInteractBox & lhs, GloveInteractBox & rhs){
    return lhs.drawLayer < rhs.drawLayer;
}



