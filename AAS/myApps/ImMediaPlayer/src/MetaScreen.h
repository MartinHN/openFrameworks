//
//  MetaScreen.h
//  ImMedia
//
//  Created by martin hermant on 11/11/14.
//
//
// class for handling meta screen : possible full screens displays in setup
// they contains anchor reacting to drop Media object.


#ifndef __ImMedia__MetaScreen__
#define __ImMedia__MetaScreen__

#include "GloveInteractBox.h"
#include "ofMain.h"

extern ofEvent<ofEventArgs> drawSyphonEvent;

class MetaScreen : public ofRectangle{
    
    public :
    MetaScreen(){};
    ~MetaScreen();
    MetaScreen(ofRectangle r,bool anchorUp = false,const string & _name = "defaultName",ofVec2f offset = ofVec2f(0));
    
    
    
    ofRectangle fullScreenAnchor;
    ofImage image;
    
    
    GloveInteractBox* fullScreened;
    
    bool isHovering;
    
    void update(ofEventArgs & a);
    void draw(ofEventArgs & a);
    
    
    ofRectangle rectScreen(){
        return *((ofRectangle*)this);
    }
    int defaultLayer;
    
    
};

#endif /* defined(__ImMedia__MetaScreen__) */
