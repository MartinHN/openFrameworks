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
    
    virtual void touch( TouchType num, TouchState s,GloveInstance* gid){
        ofMouseEventArgs a;
//        a.x=
        if(num==GLOVE_CLICK){
            if(s == GLOVE_DOWN){
                ofNotifyEvent(ofEvents().mousePressed, a, this );
            }
            else if(s == GLOVE_UP){
                ofNotifyEvent(ofEvents().mouseReleased, a, this );
            }
        }
        
        
    };
    virtual void relativeMoved(ofVec3f v, GloveInstance* gid){};
    virtual void cursor2DMoved(ofVec2f v, GloveInstance* gid){};
    
    virtual void update(){};
    
    
};




#endif
