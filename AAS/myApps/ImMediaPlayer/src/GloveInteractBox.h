//
//  Draggable.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//
// class for handling interactive rectangles with respect to the glove 

#ifndef __ImMedia__GloveInteractBox__
#define __ImMedia__GloveInteractBox__

#include <stdio.h>

#include "GloveInteract.h"





class GloveInteractBox: public GloveInteract  {
  

public:
    
    GloveInteractBox();
    ~GloveInteractBox();

    // box state
    
    string name;
    
    bool isCollider = false;
    ofRectangle box;
    ofColor backColor;
    
    
    
    
    
    bool isDraggable=true;
    static GloveInteractBox * dragged;
    void updateDrag(ofVec2f & c);
    ofVec2f dragOffset;
    
    
    bool isZoomable = true;
    static GloveInteractBox * zoomed;
    void updateZoom(float & c);
    
    bool isSelectable = true;
    static GloveInteractBox* selected;
    
    
    
  // from GloveInteract used to trigger box functions
    virtual void touch(touchEventArgs & a);
    virtual void relativeMoved(ofVec3f & pos);
    virtual void cursor2DMoved(ofVec2f & arg);
    
    virtual void update(ofEventArgs & e);
    virtual void draw(ofEventArgs & e);

    
    

    
    static ofRectangle getFreeSpace();
    static vector<GloveInteractBox* > allElements;
    bool isValid(ofRectangle & newR);
    
    
    
    // triggered functions
    
    virtual bool isHit(ofVec2f & p){return box.inside(p);}
    virtual void hover(ofVec2f & p){};
    virtual void clicked(TouchAction & state){};
    

    virtual void exited(){};
    virtual void entered(){};
    
    
    
    
    
protected:
    bool isHovered;
    
    



};



#endif /* defined(__ImMedia__Draggable__) */
