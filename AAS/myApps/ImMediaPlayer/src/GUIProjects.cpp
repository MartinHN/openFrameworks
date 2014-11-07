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


ofFile GUIProjects::currentDirectory;




GUIProjects::GUIProjects(){
    
    
}

void GUIProjects::init(){
    
    
    
    
    ofRectangle  r =Screens::instance()->walls[0]->rectScreen();
    
    projectsList = new ofxUIDropDownList("List",vector<string>(),400,r.x,r.y);
    projectsList->setAutoClose(false);
    
    
    projectsCanvas = new ofxUISuperCanvas("Projects",r.x,r.y,r.width,r.height);
    
    projectsCanvas->addWidgetDown(projectsList);
    
    projectsCanvas->DisableCallbacks();
    projectsCanvas->enableTouchEventCallbacks();
    setCurrentDirectory( "/Users/mhermant/Movies");
    
    
    ofAddListener(drawSyphonEvent, this, &GUIProjects::draw,OF_EVENT_ORDER_BEFORE_APP);
    ofAddListener(projectsCanvas->newGUIEvent,this,&GUIProjects::GUIevent);
    
    ofAddListener(watch.fileAdded, this, &GUIProjects::projectsAdded);
    ofAddListener(watch.fileRemoved, this, &GUIProjects::projectsRemoved);
    
    
}



void GUIProjects::startWatch(string s){
    
    if(watch.isRunning())watch.stop();
    projectsList->clearToggles();
    projectsList->setName("s");
    watch.start(currentDirectory.path()+s, 1000);

}

void GUIProjects::registerListeners() {
    
    
}


void GUIProjects::setCurrentDirectory(string path){
    ofFile f(path);
        if(isProject(f)){
            MediaPool::instance()->loadMedias(path);
        }
        else if(f.isDirectory()){
            currentDirectory = ofFile(path);
            startWatch("");
        }
        
    
}

bool GUIProjects::isProject(ofFile f){
    
    if(f.isDirectory()){
    ofDirectory d(f.path());
    d.listDir();
    vector<ofFile> files = d.getFiles();
    
    // if there is one media, it's a project
    for(int i = 0 ; i < files.size() ; i++){
        if(find(supported_formats.begin(),supported_formats.end(),files[i].getExtension())!=supported_formats.end()){
            return true;
        }
        
    }
    }
    return false;
}

void GUIProjects::draw(ofEventArgs & a){
    projectsCanvas->draw();
    
}


void GUIProjects::update(ofEventArgs & a){
    
    
    
}


void GUIProjects::GUIevent(ofxUIEventArgs & a){
    
    
    
}



void GUIProjects::projectsAdded(string &filename){
    ofFile file(currentDirectory.path()+filename);
    if(file.isDirectory()){
        
        projectsList->addToggle(filename);
        
        
        
    }
}

void GUIProjects::projectsRemoved(string &filename){
    
    projectsList->removeToggle(filename);
    
    
    
}