//
//  GUI.h
//  ViZa
//
//  Created by martin hermant on 14/10/14.
//
//

#ifndef __ViZa__GUI__
#define __ViZa__GUI__

#define MYPARAM(nom,init,min,max) nom.setName(#nom);nom.setMin(min);nom.setMax(max);settings.add(nom);nom=init;

#include"ofxUI.h"
#include "ofxGui.h"
#include <iostream>
#include "Container.h"
#include "ofApp.h"

class GUI{
public:

    ofxUITabBar * global;
    
    ofxUISuperCanvas *guiconf;
    ofxUIDropDownList * attr[3], *aggr[3];
    ofxUIDropDownList *scaleType[3];
    ofxUITextInput * min[3],*max[3];

    ofxUIScrollableCanvas * scrollNames;

    ofxUISuperCanvas * logCanvas;
    ofxUITextArea * Logger;
    
    ofxUISuperCanvas * viewCanvas;
    ofxUISlider * alphaView;
    ofxUIToggle * linkSongs;
    ofxUIToggle * orthoCam;
    
    
     void guiEvent(ofxUIEventArgs &e);
    
    static GUI * instance(){if(inst == NULL){inst = new GUI;} return inst;   }
    
    void setup();


    
    ofParameter<bool> isModifiying;
    vector<string> attrNames;
    vector<string> aggrNames;
    vector<string> songnames;
    static GUI * inst;
    int lastFramenum;
    string lastName;
    
    
    void maxsChanged(ofVec3f & v);
    void minsChanged(ofVec3f & v);
    static string numToAxe(int i);
    static int axeToNum(char s);
    static void LogIt(string s );
};



#endif /* defined(__ViZa__GUI__) */
