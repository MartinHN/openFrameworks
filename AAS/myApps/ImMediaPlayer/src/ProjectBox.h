//
//  ProjectBox.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#ifndef __ImMedia__ProjectBox__
#define __ImMedia__ProjectBox__

#include <stdio.h>
#include "GloveInteract.h"
#include "MediaPool.h"
#include "ofxUIToggle.h"


class ProjectBox : public GloveInteract,public ofxUIToggle{
    public :
    
    ProjectBox(GloveInteract * root,ofVec2f offset) {
        
        parent = root;
        
        box.set(parent->box.x+offset.x,parent->box.y+offset.y,PROJECTWIDTH,PROJECTBHEIGHT);
        GloveInteract::isDraggable =false;
        ofxUIToggle::ofxUIToggle();
    };
   
    
    ~ProjectBox(){
        

    };
    void draw(ofEventArgs & e);
    
    void clicked(Cursor* gId,touchType & state);
    

    
    
    GloveInteract * parent;
    ofVec2f offset;
    
};

#endif /* defined(__ImMedia__ProjectBox__) */
