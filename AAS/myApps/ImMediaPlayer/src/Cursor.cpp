//
//  Cursor.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "Cursor.h"
#include "Screens.h"


vector<Cursor*> Cursor::cursors;


Cursor::Cursor(){
    reciever.setup(LOCALPORT);
    
    
    toServer.setup("127.0.0.1", SERVERPORT);
    lastACK = ofGetElapsedTimef();

    
    touch.reserve(NUMTOUCH);
    flex.reserve(NUMFLEX);
    
    ofAddListener(ofEvents().update, this, &Cursor::update);
    ofAddListener(ofEvents().draw, this, &Cursor::update);
    
    cursors.push_back(this);
    
    
}


Cursor::~Cursor(){
    cursors.erase(find(cursors.begin(), cursors.end(), this));
}


void Cursor::update(ofEventArgs & a){
    registerOSC();
    parseMessage();

}


void Cursor::draw(ofEventArgs & a){
    cursorImg.draw(cursor2D);
}


void Cursor::registerOSC(){
    if(!isConnectedToServer && ofGetElapsedTimef()-lastACK>1){
        ofxOscMessage regMsg;
        regMsg.setAddress("/register");
        regMsg.addStringArg(APPNAME);
        regMsg.addStringArg("127.0.0.1");
        regMsg.addIntArg(LOCALPORT);
        
        toServer.sendMessage(regMsg);
        

    }

}



void Cursor::parseMessage(){
    while(reciever.hasWaitingMessages()){
        ofxOscMessage m;
        reciever.getNextMessage(&m);
        
        string addr = m.getAddress();
        
        
        //from Server
        if((addr.find_first_of("/glove"))==0){
            addr = addr.substr(6,addr.length()-7);
            
            // Glove Registration ACK
            if(addr == "/registered"){
                isConnectedToServer= true;
            }
            else if(addr == "/unregistered"){
                isConnectedToServer = false;
            }
            else if(addr == "/connected"){
                gloveID = m.getArgAsString(0);
            }
            else if(addr == "/disconnected"){
                gloveID ="" ;
            }
            else if(addr == "/orientation"){
                if(gloveID == m.getArgAsString(0)){
                    orientation = ofVec3f(m.getArgAsFloat(1),m.getArgAsFloat(2),m.getArgAsFloat(3));
                    pair<Cursor*,ofVec3f> a(this,orientation);
                    ofNotifyEvent(orientationEvent,a ,this);
                }
            }
            else if(addr == "/relative"){
                if(gloveID == m.getArgAsString(0)){
                    const ofVec3f _relativeOrientation = ofVec3f(m.getArgAsFloat(1),m.getArgAsFloat(2),m.getArgAsFloat(3));
                     pair<Cursor*,ofVec3f> a(this,relativeOrientation);
                    ofNotifyEvent(relativeOrientationEvent, a,this);
                    
                }
            }
            else if(addr == "/touch"){
                if(gloveID == m.getArgAsString(0)){
                    int tid =m.getArgAsInt32(1);
                    touchType type = (touchType)m.getArgAsInt32(2);
                    touch[tid] = type;
                    
                    touchEventArgs a;
                    a.touchId = (TouchAction)tid;
                    a.gloveId = this;
                    a.type = type;
                    ofNotifyEvent(touchEvent,a,this);
                }
            }
            else if(addr == "/flex"){
                if(gloveID == m.getArgAsString(0)){
                    flex[m.getArgAsInt32(1)] = m.getArgAsFloat(2);
                    pair<Cursor*,vector<float> >a(this,flex);
                    ofNotifyEvent(flexEvent, a,this);
                }
            }
            else if(addr == "/cursor2D"){
                if(gloveID == m.getArgAsString(0)){
                    cursor2D = ofVec2f(m.getArgAsFloat(1),m.getArgAsFloat(2));
                    cursor2D*=Screens::instance()->resolution;
                    pair<Cursor*,ofVec2f>a(this,cursor2D);
                    ofNotifyEvent(cursor2DEvent, a,this);
                }
            }
            return;
        
    }
    
        
    }
    
    
    
}



void Cursor::unregisterOSC(){
    ofxOscMessage m;
    m.setAddress("/unregister");
    m.addStringArg(APPNAME);
    toServer.sendMessage(m);
}



