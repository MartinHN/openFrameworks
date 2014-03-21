//
//  Misc.h
//  RTaudio
//
//  Created by martin hermant on 20/03/14.
//
//

#ifndef RTaudio_Misc_h
#define RTaudio_Misc_h


#define MYPARAM(nom,init,min,max) nom.setName(#nom);nom.setMin(min);nom.setMax(max);settings.add(nom);nom=init;

typedef struct{
    
    float ts;
    int localid;
    vector<float> data;
    
    
}frame;


typedef struct{
    
    string filepath;
    int localid;
    float tb;
    float te;
    
    vector<frame> * origin;
    int originIdx;
    int endIdx;
    
    ofVec3f curpos;
    
    
    float fclass;
    float lclass;
    
    
    
} Slice;






#endif
