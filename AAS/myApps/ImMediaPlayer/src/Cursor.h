//
//  Cursor.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#ifndef __ImMedia__Cursor__
#define __ImMedia__Cursor__

#include <stdio.h>

#include "ofxOsc.h"
#include "ofMain.h"

#include "Config.h"




class Cursor;



class touchEventArgs : public ofEventArgs{
public:
    Cursor* gloveId;
    TouchAction touchId;
    touchType type;
};




class Cursor {
public:
    static vector<Cursor*> cursors;    
    Cursor();
    ~Cursor();
    ofImage cursorImg;
    void update(ofEventArgs & a);
    void draw(ofEventArgs & a);
    bool isConnectedToServer;
    string gloveID;
    
    
    
    //GloveState
    
    ofVec3f orientation;
    ofVec3f relativeOrientation;
    vector<touchType>  touch;
    vector<float>  flex;
    ofVec2f cursor2D;
    
    ofEvent<std::pair<Cursor* ,ofVec3f> > orientationEvent;
    ofEvent<std::pair<Cursor* ,ofVec3f> > relativeOrientationEvent;
    ofEvent<touchEventArgs > touchEvent;
    ofEvent<std::pair<Cursor* , vector < float> > > flexEvent;
    ofEvent<std::pair<Cursor* ,ofVec2f> > cursor2DEvent;
    


    
    

    
private:
    ofxOscReceiver reciever;
    void parseMessage();
    ofxOscSender toServer;
    float lastACK;
    void registerOSC();
    void unregisterOSC();
    
    void updateOrientations();
    void updateRelativeOrientations();
    
};
#endif /* defined(__ImMedia__Cursor__) */
