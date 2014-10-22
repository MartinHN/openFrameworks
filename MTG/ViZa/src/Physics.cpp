//
//  Physics.cpp
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#include "Physics.h"

#include "ofApp.h"


vector<ofVec3f> Physics::vs;//
vector<ofFloatColor> Physics::cols;
vector<unsigned int> Physics::idxs;
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
//    if(ofApp::cam.getOrtho())ofScale(1.0/ofApp::cam.getDistance(),1.0/ofApp::cam.getDistance(),1.0/ofApp::cam.getDistance());
    ofDisableDepthTest();
    vbo.drawElements(GL_POINTS,Physics::vbo.getNumVertices());
    if(linksongs&&amountLines>0){
        vbo.draw(GL_LINE_STRIP, startLines, amountLines);
    }
    ofPopMatrix();
}






Container * Physics::Cast(ofEasyCam cam, ofVec2f mouse , float sphereMult,bool brightest){
    
    float radmult = sphereMult*cam.getDistance()*Container::radius* 1.0/((cam.getOrtho()?60.0:1)*distanceVanish(cam));
    float minDist = 99999;
    Container * res = NULL;
    float alpha=0;
    for(int i = 0; i < vbo.getNumVertices() ; i++){
        ofVec3f v =vs[i];//*(cam.getOrtho()?1.0/cam.getDistance():1);
        float screenDist = (cam.worldToScreen(v)-mouse).length();
        float worldDist = radmult*1.0/(cam.getPosition()-v).length();
        if(screenDist<worldDist){
            if( screenDist < minDist ){
                res = &Container::containers[i];
                alpha+=cols[i].a;
                minDist = screenDist;
            }

        }
    }
    
    if(brightest && alpha<2*GUI::instance()->alphaView->getValue())res=NULL;
    return res;
    
}


Container * Physics::Nearest(ofVec3f point,float radius ){
    

    float minDist = 99999;
    Container * res = NULL;
    for(int i = 0; i < vbo.getNumVertices() ; i++){
        float v =(vs[i]-point).length();//*(cam.getOrtho()?1.0/cam.getDistance():1);
            if((radius==0 || v<radius) && v < minDist ){
                res = &Container::containers[i];
                minDist = v;
            }

        }
    
    return res;
    
}




void Physics::orderBy(string _attr,int axe,int type){
    bool found = false;
    string attr = _attr;
    for (vector<string> ::iterator it = Container::attributeNames.begin(); it!=Container::attributeNames.end(); ++it) {
        if(*it==attr){
            found = true;
            break;
        }
    }
    if(!found){
        attr = ofSplitString(_attr, ".")[0];
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
        
        
        min = mean - stddev;
        max = mean + stddev;

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

    int newSize = Container::containers.size();
    if(vs.size() !=newSize){
        vs .resize(newSize);
        cols.resize(newSize);
        idxs.resize(newSize);


    }

        

    for(int i = 0 ; i < newSize ; i++){
        vs[i] = Container::containers[i].pos - ofVec3f(.5);
        cols[i]= Container::containers[i].getColor();
        idxs[i] = Container::containers[i].index;
    }
    if(vbo.getNumVertices()!=newSize){
        vbo.setVertexData(&vs[0], newSize, GL_DYNAMIC_DRAW);
        vbo.setIndexData(&idxs[0], newSize, GL_DYNAMIC_DRAW);
        vbo.setColorData(&cols[0], newSize, GL_DYNAMIC_DRAW);
    }
    else{
        vbo.updateVertexData(&vs[0], newSize);
        vbo.updateIndexData(&idxs[0], newSize);
        vbo.updateColorData(&cols[0], newSize);
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
        vbo.updateColorData(&cols[0],Container::containers.size());
}

void Physics::updateOnePos(int idx,ofVec3f & pos){
    
    
    vbo.updateOneVertexData(&pos, idx);
    Physics::vs[idx] = pos;
    
}

float Physics::distanceVanish(ofCamera cam){
    if(cam.getOrtho())return 1.0;
    else return 2.0f/tan(ofDegToRad(cam.getFov()/2.0f));
}


bool Physics::updateDrag(ofVec2f mouse){
    if(dragged!=NULL){
        ofVec3f v =ofApp::cam.screenToWorld(ofVec3f(mouse.x,mouse.y,originDrag));
        
     updateOnePos(dragged->index,v);
        return true;
    }
    return false;
}


void Physics::setSelected(int s, int end){
    startLines = s;
    amountLines = end-s;
}

