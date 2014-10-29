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
    
    GButton();
    ~GButton(){};
    GButton(ofVec2f center, ofVec2f size,const string & impath,const string & name);
    void draw(ofEventArgs & e);
    ofImage image;
    void clicked(Cursor* gId,touchType & state);

    
    
    
};

#endif /* defined(__ImMedia__GButton__) */
