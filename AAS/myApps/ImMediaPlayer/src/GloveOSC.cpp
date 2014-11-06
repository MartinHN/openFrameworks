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
                    curGlove->orientation = ofVec3f(m.getArgAsFloat(1),m.getArgAsFloat(2),m.getArgAsFloat(3));
                    pair<GloveInstance*,ofVec3f> a(curGlove,curGlove->orientation);
                    ofNotifyEvent(curGlove->orientationEvent,a ,this);
                }
            }
            else if(addr == "/relative"){
                if((curGlove = getGlove(m.getArgAsString(0)))){
                    curGlove->relativeOrientation = ofVec3f(m.getArgAsFloat(1),m.getArgAsFloat(2),m.getArgAsFloat(3));
                     pair<GloveInstance*,ofVec3f> a(curGlove,curGlove->relativeOrientation);
                    ofNotifyEvent(curGlove->relativeOrientationEvent, a,this);
                    
                }
            }
            else if(addr == "/touch"){
                if((curGlove = getGlove(m.getArgAsString(0)))){
                    int tid =m.getArgAsInt32(1);
                    TouchState state = (TouchState)m.getArgAsInt32(2);
                    curGlove->touchs[tid] = state;
                    touchEventArgs a;
                    a.touchId = (TouchType)tid;
                    a.gloveId = curGlove;
                    a.state = state;
                    ofNotifyEvent(curGlove->touchEvent,a,this);
                }
            }
            else if(addr == "/flex"){
                if((curGlove = getGlove(m.getArgAsString(0)))){
                    curGlove->flex[m.getArgAsInt32(1)] = m.getArgAsFloat(2);
                    pair<GloveInstance*,vector<float> >a(curGlove,curGlove->flex);
                    ofNotifyEvent(curGlove->flexEvent, a,this);
                }
            }
            else if(addr == "/cursor2D"){
                if((curGlove = getGlove(m.getArgAsString(0)))){
                    curGlove->cursor2D = ofVec2f(m.getArgAsFloat(1),m.getArgAsFloat(2));
                    curGlove->cursor2D*=Screens::instance()->resolution;
                    pair<GloveInstance*,ofVec2f>a(curGlove,curGlove->cursor2D);
//                    cout << cursor2D << endl;
                    ofNotifyEvent(curGlove->cursor2DEvent, a,this);
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



