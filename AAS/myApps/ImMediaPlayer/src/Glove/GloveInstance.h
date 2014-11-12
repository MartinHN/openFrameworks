//
//  GloveInstance.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//
// Class representing a glove instance




#ifndef __ImMedia__GloveInstance__
#define __ImMedia__GloveInstance__

#include <stdio.h>


#include "ofMain.h"

#include "Config.h"


class touchEventArgs : public ofEventArgs{
public:
    TouchButton touchId;
    TouchAction state;
};


class GloveInstance {
public:

//    GloveInstance();
    GloveInstance(string _gloveID);
    ~GloveInstance();
    
    
    string gloveID;
    
    static ofEvent<ofVec3f > orientationEvent;
    static ofEvent<ofVec3f > relativeOrientationEvent;
    static ofEvent<touchEventArgs > touchEvent;
    static ofEvent< vector < float> > flexEvent;
    static ofEvent<ofVec2f > cursor2DEvent;
    
    
    
    void update(ofEventArgs & a);
    void draw(ofEventArgs & a);
    
    
    
    
    //GloveState
    
    ofVec3f orientation;
    ofVec3f relativeOrientation;
    vector<bool>  touchs;
    vector<float>  flex;
    ofVec2f cursor2D;
    
    
    ofVec3f orientationVel;
    ofVec3f relativeOrientationVel;
    ofVec2f cursor2DVel;
    
    
    void setOrientation(ofVec3f o);
    void setRelativeOrientation(ofVec3f o);
    void setTouch(TouchButton t,TouchAction a);
    void setFlex(int n,float f);
    void setCursor2D(ofVec2f c);

    ofImage cursorImg;
    

    
};





#endif /* defined(__ImMedia__GloveInstance__) */
