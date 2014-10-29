//
//  Draggable.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#ifndef __ImMedia__Draggable__
#define __ImMedia__Draggable__

#include <stdio.h>
#include "ofMain.h"
#include "Cursor.h"





class GloveInteract  {
  

public:
    
    GloveInteract();
    ~GloveInteract();

  // from Glove
    void click(touchEventArgs & a);
//    void moved(Cursor* c,ofVec3f & pos);
    void relativeMoved(pair<Cursor*,ofVec3f> & pos);
    void cursor2DMoved(pair<Cursor*,ofVec2f> & arg);
    
    virtual void update(ofEventArgs & e);
    virtual void draw(ofEventArgs & e);

    bool isCollider = false;
    
    ofEvent< GloveInteract > hasMovedEvent;
    
    
    ofRectangle box;
    ofColor backColor;
    string name;
    

    static ofRectangle getFreeSpace();
    
    bool isDraggable=true;
    static GloveInteract * dragged;
    void updateDrag(ofVec2f & c);
    ofVec2f dragOffset;
    
    
    bool isZoomable = true;
    static GloveInteract * zoomed;
    void updateZoom(float & c);
    
    bool isSelectable = true;
    static GloveInteract* selected;
    
    
    static vector<GloveInteract* > allElements;
    
    // triggered functions
    
    virtual bool isHit(ofVec2f & p){return box.inside(p);}
    virtual void hover(ofVec2f & p){};
    virtual void clicked(Cursor* gId,touchType & state){};
    

    virtual void exited(){};
    virtual void entered(){};
    
    bool operator==(const string& other) {return name == other;};
    
    bool isValid(ofRectangle & newR);
    
protected:
    bool isHovered;
    
    



};



#endif /* defined(__ImMedia__Draggable__) */
