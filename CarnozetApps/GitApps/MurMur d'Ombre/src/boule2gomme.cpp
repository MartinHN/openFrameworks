//
//  boule2gomme.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 04/02/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "boule2gomme.h"



boule2gomme::boule2gomme(VisuHandler * v):VisuClass(v){
    MYPARAM(rayon,2,0,100);
    MYPARAM(center,ofVec2f(0.5,0.5),ofVec2f(0),ofVec2f(1));
    settings.setName("boule2gomme");
}


void boule2gomme::update(int w, int h){

    
}

void boule2gomme::draw(int w, int h){

    ofSetColor(255);
    dad->pipePP.dst->draw(0,300);
//    ofVec2f scale(w,h);
//    vector<AttrStruct> * points =  &dad->attr->staticA;
//     vector<AttrStruct> * opoints =  &dad->attr->destA;
//    ofSetColor(255,0,0);
////    ofRect(0,0,w,h);
//    ofFill();
//    for(int i = 0 ; i<points->size();i++)
//        ofEllipse(scale*points->at(i).p,rayon,rayon);
//    for(int i = 0 ; i<opoints->size();i++){
//        ofSetColor(opoints->at(i).type==0?255:0, opoints->at(i).type==1?255:0, opoints->at(i).type==2?255:0);
//        ofNoFill();
//        ofEllipse(scale*opoints->at(i).p,rayon,rayon);
//    }
    
    ofRect(50,50,50,50);
//    dad->bH->blobClient.draw(600,0);
//    dad->bH->gs.draw(320,240);
    
//    dad->syphonTex.src->draw(center.get().x*w,center.get().y*h,scale.x/2,scale.y/2);
    //dad->bH->syphonTex.src->draw(0,0,w/2,h/2);
//    ofTranslate(0,h/2);
//    ofSetColor(0,255,0);
//    vector<ofPolyline> pp = dad->bH->getBlobs(w/2,h/2);
//    int max = pp.size();
//    for(int i = 0 ; i<max ;i++){
//        ofPolyline tt = pp[i];
//        ofNoFill();
//        ofRect(pp[i].getBoundingBox());
////        tt.simplify(30);
////        tt = tt.getResampledByCount(10);
//        for (int j = 0 ; j<tt.size();j++){
//            if(j==0)ofSetColor(255,0,0);
//            else ofSetColor(0,255,0);
//            ofEllipse(tt[j],rayon,rayon);
//        }
//        
//    }
//    
//    ofSetColor(255,0,0);
//    vector<ofVec3f> po = dad->bH->arms;
//    for (int  i = 0 ; i< po.size();i++){
//
//        ofRect(po[i], 2*rayon,2* rayon);
//    }

}
