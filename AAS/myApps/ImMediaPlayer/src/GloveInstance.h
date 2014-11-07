//
//  GloveInstance.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//
// Class for handling glove connection with server and sending events in oF




#ifndef __ImMedia__GloveInstance__
#define __ImMedia__GloveInstance__

#include <stdio.h>


#include "ofMain.h"

#include "Config.h"
#include "GloveOSC.h"

class touchEventArgs;

class GloveInstance {
public:

//    GloveInstance();
    GloveInstance(string _gloveID);
    ~GloveInstance();
    
    
    string gloveID;
    
    
    
    
    
    void update(ofEventArgs & a);
    void draw(ofEventArgs & a);
    
    
    
    
    //GloveState
    
    ofVec3f orientation;
    ofVec3f relativeOrientation;
    vector<bool>  touchs;
    vector<float>  flex;
    ofVec2f cursor2D;
    
    

    ofImage cursorImg;
    

    
};





#endif /* defined(__ImMedia__GloveInstance__) */
