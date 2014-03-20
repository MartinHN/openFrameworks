//
//  Slicer.cpp
//  RTaudio
//
//  Created by martin hermant on 20/03/14.
//
//

#include "Slicer.h"



void ThresholdSlicer::SliceIt(vector<frame> d){
    slices.clear();
    bool isSlice = false;
    Slice tmps;
    tmps.tb = 0;
    tmps.te = 0;
    tmps.filepath = "null";
    tmps.familly = "thresholded";
    
    tmps.tb = 0;
    int dimension = d[0].data.size();
    for(vector<frame>::iterator p = d.begin() ; p!=d.end() ; ++p){
        float mean = std::accumulate(p->data.begin(),p->data.end(),0);
        mean/=dimension;
        if(isSlice ){
            if(mean<threshold){
                tmps.te = p->ts;
                slices.push_back(tmps);
            isSlice = false;
                             }
        }
        else{
            if(mean>threshold){
                tmps.tb = p->ts;
                isSlice = true;
            }
        }
    }
}

map<string, int> ThresholdSlicer::outputs(){

    map<string, int> res;
    
    res["mean"] =1;
    
    return res;
}

void ThresholdSlicer::registerParams(){
    MYPARAM(threshold,0,0,100);
}