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
Force::Force(string namein,bool isAttr){
    name = namein;
    shader.load("","shaders/"+name+".frag");
    settings = ofParameterGroup();
    settings.setName(name);
    pl.push_back(&isActive);
    isActive.setName("isActive");
    settings.add(isActive);
    isActive = false;
    
    attrFamilly = -2;
    if(isAttr) {    MYPARAM(attrFamilly,0,-1,13);
    pl.push_back(&attrFamilly);}
    
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
    netCompRatio = 8;

}


void Particles::setup(){

    settings.setName("Particles");
    
    
    int defaultgrad=0;
    changeGrad(defaultgrad);
    
  
#ifdef PIMG
    int partRes = 10 ;
    partImg.allocate(partRes,partRes,OF_IMAGE_COLOR_ALPHA);
    ofVec2f center(partRes/2);
    for(int i = 0 ; i<partRes;i++){
        for(int j = 0 ; j<partRes;j++){
            ofVec2f curp(i,j);
            ofColor cc(0,0,0,max(0.,1-curp.distance(center)*1.0/partRes));
            partImg.setColor(i,j,cc);
        }
    }
    
#endif


    //GLOBAL
    
    
    settings.setName("particles");
    
    
    MYPARAM(timeStep , 1.f,0.f,30.f);

    
    MYPARAM(numParticles , 100000,100,1000000);
    
    
    //Appearence
    MYPARAM(particleSize, 3.0f,0.f,30.f);
    MYPARAM(alphaparticle,1.f,0.f,4.f);
    MYPARAM(partcolor, ofVec3f(0,0,1.),ofVec3f(0.),ofVec3f(255.));
    MYPARAM(gradtype , 2,0,2);
    MYPARAM(gradNum,0,0,20);
    MYPARAM(mingrad,0.f,-1.f,1.f);
    MYPARAM(maxgrad,1,-1.f,1.f);
    MYPARAM(netCompRatio,8,3,20);
#ifdef PMOD
    MYPARAM(origintype,1,0,2);
#ifndef GUIMODE
    origintype.addListener(this,&Particles::changeOrigins);
#endif
#endif
    
    gradNum.addListener(this,&Particles::changeGrad);
    numParticles.addListener(this,&Particles::changeNum);

    forces.push_back(new Force("netw"));
    forces[forces.size()-1]->addParameter("k",.030f,0.f,.2f);
    forces[forces.size()-1]->addParameter("l0",1.0f,0.f,20.0f);
    forces[forces.size()-1]->addParameter("z",.0f,0.f,2.f);
    forces[forces.size()-1]->addParameter("size",2,1,10);
    
    forces.push_back(new Force("neth"));
    forces[forces.size()-1]->addParameter("k",.030f,0.f,.2f);
    forces[forces.size()-1]->addParameter("l0",1.0f,0.f,20.f);
    forces[forces.size()-1]->addParameter("z",.0f,0.f,2.f);
    forces[forces.size()-1]->addParameter("size",2,1,10);
    
    forces.push_back(new Force("origin"));
    forces[forces.size()-1]->addParameter("k",.10f,0.f,.25f);
    forces[forces.size()-1]->addParameter("z",.01f,0.f,.5f);
    forces[forces.size()-1]->addParameter("freeze",0.f,0.f,.80f);
    forces[forces.size()-1]->addParameter("damp",0.9f,0.5f,1.0f);
    
    forces.push_back(new Force("rotation",true));
    forces[forces.size()-1]->addParameter("type",0,0,1);
    forces[forces.size()-1]->addParameter("r",0.2f,0.f,.5f);
    forces[forces.size()-1]->addParameter("speed",0.f,0.f,0.5f);
    forces[forces.size()-1]->addParameter("orientation",ofVec3f(0,1,0),ofVec3f(0),ofVec3f(1));
    
    forces.push_back(new Force("gravity",true));
    forces[forces.size()-1]->addParameter("r",.50f,0.f,.5f);
    forces[forces.size()-1]->addParameter("mass",.001f,-.0002f,.02f);
        forces[forces.size()-1]->addParameter("rin",.50f,0.f,.5f);
    forces[forces.size()-1]->addParameter("massin",.00f,-.0002f,.0002f);
    forces[forces.size()-1]->addParameter("damp",1.f,.60f,1.0f);

    forces.push_back(new Force("smoothyGrav",true));
    forces[forces.size()-1]->addParameter("r",.50f,0.f,1.f);
    forces[forces.size()-1]->addParameter("mass",.1f,-.2f,.5f);
    forces[forces.size()-1]->addParameter("rin",.50f,0.f,.5f);
    forces[forces.size()-1]->addParameter("minattr",.0f,-.2f,.2f);
    forces[forces.size()-1]->addParameter("damp",1.f,.60f,1.0f);

    
    forces.push_back(new Force("spring",true));
    forces[forces.size()-1]->addParameter("k",.030f,-.2f,1.f);
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
    forces[forces.size()-1]->addParameter("f",1.f,0.99f,1.001f);
    forces[forces.size()-1]->addParameter("fmin",1.f,0.f,1.3f);
    forces[forces.size()-1]->addParameter("vmax",1.0f,0.f,1.f);
    forces[forces.size()-1]->addParameter("vmin",.0f,0.f,1.f);


    
    
    
        initFbo();
    
    updateRender.load("shaders/render.vert","shaders/render.frag");
    updatePos.load("","shaders/posUpdate.frag");
    


    

}








void Particles::update(int w, int h){
    
    if(w!=lastw||h!=lasth){
        initFbo(w, h);
    }
    
    
    
    glBlendEquation(GL_ADD);
    glBlendFunc(GL_ONE, GL_ZERO);
    
    for(int i = 0 ; i < forces.size();i++){
        if(forces[i]->isActive&&forces[i]->name.find("net")!=string::npos){
//            for(int j = 0 ; j<netCompRatio;j++){
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
                
                
                
//            }
        }
        else if(forces[i]->isActive ){
            int j = 0;
            vector<ofPoint> curattr = dad->attr->getType(forces[i]->attrFamilly);
            if((forces[i]->attrFamilly>-2?curattr.size()>0:1)){
                do{
                    velPingPong.dst->begin();
                    forces[i]->shader.begin();
                    forces[i]->shader.setUniformTexture("posData",posPingPong.src->getTextureReference(), 1);
                    if(forces[i]->name=="origin") 
                        forces[i]->shader.setUniformTexture("originData",origins.getTextureReference(), 2);
                    if(forces[i]->name=="fieldForce"){
                        forces[i]->shader.setUniform2f("inres",dad->inw,dad->inh);
            #ifdef syphon
                        forces[i]->shader.setUniformTexture("fieldData",dad->bH->syphonTex.src->getTextureReference(), 2);
            #endif
                            }
                    if(forces[i]->attrFamilly>=0&&curattr.size()>0&&j<curattr.size())
                        forces[i]->shader.setUniform3f("attr",curattr[j].x,curattr[j].y,curattr[j].z);
                    
                    forces[i]->shader.setUniform3f("screen",w,h,dad->zdepth);
                    forces[i]->shader.setUniform1i("resolution",textureRes);
                    forces[i]->updateShader();
                    
                    velPingPong.src->draw(0,0);
                    
                    forces[i]->shader.end();
                    
                    velPingPong.dst->end();
                    velPingPong.swap();
                            j++;
                }while( forces[i]->attrFamilly>=0&&j<curattr.size());
            }
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
    
    lastw = w;
    lasth = h;
    
}

void Particles::draw(int w, int h){


    
    
    updateRender.begin();

    
    updateRender.setUniformTexture("posTex", posPingPong.src->getTextureReference(), 0);
    if(gradtype==2) updateRender.setUniformTexture("velTex",velPingPong.src->getTextureReference(),1);
    if(gradtype>0) updateRender.setUniformTexture("gradient",gradient.getTextureReference(),2);
#ifdef PIMG
    updateRender.setUniformTexture("img",partImg.getTextureReference(),3);

#endif
#ifdef CAMCOLO
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
    initFbo(*dad->scrw,*dad->scrh);
}
void Particles::initFbo(int w,int h){
    
    // Seting the textures where the information ( position and velocity ) will be
//    textureResx*textureResy = numParticles;
//    textureResx/textureResy = scrw/scrh;
 
    textureRes2.x = (int)sqrt((float)numParticles* w/ h);
    textureRes2.y = (int)numParticles/textureRes2.x;
    
    textureRes = (int)sqrt((float)numParticles);
    numParticles = textureRes* textureRes;
    
    // 1. Making arrays of float pixels with position information
    float * pos = new float[numParticles*3];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            
            pos[i*3 + 0] = x*1.0/textureRes;
            pos[i*3 + 1] = y*1.0/(textureRes);
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


void Particles::changeGrad(int & i){
    ofDirectory gradDir("gradients");
    if(i<gradDir.listDir()){
    gradDir.getFile(i).getFileName();
        gradient.loadImage("gradients/"+gradDir.getFile(i).getFileName());}
}

bool sortOnZ(ofVec3f a,ofVec3f b){
    return a.z>b.z;
}
bool sortOnXYZ(ofVec3f a,ofVec3f b){
    return a.z>b.z&&a.x>b.x&&a.y>b.y;
}


void Particles::changeOrigins(int &type){
    
    float * pos ;
    bool hasChanged = true;
    
    
    
    switch (type) {
        case 0:
        {
            pos = new float[numParticles*3];
            for (int x = 0; x < textureRes; x++){
                for (int y = 0; y < textureRes; y++){
                    int i = textureRes * y + x;
                    
                    pos[i*3 + 0] = x*1.0/textureRes;
                    pos[i*3 + 1] = y*1.0/(textureRes);
                    pos[i*3 + 2] = 0.5;
                }
            }
            break;}
        case 1:
        {
            int zsplit = 2;
            int textureRes3 = pow(numParticles,1.0/3);
            numParticles= textureRes3*textureRes3*textureRes3;
            textureRes = sqrt((float)numParticles);
            int count=0;
            pos = new float[numParticles*3];
            
            for (int x = 0; x < textureRes3; x++){
                for (int y = 0; y < textureRes3; y++){
                    for (int z = 0; z < textureRes3; z++){
                    
                    int i = textureRes3 * textureRes3* x + textureRes3 * y + z;
                    
                    pos[i*3 + 0] = (float) (0.25 + x*1.0/textureRes3);
                    pos[i*3 + 1] = (float) (0.25 + y*1.0/textureRes3);
                    pos[i*3 + 2] = (float) (z*1.0/(textureRes3s));
                        cout<<i<<endl;
                    }
                }
            }
            
//            ofSort(pos,sortOnXYZ);
            break;}
#ifdef PMOD
        case 2:
        {
            vector<ofPoint> vert = readObj("models/psy203.obj",true);
            numParticles = vert.size();
            textureRes = sqrt((float)numParticles);
            numParticles = textureRes*textureRes;
            pos = new float[numParticles*3];
            for (int x = 0; x < textureRes; x++){
                for (int y = 0; y < textureRes; y++){
                    int i =  y + textureRes * x;
                    
                    if(i<vert.size()){
                    pos[i*3 + 0] = vert[i].x;
                    pos[i*3 + 1] = vert[i].y;
                    pos[i*3 + 2] = vert[i].z +1;
                    }
                    else {
                        pos[i*3 + 0] = 0;
                        pos[i*3 + 1] = 0;
                        pos[i*3 + 2] = 0;
                    }
                }
            }
            break;
        }
    
#endif
            
        default:
            hasChanged=false;
            break;
    }
    if(hasChanged){
     origins.allocate(textureRes, textureRes,GL_RGB32F);
    origins.getTextureReference().loadData(pos, textureRes, textureRes, GL_RGB);
    }
 
    delete [] pos;
}


vector<ofPoint> Particles::readObj(string pathin,bool sort){
    vector<ofPoint> points;
	string path = ofToDataPath(pathin, true);
	string line;
	


	// obj file format vertexes are 1-indexed
	points.push_back(ofPoint());
	
	ifstream myfile (path.c_str());
	if (myfile.is_open()) {
		while (! myfile.eof()) {
			getline (myfile,line);
			
			
			// parse the obj format here.
			//
			// the only things we're interested in is
			// lines beginning with 'g' - this says start of new object
			// lines beginning with 'v ' - coordinate of a vertex
			// lines beginning with 'f ' - specifies a face of a shape
			// 			we take each number before the slash as the index
			// 			of the vertex to join up to create a face.
			
			if(line.find("g ")==0) { // new object definition

			} else if(line.find("v ")==0) { // new vertex
                ofPoint p;
				vector<string> elements = ofSplitString(line, " ");
                if(elements.size()!=4) {
                    cout<<elements[0]+elements[1]+elements[2]+ofToString(elements.size())<<endl;
                    printf("Error line does not have 3 coordinates: \"%s\"\n", line.c_str());
                
                }
                
                p.x = atof(elements[1].c_str());
                p.y = atof(elements[2].c_str());
                p.z = atof(elements[3].c_str());
                points.push_back(p);
                
                
			} else if(line.find("f ")==0) { // face definition
				
				}
			}
		}
		
		
		myfile.close();
		
    
    if(sort){
        ofSort(points,sortOnXYZ);
    }
    
    return points;


}




