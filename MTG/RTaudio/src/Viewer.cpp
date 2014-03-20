//
//  Viewer.cpp
//  RTaudio
//
//  Created by martin hermant on 19/03/14.
//
//

#include "Viewer.h"


Viewer::Viewer(){
    cam.setDistance(100);
    cam.enableMouseInput();
    isDirty = true;
    vector<string> nul;
    gui = new ofxUISuperCanvas("Axes");
    
    
}

void Viewer::update(){

    if(isDirty){
        delete gui;
        gui = new ofxUISuperCanvas("Axes");
        
        
        if(p->getAxes().size()>0){
        gui->addDropDownList("Axes", p->getAxes());
            isDirty = false;
    
        
        
        
            vbo.clear();
            vector<ofVec3f> vrt;
            
            for ()
            vbo.setVertexData(, <#int total#>, <#int usage#>)
        
        
        
        
        
        }
    
        
        
        
    }

}

void Viewer::draw(){
    
    if(!isDirty){gui->draw();}
    
    cam.begin();
    
    ofSetColor(255,0,0);

    ofDrawBox(30);
    
    
    
    
    cam.end();
}


