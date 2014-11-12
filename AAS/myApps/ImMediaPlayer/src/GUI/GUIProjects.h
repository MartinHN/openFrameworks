//
//  GUI.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

// this class is responsible for GUI handling
// normal touch events are recieved from GUIGloveWrapper
// specific action can be listened too from GloveInteractBox inheritance
// GloveInteractBox is for handling drawlayer and collisions


#ifndef __ImMedia__GUIProjects__
#define __ImMedia__GUIProjects__

#include <stdio.h>
#include "GloveInteract.h"
#include "ofMain.h"
#include "ofxMtlWatchFolder.h"
#include "Config.h"
#include "MediaPool.h"


#include "ofxUI.h"



class GUIProjects : public GloveInteractBox{
    
    
    public :
    
    GUIProjects();
    ~GUIProjects(){};
    ofxMtlWatchFolder watch;
    static ofFile currentDirectory;
    
    ofxUIScrollableCanvas * projectsListScrollCanvas;
    ofxUIDropDownList* projectsList;
    
    ofxUIImageButton* backButton;
    
    
    ofxUISuperCanvas* projectsCanvas;
    
    bool isProjectOpened=false;
    
    void init();
    void registerListeners();
    
    virtual void draw(ofEventArgs & a);
    virtual void update(ofEventArgs & a);
    
    void startWatch(string s="");
    void projectsAdded(string& filename);
    void projectsRemoved(string& filename);
    
    void GUIevent(ofxUIEventArgs & a);
    void setCurrentDirectory(string  path);
    static bool isProject(ofFile d);
    
    
    
protected:
    // as we change the content of dropdownlist of project on its own GUIEvents, we have to do it asynchronously to let all current events (toggles) notifying before changing them
    string asyncPath = "";

    
    
};


#endif /* defined(__ImMedia__GUI__) */
