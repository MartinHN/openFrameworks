//
//  GUI.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "GUIProjects.h"
#include "Screens.h"




ofFile GUIProjects::currentDirectory;




GUIProjects::GUIProjects(){
    GloveInteractBox::GloveInteractBox();
    isDraggable=false;
    drawLayer=0;
    isSelectable=false;
    isZoomable=false;

}

void GUIProjects::init(){
    
    
    
    
    ofRectangle  r =Screens::instance()->screens[0]->rectScreen();
    box = r;
    targetBox = box;
    fullSizeRect = r;
    
    backButton = new ofxUIImageButton(fullSizeRect.width-40,100,false,"UI/backButton.png","backButton");
    
    
    projectsListScrollCanvas = new ofxUIScrollableCanvas(0,110,fullSizeRect.width-40,fullSizeRect.height-200);
    projectsListScrollCanvas->setName("Projects");
    projectsListScrollCanvas->setColorFill(ofColor(255,0,0,0));
    projectsList = new ofxUIDropDownList("List",vector<string>());
    projectsList->setAutoClose(false);
    projectsList->open();
    ((ofxUIToggle*)projectsList)->setVisible(false);
    
    projectsCanvas = new ofxUISuperCanvas("Projects",fullSizeRect.x,fullSizeRect.y,fullSizeRect.width,fullSizeRect.height);
    projectsCanvas->setName("Projects");
    projectsCanvas->getCanvasTitle()->setVisible(false);
    
    projectsListScrollCanvas->addWidgetDown(projectsList);
    
    projectsCanvas->addWidgetDown(backButton);
    projectsCanvas->addWidgetDown(projectsListScrollCanvas);
    
    
    
    
    projectsCanvas->disableAppDrawCallback();
    projectsListScrollCanvas->disableAppDrawCallback();
    
    
    setCurrentDirectory( "/Users/mhermant/Desktop/TestImMedia");
    
    
    
    
    ofAddListener(projectsCanvas->newGUIEvent,this,&GUIProjects::GUIevent);
    ofAddListener(projectsListScrollCanvas->newGUIEvent,this,&GUIProjects::GUIevent);
    
    ofAddListener(watch.fileAdded, this, &GUIProjects::projectsAdded);
    ofAddListener(watch.fileRemoved, this, &GUIProjects::projectsRemoved);
    
    
}



void GUIProjects::startWatch(string s){
    
    
    
    projectsList->clearToggles();
    
    watch.start(currentDirectory.path()+"/"+s, 1000,500);
    
}

void GUIProjects::registerListeners() {
    
    
}

void GUIProjects::entered(){
    Tweener.addTween(targetBox.width,fullSizeRect.width,1);
}
void GUIProjects::exited(){
    Tweener.addTween(targetBox.width,100,1);
}

void GUIProjects::boxResized(bool stable){
    
    projectsCanvas->setWidth( box.width);
    
}

void GUIProjects::setCurrentDirectory(string path){
    cout << "path : " <<  path << endl;
    if (path!= ""){
        ofFile f(path);
        
        if(f.isDirectory() && !ofDirectory::isDirectoryEmpty(path)){
            if((isProjectOpened=isProject(f))){
                MediaPool::instance()->loadMedias(path);
            }
            
            currentDirectory = ofFile(path);
            startWatch("");
        }
    }
    
}

bool GUIProjects::isProject(ofFile f){
    
    if(f.isDirectory()){
        ofDirectory d(f.path());
        d.listDir();
        vector<ofFile> files = d.getFiles();
        int foundIdx = 0;
        
        // if there is all of it's elements are medias, it's a project
        for(int i = 0 ; i < files.size() ; i++){
            string curFName = files[i].getFileName();
            // its a media
            if(find(supported_formats.begin(),supported_formats.end(),files[i].getExtension())!=supported_formats.end()){
                
            }
            // it's a presentation
            else if(files[i].isDirectory() && ((foundIdx = curFName.find_last_of("_presentation"))!=string::npos ) && foundIdx == curFName.length()-1 ){
                
                
            }
            else{
                return false;
            }
            
        }
        return true;//files.size()>0?true:false;
    }
    return false;
    
}

void GUIProjects::draw(ofEventArgs & a){

    projectsCanvas->draw();


    
}


void GUIProjects::update(ofEventArgs & a){
    GloveInteractBox::update(a);
    if(asyncPath!=""){
        setCurrentDirectory(asyncPath);
        
        asyncPath = "";
    }
    
    
}


void GUIProjects::GUIevent(ofxUIEventArgs & a){
    ofxUIWidget* canvas = a.widget->getCanvasParent();
    if(canvas!=NULL){
        ofxUIWidget* parent = a.widget->getParent();
        
        
        if(canvas->getName()=="Projects"){
            if (a.widget->getName()=="backButton"){
                if(!a.getButton()->getValue()){
                vector<string> pathF = ofSplitString(currentDirectory.path(),"/");
                if(pathF.size()<=1)return;
                pathF.resize(pathF.size()-1);
                if(pathF.size()<=1)return;
                string curpath = ofJoinString(pathF,"/");
                curpath="/"+curpath;
                asyncPath = curpath;
                }
                
            }
            
            // an element has been triggerd in project list
            else if(parent->getName()=="List"){
                
                if(isProjectOpened){
                    
                    
                    // select in playground too
                    string curName = a.widget->getName();
                    vector<Media*> el = MediaPool::instance()->medias;
                    for(vector<Media*>::iterator it = el.begin() ; it!= el.end() ; ++it){
                        cout << (*it)->name << endl;
                        if((*it)->name==curName){
                            selected->isSelected = false;
                            selected = *it;
                            selected->isSelected = true;
                            break;
                        }
                    }
                    
                }
                
                // open
                else{
                    asyncPath = currentDirectory.path()+"/"+a.widget->getName();
                }
                
                
            }
        }
        
        
        
    }
    
    
}



void GUIProjects::projectsAdded(string &filename){
    ofFile file(currentDirectory.path()+"/"+filename);
    if(file.isDirectory() || ofFind(supported_formats,file.getExtension())){
        projectsList->addToggle(file.getBaseName());
    }
}

void GUIProjects::projectsRemoved(string &filename){
    
    projectsList->removeToggle(filename);
    
    
    
}