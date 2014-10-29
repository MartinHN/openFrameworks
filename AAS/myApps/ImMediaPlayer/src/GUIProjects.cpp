//
//  GUI.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "GUIProjects.h"



GUIProjects::GUIProjects(){

    name = "Projects";
    
    box = *Screens::instance()->walls[0];
    box.width = PROJECTWIDTH;
}



void GUIProjects::startWatch(string s){
    
    if(watch.isRunning())watch.stop();
    
    for(vector<ProjectBox*>::iterator it = projects.begin();it!=projects.end();++it){
        delete (*it);
    }
    projects.clear();
    
    watch.start(PROJECTPATH+s, 1000);
    ofAddListener(watch.fileAdded, this, &GUIProjects::projectsAdded);
    ofAddListener(watch.fileRemoved, this, &GUIProjects::projectsRemoved);
    
}


void GUIProjects::draw(ofEventArgs & a){
    GloveInteract::draw(a);
    
}


void GUIProjects::update(ofEventArgs & a){
    GloveInteract::update(a);
    
    
    
}



void GUIProjects::projectsAdded(string &filename){
    ofFile file(PROJECTPATH+filename);
    if(file.isDirectory()){
        ofVec2f offset(box.x,projects.size()* (PROJECTBHEIGHT+PROJECTPAD));
        ProjectBox* pb = new ProjectBox(this,offset);
        pb->name = filename;


        projects.push_back(pb);
    }
}

void GUIProjects::projectsRemoved(string &filename){
    
    int idx = 0;
    for(vector<ProjectBox*>::iterator it = projects.begin();it!=projects.end();++it){
        if((*it)->name == filename){
            break;
        }
        idx++;
    }
    if(idx != projects.size()){
        delete projects[idx];
        projects.erase(projects.begin()+idx);
    }
}