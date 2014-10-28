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
//#include "IGloveListener.h"
#include "Cursor.h"





class GloveInteract : public  IGloveListener {
  

public:
    
    GloveInteract();
    ~GloveInteract();
  
  // from Glove
    void click(touchEventArgs & a);
//    void moved(Cursor* c,ofVec3f & pos);
//    void relativeMoved(Cursor* c,ofVec3f & pos);
    void cursor2DMoved(pair<Cursor*,ofVec2f> & arg);
    
    void update(ofEventArgs & e);
    void draw(ofEventArgs & e);
    void draw();
    
    
    ofRectangle box;
    ofColor backColor;
    bool isDraggable;
    
    
    // triggered functions
    
    virtual bool isHit(ofVec2f & p){box.inside(p);}
    virtual void hover(ofVec2f & p){};
    virtual void clicked(Cursor* gId,int & tid,touchType & state){};
    virtual void exited(){};
    virtual void entered(){};
    
    
protected:
    bool isHovered;
    
    



};


#endif /* defined(__ImMedia__Draggable__) */
