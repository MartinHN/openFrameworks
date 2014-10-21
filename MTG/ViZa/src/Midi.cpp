//
//  Midi.cpp
//  ViZa
//
//  Created by martin hermant on 21/10/14.
//
//

#include "Midi.h"

#define MAX_MIDI_QUEUE 60

Midi * Midi::midi;
ofxMidiIn Midi::midiIn;
map<int,ofPoint> Midi::curpoints;
int Midi::midiModulo=12;
int Midi::midiRoot=24;
int Midi::midiMax = 60;
float Midi::radius = .05;
ofVec2f Midi::velScale(0,1);

bool Midi::isReading=false;
swaplist Midi::msg;
map<int,Container*> Midi::curCont;




vector<string> Midi::getPorts(){
    midiIn.listPorts();
    midiIn.openPort("USB2.0-MIDI Port 1");
    return midiIn.getPortList();

}

void Midi::newMidiMessage(ofxMidiMessage& msgin){
    if(!isReading){
    if(msg.back->size()<MAX_MIDI_QUEUE){
        msg.back->push_back(msgin);

    }
    }
    
    else {
        cout<< "dropping MIDI" << endl;
        
    }

   
}

void Midi::update(){
    isReading = true;
    msg.swap();
    isReading = false;
//    for(vector<ofxMidiMessage>::iterator it = msg.front->begin() ; it!=msg.front->end() ; ++it){
//        for(vector<ofxMidiMessage>::iterator itt = it+1 ; itt!=msg.front->end() ; ++itt){
//            if(( it->status == MIDI_NOTE_OFF || it->status == MIDI_NOTE_ON) && it->pitch == itt->pitch && it->status != itt->status ){
//                msg.front->erase(it++);
//                msg.front->erase(itt);
//                break;
//            }
//        }
//    }
    
    for(vector<ofxMidiMessage>::iterator it = msg.front->begin() ; it!=msg.front->end() ; ++it){
    
        if(it->status==MIDI_NOTE_ON){
            ofVec3f v (((it->pitch-midiRoot)%midiModulo)*1.0/(midiModulo) ,(int)((it->pitch-midiRoot)/midiModulo)*1.0/((midiMax-midiRoot)/midiModulo), ofMap(it->velocity, 0, 127, velScale.x, velScale.y));
            v-= ofVec3f(.5);
            curpoints[it->pitch] = v;
            Container* cc =Physics::Nearest(v,radius);
            if(cc!=NULL && cc->state==0){
                cc->state = 1;
                curCont[it->pitch]=cc;
            }
            
        }
        else if(it->status==MIDI_NOTE_OFF){
            curpoints.erase(it->pitch);
            if(curCont[it->pitch]!=NULL){
                curCont[it->pitch]->state=0;
            curCont.erase(it->pitch);
            }
        }
    
    }
    


    msg.front->clear();
    
}


void Midi::draw(){
    ofPushMatrix();
    ofPushStyle();
//    ofVec3f r = ofApp::cam.getOrientationEuler();
    if(radius>0){
    ofColor c = ofColor::seaGreen;
    c.a = 100;
    ofSetColor(c);
    for(map<int,ofVec3f>::iterator it = curpoints.begin(); it!= curpoints.end() ; ++it){

        ofDrawSphere(it->second,radius);
        
    }
    }
    ofPopStyle();
    ofPopMatrix();
}