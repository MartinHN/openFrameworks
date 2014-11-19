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
    
    static map<GloveInstance*,GloveInteractBox*>  dragged,lastDragged;
//    static map<GloveInstance*,GloveInteractBox*>  zoomed;
    static map<GloveInstance*,GloveInteractBox*>  selected;
    static map<GloveInstance*,GloveInteractBox*> hovered;
    static ofColor hoverColor;
    static ofColor selectedColor;
    
    
    static vector<GloveInteractBox* > allElements;
    
    // box characteristics
    string name;
    
    bool isCollider = false;
    
    
    float alphaTarget = 0.2;
    ofParameter<int> drawLayer = 0;
    
    bool isDraggable=true;
    bool isZoomable = true;
    bool isSelectable = true;
   
    bool drawBaseFeedBack = true;
    
    
    
    // box state
    
    bool isHovered;
    bool isSelected;
    bool isDragged;
    ofRectangle box;
    ofParameter<ofRectangle> targetBox;
    // not moving anymore (last move event sent)
    bool isStable;
    // was selected before this click (allow different reaction for same button after selection, playing Video for example)
    bool wasSelected;
    
    
    
    // from GloveInteract used to trigger box functions
    
    
    virtual void touch(TouchButton num, TouchAction s);
    virtual void relativeMoved( ofVec3f v);
    virtual void cursor2DMoved( ofVec2f v );
    
    
    // triggered functions
    //overriden in inherited class
    
    virtual bool isHit(ofVec2f & p){return box.inside(p);}
    virtual void hover(ofVec2f & p){};
    virtual void clicked(TouchButton & button){};
    virtual void boxResized(bool stable){};
    virtual void boxMoved(bool stable){};
    virtual void boxChanged(bool stable){};
    
    virtual void exited(){};
    virtual void entered(){};
    
    virtual void draw(){};
    virtual void update(){};
    
    
    
    
    static ofRectangle getFreeSpace();
    
    
    // common intern handling methods
    
    virtual void update(ofEventArgs & e);
    virtual void draw(ofEventArgs & e);
    
    
    // layerize drawing events AND touch events so that foreground objects are notified before others
    // each Layer contain one component so that drawing order is representing exact (inverse) order of events reception
    void sendForeground();
    void sendBackground();
    void sendBack();
    void sendToLayer(int l);
    
protected:
    

    
    ofVec2f dragOffset;

    
    
    
    
    
    
    
    // automatic draw hover and selected masks
    void drawFrontMask();
    
    void updateBox();
    void makeValid(ofRectangle & newR);
    void resolveCollision(ofRectangle newR);
    void updateDrag(ofVec2f & c);
    void updateZoom(float & c);
    void setDrawLayer(int & l);
    
    

    
    
    // hack to see in the future when sequentially triggering event...
    //    bool amIFirstLayer(ofVec2f & p);
    
    
    static bool layerCompare(GloveInteractBox & lhs, GloveInteractBox & rhs);
    
    
    
    
};



#endif /* defined(__ImMedia__Draggable__) */
