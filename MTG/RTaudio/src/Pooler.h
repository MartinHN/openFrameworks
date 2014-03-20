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




class LocalPool{
public:

    void create(string name,vector<ofVec2f> inv);
    void add(string name,vector<ofVec2f> inv);
    void del(string s);
    vector<string> get1dNames();
    bool exists(string name);
    
    string filepath;
    std::map<string , vector<ofVec2f> > pool1d;
    std::map<string , vector<ofVec2f> > pool2d;
    std::map<string , vector< vector<float> > > poolnd;
    
};

class Pooler{
public:
    
    
    void load(string fn);
    
    void getAll(){};
    vector<string> getAxes();
    
    
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
