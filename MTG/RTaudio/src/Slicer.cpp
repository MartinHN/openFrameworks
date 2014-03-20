//
//  Slicer.cpp
//  RTaudio
//
//  Created by martin hermant on 20/03/14.
//
//

#include "Slicer.h"



void ThresholdSlicer::SliceIt(vector<frame> &d){
    slices.clear();
    bool isSlice = false;
    Slice tmps;
    tmps.tb = 0;
    tmps.te = 0;
    tmps.filepath = "null";
    tmps.familly = "thresholded";
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
                tmps.originIdx = curidx;
                slices.push_back(tmps);
            isSlice = false;
                             }
        }
        else{
            if(mean>threshold){
                tmps.tb = p->ts;
                tmps.endIdx = curidx;
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
}