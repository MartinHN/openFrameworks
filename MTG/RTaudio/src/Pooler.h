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


typedef struct{

    float ts;
    vector<float> data;
    
    
}frame;



class LocalPool{
public:

    void create(string name,vector<frame>   inv);
    void add(string name,vector<ofVec2f> inv);
    void del(string s);
    vector<string> getNames(int dim);
    map<string,int> getNdNames();
    bool exists(string name);
    
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
