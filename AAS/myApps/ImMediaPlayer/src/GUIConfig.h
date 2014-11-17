//
//  GUIConfig.h
//  ImMedia
//
//  Created by martin hermant on 17/11/14.
//
//

#ifndef __ImMedia__GUIConfig__
#define __ImMedia__GUIConfig__
#include "GloveInteractBox.h"
#include "ofxUI.h"
#include "Savable.h"

class GUIConfig : public GloveInteractBox{
    
public:
    GUIConfig();
    ~GUIConfig();
    
    
    
    ofxUISuperCanvas* canvas;
    
    ofxUIButton* saveButton;
    
    
    
    ofRectangle fullSizeRect;
    
    
    void init();
    void draw(ofEventArgs & a);
    void boxMoved(bool stable);
    void entered();
    void exited();
    void GUIEvent(ofxUIEventArgs & a);
    
};

#endif /* defined(__ImMedia__GUIConfig__) */
