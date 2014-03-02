//
//  Gui.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 07/02/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//



#include "Gui.h"





void Gui::load(ofParameterGroup & pgtmp){

    guiParam.push_back(new ofxPanel());
    guiParam.back()->setup(pgtmp.getGroup("global"),pgtmp.getGroup("global").getName(),(baseC)*PARAMW);
    
    baseC++;
    int h = guiParam.back()->getHeight() + 10;
    
    guiParam.push_back(new ofxPanel());
    guiParam.back()->setup(pgtmp.getGroup("VisuHandler"),pgtmp.getGroup("VisuHandler").getName(),(baseC)*PARAMW);
    
    baseC++;
    h += guiParam.back()->getHeight() + 10;

    guiParam.push_back(new ofxPanel());
    guiParam.back()->setup(pgtmp.getGroup("attrCtl"),pgtmp.getGroup("attrCtl").getName(),(baseC)*PARAMW);
//    guiParam.back()->minimizeAll();
    
    h += guiParam.back()->getHeight() + 10;
    baseC++;
    guiParam.push_back(new ofxPanel());
    guiParam.back()->setup(pgtmp.getGroup("blobsettings"),pgtmp.getGroup("blobsettings").getName(),(baseC)*PARAMW);
//    guiParam.back()->minimizeAll();
    
    h += guiParam.back()->getHeight() + 10;
    baseC++;
    
    
    guiParam.push_back(new ofxPanel());
    guiParam.back()->setup(pgtmp.getGroup("screens"),pgtmp.getGroup("screens").getName(),(baseC)*PARAMW);
    guiParam.back()->minimizeAll();
    
    
    baseC= 0;
    
    ofParameterGroup pgtmp2 = pgtmp.getGroup("Visu");
    
    
    
    for (int i = 0 ; i< pgtmp2.size();i++){
        ofAbstractParameter * p = &pgtmp2.get(i); 
        if(p->type()==typeid(ofParameterGroup).name()){
            guiVisu.push_back(new ofxPanel());
            guiVisu.back()->setup(pgtmp2.getGroup(i),pgtmp2.getName(),(int)((baseC)*PARAMW)%(int)(ofGetWidth()*(4./5)),3*PARAMW*int((baseC)*PARAMW/(ofGetWidth()*(4./5))));
            guiVisu.back()->minimizeAll();
            guiVisu.back()->getShape();
            baseC++;
    }
    }


}


void Gui::draw(){
    
    if(!visuSettings){
    for(std::list<ofxPanel*>::iterator ggg = guiParam.begin() ; ggg!=guiParam.end();++ggg){
    (*ggg)->draw();
    }
    }
    else{
        for(std::list<ofxPanel*>::iterator ggg = guiVisu.begin() ; ggg!=guiVisu.end();++ggg){
            (*ggg)->draw();
        }
    }
}
