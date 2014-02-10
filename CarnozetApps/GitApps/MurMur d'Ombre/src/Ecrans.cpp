//
//  Ecrans.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 14/01/14.
//  Copyright 2014 __MyCompanyName__. All rights reserved.
//

#include "Ecrans.h"

Ecran::Ecran(int number,vector<ofVec3f> vert,int scrwin, int  scrhin):number(number){ 

    scrw = scrwin;
    scrh = scrhin;
    
    vertices.setName("screen"+ofToString(number));
    for (int i = 0 ; i < vert.size();i++){
         
        vl.push_back(new ofParameter<ofVec3f>());
        vl.back()->set(vert[i]);
        vl.back()->setName("p"+ofToString(i));
        vl.back()->setMin(ofVec3f(0));
        vl.back()->setMax(ofVec3f(1));
    
    }
    
    
    calcRectMax();
#ifdef HOMOGRAPHY
    screenWarp=Warp();
    screenWarp.setSrc(ofPoint(0,0), ofPoint(rectMax.width,0), ofPoint(rectMax.width,rectMax.height), ofPoint(0,rectMax.height));
    ofVec2f scale(scrw,scrh);
    if(vert.size()<4)ofLogWarning("less than 4 vertices for screen : "+ofToString(number));
    else screenWarp.setDst(vert[0]*scale,vert[1]*scale,vert[2]*scale, vert[3]*scale);
#endif

//    createMask();
}


vector<ofVec3f> Ecran::getVerticesNorm(){
    vector<ofVec3f> res;
    for (std::list<ofParameter<ofVec3f> *>::iterator vv(vl.begin()); vv !=  vl.end(); ++vv){
        res.push_back(**vv);
    }
    return res;
}

vector<ofVec3f> Ecran::getVertices(){
    vector<ofVec3f> res;
    ofVec2f scale(scrw,scrh);
    for (std::list<ofParameter<ofVec3f> *>::iterator vv(vl.begin()); vv !=  vl.end(); ++vv){
        
        res.push_back(**vv );
//        cout<<vertices.getName()+ofToString((**vv).get().x)<<endl;
    }
    for (int i = 0 ; i< res.size();i++){
        res[i]*=scale;
    }
    return res;
}


void Ecran::updateMatrix(ofVec3f & dummy){
    vector<ofVec3f> vert = getVertices();
    screenWarp.setDst(vert[0],vert[1],vert[2], vert[3]);
    calcRectMax();
    
}


void Ecran::registerParams(){
    for(std::list<ofParameter<ofVec3f>*>::iterator vv(vl.begin()); vv !=  vl.end(); ++vv){
        (*vv)->removeListener(this,&Ecran::updateMatrix);
    }
    
    vertices.clear();
    vertices.setName("screen"+ofToString(number));
    for (std::list<ofParameter<ofVec3f>* >::iterator vv(vl.begin()); vv !=  vl.end(); ++vv){
        vertices.add(**vv);
        (*vv)->addListener(this,&Ecran::updateMatrix);
    }
//    for(std::list<ofParameter<ofVec3f> >::iterator vv(vl.begin()); vv !=  vl.end(); ++vv){
//        vl[i].addListener(this,&Ecran::updateMatrix);
//    }
}

void Ecran::calcRectMax(){
    int minX = 99999;
    int minY = 99999;
    
    int maxX = -99999;
    int maxY = -99999;
    vector<ofVec3f> vert = getVertices();
    for( int i = 0 ; i<  vert.size() ; i++ ){
        ofVec3f tmp = vert[i];
        if(tmp.x<minX){minX = tmp.x;}
        else if(tmp.x>maxX){maxX = tmp.x;}
        
        if(tmp.y<minY){minY = tmp.y;}
        else if(tmp.y>maxY){maxY = tmp.y;}
    }
    rectMax = ofRectangle(minX, minY, maxX-minX, maxY-minY);
}

