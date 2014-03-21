//
//  Analyser.cpp
//  RTaudio
//
//  Created by martin hermant on 20/03/14.
//
//

#include "Analyser.h"




void DirectAnalyzer::analyze(vector<Slice> &v){
    ofVec2f bx(999999,-999999),by(999999,-999999),bz(999999,-999999);
    for(vector<Slice>::iterator vi = v.begin() ; vi != v.end() ; ++vi){
        vi->curpos.set(0);
        
        for(int i = vi->originIdx;i<vi->endIdx ; i++ ){
            vi->curpos.x +=  vi->origin->at(i).data[x];
            vi->curpos.y +=  vi->origin->at(i).data[y];
            vi->curpos.z +=  vi->origin->at(i).data[z];
            
        }
    }
    
}


void DirectAnalyzer::registerParams(){
    MYPARAM(x, 0, 0, 10);
    MYPARAM(y, 1, 0, 10);
    MYPARAM(z, 2, 0, 10);
    
    settings.setName("Direct");
    
    
}

void AnalyzerH::setup(SlicerH * s){
    sH = s;
    analyzers.push_back(new DirectAnalyzer());
    registerParams();
}


void AnalyzerH::registerParams(){
    for (int k = 0 ; k < analyzers.size() ; k++){
        analyzers[k]->registerParams();
    }
}

Analyzer * AnalyzerH::get(string n){
    for (int k = 0 ; k < analyzers.size() ; k++){
        if( n==analyzers[k]->settings.getName()) return analyzers[k];
    }
    return NULL;
}


void AnalyzerH::analyzeIt(string aname,string sname){
    Analyzer * a = get(aname);
    Slicer * s = sH->get(sname);
    if(a==NULL){
        ofLogWarning("analyserH : analyzer not found : "+aname);
    }
    else if(s==NULL){
        ofLogWarning("analyserH : slicer not found : "+sname);
    }
    else{
    a->analyze(s->slices);
    
    curslice = &s->slices;
    }
    
}