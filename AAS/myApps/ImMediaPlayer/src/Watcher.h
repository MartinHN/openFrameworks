//
//  Watcher.h
//  ImMedia
//
//  Created by Tinmar on 26/10/2014.
//
//

#ifndef __ImMedia__Watcher__
#define __ImMedia__Watcher__
#include "ofxMtlWatchFolder.h"


class Watcher {


public:

    
    
    ofxMtlWatchFolder watch;
    
    
    void setup();
    
    void watchFolderFileAdded(string& filename);
    void watchFolderFileRemoved(string& filename);
    
    static vector<ofFile> files;
    
    
    private :
    static string folderPath ;
    
};



#endif /* defined(__ImMedia__Watcher__) */
