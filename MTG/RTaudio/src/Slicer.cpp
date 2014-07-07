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

void SlicerH::sliceIt(){

            
        curSlicer->clear();
    
    
            for (int i = 0 ; i< pool->globalPool.size() ; i++){
                curSlicer->SliceIt(pool->at(i));
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









void ThresholdSlicer::SliceIt(LocalPool * lp){
    
    vector<frame> d = lp->poolnd[novelty];
    bool isSlice = false;
    Slice tmps;
    tmps.tb = 0;
    tmps.te = 0;
    tmps.origin = &lp->poolnd[novelty];
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
            if((invert&&mean>threshold) || (!invert&&mean<threshold)){
                tmps.te = p->ts;
                tmps.endIdx = curidx;
                slices.push_back(tmps);
            isSlice = false;
                             }
        }
        else{
            if((invert&&mean<threshold) || (!invert&&mean>threshold)){
                tmps.tb = p->ts;
                tmps.originIdx = curidx;
                isSlice = true;
            }
        }
        curidx++;}
}


void ThresholdSlicer::registerParams(){
    Slicer::registerParams();
    MYPARAM(threshold,0.01,0,1);
    MYPARAM(invert,false,false,true);
    settings.setName("threshold");
}