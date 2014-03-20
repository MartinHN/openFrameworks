//
//  Pooler.cpp
//  RTaudio
//
//  Created by martin hermant on 19/03/14.
//
//

#include "Pooler.h"

void LocalPool::create(string s,vector <frame>  inv){
    poolnd[s] = inv;
    features[s] = 1;
    
}

vector<string> LocalPool::getNames(int dim){
    vector<string> res;
    for (map<string,int >::iterator p = features.begin() ; p!=features.end() ; ++p){
        if(dim == p->second){
        if(std::find(res.begin(),res.end(),p->first)!=res.end()) res.push_back(p->first);
            else ofLogWarning("recopy of nd features");
        }
    }
    return res;
}

map<string,int> LocalPool::getNdNames(){
    map<string,int> res;
    for (map<string,int >::iterator p = features.begin() ; p!=features.end() ; ++p){
        if(p->second>3){
            if(res.find(p->first)!=res.end()) res[p->first] = p->second;
            else ofLogWarning("recopy of nd features");
        }
    }
    return res;
    
}



bool LocalPool::exists(string name){
    return features.find(name)!=features.end();
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
            vector<frame> data;
            float fs = (*curf)["fs"].asFloat();
            bool isTimestamped = fs==0;
            string name = features[j];
            for (int k = 0 ; k<(*curf)["data"].size(); k++){
                frame d;
                Json::Value * curdata = &(*curf)["data"][k];
                if(isTimestamped) {
                    
                    d.ts = (*curdata)[0][0].asFloat();
                    curdata = &(*curdata)[1];
                }
                else {
                 d.ts=k*fs;
                    
                }
                int ds =curdata->size();
                
                
                if (ds ==0){
                    d.data.push_back((*curdata).asFloat());
                    
                }
                else{
                for( int ll = 0 ; ll< ds ; ll++ ){
                    
                    d.data.push_back((*curdata)[ll].asFloat());
                    
                }
                }

                
                
            }
            
            p.create(features[j],data);
            
        }
        globalPool.push_back(p);

    }
}

void Pooler::Slice(){
    
}

map<string,int> Pooler::getAxes(){
    map<string,int> res;
    for(vector<LocalPool>::iterator lp = globalPool.begin() ; lp!= globalPool.end() ; ++lp){
        for (map<string,int >::iterator p = lp->features.begin() ; p!=lp->features.end() ; ++p){
            if(res.find(p->first)==res.end()) res[p->first] = p->second;
            
        }
    }
    
    return res;
}

vector<string> Pooler::getAxesNames(){
    vector<string> res;
    map<string, int> tmp = getAxes();
    for (map<string,int >::iterator p = tmp.begin() ; p!=tmp.end() ; ++p){
        res.push_back(p->first);
        
    }

    
    return res;
}


