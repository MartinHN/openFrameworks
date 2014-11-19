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

map<GloveInstance*,GloveInteractBox*>  GloveInteractBox::dragged;
map<GloveInstance*,GloveInteractBox*>  GloveInteractBox::lastDragged;
map<GloveInstance*,GloveInteractBox*>  GloveInteractBox::selected;
map<GloveInstance*,GloveInteractBox*>  GloveInteractBox::hovered;


ofColor GloveInteractBox::hoverColor(100,100,100,30);
ofColor GloveInteractBox::selectedColor(255,0,0,100);

bool GloveInteractBox::isCollision=true;



vector<GloveInteractBox*> GloveInteractBox::allElements;





GloveInteractBox::GloveInteractBox():GloveInteract(){
    ofAddListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+drawLayer);
    drawLayer.addListener(this, &GloveInteractBox::setDrawLayer);
    ofRectangle tmpR(1+ofRandom(1000),1+ofRandom(1000),400,400);
    box = tmpR;
    targetBox = tmpR;
    //start layer on top of fullScreens layers
    drawLayer = screens.totalScreens+allElements.size();
    allElements.push_back(this);
    isSelected = false;
    isDragged=false;
    isHovered=false;
    
    
}



GloveInteractBox::~GloveInteractBox(){
    
    ofRemoveListener(drawSyphonEvent, this, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+drawLayer);
    for(map<GloveInstance*,GloveInteractBox*>::iterator it = selected.begin() ; it!= selected.end();++it){
        if(it->second == this)selected[it->first] = NULL;
    }
    for(map<GloveInstance*,GloveInteractBox*>::iterator it = dragged.begin() ; it!= dragged.end();++it){
        if(it->second == this)dragged[it->first] = NULL;
    }
    
    
    vector<GloveInteractBox*>::iterator it =find(allElements.begin(),allElements.end(),this);
    
    if(it!=allElements.end())allElements.erase(it);
}

void GloveInteractBox::cursor2DMoved(ofVec2f  pos){
    
    // is the current glove hovering
    bool _isHovered =  this->isHit(pos);
    
    // send change Events for each glove (watch isHovered if you want to be sure that no glove are hovering)
    if(_isHovered && !(hovered[curGlove]==this)){
        this->entered();
        
        hovered[curGlove]=this;
        
    }
    else if(!_isHovered && (hovered[curGlove]==this)){
        this->exited();
        
        hovered[curGlove]=NULL;
        
    }
    
    
    // check for every glove to stay hovered until the last
    isHovered=false;
    for(map<GloveInstance*,GloveInteractBox*>::iterator it =  hovered.begin();it!=hovered.end();++it){
        if(it->second==this){
            isHovered = true;
            break;
        }
        
    }
    
    // if the current glove hovers, send call hover function
    if(hovered[curGlove]==this){
        ofVec2f newPos=pos-ofVec2f(box.x,box.y);
        this->hover(newPos);
    }
    
    updateDrag(pos);
    
}

void GloveInteractBox::relativeMoved(ofVec3f  pos){
    if(curGlove->touchs[GLOVE_BUTTON_ZOOM]){
    float ofZoom = 1.0+ZOOM_FACTOR*pos.z;
    updateZoom(ofZoom);
    }
}

void GloveInteractBox::touch(TouchButton touchId,TouchAction state){
    
    if(state == GLOVE_ACTION_UP){
        if(dragged[curGlove]!=NULL){
            lastDragged[curGlove] = dragged[curGlove];
            
        }
        if(dragged[curGlove]==this){
            isDragged=false;
            dragged[curGlove]=NULL;
        }
        //        zoomed == NULL;
        
    }
    if(state == GLOVE_ACTION_SHORTPRESS){
        
        int EEE=00;
    }
    
    
    // intern handling function
    // prefer using GLOVE_ACTION_DOWN for intern state update and GLOVE_ACTION_xxxPRESS for callbacks
    
    if( isHovered ){
        
        
        
        if( state == GLOVE_ACTION_DOWN){
            
            // select it
            
            if( isSelectable && touchId == GLOVE_BUTTON_CLICK  ){
                wasSelected = selected[curGlove]==this;
                // activate if no one selected
                if(selected[curGlove] == NULL){//
                    selected[curGlove] = this;
                    isSelected = true;
                    if(drawLayer>=screens.totalScreens)sendForeground();
                    
                }
                
                // prefer frontmost layer ( should be useless with eventing system synced on draw layers)
                else if(selected[curGlove]!=NULL && drawLayer>selected[curGlove]->drawLayer){
                    selected[curGlove]->isSelected = false;
                    selected[curGlove]->sendBack();
                    selected[curGlove] = NULL;
                    cout << "eventing system corrupted" << endl;
                }
                
                
            }
            
            // drag it
            if(touchId == GLOVE_BUTTON_DRAG && isDraggable ){
                
                
                if(dragged[curGlove] == NULL){
                    
                    dragged[curGlove] = this;
                    if(drawLayer>=screens.totalScreens)dragged[curGlove]->sendForeground();
                    // underlying evets will be produced if box actually move see boxMoved() function called in updateBox()
                    targetBox = box;
                    
                    dragOffset = curGlove->cursor2D-box.getCenter();
                    isDragged=true;
                }
                
                
            }
            //        else if(isZoomable && touchId == GLOVE_BUTTON_ZOOM){
            //            if(state == GLOVE_ACTION_DOWN ){
            //                                zoomed = selected;
            //            }
            
            
        }
        
        
        // callbacks
        if( wasSelected &&  state == GLOVE_ACTION_SHORTPRESS){
            this->clicked(touchId);
        }
        
        
    }
    // not hovered
    
    // un-select last
    else if(state == GLOVE_ACTION_DOWN && touchId == GLOVE_BUTTON_CLICK && selected[curGlove]==this){
        cout << "unselect = " << name << endl;
        isSelected = false;
        wasSelected = false;
        selected[curGlove] = NULL;
    }
    
    
    
    
    
    
}


void GloveInteractBox::sendForeground(){

    sendToLayer(GloveInteractBox::allElements.size());

}

void GloveInteractBox::sendBackground(){
sendToLayer(screens.totalScreens);
    
}

void GloveInteractBox::sendBack(){
   sendToLayer(drawLayer-1);
}

void GloveInteractBox::sendToLayer(int l){
    vector<GloveInteractBox*> vec = GloveInteractBox::allElements;
    if(l> drawLayer){
        for(vector<GloveInteractBox*>::iterator it = vec.begin() ; it!= vec.end() ; it ++){
            if((*it)->drawLayer<=l && (*it)->drawLayer>=drawLayer && (*it)->drawLayer>=screens.totalScreens) (*it)->drawLayer = (*it)->drawLayer  -1;
        }
    }
    else{
        for(vector<GloveInteractBox*>::iterator it = vec.begin() ; it!= vec.end() ; it ++){
            cout << (*it)->drawLayer << endl;
            if((*it)->drawLayer<=drawLayer && (*it)->drawLayer>=l && (*it)->drawLayer>=screens.totalScreens ) (*it)->drawLayer = (*it)->drawLayer  +1;
        }
    }
    drawLayer = l;
}

void GloveInteractBox::drawFrontMask(){
    ofPushStyle();
    
    
    ofFill();
    
    if(isHovered){
        ofRectangle frame;
        ofSetColor(hoverColor);
        frame.setFromCenter(box.getCenter(), box.width, box.height);
        ofDrawRectangle(frame);
        
    }
    
    
    
    
    
    if(isSelected){
        
        
        float lineW = 10;
        ofSetColor(selectedColor);
        ofDrawRectangle(box.getMinX(),box.getMinY(),box.width,lineW);
        ofDrawRectangle(box.getMaxX()-lineW,box.getMinY()+lineW,lineW,box.height-2*lineW);
        ofDrawRectangle(box.getMinX(),box.getMaxY()-lineW,box.width,lineW);
        ofDrawRectangle(box.getMinX(),box.getMinY()+lineW,lineW,box.height-2*lineW);
    }
    
    
    
    
    if(isDragged){
        ofSetColor(255,0,0);
        ofNoFill();
        ofSetLineWidth(box.width/400.0);
        ofDrawCircle(box.getCenter(), box.width/40.0);
        ofDrawLine(box.getCenter() - ofVec2f(box.width/30.0,0), box.getCenter() + ofVec2f(box.width/30.0,0));
        ofDrawLine(box.getCenter() - ofVec2f(0,box.width/30.0), box.getCenter() + ofVec2f(0,box.width/30.0));
    }
    
    ofPopStyle();
    
}



void GloveInteractBox::updateDrag(ofVec2f & v){
    
    if(dragged[curGlove]==this){
        
        ofRectangle newR;
        newR.setFromCenter(v-dragOffset, box.width, box.height);
        makeValid(newR);
        targetBox.set(newR);
        
        
    }
}

void GloveInteractBox::updateZoom(float & z){
    
    if(selected[curGlove]==this ){
        ofRectangle newR;
        newR.setFromCenter(box.getCenter(),box.width*z,box.height* z);
        makeValid(newR);
        targetBox.set(newR);
        
        
    }
}

void GloveInteractBox::makeValid(ofRectangle & newR){
    ofRectangle fullScreen = (*screens.full);
    
    // lock to minimal size
    if(newR.width<MIN_BOX_WIDTH){
        newR.setFromCenter(box.getCenter(), MIN_BOX_WIDTH, newR.height);
    }
    if(newR.height<MIN_BOX_HEIGHT){
        newR.setFromCenter(box.getCenter(), newR.width,MIN_BOX_HEIGHT);
    }
    
    // lock to maximal size
    if(newR.width>fullScreen.width){
        newR.setFromCenter(box.getCenter(), fullScreen.width, newR.height);
    }
    if(newR.height>fullScreen.height){
        newR.setFromCenter(box.getCenter(), newR.width,fullScreen.height);
    }
    
    // check that box center is inside full screen, one pixel offset for easing every rectangle inside function that are strict : rectangles are often related to image so we have a true drawing
    if(!fullScreen.inside(newR.getCenter())){
        ofVec2f center;
        center.x = ofClamp(newR.getCenter().x,fullScreen.getMinX()+1,fullScreen.getMaxX()-1);
        center.y = ofClamp(newR.getCenter().y,fullScreen.getMinY()+1,fullScreen.getMaxY()-1);
        newR.setFromCenter(center,box.width,box.height);
        
    }
    
    
    
}
void GloveInteractBox::updateBox(){
    
    if(targetBox.get()!=box){
        
        //        resolveCollision(targetBox);
        
        isStable = false;
        bool isMoving = (targetBox->getCenter().x != box.getCenter().x || targetBox->getCenter().y != box.getCenter().y);
        
        bool isResizing = (targetBox->width != box.width || targetBox->height != box.height);
        
        
        
        box.setFromCenter(      ofLerp(box.getCenter().x,   targetBox->getCenter().x,   alphaTarget),
                          ofLerp(box.getCenter().y,   targetBox->getCenter().y,   alphaTarget),
                          ofLerp(box.width,           targetBox->width,                            alphaTarget),
                          ofLerp(box.height,          targetBox->height,                           alphaTarget));
        
        
        
        
        // avoid infinitesimal changes
        if(abs(targetBox->x - box.x)<.1 &&
           abs(targetBox->y - box.y)<.1 &&
           abs(targetBox->width - box.width)<.05 &&
           abs(targetBox->height - box.height)<.05){
            box.setFromCenter(targetBox->getCenter(), targetBox->width, targetBox->height);
            
        }
        
        
        
        // Callbacks
        
        if(isResizing)this->boxResized(false);
        if(isMoving)this->boxMoved(false);
        if(isResizing || isMoving) this->boxChanged(false);
    }
    else if(!isStable){
        isStable = true;
        this->boxResized(true);
        this->boxMoved(true);
        this->boxChanged(true);
    }
    
    
}


// find new place for colliding object

void GloveInteractBox::resolveCollision(ofRectangle  newR){
    // check 2d collision
    
    
    if(isCollision && isCollider){
        bool isColliding = false;
        
        
        if(!dragged[curGlove] && lastDragged[curGlove] == this){
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
                //                targetMagnet =_targetMagnet;
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
    if(drawBaseFeedBack){
        ofRectangle tmpB = box;
        ofSetColor(ofColor::black, 100);
        ofDrawRectangle(tmpB+ofVec2f(-5,5));
        ofSetColor(0);
        ofDrawRectangle(tmpB);
    }
    ofSetColor(255);
    this->draw();
    if(drawBaseFeedBack){
        drawFrontMask();
    }
    ofPopStyle();
};


void GloveInteractBox::update(ofEventArgs & e){
    updateBox();
    this->update();
}





bool GloveInteractBox::layerCompare(GloveInteractBox & lhs, GloveInteractBox & rhs){
    return lhs.drawLayer < rhs.drawLayer;
}



