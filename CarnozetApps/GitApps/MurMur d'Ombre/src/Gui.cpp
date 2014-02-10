//
//  Gui.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 07/02/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//



#include "Gui.h"





void Gui::load(ofParameterGroup & pgtmp){
    FreeClear(gui);


    globP.setName("global");
    int g=0;
//    ofUnregisterMouseEvents(gui.data());

//for (int i = 0 ; i< pgtmp.size();i++){
//    ofAbstractParameter * p = &pgtmp.get(i); 
//    if(p->type()==typeid(ofParameter<string>).name()){
//        ofParameter<string> pgg = pgtmp.getString(i);
//        pgg.makeReferenceTo(pgtmp.getString(i));
//        globP.add(pgg);
//    }
//    else g++;
//}
//    gui.push_back(new ofxGuiGroup(globP,globP.getName()));
    int w = ofGetScreenWidth()/(pgtmp.size()+1);
    
    for (int i = 0 ; i< pgtmp.size();i++){
        ofAbstractParameter * p = &pgtmp.get(i); 
        if(p->type()==typeid(ofParameterGroup).name()){
            ofParameterGroup pgg = pgtmp.getGroup(i);
            gui.push_back(new ofxPanel());
            gui.back()->setup(pgtmp.getGroup(i),pgtmp.getName(),(i+1)*w);
    }
    }

    
    
//ofRegisterMouseEvents(gui.data()); 
}

void Gui::draw(){
    for(std::list<ofxPanel*>::iterator ggg = gui.begin() ; ggg!=gui.end();++ggg){
    (*ggg)->draw();
    }
}
