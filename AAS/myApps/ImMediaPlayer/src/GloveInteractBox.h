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
    
    float format = 1;
    
    
    
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
    
    virtual void touch( TouchType num, TouchAction s);
    virtual void relativeMoved( ofVec3f v);
    virtual void cursor2DMoved( ofVec2f v );
    

    
    static ofRectangle getFreeSpace();
    static vector<GloveInteractBox* > allElements;
    void makeValid(ofRectangle & newR);
    
    
    
    // triggered functions
    
    virtual bool isHit(ofVec2f & p){return box.inside(p);}
    virtual void hover(ofVec2f & p){};
    virtual void clicked(TouchAction & state){};
    virtual void resize(int x,int y){};

    virtual void exited(){};
    virtual void entered(){};
    
    
    virtual void update(ofEventArgs & e){
        this->update();
    }
    virtual void draw(ofEventArgs & e){
        this->draw();
        drawFrontMask();
    };
    
    virtual void draw(){};
    virtual void update(){};
    
    
    // automatic draw hover and selected masks
    void drawFrontMask();
    
protected:
    bool isHovered;
    
    



};



#endif /* defined(__ImMedia__Draggable__) */
