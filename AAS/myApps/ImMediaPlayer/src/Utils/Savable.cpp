//
//  Savable.cpp
//  ImMedia
//
//  Created by martin hermant on 17/11/14.
//
//

#include "Savable.h"

vector<Savable*> Savable::registeredObjects;
ofXml Savable::xml;
ofParameterGroup Savable::global;

Savable::Savable(){
    settings = new ofParameterGroup();
    registeredObjects.push_back(this);
    populateSettings();
    
};

Savable::~Savable(){
    vector<Savable*>::iterator it =  find(registeredObjects.begin(),registeredObjects.end(),this);
    if(it!=registeredObjects.end()){
        registeredObjects.erase(it);
    }
}




void Savable::save(string filePath){
    xml.clear();
    global.clear();
    global.setName("global");
    for(vector<Savable*>::iterator it =  registeredObjects.begin() ; it!= registeredObjects.end() ; ++it){
        (*it)->populateSettings();
        global.add(*(*it)->settings);
    }
    
    
    xml.serialize(global);
    cout << "saving" <<filePath << endl;
    xml.save(filePath+"/settings.xml");

}


void Savable::load(string filePath){
    global.clear();
    if(xml.load(filePath)){
    
    global.setName("global");
    for(vector<Savable*>::iterator it =  registeredObjects.begin() ; it!= registeredObjects.end() ; ++it){
        (*it)->populateSettings();
        global.add(*(*it)->settings);
    }
    
    
    xml.deserialize(global);
    }
    
 
}