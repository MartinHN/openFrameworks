//
//  ParticlesHandler.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 14/08/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "ParticlesHandler.h"

Force::Force(){
    settings = ofParameterGroup();
    settings.setName("empty");
    isActive = true;
}
Force::Force(string namein,bool isAttrin){
    name = namein;
    shader.load("","shaders/"+name+".frag");
    settings = ofParameterGroup();
    settings.setName(name);
    pl.push_back(&isActive);
    isActive.setName("isActive");
    settings.add(isActive);
    isActive = false;
    
    isAttr = isAttrin;
    
}





ofShader Force::getShader(){
    return shader;
}


void Force::updateShader(){

    for (int i = 0 ; i < settings.size();i++){
        ofAbstractParameter *p = &settings[i];
   if(p->type()==typeid(ofParameter<int>).name())
        shader.setUniform1i(p->getName(),p->cast<int>());
    else if(p->type()==typeid(ofParameter<float>).name())
        shader.setUniform1f(p->getName(),p->cast<float>());
    else if(p->type()==typeid(ofParameter<ofVec3f>).name())
        shader.setUniform3f(p->getName(),p->cast<ofVec3f>().get()[0],p->cast<ofVec3f>().get()[1],p->cast<ofVec3f>().get()[2]);

    
}
}



////////////////////////////////////////
//
//PArticles
//
//////////////////////////////////////////////////





Particles::Particles(VisuHandler * v):VisuClass(v){
    setup();
    isHighFPS = true;
    int scrw = v->sH.sizeOfScreen(screenN).x;
    int scrh = v->sH.sizeOfScreen(screenN).y;
    netCompRatio = 2;

}


void Particles::setup(){

    settings.setName("Particles");
    
    
    gradient.loadImage("gradients/rasta.tiff");
    

    
    //GLOBAL
    
    
    settings.setName("particles");
    
    
    MYPARAM(timeStep , 1.f,0.f,30.f);

    
    MYPARAM(numParticles , 100000,100,1000000);
    
    
    //Appearence
    MYPARAM(particleSize, 3.0f,0.f,30.f);
    MYPARAM(alphaparticle,1.f,0.f,4.f);
    MYPARAM(partcolor, ofVec3f(0,0,1.),ofVec3f(0.),ofVec3f(255.));
    MYPARAM(isGradient,false,false,true);
    MYPARAM(gradtype , 1,0,3);
    MYPARAM(mingrad,0.f,-9999.f,9999.f);
    MYPARAM(gradName,"rasta","","");
    MYPARAM(maxgrad,1,-9999.f,9999.f);
    
    numParticles.addListener(this,&Particles::changeNum);
    
    forces.push_back(new Force("origin"));
    forces[forces.size()-1]->addParameter("k",.10f,0.f,.25f);
    forces[forces.size()-1]->addParameter("z",.01f,0.f,.05f);
    forces[forces.size()-1]->addParameter("freeze",0.f,0.f,1.f);

    
    
    forces.push_back(new Force("gravity",true));
    forces[forces.size()-1]->addParameter("r",.90f,0.f,.8f);
    forces[forces.size()-1]->addParameter("m",.001f,-.0002f,.002f);
    forces[forces.size()-1]->addParameter("damp",0.04f,-1.0f,1.0f);

    forces.push_back(new Force("netw"));
    forces[forces.size()-1]->addParameter("k",.30f,0.f,4.5f);
    forces[forces.size()-1]->addParameter("l0",1.0f,0.f,2.0f);
    forces[forces.size()-1]->addParameter("z",.0f,0.f,.05f);
    
    forces.push_back(new Force("neth"));
    forces[forces.size()-1]->addParameter("k",.030f,0.f,4.5f);
    forces[forces.size()-1]->addParameter("l0",1.0f,0.f,2.f);
    forces[forces.size()-1]->addParameter("z",.0f,0.f,.5f);
    
    forces.push_back(new Force("spring",true));
    forces[forces.size()-1]->addParameter("k",.030f,0.f,.5f);
    forces[forces.size()-1]->addParameter("l0",.010f,0.f,.1f);
    forces[forces.size()-1]->addParameter("z",.0f,-.5f,.5f);    
    forces[forces.size()-1]->addParameter("mode",1,0,1);
    
    forces.push_back(new Force("fieldForce"));
    forces[forces.size()-1]->addParameter("k",.030f,0.f,.5f);
    forces[forces.size()-1]->addParameter("velouty",.030f,0.f,.5f);
    forces[forces.size()-1]->addParameter("veloutz",.030f,0.f,.5f);
    forces[forces.size()-1]->addParameter("veloutborder",.00f,0.f,.5f);
    forces[forces.size()-1]->addParameter("minv",.10f,0.f,1.f);
    forces[forces.size()-1]->addParameter("maxv",.80f,0.f,1.f);


    forces.push_back(new Force("globalForce"));
    forces[forces.size()-1]->addParameter("f",1.f,0.f,1.3f);
    forces[forces.size()-1]->addParameter("fmin",1.f,0.f,1.3f);
    forces[forces.size()-1]->addParameter("vmax",.010f,0.f,1.f);
    forces[forces.size()-1]->addParameter("vmin",.0f,0.f,1.f);

    
    
    
        initFbo();
    
    updateRender.load("shaders/render.vert","shaders/render.frag");
    updatePos.load("","shaders/posUpdate.frag");
    


    

}








void Particles::update(int w, int h){
    glBlendEquation(GL_ADD);
    glBlendFunc(GL_ONE, GL_ZERO);
    
    for(int i = 0 ; i < forces.size();i++){
        if(forces[i]->isActive&&forces[i]->name.find("net")!=string::npos){
            for(int j = 0 ; j<netCompRatio;j++){
            velPingPong.dst->begin();
            forces[i]->shader.begin();
            forces[i]->shader.setUniformTexture("posData",posPingPong.src->getTextureReference(), 1);
            forces[i]->shader.setUniform3f("screen",w,h,dad->zdepth);
            forces[i]->shader.setUniform1i("resolution",textureRes);
            forces[i]->updateShader();
            
            velPingPong.src->draw(0,0);
            
            forces[i]->shader.end();
            
            velPingPong.dst->end();
            velPingPong.swap();
            }
        }
        else if(forces[i]->isActive && (forces[i]->isAttr?dad->attr->curp.size()>0:1)){
            int j = 0;
            do{
        velPingPong.dst->begin();
        forces[i]->shader.begin();
        forces[i]->shader.setUniformTexture("posData",posPingPong.src->getTextureReference(), 1);
        if(forces[i]->name=="origin") 
            forces[i]->shader.setUniformTexture("originData",origins.getTextureReference(), 2);
                if(forces[i]->name=="fieldForce"){
                forces[i]->shader.setUniform2f("inres",dad->inw,dad->inh);
#ifdef syphon
                    forces[i]->shader.setUniformTexture("fieldData",dad->syphonTex.src->getTextureReference(), 2);
#endif
                    
                    
                    
                    
                    
                }
//            forces[i]->shader.setUniformTexture("fieldData",dad->blobFbo.getTextureReference(), 2);
        if(forces[i]->isAttr&&j<dad->attr->curp.size())
            forces[i]->shader.setUniform3f("attr",dad->attr->curp[j].x,dad->attr->curp[j].y*scrh/scrw,dad->attr->curp[j].z);
        forces[i]->shader.setUniform3f("screen",w,h,dad->zdepth);
        forces[i]->shader.setUniform1i("resolution",textureRes);
        forces[i]->updateShader();
        
        velPingPong.src->draw(0,0);
        
        forces[i]->shader.end();
        
        velPingPong.dst->end();
        velPingPong.swap();
                j++;
            }while( forces[i]->isAttr&&j<dad->attr->curp.size());

        }
//        glEnd();
    }
    
    
    
    
    
    posPingPong.dst->begin();
    updatePos.begin();  // Previus position
    updatePos.setUniformTexture("velData", velPingPong.src->getTextureReference(), 1);      // Velocity
    updatePos.setUniform1f("timestep",timeStep*1.0/FPS );
    updatePos.setUniform1i("resolution",textureRes);
    
    posPingPong.src->draw(0,0);
    
    
    
    updatePos.end();
    posPingPong.dst->end();
    
    
    posPingPong.swap();
    
    
    
}

void Particles::draw(int w, int h){


    
    
    updateRender.begin();

    
    updateRender.setUniformTexture("posTex", posPingPong.src->getTextureReference(), 0);
    if(gradtype==2) updateRender.setUniformTexture("velTex",velPingPong.src->getTextureReference(),1);
    if(gradtype>0) updateRender.setUniformTexture("gradient",gradient.getTextureReference(),2);
#ifdef CAMCOLOR
    updateRender.setUniformTexture("camin", backImage.getTextureReference(), 4);
    updateRender.setUniform1i("iscam",iscam);
    updateRender.setUniform2f("insize",backImage.getWidth(),backImage.getHeight());
#endif

    updateRender.setUniform1i("resolution", (int)textureRes); 
    updateRender.setUniform3f("screen", w, (float)h,(float)dad->zdepth);

    updateRender.setUniform4f("colorpart",gradtype,partcolor.get()[0],partcolor.get()[1],partcolor.get()[2]);
    updateRender.setUniform2f("gradbounds",(float)mingrad,(float)maxgrad);
//    updateRender.setUniform3f("mouse",(float)(ofGetMouse().x),(float)(height-mouseY),(float)zdepth/2 + f.mousez);
    updateRender.setUniform1f("alpha",alphaparticle);
    
    
    
   
    
    glPointSize(particleSize);
    glBegin( GL_POINTS ); 
    for(int x = 0; x < textureRes; x++){
        for(int y = 0; y < textureRes; y++){
            
            glVertex3d(x,y,0);
            
        }
    }
    
    glEnd();

    
    
    updateRender.end();
    
//    posPingPong.src->draw(0,0,scrw/2,scrh);
//    velPingPong.src->draw(scrw/2,0,scrw/2,scrh);    

}

void Particles::initFbo(){
    
    // Seting the textures where the information ( position and velocity ) will be
//    textureResx*textureResy = numParticles;
//    textureResx/textureResy = scrw/scrh;
 
    textureRes2.x = (int)sqrt((float)numParticles*scrw/scrh);
    textureRes2.y = (int)numParticles/textureRes2.x;
    
    textureRes = (int)sqrt((float)numParticles);
    numParticles = textureRes * textureRes;
    
    // 1. Making arrays of float pixels with position information
    float * pos = new float[numParticles*3];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            
            pos[i*3 + 0] = x*1.0/textureRes;
            pos[i*3 + 1] = y*1.0*scrh/(textureRes*scrw);
            pos[i*3 + 2] = 0.5;
        }
    }
    
    // Load this information in to the FBO´s texture
    posPingPong.allocate(textureRes, textureRes,GL_RGB32F);
    posPingPong.src->getTextureReference().loadData(pos, textureRes, textureRes, GL_RGB);
    posPingPong.dst->getTextureReference().loadData(pos, textureRes, textureRes, GL_RGB);
       
    origins.allocate(textureRes, textureRes,GL_RGB32F);
    origins.getTextureReference().loadData(pos, textureRes, textureRes, GL_RGB);
    
    delete [] pos;    // Delete the array
    
    
    
    // 2. Making arrays of float pixels with velocity information and the load it to a texture
    float * vel = new float[numParticles*3];
    for (int i = 0; i < numParticles; i++){
        vel[i*3 + 0] = 0.0;
        vel[i*3 + 1] = 0.0;
        vel[i*3 + 2] = 0.0;//i*1.0/numParticles ;
    }
    // Load this information in to the FBO´s texture
    velPingPong.allocate(textureRes, textureRes,GL_RGB32F);
    velPingPong.src->getTextureReference().loadData(vel, textureRes, textureRes, GL_RGB);
    velPingPong.dst->getTextureReference().loadData(vel, textureRes, textureRes, GL_RGB);
    delete [] vel; // Delete the array
    

    
    
}



void Particles::registerParam(){
    for(int i = 0; i<forces.size();i++){
//        vector<ofAbstractParameter * > pl = forces[i].pl; 
        forces[i]->settings.clear();
        settings.add(forces[i]->settings);
        
        for(int j = 0 ;j<forces[i]->pl.size();j++){
           
            
            forces[i]->settings.add(*forces[i]->pl[j]);
           
        }
         
    }
}

void Particles::changeNum(int & num){
    initFbo();
}



