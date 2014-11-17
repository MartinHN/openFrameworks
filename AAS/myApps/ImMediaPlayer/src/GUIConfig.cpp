//
//  GUIConfig.cpp
//  ImMedia
//
//  Created by martin hermant on 17/11/14.
//
//

#include "GUIConfig.h"
#include "MediaPool.h"

#include "Screens.h"

GUIConfig::GUIConfig(){
    GloveInteractBox::GloveInteractBox();
    isDraggable=false;
    drawLayer=0;
    isSelectable=false;
    isZoomable=false;
}


GUIConfig::~GUIConfig(){
    delete canvas;
}



void GUIConfig::init(){
    ofRectangle  r =Screens::instance()->screens[5]->rectScreen();
    box = r;
    targetBox = box;

    fullSizeRect = r;
    
    
    // Panel instanciation
    
    
    
    saveButton = new ofxUIImageButton(fullSizeRect.width-40,100,false,"UI/saveButton.png","saveButton");
    
    
    
    canvas = new ofxUISuperCanvas("Config",fullSizeRect.x,fullSizeRect.y,fullSizeRect.width,fullSizeRect.height);
    canvas->setName("Config");
    canvas->getCanvasTitle()->setVisible(false);
    
    
    
    // layout
    
    canvas->addWidgetDown(saveButton);
    
    
    // events
    
    canvas->disableAppDrawCallback();
    ofAddListener(canvas->newGUIEvent,this,&GUIConfig::GUIEvent);
    
    
    
}


void GUIConfig::GUIEvent(ofxUIEventArgs & a){
    
    ofxUIWidget* canvas = a.widget->getCanvasParent();
    if(canvas!=NULL){
        ofxUIWidget* parent = a.widget->getParent();
        
        
        if(canvas->getName()=="Config"){
            if (a.widget->getName()=="saveButton" && !a.getButton()->getValue()){
                Savable::save(MediaPool::projectPath);
            }
        }
    }
    
}


void GUIConfig::draw(ofEventArgs & a){
    
    canvas->draw();

}

void GUIConfig::boxMoved(bool stable){
    
    canvas->setPosition(box.x,box.y);
    
}

void GUIConfig::entered(){
    Tweener.addTween(targetBox->x,fullSizeRect.x,1);
}
void GUIConfig::exited(){
    Tweener.addTween(targetBox->x,fullSizeRect.getMaxX()-100,1);
}