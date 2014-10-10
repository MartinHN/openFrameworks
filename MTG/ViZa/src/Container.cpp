//
//  Container.cpp
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#include "Container.h"
//#include "ofxTweener.h"





vector<Container> Container::containers;
ofVbo Container::vbo;


ofVec3f* Container::vs=NULL;//
ofFloatColor* Container::cols=NULL ;
unsigned int* Container::idxs=NULL;
float Container::radius = 10;
ofFloatColor Container::stateColor[]= {ofFloatColor::black, ofFloatColor::white, ofFloatColor::whiteSmoke };

void Container::updateVBO(){
    if(vs==NULL){
        vs =  new ofVec3f[containers.size()];
        cols =new ofFloatColor[containers.size()];
        idxs =new unsigned int[containers.size()];
        stateColor[0] = ofFloatColor::black*ofFloatColor(1,1,1,0.1);
        stateColor[1] =ofFloatColor::white*ofFloatColor(1,1,1,0.5);
        stateColor[2] =ofFloatColor::whiteSmoke*ofFloatColor(1,1,1,0.5);
    }
    else if (vbo.getNumVertices() !=containers.size() ){
        vs=(ofVec3f*)realloc(vs, containers.size()*sizeof(ofVec3f));
        cols=(ofFloatColor*)realloc(vs, containers.size()*sizeof(ofFloatColor));
        idxs=(unsigned int*)realloc(vs, containers.size()*sizeof(unsigned int));
    }

    for(int i = 0 ; i < containers.size() ; i++){
        vs[i] = containers[i].pos - ofVec3f(.5);
        cols[i]= stateColor[0];
        idxs[i] = (unsigned int)i;
        containers[i].index=(unsigned int)i;
    }
    if(vbo.getNumVertices()==0){
        vbo.setVertexData(vs, containers.size(), GL_DYNAMIC_DRAW);
        vbo.setIndexData(idxs, containers.size(), GL_DYNAMIC_DRAW);
        vbo.setColorData(cols, containers.size(), GL_DYNAMIC_DRAW);
    }
    else{
        vbo.updateVertexData(vs, containers.size());
        vbo.updateIndexData(idxs, containers.size());
        vbo.updateColorData(cols, containers.size());
    }
    registerListener();

}

void Container::registerListener(){
    for(int i = 0 ; i < containers.size() ; i++){
        containers[i].state = ofParameter<float>();
        
        containers[i].state.addListener(&containers[i], &Container::setState);
        containers[i].state = 0;
    }
}

void Container::updateOneColor(int idx,ofColor col){
    ofFloatColor* c = new ofFloatColor[1];
    c[0] = (ofFloatColor)col;
    vbo.updateOneColorData(c,idx);
    delete [] c;
}

void Container::setState(float & s){
   cols[index] = stateColor[(int)s];
    updateOneColor(index,cols[index]);
    AudioPlayer::instance()->Play(*this,(int)s);
//    AudioPlayer::instance()->Play(index,path,begin,end,state);
    
}




void Container::Cast(ofCamera cam, ofVec2f mouse){
   
    float mult = Container::radius /distanceVanish(cam);
    
    for(int i = 0; i < vbo.getNumVertices() ; i++){
        
        if((cam.worldToScreen(vs[i])-mouse).length()<mult/(cam.getPosition()-vs[i]).length()){
            cout << mult/(cam.getPosition()-vs[i]).length()<< endl;
            
            containers[i].state =containers[i].state==0?1:0;
            
            
            cout << "click" << i << endl;
            break;
        }
    }
    
}
float Container::distanceVanish(ofCamera cam){

    return 2.0f/tan(ofDegToRad(cam.getFov()/2.0f));
}

void Container::freeVbo(){
    
}