//
//  GloveOSC.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//
// Class for handling glove connection with server and sending events in oF




#ifndef __ImMedia__GloveOSC__
#define __ImMedia__GloveOSC__

#include <stdio.h>

#include "ofxOsc.h"
#include "ofMain.h"

#include "Config.h"

#include "GloveInstance.h"



class GloveInstance;

class touchEventArgs : public ofEventArgs{
public:
    TouchType touchId;
    TouchAction state;
};




class GloveOSC {
public:
    static vector<GloveInstance*> gloves;
    static GloveInstance * getGlove(string gloveID);
    static void deleteGlove(string gloveID);
    
    
    GloveOSC();
    ~GloveOSC();
    
    
    
    void update(ofEventArgs & a);
    void draw(ofEventArgs & a);
    
    
    
    bool isConnectedToServer;
    void registerOSC();
    void unregisterOSC();
    
    static ofEvent<ofVec3f > orientationEvent;
    static ofEvent<ofVec3f > relativeOrientationEvent;
    static ofEvent<touchEventArgs > touchEvent;
    static ofEvent< vector < float> > flexEvent;
    static ofEvent<ofVec2f > cursor2DEvent;
    
    
private:
    static ofxOscReceiver reciever;
    void parseMessage();
    static ofxOscSender toServer;
    float lastACK;
    
};





#endif /* defined(__ImMedia__GloveOSC__) */
