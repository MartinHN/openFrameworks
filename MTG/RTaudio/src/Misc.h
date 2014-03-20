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
    string filepath;
    float tb;
    float te;
    
    string familly;
    
    vector<float> dim;
    float fclass;
    float lclass;
    
    
    
} Slice;


#endif
