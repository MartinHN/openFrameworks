//
//  Gui.h
//  MurMur d'Ombre
//
//  Created by Tinmar on 07/02/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofxGui.h"
#include "ofMain.h"
#include "Constants.h"
#define PARAMW 210


class Gui{
    
    
public:
    Gui():bHide(true),baseC(0){};
    ~Gui(){};
//    Gui(ofParameterGroup * p);

    void draw();
    void load(ofParameterGroup & pg);


    ofxPanel glob;
    ofParameterGroup globP;
    list<ofxPanel *> gui;
    bool bHide;
     int baseC ;
    
    
};


