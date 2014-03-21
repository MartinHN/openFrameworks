//
//  Slicer.cpp
//  RTaudio
//
//  Created by martin hermant on 20/03/14.
//
//

#include "Slicer.h"


void SlicerH::setup(Pooler * p){
    slicers.push_back(new ThresholdSlicer());
    pool = p;
    
    registerParams();
}
vector<string> SlicerH::getSlicerNames(){
    vector<string> res;
    for (int k = 0 ; k < slicers.size() ; k++){
        res.push_back(slicers[k]->settings.getName());
    }
    return res;
}
void SlicerH::registerParams(){
    for (int k = 0 ; k < slicers.size() ; k++){
        slicers[k]->registerParams();
    }
}

void SlicerH::sliceIt(string slicer,string novelty){

    Slicer * s = get(slicer);
    if(s==NULL){ofLogWarning("SlicerH : Slicer not found :" + slicer);}
    
    else{
            
        s->clear();
    
    
            for (int i = 0 ; i< pool->globalPool.size() ; i++){
                s->SliceIt(pool->globalPool[i].poolnd[novelty]);
            }

        
    
    }

}


Slicer * SlicerH::get(string n){
    for (int k = 0 ; k < slicers.size() ; k++){
        if( n==slicers[k]->settings.getName()) return slicers[k];
    }
    return NULL;
}

void Slicer::clear(){
   
    slices.clear();
}









void ThresholdSlicer::SliceIt(vector<frame> &d){
    
    bool isSlice = false;
    Slice tmps;
    tmps.tb = 0;
    tmps.te = 0;
    tmps.origin = &d;
    tmps.localid = d[0].localid;
    tmps.tb = 0;
    int dimension = d[0].data.size();
    int curidx = 0;
    for(vector<frame>::iterator p = d.begin() ; p!=d.end() ; ++p){
        float mean = 0;
        for (int k = 0 ;k< dimension ; k++){
            mean+=p->data[k];
        }

        mean/=dimension;
        if(isSlice ){
            if(mean<threshold){
                tmps.te = p->ts;
                tmps.endIdx = curidx;
                slices.push_back(tmps);
            isSlice = false;
                             }
        }
        else{
            if(mean>threshold){
                tmps.tb = p->ts;
                tmps.originIdx = curidx;
                isSlice = true;
            }
        }
        curidx++;}
}

map<string, int> ThresholdSlicer::outputs(){

    map<string, int> res;
    
    res["mean"] =1;
    
    return res;
}

void ThresholdSlicer::registerParams(){
    MYPARAM(threshold,0.01,0,100);
    settings.setName("threshold");
}