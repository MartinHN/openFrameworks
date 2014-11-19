//
//  GloveOSC.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//
// Class for handling glove connections with server and sending events in oF




#ifndef __ImMedia__GloveOSC__
#define __ImMedia__GloveOSC__

#include <stdio.h>

#include "ofxOsc.h"
#include "ofMain.h"
#include "Config.h"
#include "GloveInstance.h"





class GloveOSC {
public:
    static vector<GloveInstance*> gloves;
    static GloveInstance * getGlove(string gloveID);
    static void deleteGlove(string gloveID);
    static void deleteAll();
    
    GloveOSC();
    ~GloveOSC();
    
    
    
    void update(ofEventArgs & a);
    void draw(ofEventArgs & a);
    
    
    
    ofParameter<bool> isConnectedToServer;
    void registerOSC();
    void unregisterOSC();
    bool isGlove(string gloveID);

    
    
private:
    static ofxOscReceiver reciever;
    void parseMessage();
    float lastPingTime;
    static ofxOscSender toServer;
    float lastACK;
    
    void setConnected(bool & b);
    
};





#endif /* defined(__ImMedia__GloveOSC__) */
