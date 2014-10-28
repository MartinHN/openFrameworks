//
//  Watcher.cpp
//  ImMedia
//
//  Created by Tinmar on 26/10/2014.
//
//

#include "Watcher.h"

vector<ofFile> Watcher::files;
string Watcher::folderPath = "/Users/Sha/Movies";

void Watcher::setup(){
    watch.allowExt("avi");
    watch.allowExt("mp4");
    watch.allowExt("mov");
    
    
    watch.allowExt("pdf");
    
    watch.allowExt("png");
    watch.allowExt("jpg");
    watch.allowExt("jpeg");
    
    watch.allowExt("mp3");
    watch.allowExt("wav");

//    watch.isRecursive = true;
    
    watch.start(folderPath, 2000);
    
    
    ofAddListener(watch.fileAdded, this, &Watcher::watchFolderFileAdded);
    ofAddListener(watch.fileRemoved, this, &Watcher::watchFolderFileRemoved);
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