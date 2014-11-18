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
    isHoverable = false;


}

GUIProjects::~GUIProjects(){
    delete projectsCanvas;
}

void GUIProjects::init(){
    
    
    
    
    ofRectangle  r =Screens::instance()->screens[0]->rectScreen();
    box = r+ofVec2f(-1,0);
    targetBox = box;
    fullSizeRect = r;
    
    
    // projectPanel instanciations
    backButton = new ofxUIImageButton(fullSizeRect.width-40,100,false,"UI/backButton.png","backButton");
    
    projectsListScrollCanvas = new ofxUIScrollableCanvas(0,110,fullSizeRect.width-40,fullSizeRect.height-50);
    projectsListScrollCanvas->setName("Projects");
    

    projectsList = new ofxUIDropDownList("List",vector<string>(),fullSizeRect.width-40,0,0,OFX_UI_FONT_LARGE);


    projectsList->setAutoClose(false);
    projectsList->open();
    ((ofxUIToggle*)projectsList)->setVisible(false);
    
    projectsCanvas = new ofxUISuperCanvas("Projects",fullSizeRect.x,fullSizeRect.y,fullSizeRect.width,fullSizeRect.height);
    projectsCanvas->setName("Projects");
    projectsCanvas->getCanvasTitle()->setVisible(false);
    projectsCanvas->setColorBack(ofColor(0,0,0,100));
    
    // layout

    
    
    projectsCanvas->addWidgetDown(backButton);
    projectsCanvas->addWidgetDown(projectsListScrollCanvas);
    
    projectsListScrollCanvas->addWidgetDown(projectsList);
    

    // events
    projectsCanvas->disableAppDrawCallback();
    projectsListScrollCanvas->disableAppDrawCallback();
    ofAddListener(projectsCanvas->newGUIEvent,this,&GUIProjects::GUIevent);
    ofAddListener(projectsListScrollCanvas->newGUIEvent,this,&GUIProjects::GUIevent);

    
    
    
    // fix internal bug of scrollCanvas
    projectsListScrollCanvas->getSRect()->setParent(projectsCanvas->getRect());
    
    // color Mods
    projectsListScrollCanvas->setColorBack(ofColor(0,0,0,0));
    backButton->setColorBack(ofColor::grey);
    
    
    // file watcher

    setCurrentDirectory( "/Users/mhermant/Desktop/TestImMedia");
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
    Tweener.addTween(&targetBox,fullSizeRect,1);
}
void GUIProjects::exited(){
    ofRectangle tmpTarget(fullSizeRect);
    tmpTarget.translateX(-tmpTarget.width+100);
   Tweener.addTween(&targetBox,tmpTarget,1);
}

void GUIProjects::boxMoved(bool stable){
    
    projectsCanvas->setPosition(box.x,box.y);

    
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
            if(find(supported_formats.begin(),supported_formats.end(),files[i].getExtension())!=supported_formats.end()|| files[i].getExtension() == "xml"){
                
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
        
         cout << a.widget->getName() << endl;
        
        
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
                
                if(isProjectOpened && a.widget->getTouchID()%12 == GLOVE_BUTTON_CLICK){
                    
                    
                    // select in playground too
                    string curName = a.widget->getName();
                    vector<Media*> el = MediaPool::instance()->medias;
                    for(vector<Media*>::iterator it = el.begin() ; it!= el.end() ; ++it){
                        
                        if((*it)->name==curName){
                            if(selected[curGlove]!=NULL)selected[curGlove]->isSelected = false;
                            selected[curGlove] = *it;
                            selected[curGlove]->isSelected = true;
                            break;
                        }
                    }
                    
                }
                
                // open
                else if(a.widget->getTouchID()%12 == GLOVE_BUTTON_CLICK){
                    
                    asyncPath = currentDirectory.path()+"/"+a.widget->getName();
                }
                
                
            }
        }
        
        
        
    }
    
    
}



void GUIProjects::projectsAdded(string &filename){
    ofFile file(currentDirectory.path()+"/"+filename);
    if(file.isDirectory() || find(supported_formats.begin(),supported_formats.end(),file.getExtension())!=supported_formats.end()){
        projectsList->addToggle(file.getBaseName());
    }
}

void GUIProjects::projectsRemoved(string &filename){
    
    projectsList->removeToggle(filename);
    
    
    
}