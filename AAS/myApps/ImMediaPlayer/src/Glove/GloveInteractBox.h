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
    
    //static members
    
    static bool isCollision;
    
    static GloveInteractBox * dragged,*lastDragged;
    static GloveInteractBox * zoomed;
    static GloveInteractBox* selected;
    static ofColor hoverColor;
    static ofColor selectedColor;
    
    
    static vector<GloveInteractBox* > allElements;
    
    // box state
    
    string name;
    
    bool isCollider = false;
    ofRectangle box;
    ofRectangle drawBox;
    ofRectangle targetBox;
    ofVec2f targetMagnet;
    float alphaTarget = 0.2;
    ofParameter<int> drawLayer = 0;
    float format = 1;
    bool isDraggable=true;
    bool isZoomable = true;
    void updateZoom(float & c);
    bool isSelectable = true;
    
    
    
    
    // from GloveInteract used to trigger box functions
    
    virtual void touch( TouchType num, TouchAction s);
    virtual void relativeMoved( ofVec3f v);
    virtual void cursor2DMoved( ofVec2f v );
    
    
    
    
    
    
    
    
    
    // triggered functions
    //overriden in inherited class
    
    virtual bool isHit(ofVec2f & p){return box.inside(p);}
    virtual void hover(ofVec2f & p){};
    virtual void clicked(TouchAction & state){};
    virtual void resize(int x,int y){};
    
    virtual void exited(){};
    virtual void entered(){};
    
    virtual void draw(){};
    virtual void update(){};

    
    void updateDrawBox();
    
    static ofRectangle getFreeSpace();
    
    
    
protected:
    bool isHovered;
    ofVec2f dragOffset;
    
    
    // common intern handling methods
    
    virtual void update(ofEventArgs & e);
    virtual void draw(ofEventArgs & e);
    
    
    // automatic draw hover and selected masks
    void drawFrontMask();
    
    void smooth();
    void makeValid(ofRectangle & newR);
    void resolveCollision(ofRectangle newR);
    void updateDrag(ofVec2f & c);
    void setDrawLayer(int & l);
    
    
};



#endif /* defined(__ImMedia__Draggable__) */
