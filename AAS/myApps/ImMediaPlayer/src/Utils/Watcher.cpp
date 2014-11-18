//
//  Watcher.cpp
//  ImMedia
//
//  Created by Tinmar on 26/10/2014.
//
//

#include "Watcher.h"
#include "Config.h"


vector<ofFile> Watcher::files;
string Watcher::folderPath = "/Users/Sha/Movies";

void Watcher::setup(){
    
    for(vector<string>::iterator it = supported_formats.begin() ; it !=supported_formats.end();++it){
    watch.allowExt(*it);
    }


//    watch.isRecursive = true;
    
    watch.start(folderPath, 1000);
    
    
    ofAddListener(watch.fileAdded, this, &Watcher::watchFolderFileAdded);
    ofAddListener(watch.fileRemoved, this, &Watcher::watchFolderFileRemoved);
}


void Watcher::changePath(string path){
    if(watch.lock()){
        
    }
    
}



void Watcher::watchFolderFileAdded(string & s){
    
    files.push_back(ofFile(s));
    
}


void Watcher::watchFolderFileRemoved(string & s){
    for( vector<ofFile>::iterator it = files.begin() ; it!= files.end() ; ++it){
        if(it->path() == s){
            files.erase(it++);
            break;
        }
    }
 
    
}