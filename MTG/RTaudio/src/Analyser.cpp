//
//  Analyser.cpp
//  RTaudio
//
//  Created by martin hermant on 20/03/14.
//
//

#include "Analyser.h"


void DirectAnalyzer::Analyze(vector<Slice> &v){
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
    
    
    
}