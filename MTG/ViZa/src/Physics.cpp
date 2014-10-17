//
//  Physics.cpp
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#include "Physics.h"

#include "ofApp.h"


ofVec3f* Physics::vs=NULL;//
ofFloatColor* Physics::cols=NULL ;
unsigned int* Physics::idxs=NULL;
ofVbo Physics::vbo;
int Physics::startLines;
int Physics::amountLines;

ofParameter<ofVec3f> Physics::maxs;
ofParameter<ofVec3f> Physics::mins;
Container* Physics::dragged;
float Physics::originDrag;
bool Physics::linksongs = false;

void updatePhy(float time){
    
    
}

void buildNetwork(){
    
}


void Physics::draw(){
    ofPushMatrix();
    float ratio = 1;//ofGetScreenHeight()*1.0/ofGetScreenWidth();
//    if(ofApp::cam.getOrtho())ofScale(ofApp::cam.getDistance()*ratio,ofApp::cam.getDistance()*ratio,ofApp::cam.getDistance()*ratio);
    ofDisableDepthTest();
    vbo.drawElements(GL_POINTS,Physics::vbo.getNumVertices());
    if(linksongs&&amountLines>0){
        vbo.draw(GL_LINE_STRIP, startLines, amountLines);
    }
    ofPopMatrix();
}






Container * Physics::Cast(ofEasyCam cam, ofVec2f mouse , float sphereMult){
    
    float radmult = sphereMult*cam.getDistance()*Container::radius*1.0/distanceVanish(cam);
    float minDist = 99999;
    Container * res = NULL;
    for(int i = 0; i < vbo.getNumVertices() ; i++){
        ofVec3f v =vs[i]*(cam.getOrtho()?cam.getDistance():1);
        float screenDist = (cam.worldToScreen(v)-mouse).length();
        float worldDist = (cam.getPosition()-v).length();
        if(screenDist<radmult*1.0/worldDist){
            if(sphereMult>2 && screenDist < minDist){
                res = &Container::containers[i];
                minDist = screenDist;
            }
            else if (sphereMult<=2){
            return &Container::containers[i];
            }
        }
    }
    return res;
    
}



void Physics::orderBy(string attr,int axe,int type){
    bool found = false;
    for (vector<string> ::iterator it = Container::attributeNames.begin(); it!=Container::attributeNames.end(); ++it) {
        if(*it==attr){
            found = true;
            break;
        }
    }
    if(!found){
        attr = ofSplitString(attr, ".")[0];
    }
    float max = maxs.get()[axe];
    float min = mins.get()[axe];
    float mean = 1;
    if(type<2){
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
    if(type==1){
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
    
    ofVec3f mask(axe==0?1:0,axe==1?1:0,axe==2?1:0);
    maxs = max*mask + (-mask+ofVec3f(1))*maxs;
    mins = min*mask + (-mask+ofVec3f(1))*mins;
    Physics::updateVBO();
}





void Physics::updateVBO(){
    if(vs==NULL){
        vs =  new ofVec3f[Container::containers.size()];
        cols =new ofFloatColor[Container::containers.size()];
        idxs =new unsigned int[Container::containers.size()];


    }
    else if (vbo.getNumVertices() !=Container::containers.size() ){
        vs=(ofVec3f*)realloc(vs, Container::containers.size()*sizeof(ofVec3f));
        cols=(ofFloatColor*)realloc(vs, Container::containers.size()*sizeof(ofFloatColor));
        idxs=(unsigned int*)realloc(vs, Container::containers.size()*sizeof(unsigned int));
    }
    
    for(int i = 0 ; i < Container::containers.size() ; i++){
        vs[i] = Container::containers[i].pos - ofVec3f(.5);
        cols[i]= Container::containers[i].getColor();
        idxs[i] = Container::containers[i].index;
//        Container::containers[i].index=(unsigned int)i;
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
    Physics::cols[idx] = col;
    delete [] c;
}

void Physics::updateAllColors(){
for(vector<Container>::iterator it = Container::containers.begin() ; it != Container::containers.end();++it){

    Physics::cols[it->index] = Container::stateColor[it->isSelected?2:it->isHovered?3:(int)it->state];
        }
        vbo.updateColorData(Physics::cols,Container::containers.size());
}

void Physics::updateOnePos(int idx,ofVec3f & pos){
    
    
    vbo.updateOneVertexData(&pos, idx);
    Physics::vs[idx] = pos;
    
}

float Physics::distanceVanish(ofCamera cam){
    if(cam.getOrtho())return 100.0*cam.getScale().x;
    return 2.0f/tan(ofDegToRad(cam.getFov()/2.0f));
}


void Physics::updateDrag(ofVec2f mouse){
    if(dragged!=NULL){
        ofVec3f v =ofApp::cam.screenToWorld(ofVec3f(mouse.x,mouse.y,originDrag));
     updateOnePos(dragged->index,v);
    }
}


void Physics::setSelected(int s, int end){
    startLines = s;
    amountLines = end-s;
}

