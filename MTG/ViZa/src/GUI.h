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


class GUI{
public:
    ofxUIDropDownList *attributesX,*attributesY,*attributesZ;
    ofxUISuperCanvas *guiconf;
    ofxUIScrollableCanvas * scrollNames;
    
    ofParameterGroup settings;
    
     void guiEvent(ofxUIEventArgs &e);
    
    static GUI * instance(){if(inst == NULL){inst = new GUI;} return inst;   }
    
    void setup();

    void draw();
    
    ofParameter<bool> isModifiying;
    vector<string> attrNames;
    vector<string> songnames;
    static GUI * inst;
    int lastFramenum;
    string lastName;
};



#endif /* defined(__ViZa__GUI__) */
