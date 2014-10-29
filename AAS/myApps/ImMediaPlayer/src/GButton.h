//
//  GButton.h
//  ImMedia
//
//  Created by Tinmar on 29/10/2014.
//
//

#ifndef __ImMedia__GButton__
#define __ImMedia__GButton__


#include <stdio.h>
#include "GloveInteract.h"




class GButton : public GloveInteract{
    public :
    
    GButton() {GloveInteract::GloveInteract();isDraggable =false;img.loadImage("UI/screenAnchor.png");};
    ~GButton(){};
    void draw(ofEventArgs & e);
    ofImage img;
    void clicked(Cursor* gId,touchType & state);
    
    
    
    
};

#endif /* defined(__ImMedia__GButton__) */
