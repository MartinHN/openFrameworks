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

    surech = 9;
    attrotate=0;
    //echcoef = (float*)malloc(surech*sizeof(float));
    echcoef[0]  = 1/2.0;
    echcoef[1]  =3/4.0;
    echcoef[2]  =1/4.0;
    echcoef[3]  =7/8.0;
    echcoef[4]  =1/8.0;
    echcoef[5]  =3/8.0;
    echcoef[6]  =5/8.0;
    echcoef[7]  =3/8.0;
    echcoef[8]  =7/8.0;
    curech=0;
    
    attr = (float*)calloc(MAXATTRACTORS, 5*sizeof(float));
    
    for(int i=0;i<3;i++){
        attrmirorx[i]=false;
        attrmirory[i]=false;
    }
    numattr=0;
    lastp = vector<ofPoint>();
    destp = vector<ofPoint>();
    


   
    
}

void AttrCtl::registerParam(){
    settings.setName("attrCtl");
    
    zoffset.setName("zoffset");
    zoffset.setMin(0.);
    zoffset.setMax(1.);
    zoffset=0.5;
    settings.add(zoffset);
}

void AttrCtl::update(vector<ofPoint> curpin,vector<int> famillyin){
    
  if(destp.size()>0)lastp=destp;
    destp.clear();
     if(familly.size()>0)lastfamilly=familly;
    familly=famillyin;
    for(int i = 0 ; i< MIN(MAXATTRACTORS,curpin.size()) ; i++){
if(attrotate!=0){
        destp.push_back(ofPoint(attrotate*(curpin[i].y),(1-curpin[i].x),curpin[i].z+zoffset));
    }
else{
    destp.push_back(ofPoint(1-curpin[i].x,curpin[i].y,curpin[i].z+zoffset));
}
        
        
     }
    //cout<<curech<<endl;   
    curech=0;
    
 
    
}

vector<ofPoint> AttrCtl::getFamilly(int f){
    vector<ofPoint> res;
    for(int i = 0 ; i< curp.size() ; i++){
        if(familly[i]==f)res.push_back(curp[i]);
    }
    return res;
}



void AttrCtl::smooth(){
    
    curp.clear();
    newfamilly.clear();
    
    for(int i=0;i<destp.size();i++){
        bool isdetected=false;
//        for(int j=0;j<lastp.size();j++){
//            if((destp[j]-lastp[i]).length()<1/8.0){
//                if(curech<=surech){
//                    newfamilly.push_back(lastfamilly[i]);
//                    curp.push_back(lastp[i]+(destp[j]-lastp[i])*echcoef[curech]);//(1+curech)*1.0/(surech+1));//
//
//                    isdetected=true;}          
//                break;          
//            }          
//        }
        if(!isdetected){
            newfamilly.push_back(familly[i]);
            curp.push_back(destp[i]);
        }
        
        
    }
    familly.clear();
    for(int i = 0 ; i< newfamilly.size();i++){
    familly.push_back(newfamilly[i]);
    }
    curech++;


}

void AttrCtl::addpoints(){
    addpoint.clear();
    addfamilly.clear();
    
    for(int i = 0;i<curp.size();i++){
        addpoint.push_back(curp[i]);
        addfamilly.push_back(familly[i]);
    }
    
    
    if(curp.size()==2){
        addpoint.push_back((curp[1]+curp[0])/2.0);
        addfamilly.push_back(2);
    }
int idx = addpoint.size();
    for(int i=0;i<idx;i++){
        if(attrmirorx[addfamilly[i]]){
            addpoint.push_back(ofPoint(1-addpoint[i].x,addpoint[i].y,addpoint[i].z));
            addfamilly.push_back(addfamilly[i]);
        }

    }
    idx = addpoint.size();
      for(int i=0;i<idx;i++){
    if(attrmirory[addfamilly[i]]){
        addpoint.push_back(ofPoint(addpoint[i].x,1-addpoint[i].y,addpoint[i].z));
        addfamilly.push_back(addfamilly[i]);
    }
      }
    
}

void AttrCtl::setattr(){

    numattr = MIN(MAXATTRACTORS,addpoint.size());


    for(int i = 0; i< numattr;i++){
        attr[5*i]=addfamilly[i];
        attr[5*i+1]=addpoint[i].x;
        attr[5*i+2]=addpoint[i].y;
        attr[5*i+2]=addpoint[i].z;
    }
    
    

}
