//
//  GloveOSC.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "GloveOSC.h"
#include "Screens.h"




vector<GloveInstance*> GloveOSC::gloves;

ofxOscReceiver GloveOSC::reciever;
ofxOscSender GloveOSC::toServer;




GloveOSC::GloveOSC(){
    reciever.setup(LOCALPORT);
    
    
    toServer.setup(SERVERIP, SERVERPORT);
    lastACK = ofGetElapsedTimef();
    
    
    
}


GloveOSC::~GloveOSC(){
    
    for(  vector<GloveInstance*>::iterator it =gloves.begin() ; it!= gloves.end();++it){
        delete *it;
    }
    
}


void GloveOSC::update(ofEventArgs & a){
    registerOSC();
    parseMessage();
    
}


void GloveOSC::draw(ofEventArgs & a){
    
}


void GloveOSC::registerOSC(){
    if(!isConnectedToServer && ofGetElapsedTimef()-lastACK>1){
        ofxOscMessage regMsg;
        regMsg.setAddress("/register");
        regMsg.addStringArg(APPNAME);
        regMsg.addStringArg(LOCALIP);
        regMsg.addIntArg(LOCALPORT);
        
        toServer.sendMessage(regMsg);
        
        
    }
    
}



void GloveOSC::parseMessage(){
    while(reciever.hasWaitingMessages()){
        ofxOscMessage m;
        reciever.getNextMessage(&m);
        
        string addr = m.getAddress();
        GloveInstance *curGlove = NULL;
        //        cout << m.getArgAsString(0) << endl;
        //from Server
        if((addr.find_first_of("/glove"))==0){
            addr = addr.substr(6,addr.length()-6);
            
            // Glove Registration ACK
            if(addr == "/registered"){
                isConnectedToServer= true;
            }
            else if(addr == "/unregistered"){
                isConnectedToServer = false;
            }
            else if(addr == "/connected"){
                gloves.push_back(new GloveInstance(m.getArgAsString(0)));
                
            }
            else if(addr == "/disconnected"){
                deleteGlove(m.getArgAsString(0));
            }
            else if(addr == "/orientation"){
                if((curGlove = getGlove(m.getArgAsString(0)))){
                    ofVec3f orientation(m.getArgAsFloat(1),m.getArgAsFloat(2),m.getArgAsFloat(3));
                    curGlove->setOrientation ( orientation);
                    
                }
            }
            else if(addr == "/relative"){
                if((curGlove = getGlove(m.getArgAsString(0)))){
                    curGlove->setRelativeOrientation (ofVec3f(m.getArgAsFloat(1),m.getArgAsFloat(2),m.getArgAsFloat(3)));
                    
                    
                }
            }
            else if(addr == "/touch"){
                if((curGlove = getGlove(m.getArgAsString(0)))){
                    curGlove->setTouch((TouchType)m.getArgAsInt32(1),(TouchAction)m.getArgAsInt32(2));

                }
            }
            else if(addr == "/flex"){
                if((curGlove = getGlove(m.getArgAsString(0)))){
                    curGlove->setFlex(m.getArgAsInt32(1), m.getArgAsFloat(2));

                }
            }
            else if(addr == "/cursor2D"){
                if((curGlove = getGlove(m.getArgAsString(0)))){
                    curGlove->setCursor2D ( ofVec2f(m.getArgAsFloat(1),m.getArgAsFloat(2))*Screens::instance()->resolution);
                   

                }
            }
            return;
            
        }
    }
}


GloveInstance* GloveOSC::getGlove(string gloveID){
    
    for(vector<GloveInstance*>::iterator it = gloves.begin();it!=gloves.end();++it){
        if((*it)->gloveID==gloveID)return *it;
        
    }
    return NULL;
}

void GloveOSC::deleteGlove(string gloveID){
    for(vector<GloveInstance*>::iterator it = gloves.begin();it!=gloves.end();++it){
        if((*it)->gloveID==gloveID){
            delete *it;
            gloves.erase(it);
            return;
        }
        
    }
    
}


void GloveOSC::unregisterOSC(){
    ofxOscMessage m;
    m.setAddress("/unregister");
    m.addStringArg(APPNAME);
    toServer.sendMessage(m);
}



