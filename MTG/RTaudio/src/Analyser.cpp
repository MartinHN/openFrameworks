//
//  Analyser.cpp
//  RTaudio
//
//  Created by martin hermant on 20/03/14.
//
//

#include "Analyser.h"




void DirectAnalyzer::analyze(vector<Slice> * v,Pooler * p){

    
    vector<string> ftn;
    ftn = p->getAxesNames();
    if(find(ftn.begin(),ftn.end(),feature1.get())!=ftn.end() && find(ftn.begin(),ftn.end(),feature2.get())!=ftn.end() && find(ftn.begin(),ftn.end(),feature3.get())!=ftn.end()){
    
    for(vector<Slice>::iterator vi = v->begin() ; vi != v->end() ; ++vi){
        vi->curpos.set(0);
        int id = vi->localid;
        
        
        for(int i = vi->originIdx;i<vi->endIdx ; i++ ){
            
            vi->curpos.x +=  p->at(id)->poolnd[feature1][0].data[i];
            vi->curpos.y +=  p->at(id)->poolnd[feature2][0].data[i];
            vi->curpos.z +=  p->at(id)->poolnd[feature3][0].data[i];

            
        }
        vi->curpos/=(vi->endIdx-vi->originIdx);
        vi->curpos.z = (vi->endIdx-vi->originIdx);
    }
    
    }
    else{ofLogWarning("analyzer :feature not found" + feature1.get() + feature2.get() + feature3.get());}
    
}


void DirectAnalyzer::registerParams(){
    Analyzer::registerParams();

    
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


bool AnalyzerH::analyzeIt(){

    if(curAnalyzer==NULL){
        ofLogWarning("analyserH : curanalyzer not found");
    }
    else if(curSlice==NULL){
        ofLogWarning("analyserH : curslicer not found");
    }
    else{
        if(curSlice->size()>0){
    curAnalyzer->analyze(curSlice,sH->pool);
            return true;
        }
        else ofLogWarning("analyzerH : not analyzing (empty slices)");
    }
    return false;
    
}


vector<string> AnalyzerH::getAnalyzerNames(){
    vector<string> res;
    for (int k = 0 ; k < analyzers.size() ; k++){
        res.push_back(analyzers[k]->settings.getName());
    }
    return res;

    
}
