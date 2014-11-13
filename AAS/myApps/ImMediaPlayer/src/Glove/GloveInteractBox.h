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
    
    // box characteristics
    string name;
    
    bool isCollider = false;

    
    float alphaTarget = 0.2;
    ofParameter<int> drawLayer = 0;
    float format = 1;
    bool isDraggable=true;
    bool isZoomable = true;
    bool isSelectable = true;
    
    
    
    
    // box state
    
    bool isHovered;
    bool isSelected;
    ofRectangle box;
    ofRectangle targetBox;
    
    
    
    // from GloveInteract used to trigger box functions

    
    virtual void touch( TouchButton num, TouchAction s);
    virtual void relativeMoved( ofVec3f v);
    virtual void cursor2DMoved( ofVec2f v );

    
    // triggered functions
    //overriden in inherited class
    
    virtual bool isHit(ofVec2f & p){return box.inside(p);}
    virtual void hover(ofVec2f & p){};
    virtual void clicked(TouchButton & button){};
    virtual void boxResized(){};
    virtual void boxMoved(){};
    
    virtual void exited(){};
    virtual void entered(){};
    
    virtual void draw(){};
    virtual void update(){};

    

    
    static ofRectangle getFreeSpace();
    
    
    
protected:
    
    static int frontDragLayer;
    
    ofVec2f dragOffset;
    ofVec2f targetMagnet;
    
    
    
    // common intern handling methods
    
    virtual void update(ofEventArgs & e);
    virtual void draw(ofEventArgs & e);
    
    
    // automatic draw hover and selected masks
    void drawFrontMask();
    
    void updateBox();
    void makeValid(ofRectangle & newR);
    void resolveCollision(ofRectangle newR);
    void updateDrag(ofVec2f & c);
    void updateZoom(float & c);
    void setDrawLayer(int & l);
    
    
    // layerize drawing events AND touch events so that foreground objects are notified before others
    // each Layer contain one component so that drawing order is representing exact (inverse) order of events reception
    void sendForeground();
    void sendBackground();
    void sendBack();
    
    
    // hack to see in the future when sequentially triggering event...
//    bool amIFirstLayer(ofVec2f & p);
    
    
    static bool layerCompare(GloveInteractBox & lhs, GloveInteractBox & rhs);
    
    

    
};



#endif /* defined(__ImMedia__Draggable__) */
