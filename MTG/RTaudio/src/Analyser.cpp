//
//  Analyser.cpp
//  RTaudio
//
//  Created by martin hermant on 20/03/14.
//
//

#include "Analyser.h"




void DirectAnalyzer::analyze(vector<Slice> &v){

    int dim = v[0].origin->at(0).data.size();
    for(vector<Slice>::iterator vi = v.begin() ; vi != v.end() ; ++vi){
        vi->curpos.set(0);
        
        for(int i = vi->originIdx;i<vi->endIdx ; i++ ){
            vi->curpos.x +=  vi->origin->at(i).data[x];
            vi->curpos.y += dim>1?vi->origin->at(i).data[y] : 0;
            vi->curpos.z +=  dim>2 ? vi->origin->at(i).data[z]:0;
            
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
        if(s->slices.size()>0){
    a->analyze(s->slices);
    curslice = &s->slices;
        }
        else ofLogWarning("analyzerH : analyzing empty slices");
    }
    
}


vector<string> AnalyzerH::getAnalyzerNames(){
    vector<string> res;
    for (int k = 0 ; k < analyzers.size() ; k++){
        res.push_back(analyzers[k]->settings.getName());
    }
    return res;

    
}
