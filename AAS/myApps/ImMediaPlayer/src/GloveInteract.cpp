//
//  Draggable.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "GloveInteract.h"

ofVec2f* GloveInteract::cursor2D;
ofVec3f* GloveInteract::relative;
ofVec3f* GloveInteract::orientation;
vector<TouchState>* GloveInteract::touchs;



GloveInteract::GloveInteract(){

    
    for(vector<GloveInstance*>::iterator it =GloveOSC::gloves.begin() ; it!= GloveOSC::gloves.end() ; ++it){
        ofAddListener((*it)->cursor2DEvent,this,&GloveInteract::cursor2DMoved);
        ofAddListener((*it)->touchEvent,this,&GloveInteract::touch);
        ofAddListener((*it)->relativeOrientationEvent,this,&GloveInteract::relativeMoved);
    }
    
    ofAddListener(ofEvents().update, this, &GloveInteract::update);

}



GloveInteract::~GloveInteract(){
    
    for(vector<GloveInstance*>::iterator it =GloveOSC::gloves.begin() ; it!= GloveOSC::gloves.end() ; ++it){
        ofRemoveListener((*it)->cursor2DEvent,this,&GloveInteract::cursor2DMoved);
        ofRemoveListener((*it)->touchEvent,this,&GloveInteract::touch);
        ofRemoveListener((*it)->relativeOrientationEvent,this,&GloveInteract::relativeMoved);
        
    }
    
    ofRemoveListener(ofEvents().update, this, &GloveInteract::update);

}


void GloveInteract::setcurrentData(GloveInstance *gid){
    vector<GloveInstance*>::iterator it =std::find(GloveOSC::gloves.begin(),GloveOSC::gloves.end(),gid);
    if(it!= GloveOSC::gloves.end()){
        cursor2D = &(*it)->cursor2D;
        touchs = &(*it)->touchs;
        relative = &(*it)->relativeOrientation;
        orientation = &(*it)->orientation;
    }
}



