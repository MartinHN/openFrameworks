//
//  AttrCtl.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 23/03/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "AttrCtl.h"


//AttrCtl::AttrCtl(){
//}

AttrCtl::AttrCtl(){
    attrotate=0;
    
}

void AttrCtl::update(){
    smooth();
    staticpoints();
    timedPoints();
}

void AttrCtl::registerParam(){
    settings.setName("attrCtl");
    
    MYPARAM(zoffset,0.5f,0.f,1.f);
    MYPARAM(attrmirrorx,4,0,16);
    MYPARAM(attrmirrory,0,0,16);
    MYPARAM(attrmirrorz,0,0,16);
    MYPARAM(zonefamilly3,ofVec4f(0,0,1,1),ofVec4f(0),ofVec4f(1));
    MYPARAM(zonefamilly1,ofVec4f(0,0,1,1),ofVec4f(0),ofVec4f(1));
    MYPARAM(zonefamilly2,ofVec4f(0,0,1,1),ofVec4f(0),ofVec4f(1));
}



void AttrCtl::updatePoints(vector<ofPoint> curpin){
    
  if(destA.size()>0)lastA=destA;
    destA.clear();
    for(int i = 0 ; i< curpin.size() ; i++){
        vector<ofRectangle> zones;
        zones.push_back(ofRectangle(zonefamilly3.get().x, zonefamilly3.get().y, zonefamilly3.get().z, zonefamilly3.get().w));
        zones.push_back(ofRectangle(zonefamilly1.get().x, zonefamilly1.get().y, zonefamilly1.get().z, zonefamilly1.get().w));
        zones.push_back(ofRectangle(zonefamilly2.get().x, zonefamilly2.get().y, zonefamilly2.get().z, zonefamilly2.get().w));
        
        for(int k = 0 ; k< zones.size();k++){
                //destp.push_back(ofPoint(attrotate*(curpin[i].y),(1-curpin[i].x),curpin[i].z+zoffset));
            ofPoint p = ofPoint(1-curpin[i].x,curpin[i].y,curpin[i].z+zoffset);
            if(zones[k].inside(p)){
                destA.push_back(AttrStruct(p,k+1));
                break;
            }
        }

                
    }

    
}

vector<ofPoint> AttrCtl::getAll(){
    vector<ofPoint> res;
    for(int i = 0 ; i< destA.size() ; i++){
        res.push_back(destA[i].p);
        
    }
    return res;
}

vector<ofPoint> AttrCtl::getFamilly(int f){
    vector<ofPoint> res;
    vector<int> targetf;
    
    if(f<0){
     return res;
    }
    
    int curf = f%10;
    int k = 0;
     do{
         targetf.push_back(curf);
     k++;
    curf = f/(pow(10.,k));

}while (f>10&&curf>0);

    for(int i = 0 ; i< destA.size() ; i++){
        if(destA[i].f==f){res.push_back(destA[i].p);}
        
    }
    return res;
}



void AttrCtl::smooth(){
    
    smoothedA.clear();
    
    for(int i=0;i<destA.size();i++){
            smoothedA.push_back(destA[i]);
        
    }




}

void AttrCtl::staticpoints(){
    staticA.clear();
    
    if(destA.size()==2){
        staticA.push_back(AttrStruct((destA[1].p+destA[0].p)/2.0,destA[0].f));
    }

    vector<int> mirrorFam = getActiveFromInt(attrmirrorx);

    for(int i=0;i<mirrorFam.size();i++){
        for(int j = 0 ; j< destA.size();j++){
            if(destA[j].f==mirrorFam[i])
                staticA.push_back(AttrStruct(ofPoint(1-destA[j].p.x,destA[j].p.y,destA[j].p.z),destA[j].f));
        }

    }
    
    mirrorFam = getActiveFromInt(attrmirrory);
    
    for(int i=0;i<mirrorFam.size();i++){
        for(int j = 0 ; j< destA.size();j++){
            if(destA[j].f==mirrorFam[i])
                staticA.push_back(AttrStruct(ofPoint(1-destA[j].p.x,destA[j].p.y,destA[j].p.z),destA[j].f));
        }
        
    }
    
    mirrorFam = getActiveFromInt(attrmirrorz);
    
    for(int i=0;i<mirrorFam.size();i++){
        for(int j = 0 ; j< destA.size();j++){
            if(destA[j].f==mirrorFam[i])
                staticA.push_back(AttrStruct(ofPoint(1-destA[j].p.x,destA[j].p.y,destA[j].p.z),destA[j].f));
        }
        
    }
    
}

void AttrCtl::timedPoints(){
    
}

