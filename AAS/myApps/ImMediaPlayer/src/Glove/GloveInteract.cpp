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


        ofAddListener( GloveInstance::cursor2DEvent,this,&GloveInteract::cursor2DMoved);
        ofAddListener(GloveInstance::touchEvent,this,&GloveInteract::touch);
        ofAddListener(GloveInstance::relativeOrientationEvent,this,&GloveInteract::relativeMoved);
    
    
    ofAddListener(ofEvents().update, this, &GloveInteract::update);

}



GloveInteract::~GloveInteract(){

        ofRemoveListener( GloveInstance::cursor2DEvent,this,&GloveInteract::cursor2DMoved);
        ofRemoveListener(GloveInstance::touchEvent,this,&GloveInteract::touch);
        ofRemoveListener(GloveInstance::relativeOrientationEvent,this,&GloveInteract::relativeMoved);
        
    
    
    ofRemoveListener(ofEvents().update, this, &GloveInteract::update);

}






