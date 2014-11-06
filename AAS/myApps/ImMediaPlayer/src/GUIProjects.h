//
//  GUI.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#ifndef __ImMedia__GUIProjects__
#define __ImMedia__GUIProjects__

#include <stdio.h>
#include "GloveInteract.h"
#include "ofMain.h"
#include "ofxMtlWatchFolder.h"
#include "Config.h"



#include "ofxUI.h"



class GUIProjects{
    
    
    public :
    
    GUIProjects();
    virtual ~GUIProjects(){};
    ofxMtlWatchFolder watch;
    
    
    void init();
    void registerListeners();
    
    void draw(ofEventArgs & a);
    void update(ofEventArgs & a);
    
    void startWatch(string s="");
    void projectsAdded(string& filename);
    void projectsRemoved(string& filename);
    
    void GUIevent(ofxUIEventArgs & a);
    

    
    

    ofxUIDropDownList* projectsList;
    ofxUISuperCanvas* projectsCanvas;
    
    
};


#endif /* defined(__ImMedia__GUI__) */
