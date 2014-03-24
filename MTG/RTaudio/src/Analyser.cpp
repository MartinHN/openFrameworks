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
    
    for(vector<Slice>::iterator vi = v->begin() ; vi != v->end() ; ++vi){
        vi->curpos.set(0);
        int id = vi->localid;
        ftn = p->getAxesNames();
        
        
        for(int i = vi->originIdx;i<vi->endIdx ; i++ ){
            
            vi->curpos.x +=  p->at(id)->poolnd[ftn[feature1]][0].data[i]/1000.;
            vi->curpos.y +=  p->at(id)->poolnd[ftn[feature2]][0].data[i]/1000.;
            vi->curpos.z +=  p->at(id)->poolnd[ftn[feature3]][0].data[i]/1000.;

            
        }
    }
    
    
    
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


void AnalyzerH::analyzeIt(){

    if(curAnalyzer==NULL){
        ofLogWarning("analyserH : curanalyzer not found");
    }
    else if(curSlice==NULL){
        ofLogWarning("analyserH : curslicer not found");
    }
    else{
        if(curSlice->size()>0){
    curAnalyzer->analyze(curSlice,sH->pool);
    
        }
        else ofLogWarning("analyzerH : not analyzing (empty slices)");
    }
    
}


vector<string> AnalyzerH::getAnalyzerNames(){
    vector<string> res;
    for (int k = 0 ; k < analyzers.size() ; k++){
        res.push_back(analyzers[k]->settings.getName());
    }
    return res;

    
}
