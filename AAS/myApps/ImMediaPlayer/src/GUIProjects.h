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
#include "ProjectBox.h"

class GUIProjects: public GloveInteract{
    
    
    public :
    
    GUIProjects();
    ofxMtlWatchFolder watch;
    vector<ProjectBox*> projects;
    
    void draw(ofEventArgs & a);
    void update(ofEventArgs & a);
    
    void startWatch(string s="");
    void projectsAdded(string& filename);
    void projectsRemoved(string& filename);
    
    
//    hide(bool t);
    
    
};


#endif /* defined(__ImMedia__GUI__) */
