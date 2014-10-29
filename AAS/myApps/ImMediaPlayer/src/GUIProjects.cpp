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
    box.width = PROJECTWIDTH;
}

void GUIProjects::startWatch(){
    watch.start(PROJECTPATH, 2000);
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
        ProjectBox* pb = new ProjectBox();
        pb->name = filename;
        pb->box.set(box.x,projects.size()* (PROJECTBHEIGHT+PROJECTPAD),PROJECTWIDTH,PROJECTBHEIGHT);

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