//
//  Gui.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 07/02/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//



#include "Gui.h"





void Gui::load(ofParameterGroup & pgtmp){

    gui.push_back(new ofxPanel());
    gui.back()->setup(pgtmp.getGroup("global"),pgtmp.getGroup("global").getName(),(baseC)*PARAMW);
    
    int h = gui.back()->getHeight() + 10;

    gui.push_back(new ofxPanel());
    gui.back()->setup(pgtmp.getGroup("attrCtl"),pgtmp.getGroup("attrCtl").getName(),(baseC)*PARAMW,h);
    gui.back()->minimizeAll();
    
    h += gui.back()->getHeight() + 10;
    gui.push_back(new ofxPanel());
    gui.back()->setup(pgtmp.getGroup("blobsettings"),pgtmp.getGroup("blobsettings").getName(),(baseC)*PARAMW,h);
    gui.back()->minimizeAll();
    
    h += gui.back()->getHeight() + 10;

    gui.push_back(new ofxPanel());
    gui.back()->setup(pgtmp.getGroup("screens"),pgtmp.getGroup("screens").getName(),(baseC)*PARAMW,h);
    gui.back()->minimizeAll();
    
    
    baseC++;
    
    ofParameterGroup pgtmp2 = pgtmp.getGroup("Visu");

    
    for (int i = 0 ; i< pgtmp2.size();i++){
        ofAbstractParameter * p = &pgtmp2.get(i); 
        if(p->type()==typeid(ofParameterGroup).name()){
            gui.push_back(new ofxPanel());
            gui.back()->setup(pgtmp2.getGroup(i),pgtmp2.getName(),(baseC)*PARAMW);
            gui.back()->minimizeAll();
            gui.back()->getShape();
            baseC++;
    }
    }


}


void Gui::draw(){
    for(std::list<ofxPanel*>::iterator ggg = gui.begin() ; ggg!=gui.end();++ggg){
    (*ggg)->draw();
    }
}
