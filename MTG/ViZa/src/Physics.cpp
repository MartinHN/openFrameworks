//
//  Physics.cpp
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#include "Physics.h"

ofVec3f* Physics::vs=NULL;//
ofFloatColor* Physics::cols=NULL ;
unsigned int* Physics::idxs=NULL;
ofVbo Physics::vbo;

void updatePhy(float time){
    
    
}

void buildNetwork(){
    
}






Container * Physics::Cast(ofCamera cam, ofVec2f mouse){
    
    float mult = Container::radius /distanceVanish(cam);
    
    for(int i = 0; i < vbo.getNumVertices() ; i++){
        
        if((cam.worldToScreen(vs[i])-mouse).length()<mult/(cam.getPosition()-vs[i]).length()){
            
            
            return &Container::containers[i];

        }
    }
    return NULL;
    
}



void Physics::orderBy(string attr,int axe,bool norm,bool stdnorm){
    float max = 1;
    float min = 0;
    float mean = 1;
    if(norm || stdnorm){
        max = -99999;
        min = 999999;
        mean = 0;
        for(vector<Container>::iterator it = Container::containers.begin() ; it!=Container::containers.end();++it){
            max = MAX(max,it->attributes[attr]);
            min = MIN(min,it->attributes[attr]);
            mean+=it->attributes[attr];
        }
        mean/=Container::containers.size();
    }
    if(stdnorm){
        float stddev = 0;
        for(vector<Container>::iterator it = Container::containers.begin() ; it!=Container::containers.end();++it){
            stddev+= (it->attributes[attr]-mean)*(it->attributes[attr]-mean);
        }
        
        stddev/=Container::containers.size();
        stddev = sqrt(stddev);
        
        
        min = mean - stddev/2;
        max = mean + stddev/2;

    }
    for(vector<Container>::iterator it = Container::containers.begin() ; it!=Container::containers.end();++it){
        if(axe==0)          it->pos.x = (it->attributes[attr]-min)/(max-min);
        else if(axe==1)     it->pos.y = (it->attributes[attr]-min)/(max-min);
        else                it->pos.z = (it->attributes[attr]-min)/(max-min);
    }
    Physics::updateVBO();
}





void Physics::updateVBO(){
    if(vs==NULL){
        vs =  new ofVec3f[Container::containers.size()];
        cols =new ofFloatColor[Container::containers.size()];
        idxs =new unsigned int[Container::containers.size()];
        Container::stateColor[0] = ofFloatColor::black;
        Container::stateColor[0].a=.1;
        Container::stateColor[1] =ofFloatColor::white;
        Container::stateColor[1].a=.3;
        Container::stateColor[2] =ofFloatColor::red;
        Container::stateColor[2].a=.5;
    }
    else if (vbo.getNumVertices() !=Container::containers.size() ){
        vs=(ofVec3f*)realloc(vs, Container::containers.size()*sizeof(ofVec3f));
        cols=(ofFloatColor*)realloc(vs, Container::containers.size()*sizeof(ofFloatColor));
        idxs=(unsigned int*)realloc(vs, Container::containers.size()*sizeof(unsigned int));
    }
    
    for(int i = 0 ; i < Container::containers.size() ; i++){
        vs[i] = Container::containers[i].pos - ofVec3f(.5);
        cols[i]= Container::stateColor[0];
        idxs[i] = (unsigned int)i;
        Container::containers[i].index=(unsigned int)i;
    }
    if(vbo.getNumVertices()==0){
        vbo.setVertexData(vs, Container::containers.size(), GL_DYNAMIC_DRAW);
        vbo.setIndexData(idxs, Container::containers.size(), GL_DYNAMIC_DRAW);
        vbo.setColorData(cols, Container::containers.size(), GL_DYNAMIC_DRAW);
    }
    else{
        vbo.updateVertexData(vs, Container::containers.size());
        vbo.updateIndexData(idxs, Container::containers.size());
        vbo.updateColorData(cols, Container::containers.size());
    }
    Container::registerListener();
    
}





void Physics::updateOneColor(int idx,ofColor col){

    ofFloatColor* c = new ofFloatColor[1];
    c[0] = (ofFloatColor)col;
    vbo.updateOneColorData(c,idx);
//    cout << col << endl;
    Physics::cols[idx] = col;
    delete [] c;
}

float Physics::distanceVanish(ofCamera cam){
    
    return 2.0f/tan(ofDegToRad(cam.getFov()/2.0f));
}

