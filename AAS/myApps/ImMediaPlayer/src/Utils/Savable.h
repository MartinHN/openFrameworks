//
//  Savable.h
//  ImMedia
//
//  Created by martin hermant on 17/11/14.
//
//

#ifndef ImMedia_Savable_h
#define ImMedia_Savable_h
#include "ofMain.h"

class Savable{
public:
    
    static vector<Savable*> registeredObjects;
    Savable();
    
    virtual ~Savable();
    ofParameterGroup * settings;
    
    
    // overridable method
    virtual void populateSettings(){};
    
    
    static void save(string filePath);
    static void load(string filePath);;
    
    static ofXml xml;
    static ofParameterGroup global;
    
};



#endif
