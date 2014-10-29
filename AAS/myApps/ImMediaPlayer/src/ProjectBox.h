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



class ProjectBox : public GloveInteract{
    public :
    
    ProjectBox(GloveInteract * root,ofVec2f offset) {
        
        parent = root;
        ofAddListener(parent->hasMovedEvent, this,&ProjectBox::parentHasMoved);
        box.set(parent->box.x+offset.x,parent->box.y+offset.y,PROJECTWIDTH,PROJECTBHEIGHT);
        isDraggable =false;};
    ~ProjectBox(){
        ofRemoveListener(parent->hasMovedEvent,this,&ProjectBox::parentHasMoved);

    };
    void draw(ofEventArgs & e);
    
    void clicked(Cursor* gId,touchType & state);
    
    void parentHasMoved(GloveInteract  &p);
    
    
    GloveInteract * parent;
    ofVec2f offset;
    
};

#endif /* defined(__ImMedia__ProjectBox__) */
