//
//  Pooler.cpp
//  RTaudio
//
//  Created by martin hermant on 19/03/14.
//
//

#include "Pooler.h"

void LocalPool::create(string s,vector<ofVec2f> inv){
    pool1d[s] = inv;
    
}

vector<string> LocalPool::get1dNames(){
    vector<string> res;
    for (map<string,vector<ofVec2f> >::iterator p = pool1d.begin() ; p!=pool1d.end() ; ++p){
        if(std::find(res.begin(),res.end(),p->first)!=res.end()) res.push_back(p->first);
    }
    return res;
}


bool LocalPool::exists(string name){
    return pool1d.find(name)!=pool1d.end();
}














void Pooler::load(string s){
    ofxJSONElement json;
    json.open(s);


    vector<string>  names = json["corpus"].getMemberNames();

    for (vector<string>::iterator n = names.begin() ; n!=names.end() ; ++n){
        LocalPool p;
        Json::Value * curfile  =&json["corpus"][n->c_str()];
        p.filepath = (*curfile)["path"][0].asString();
        vector<string> features = (*curfile)["features"].getMemberNames();
        
        int numFeatures = features.size();
        for (int j = 0; j <numFeatures ; j++){
            Json::Value * curf  = &(*curfile)["features"][features[j]];
            vector<ofVec2f> data;
            float fs = (*curf)["fs"].asFloat();
            bool isTimestamped = fs==0;
            for (int k = 0 ; k<(*curf)["data"].size(); k++){
                ofVec2f d;
                if(isTimestamped)d.set((*curf)["data"][0].asFloat(),(*curf)["data"][1].asFloat());
                else d.set(k*fs,(*curf)["data"][0].asFloat());
                data.push_back(d);
                
            }
            
            p.create(features[j],data);
            
        }
        globalPool.push_back(p);

    }
    
}

vector<string> Pooler::getAxes(){
    vector<string> res;
    for(vector<LocalPool>::iterator lp = globalPool.begin() ; lp!= globalPool.end() ; ++lp){
        for (map<string,vector<ofVec2f> >::iterator p = lp->pool1d.begin() ; p!=lp->pool1d.end() ; ++p){
            if(std::find(res.begin(),res.end(),p->first)==res.end()) res.push_back(p->first);
        }
        
        
    }
    
    return res;
}