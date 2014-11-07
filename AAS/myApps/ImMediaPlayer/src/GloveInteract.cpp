//
//  Draggable.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "GloveInteract.h"

GloveInstance * GloveInteract::curGlove = NULL;



GloveInteract::GloveInteract(){


        ofAddListener( GloveOSC::cursor2DEvent,this,&GloveInteract::cursor2DMoved);
        ofAddListener(GloveOSC::touchEvent,this,&GloveInteract::touch);
        ofAddListener(GloveOSC::relativeOrientationEvent,this,&GloveInteract::relativeMoved);
    
    
    ofAddListener(ofEvents().update, this, &GloveInteract::update);

}



GloveInteract::~GloveInteract(){

        ofRemoveListener( GloveOSC::cursor2DEvent,this,&GloveInteract::cursor2DMoved);
        ofRemoveListener(GloveOSC::touchEvent,this,&GloveInteract::touch);
        ofRemoveListener(GloveOSC::relativeOrientationEvent,this,&GloveInteract::relativeMoved);
        
    
    
    ofRemoveListener(ofEvents().update, this, &GloveInteract::update);

}






