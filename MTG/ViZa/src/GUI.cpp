//
//  GUI.cpp
//  ViZa
//
//  Created by martin hermant on 14/10/14.
//
//

#include "GUI.h"


GUI * GUI::inst;


void GUI::setup(){
    
    
    
    
    int ch = 0;
    int pad=10;
    guiconf = new ofxUISuperCanvas("Config",0,0,400,400);
    guiconf->setName("Config");
    ch+=guiconf->getRect()->height+pad;
    
    scrollNames = new ofxUIScrollableCanvas(0,ch,700,400);
    scrollNames->setName("songNames");
    scrollNames->setScrollableDirections(false, true);
    if(Container::containers[0].attributes.size()>0){
        
        for(map<string,float>::iterator it = Container::containers[0].attributes.begin() ; it != Container::containers[0].attributes.end() ;++it){
            attrNames.push_back(it->first);
        }
        float ddSize = 100;

        
        guiconf->addWidgetDown(new ofxUIDropDownList("AttributeX", attrNames) );
        guiconf->addWidgetDown(new ofxUIDropDownList("AttributeY", attrNames) );
        guiconf->addWidgetDown(new ofxUIDropDownList("AttributeZ", attrNames) );
//        guiconf->addWidgetDown();
        for(map<string,vector<Container*> > ::iterator it = Container::songs.begin() ; it!=Container::songs.end() ; ++it){
            songnames.push_back(it->first);
            
            
        }
        scrollNames->addWidgetDown(new ofxUIDropDownList("Songs", songnames,0,0,0,OFX_UI_FONT_SMALL));
        ((ofxUIDropDownList*)scrollNames->getWidgetsOfType(OFX_UI_WIDGET_DROPDOWNLIST)[0])->open();
        
        ofxUIRectangle * r =((ofxUIDropDownList*)scrollNames->getWidgetsOfType(OFX_UI_WIDGET_DROPDOWNLIST)[0])->getRect();
        scrollNames->setSnapping(true);
        scrollNames->setDimensions(700,songnames.size()*r->height);

        

    }
    
    vector<ofxUIWidget*> ddls= guiconf->getWidgetsOfType(OFX_UI_WIDGET_DROPDOWNLIST);
    for(int i = 0 ; i < ddls.size(); i++){
        ((ofxUIDropDownList*) ddls[i])->setAutoClose(true);
        ((ofxUIDropDownList*) ddls[i])->setShowCurrentSelected(true);
    }
    

    //guiconf->addToggle("Recomp", false);
    
   	ofAddListener(guiconf->newGUIEvent,this,&GUI::guiEvent);
    ofAddListener(scrollNames->newGUIEvent,this,&GUI::guiEvent);
   // guiconf->setMinified(true);
    
}

void GUI::guiEvent(ofxUIEventArgs &e){
    string name = e.getName();
	int kind = e.getKind();
    cout << name << "//gg//" << kind <<endl;
    ofxUIScrollableCanvas * ee;
//Check modifications
    switch (kind) {
        case OFX_UI_WIDGET_SCROLLABLECANVAS:
            ee = ((ofxUIScrollableCanvas*)e.widget);
            isModifiying = ((ofxUIScrollableCanvas*)e.widget)->isScrolling;
            cout << isModifiying << endl;
            break;
        
        
        default:
            break;
    }
    string parentName = "";
    if(e.widget->getParent()!=NULL){
        parentName = e.widget->getParent()->getName();
    }
    if(parentName == "")return;
    cout << parentName << "//" << name <<endl;
    
    if(parentName == "AttributeX"){
        Physics::orderBy(name, 0, true,true);
        
    }
    else if(parentName == "AttributeY"){
         Physics::orderBy(name, 1, true,true);
           }
    else if(parentName == "AttributeZ"){
         Physics::orderBy(name, 2, true,true);

    }
    else if(parentName == "Songs"){
        Container::selectSong(name);
        
    }
        

    
    
    lastFramenum = ofGetFrameNum();
    
}


void GUI::draw(){

    //guiconf->draw();
    
}