//
//  GUIGloveWrapper.h
//  ImMedia
//
//  Created by Tinmar on 30/10/2014.
//
//

// class thats wraps glove events into standards mouse events for using any interactive openframeworks functionality


#ifndef ImMedia_GloveWrapper_h
#define ImMedia_GloveWrapper_h
#include "ofMain.h"


#include "GloveInteract.h"




class GUIGloveWrapper : GloveInteract {
public:
    GUIGloveWrapper(){};
    ~GUIGloveWrapper(){};
    
    virtual void touch( TouchButton num, TouchAction s){
        ofTouchEventArgs a;
        int  idTouch = 0 ;
        for(int i = 0 ; i < GloveOSC::gloves.size() ; i ++){
            if(GloveOSC::gloves[i] == curGlove)idTouch = i*13 +  num;
        };
        a.id = idTouch;
        a.set(curGlove->cursor2D);
        
        if(num==GLOVE_BUTTON_CLICK || num == GLOVE_BUTTON_DRAG){
            if(s == GLOVE_ACTION_DOWN){
                a.type = ofTouchEventArgs::down;
                ofNotifyEvent(ofEvents().touchDown, a );
            }
            else if(s == GLOVE_ACTION_UP){
                a.type = ofTouchEventArgs::up;
                ofNotifyEvent(ofEvents().touchUp, a);
            }
        }
        
        
    };
    virtual void relativeMoved(ofVec3f v){
    };
    virtual void cursor2DMoved(ofVec2f v){
        ofTouchEventArgs a;
        int  idTouch = -1 ;
        for(int i = 0 ; i < GloveOSC::gloves.size() ; i ++){
            if(GloveOSC::gloves[i] == curGlove){
                int tid = 0;
                for(int j = 0 ; j<curGlove->touchs.size() ; j++){
                    if(curGlove->touchs[j]){
                     tid = j;
                    }
                }
                idTouch=i*13+tid;
                if(tid>0){
                    int ttt=0;
                }
                break;
            }
        };
        a.id = idTouch;
        a.set(v);
        
        ofNotifyEvent(ofEvents().touchMoved,a);
        
        
        
    };
    
    virtual void update(){};
    
    
};




#endif
