//
//  Gui.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 07/02/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//



#include "Gui.h"





void Gui::load(ofParameterGroup & pgtmp){

    globP.setName("global");

    
   
    for (int i = 0 ; i< pgtmp.size();i++){
        ofAbstractParameter * p = &pgtmp.get(i); 
        if(p->type()==typeid(ofParameterGroup).name()){
            ofParameterGroup pgg = pgtmp.getGroup(i);
            gui.push_back(new ofxPanel());
            gui.back()->setup(pgtmp.getGroup(i),pgtmp.getName(),(baseC)*PARAMW);
            baseC++;
    }
    }


}

void Gui::draw(){
    for(std::list<ofxPanel*>::iterator ggg = gui.begin() ; ggg!=gui.end();++ggg){
    (*ggg)->draw();
    }
}
