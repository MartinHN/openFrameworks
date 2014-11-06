//
//  GUI.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "GUIProjects.h"
#include "Screens.h"

extern ofEvent<ofEventArgs> drawSyphonEvent;


GUIProjects::GUIProjects(){
    
    
}

void GUIProjects::init(){
    ofRectangle  r =Screens::instance()->walls[0]->rectScreen();
    
    projectsList = new ofxUIDropDownList("List",vector<string>(),PROJECTWIDTH,r.x,r.y);
    projectsList->setAutoClose(false);
    
    
    projectsCanvas = new ofxUISuperCanvas("Projects",r.x,r.y,r.width,r.height);
    
    projectsCanvas->addWidgetDown(projectsList);
    
    projectsCanvas->setAutoDraw(false);
    projectsCanvas->DisableCallbacks();
    
    ofAddListener(drawSyphonEvent, this, &GUIProjects::draw,OF_EVENT_ORDER_BEFORE_APP);
    ofAddListener(projectsCanvas->newGUIEvent,this,&GUIProjects::GUIevent);
    
    
    
}



void GUIProjects::startWatch(string s){
    
    if(watch.isRunning())watch.stop();
    projectsList->clearToggles();
    watch.start(PROJECTPATH+s, 1000);
    ofAddListener(watch.fileAdded, this, &GUIProjects::projectsAdded);
    ofAddListener(watch.fileRemoved, this, &GUIProjects::projectsRemoved);
}

void GUIProjects::registerListeners() {
    
    
}



void GUIProjects::draw(ofEventArgs & a){
    projectsCanvas->draw();
    
}


void GUIProjects::update(ofEventArgs & a){
    
    
    
}


void GUIProjects::GUIevent(ofxUIEventArgs & a){
    
}



void GUIProjects::projectsAdded(string &filename){
    ofFile file(PROJECTPATH+filename);
    if(file.isDirectory()){
        
        projectsList->addToggle(filename);
        
        
        
    }
}

void GUIProjects::projectsRemoved(string &filename){
    
    projectsList->removeToggle(filename);
    
    
    
}