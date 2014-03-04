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
    zonefamilly1.addListener(this,&AttrCtl::zoneChanged);
    zonefamilly2.addListener(this,&AttrCtl::zoneChanged);
    zonefamilly3.addListener(this,&AttrCtl::zoneChanged);
}

void AttrCtl::update(){
    smooth();
    staticpoints();
    timedPoints();
}

void AttrCtl::registerParam(){
    settings.setName("attrCtl");
    MYPARAM(isAttr,true,false,true);
    MYPARAM(zoffset,0.5f,0.f,1.f);
    MYPARAM(attrmirrorx,4,0,16);
    MYPARAM(attrmirrory,0,0,16);
    MYPARAM(attrmirrorz,0,0,16);

    MYPARAM(zonefamilly1,ofVec4f(0,0,1,1),ofVec4f(0),ofVec4f(1));
    MYPARAM(zonefamilly2,ofVec4f(0,0,1,1),ofVec4f(0),ofVec4f(1));
    MYPARAM(zonefamilly3,ofVec4f(0,0,1,1),ofVec4f(0),ofVec4f(1));
}


void AttrCtl::zoneChanged(ofVec4f & dumb){
    zones.clear();
    zones.push_back(ofRectangle(zonefamilly1.get().x, zonefamilly1.get().y, zonefamilly1.get().z, zonefamilly1.get().w));
    zones.push_back(ofRectangle(zonefamilly2.get().x, zonefamilly2.get().y, zonefamilly2.get().z, zonefamilly2.get().w));
    zones.push_back(ofRectangle(zonefamilly3.get().x, zonefamilly3.get().y, zonefamilly3.get().z, zonefamilly3.get().w));
}

void AttrCtl::clearPoints(){
    destA.clear();
}
void AttrCtl::addPoints(vector<ofPoint> curcentroids,int type){
    

    if(destA.size()>0){
        std::vector<AttrStruct>::iterator g = destA.begin();
        while(g!=destA.end()){
            if(g->type==type){g=destA.erase(g);}
            else{++g;}
        };
        lastA=destA;
    }


    for(int i = 0 ; i< curcentroids.size() ; i++){
        AttrStruct p = AttrStruct(ofPoint(curcentroids[i].x,curcentroids[i].y,curcentroids[i].z+zoffset),type,0);
        for(int k = 0 ; k< zones.size();k++){
            if(zones[k].inside(p.p)){
                p.zone=k+1;
                break;
            }
            
        }
        destA.push_back(p);
                
    }


    
}

vector<ofPoint> AttrCtl::getAll(){
    vector<ofPoint> res;
    if(isAttr){
    for(int i = 0 ; i< destA.size() ; i++){
        res.push_back(destA[i].p);
        
    }
    }
    return res;
}

vector<ofPoint> AttrCtl::getType(int f,int z){
    vector<ofPoint> res;
    vector<int> targetf;
    
    if(isAttr){
    
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
        if(destA[i].type==f){
            if(z==0 || zones[z-1].inside(destA[i].p))
            res.push_back(destA[i].p);
        }
        
    }
        
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
        staticA.push_back(AttrStruct((destA[1].p+destA[0].p)/2.0,destA[0].type,destA[0].zone));
    }

    vector<int> mirrorFam = getActiveFromInt(attrmirrorx);

    for(int i=0;i<mirrorFam.size();i++){
        for(int j = 0 ; j< destA.size();j++){
            if(destA[j].type==mirrorFam[i])
                staticA.push_back(AttrStruct(ofPoint(1-destA[j].p.x,destA[j].p.y,destA[j].p.z),destA[j].type,destA[j].zone));
        }

    }
    
    mirrorFam = getActiveFromInt(attrmirrory);
    
    for(int i=0;i<mirrorFam.size();i++){
        for(int j = 0 ; j< destA.size();j++){
            if(destA[j].type==mirrorFam[i])
                staticA.push_back(AttrStruct(ofPoint(destA[j].p.x,1-destA[j].p.y,destA[j].p.z),destA[j].type,destA[j].zone));
        }
        
    }
    
    mirrorFam = getActiveFromInt(attrmirrorz);
    
    for(int i=0;i<mirrorFam.size();i++){
        for(int j = 0 ; j< destA.size();j++){
            if(destA[j].type==mirrorFam[i])
                staticA.push_back(AttrStruct(ofPoint(destA[j].p.x,destA[j].p.y,1-destA[j].p.z),destA[j].type,destA[j].zone));
        }
        
    }
    
}

void AttrCtl::timedPoints(){
    
}

