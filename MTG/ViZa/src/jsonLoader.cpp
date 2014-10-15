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
        segpath = "/Users/mhermant/Documents/Work/Datasets/beatles/viza/";
    }
    
    ofDirectory ad =ofDirectory(audiopath);
    ad.allowExt("wav");
    ad.listDir();
    vector<ofFile> audioL = ad.getFiles();

    ad = ofDirectory(segpath);
    ad.allowExt("seg");
    ad.allowExt("csv");
    ad.allowExt("lab");
    ad.allowExt("json");
    
    ad.listDir();
    vector<ofFile> segL = ad.getFiles();

    
    
    
    std::map<ofFile,ofFile> mapL;
    
    for(int i = 0 ; i < audioL.size();i++){
            for(int j = 0 ; j < segL.size();j++){
                if(audioL[i].getBaseName()==segL[j].getBaseName()){
                    mapL[audioL[i]] = segL[j];
                    break;
                }
            }
    }
    
    int j = 0;

    for(std::map<ofFile, ofFile>::iterator p=mapL.begin();p!= mapL.end();++p){
        wng::ofxCsv csv;
        if(p->second.getExtension() =="seg"){
        csv.loadFile(p->second.path(), "\t");
        
        for(int i = 0 ; i < csv.numRows ; i++){
            Container::containers.push_back(Container(p->first.path(), csv.getFloat(i, 0), csv.getFloat(i, 1),j));
            j++;
            
            
        }
            
            
        }
        if(p->second.getExtension() =="json"){
            ofxJSONElement json;
            json.open(p->second.path());
            
            

            map<string,vector<float> > onsets = crawl(json.get("onsets",NULL));
            
            if(onsets["slice.time"].size()>1){
                int ii = 0;
            for(vector<float>::iterator it  = onsets["slice.time"].begin()+1 ; it!= onsets["slice.time"].end() ; ++it){
                 Container::containers.push_back(Container(p->first.path(), *(it-1),*it,j));
                
                for(map<string,vector<float> >::iterator itt=onsets.begin();itt!=onsets.end() ; ++itt){
                    if(itt->first!="slice.time"){
                        Container::containers.back().setAttribute(itt->first,itt->second[ii]);
                    }
                    else{
                        Container::containers.back().setAttribute("length",*it - *(it-1));
                    }
                    
                }
                Container::containers.back().setAttribute("songIdx",globalCount);
                
                j++;
                ii++;
            }
            }
        
        }
        

        
       
        
        globalCount++;
       
    }
    
}


map<string,vector<float> > jsonLoader::crawl(Json::Value j){
    
    map<string,vector<float> >  RES;
    
    for (Json::Value::iterator it = j.begin() ; it != j.end() ; ++it ){

        string attrname =it.memberName();
                for (Json::Value::iterator itt = (*it).begin() ; itt != (*it).end() ; ++itt ){
                            string attrtype =itt.memberName();
                    for (Json::Value::iterator ittt = (*itt).begin() ; ittt != (*itt).end() ; ++ittt ){
                        RES[attrname+"."+attrtype].push_back((*ittt).asFloat());
                    }
                    
                }
    
    }
               return RES;
}




jsonLoader * jsonLoader::instance(){
    if(inst==NULL){ inst =new  jsonLoader;
       }
        return inst;
    
}
