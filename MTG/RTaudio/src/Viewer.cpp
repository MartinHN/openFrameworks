//
//  Viewer.cpp
//  RTaudio
//
//  Created by martin hermant on 19/03/14.
//
//

#include "Viewer.h"
#include "Misc.h"


Viewer::Viewer(){
    cam.setDistance(100);
    cam.enableMouseInput();
    isCacheDirty = isGuiDirty =isViewDirty = true;


    setupGui();
    
    colors.push_back(ofColor::cadetBlue);
    colors.push_back(ofColor::red);
    colors.push_back(ofColor::rosyBrown);
    colors.push_back(ofColor::tomato);
    colors.push_back(ofColor::yellow);
    colors.push_back(ofColor::yellowGreen);
    colors.push_back(ofColor::chartreuse);
    colors.push_back(ofColor::darkCyan);
    
    
    
    
    
    
}

void Viewer::setupGui(){
    
    
    registerParams();
    
    guip = new ofxPanel(settings);
    gui = new ofxUISuperCanvas("Axes",ofGetWidth()/2,0,300,200);
//    
//    if(analyzers->getAxes().size()>0){
//        
//        gui->addDropDownList("Axesx", p->getAxesNames());
//        gui->addDropDownList("Axesy", p->getAxesNames());
//        gui->addDropDownList("Axesz", p->getAxesNames());
//    }
}

void Viewer::registerParams(){
    MYPARAM(drawSlice,true,false,true);
    
    
    
    
}


void Viewer::updateGui(){
//    ((ofxUIDropDownList*)gui->getWidget("Axesx"))->clearToggles();
//    axesx = p->getAxesNames();
//    ((ofxUIDropDownList*)gui->getWidget("Axesx"))->addToggles(axesx);
//    ((ofxUIDropDownList*)gui->getWidget("Axesy"))->clearToggles();
//    axesy = p->getAxesNames();
//    ((ofxUIDropDownList*)gui->getWidget("Axesy"))->addToggles(axesy);
//    ((ofxUIDropDownList*)gui->getWidget("Axesz"))->clearToggles();
//    axesz = p->getAxesNames();
//    ((ofxUIDropDownList*)gui->getWidget("Axesz"))->addToggles(axesz);
//    
//    


}

void Viewer::guiEvent(ofxUIEventArgs &e){
    string name = e.getName();
	int kind = e.getKind();
	cout << "got event from: " << name << endl;
    if(name == "Axesx"){
        
    }
    else if(name == "Axesy"){
        
    }
    else if(name == "Axesz"){
        
    }
    

    
}

void Viewer::updateView(){
    if(drawSlice){
        
        
    }
    else{
        
    }


}

void Viewer::updateCache(){
    
  
    for (int i = 0 ; i < slices->size() ; i++){
  
    }
}

void Viewer::update(){

    if(isGuiDirty){
        updateGui();
        isGuiDirty = false;
    }
    if(isViewDirty){
        updateView();
        isViewDirty = false;
    }
    if(isCacheDirty){
        updateCache();
        isCacheDirty = false;
    }

}

void Viewer::draw(){
    
    gui->draw();
    guip->draw();
    
    cam.begin();
    
   for (int i = 0 ; i < slices->size() ; i++){
       ofPushMatrix();
       ofSetColor(colors[slices->at(i).localid%colors.size()]);
       ofTranslate( slices->at(i).curpos);
       ofRect(0,0,10,10);
       ofPopMatrix();
   }
    
    
    
    
    
    cam.end();
}


