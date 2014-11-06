//
//  GloveInteract.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//




// Base class for handling glove events



#ifndef __ImMedia__GloveInteract__
#define __ImMedia__GloveInteract__

#include <stdio.h>

#include "GloveInstance.h"





class GloveInteract  {
  

public:
    
    GloveInteract();
    ~GloveInteract();
    
    static ofVec2f* cursor2D;
    static ofVec3f* relative;
    static ofVec3f* orientation;
    static vector<TouchState>* touchs;
    

  // from Glove events
    virtual void touch(touchEventArgs & a){
        setcurrentData(a.gloveId);
        touch(a.touchId,a.state,a.gloveId);
    };
    virtual void relativeMoved(pair<GloveInstance*,ofVec3f> & pos){
        setcurrentData(pos.first);
        relativeMoved(pos.second, pos.first);
    };
    virtual void cursor2DMoved(const void* ee,pair<GloveInstance*,ofVec2f> & pos){
        setcurrentData(pos.first);
        cursor2DMoved(pos.second, pos.first);
    };
    virtual void update(const void * ee,ofEventArgs & e){
        update();
    };
    
    
    // overriden in sub class
    virtual void touch( TouchType num, TouchState s,GloveInstance* gid){};
    virtual void relativeMoved(ofVec3f v, GloveInstance* gid){};
    virtual void cursor2DMoved(ofVec2f v, GloveInstance* gid){};
    
    virtual void update(){};
    
    
    void setcurrentData(GloveInstance* gid);


};



#endif /* defined(__ImMedia__Draggable__) */
