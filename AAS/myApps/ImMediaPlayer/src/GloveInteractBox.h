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

  // from Glove
    virtual void click(touchEventArgs & a);
//    void moved(GloveOSC* c,ofVec3f & pos);
    virtual void relativeMoved(pair<GloveInstance*,ofVec3f> & pos);
    virtual void cursor2DMoved(pair<GloveInstance*,ofVec2f> & arg);
    
    virtual void update(ofEventArgs & e);
    virtual void draw(ofEventArgs & e);

    bool isCollider = false;
    
    ofEvent< GloveInteractBox > hasMovedEvent;
    
    
    ofRectangle box;
    ofColor backColor;
//    string name;
    

    static ofRectangle getFreeSpace();
    
    bool isgDraggable=true;
    static GloveInteractBox * dragged;
    void updateDrag(ofVec2f & c);
    ofVec2f dragOffset;
    
    
    bool isZoomable = true;
    static GloveInteractBox * zoomed;
    void updateZoom(float & c);
    
    bool isSelectable = true;
    static GloveInteractBox* selected;
    
    
    static vector<GloveInteractBox* > allElements;
    
    // triggered functions
    
    virtual bool isHit(ofVec2f & p){return box.inside(p);}
    virtual void hover(ofVec2f & p){};
    virtual void clicked(GloveInstance* gId,TouchState & state){};
    

    virtual void exited(){};
    virtual void entered(){};
    
//    bool operator==(const string& other) {return name == other;};
    
    bool isValid(ofRectangle & newR);
    
    
    string name;
protected:
    bool isHovered;
    
    



};



#endif /* defined(__ImMedia__Draggable__) */
