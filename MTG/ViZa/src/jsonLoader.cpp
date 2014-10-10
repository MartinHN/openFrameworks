//
//  jsonLoader.cpp
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#include "jsonLoader.h"


jsonLoader* jsonLoader::inst = NULL;
int jsonLoader::globalCount = 0;





void jsonLoader::loadSegments(string audiopath,string segpath){
    
    if(audiopath==""){
        audiopath = "/Users/mhermant/Documents/Work/Datasets/beatles/audio/wav";
        segpath = "/Users/mhermant/Documents/Work/Datasets/beatles/annotations/segmentation/";
    }
    
    ofDirectory ad =ofDirectory(audiopath);
    ad.allowExt("wav");
    ad.listDir();
    vector<ofFile> audioL = ad.getFiles();

    ad = ofDirectory(segpath);
    ad.allowExt("seg");
    ad.listDir();
    vector<ofFile> segL = ad.getFiles();

    
    
    
    std::map<string,string> mapL;
    
    for(int i = 0 ; i < audioL.size();i++){
            for(int j = 0 ; j < segL.size();j++){
                if(audioL[i].getBaseName()==segL[j].getBaseName()){
                    mapL[audioL[i].path()] = segL[j].path();
                    break;
                }
            }
    }
    
    int j = 0;
    for(std::map<string, string>::iterator p=mapL.begin();p!= mapL.end();++p){
        wng::ofxCsv csv;
        csv.loadFile(p->second, "\t");
        
        for(int i = 0 ; i < csv.numRows ; i++){
            Container::containers.push_back(Container(p->first, csv.getFloat(i, 0), csv.getFloat(i, 1),j));
            j++;
            
            
        }
        cout << j << endl;
        globalCount++;
       
    }
    
}


bool jsonLoader::audioExt(const ofFile & f){
    return f.getExtension() == ".wav";
    
}

bool jsonLoader::segExt(const ofFile & f){
    return f.getExtension() == ".csv";
}





jsonLoader * jsonLoader::instance(){
    if(inst==NULL){ inst =new  jsonLoader;
       }
        return inst;
    
}
