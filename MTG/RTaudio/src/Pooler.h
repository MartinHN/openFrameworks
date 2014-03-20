//
//  Pooler.h
//  RTaudio
//
//  Created by martin hermant on 19/03/14.
//
//

#ifndef __RTaudio__Pooler__
#define __RTaudio__Pooler__

#include <iostream>
#include "ofMain.h"
#include "ofxJSONElement.h"
#include "Misc.h"



class LocalPool{
public:

    void create(string name,vector<frame>   inv);
    void add(string name,vector<ofVec2f> inv);
    void del(string s);
    vector<string> getNames(int dim);
    map<string,int> getNdNames();
    bool exists(string name);
    const vector<frame> operator [] (string s){return poolnd[s];};
    
    string filepath;
    std::map<string , vector< frame > > poolnd;
    
    std::map<string, int> features;
    
};

class Pooler{
public:
    
    
    void load(string fn);
    void Slice();
    
    void getAll(){};
    map<string,int> getAxes();
    vector<string> getAxesNames();
    vector<frame> operator [] (string s){vector<frame> res;
        for(vector<LocalPool>::iterator it  = globalPool.begin();it!=globalPool.end();++it){
            vector<frame> tmp  = (*it)[s];
            res.reserve(res.size()+tmp.size());
            res.insert(res.end(),tmp.begin(),tmp.end());
        }
        return res;
    };
    
    
    vector<LocalPool> globalPool;
    
};


template <class C> void FreeClear( C & cntr ) {
    for ( typename C::iterator it = cntr.begin();
         it != cntr.end(); ++it ) {
    	delete * it;
    }
    cntr.clear();
}



#endif /* defined(__RTaudio__Pooler__) */
