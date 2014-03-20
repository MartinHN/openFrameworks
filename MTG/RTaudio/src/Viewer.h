//
//  Viewer.h
//  RTaudio
//
//  Created by martin hermant on 19/03/14.
//
//

#ifndef __RTaudio__Viewer__
#define __RTaudio__Viewer__

#include <iostream>
#include "Pooler.h"
#include "ofxUI.h"


class Viewer{
public:
    Viewer();
    
    void update();
    void draw();
    void link2pool(Pooler * pin){p =pin;};
    
    
    ofEasyCam cam;
    
    ofxUIDropDownList *dl;
    ofxUISuperCanvas *gui;
    Pooler * p;
    
    bool isDirty;
    ofVbo vbo;
    
    
};

#endif /* defined(__RTaudio__Viewer__) */
