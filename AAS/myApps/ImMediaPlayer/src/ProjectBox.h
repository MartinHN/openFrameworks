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

    ProjectBox() {isDraggable =false;};
    ~ProjectBox(){};
    void draw(ofEventArgs & e);
    
    void clicked(Cursor* gId,touchType & state);
    
   
    
    
};

#endif /* defined(__ImMedia__ProjectBox__) */
