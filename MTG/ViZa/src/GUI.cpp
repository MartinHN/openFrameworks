//
//  GUI.cpp
//  ViZa
//
//  Created by martin hermant on 14/10/14.
//
//

#include "GUI.h"


GUI * GUI::inst;


void GUI::setup(){
    
    int scrollW = 700;
    
    
    int ch = 0;
    int pad=50;
    guiconf = new ofxUISuperCanvas("Axes",0,0,700,100);
    guiconf->setName("Axes");
    ch+=guiconf->getRect()->height+pad;
    scrollNames = new ofxUIScrollableCanvas(0,ch,scrollW,400);
    
    scrollNames->setName("Songs");
    scrollNames->setScrollableDirections(false, true);
    if(Container::attributeNames.size()>0){
        
        for(vector<string>::iterator it = Container::attributeNames.begin() ; it != Container::attributeNames.end() ;++it){
            vector <string> nnn =ofSplitString(*it, ".");
            {
                bool found = false;
                for(vector<string>::iterator itt = attrNames.begin() ; itt!= attrNames.end() ; ++itt){
                    if(*itt==nnn[0]){
                        found = true;
                        break;
                    }
                }
                if(!found)attrNames.push_back(nnn[0]);
            }
            if(nnn.size()>1 ){
                bool found = false;
                for(vector<string>::iterator itt = aggrNames.begin() ; itt!= aggrNames.end() ; ++itt){
                    if(*itt==nnn[1]){
                        found = true;
                        break;
                    }
                }
             if(!found)aggrNames.push_back(nnn[1]);
            }
        }
        float ddSize = 100;
        
        vector<string> typescales;
        typescales.push_back("min/max");
        typescales.push_back("standard deviation");
        typescales.push_back("range");

        
        for(int i = 0 ; i < 3 ; i++){
            attr[i] = new ofxUIDropDownList("Attribute"+numToAxe(i), attrNames,150,0,0,OFX_UI_FONT_SMALL);
            aggr[i] =         new ofxUIDropDownList("Aggregate"+numToAxe(i), aggrNames,150,0,0,OFX_UI_FONT_SMALL);
            scaleType[i] =    new ofxUIDropDownList("scaleType"+numToAxe(i), typescales,150,0,0,OFX_UI_FONT_SMALL);
            min[i] =          new ofxUITextInput("min"+numToAxe(i),"",50);
            max[i] =          new ofxUITextInput("max"+numToAxe(i),"",50);
            guiconf->addWidgetDown(attr[i]);
            guiconf->addWidgetRight(aggr[i] );
            guiconf->addWidgetRight(scaleType[i] );
            guiconf->addWidgetRight(min[i] );
            guiconf->addWidgetRight(max[i] );
            
            attr[i]->getToggles()[i]->setValue(true);
            attr[i]->getToggles()[i]->triggerSelf();
            aggr[i]->getToggles()[0]->setValue(true);
            aggr[i]->getToggles()[0]->triggerSelf();
            scaleType[i]->getToggles()[1]->setValue(true);
            scaleType[i]->getToggles()[1]->triggerSelf();
            min[i]->setAutoClear(false);
            min[i]->setTriggerOnClick(false);
            max[i]->setAutoClear(false);
            max[i]->setTriggerOnClick(false);
        }


        for(map<string,vector<Container*> > ::iterator it = Container::songs.begin() ; it!=Container::songs.end() ; ++it){
            songnames.push_back(it->first);

        }
        scrollNames->addWidgetDown(new ofxUIDropDownList("songNames", songnames,0,0,0,OFX_UI_FONT_SMALL));
        ((ofxUIDropDownList*)scrollNames->getWidgetsOfType(OFX_UI_WIDGET_DROPDOWNLIST)[0])->open();
        
        ofxUIRectangle * r =((ofxUIDropDownList*)scrollNames->getWidgetsOfType(OFX_UI_WIDGET_DROPDOWNLIST)[0])->getRect();
        scrollNames->setSnapping(true);
        scrollNames->setDimensions(scrollW,songnames.size()*r->height);

        global = new ofxUITabBar();
        global->addCanvas(scrollNames);
        global->addCanvas(guiconf);
        

    }

    
    vector<ofxUIWidget*> ddls= guiconf->getWidgetsOfType(OFX_UI_WIDGET_DROPDOWNLIST);
    for(int i = 0 ; i < ddls.size(); i++){
        ((ofxUIDropDownList*) ddls[i])->setAutoClose(true);
        ((ofxUIDropDownList*) ddls[i])->setShowCurrentSelected(true);
    }

   	ofAddListener(guiconf->newGUIEvent,this,&GUI::guiEvent);
    ofAddListener(scrollNames->newGUIEvent,this,&GUI::guiEvent);
    
    Physics::maxs.addListener(this,&GUI::maxsChanged);
    Physics::mins.addListener(this,&GUI::minsChanged);
    
    for(int i = 0 ; i < 3;i++){
        attr[i]->getToggles()[i]->triggerSelf();
    }
    

}

void GUI::guiEvent(ofxUIEventArgs &e){
    string name = e.getName();
	int kind = e.getKind();
    
    ofxUICanvas * ee;
    bool hideothers;
    
    //Check modifications
    switch (kind) {
        case OFX_UI_WIDGET_SCROLLABLECANVAS:
            
            isModifiying = ((ofxUIScrollableCanvas*)e.widget)->isScrolling;
            break;
        case OFX_UI_WIDGET_DROPDOWNLIST:
        {hideothers = ((ofxUIDropDownList*)e.widget)->getValue();
            ee= (ofxUICanvas*)e.widget->getCanvasParent();
            if(ee!=NULL){
                vector<ofxUIWidget*> vv = ee->getWidgetsOfType(OFX_UI_WIDGET_DROPDOWNLIST);
                for(vector<ofxUIWidget*>::iterator it = vv.begin() ; it !=vv.end() ; ++it){
                    if(e.widget->getRect()->x ==  (*it)->getRect()->x && e.widget->getRect()->y <  (*it)->getRect()->y &&((ofxUIDropDownList*)*it)!=e.widget){
                        cout << (*it)->getName().substr(0,(*it)->getName().length()-1) << endl;
                        ((ofxUIDropDownList*)*it)->setVisible(!hideothers);
                    }
                }
            }
        }
            break;
        
        
        default:
            break;
    }
    
    
    
    string parentName = "";
    if(e.widget->getParent()!=NULL){
        parentName = e.widget->getParent()->getName();
    }
    if(parentName == "")return;
    
    
    cout << parentName << "//" << name <<endl;
    int axe = axeToNum(parentName[parentName.length()-1]);
    
    
    // attributes and aggregator modification
    if(axe!=-1)
        Physics::orderBy(attr[axe]->getSelected()[0]->getName()+"."+aggr[axe]->getSelected()[0]->getName(), axe, scaleType[axe]->getSelectedIndeces()[0]);
        
    // mins maxs modifications
    else if (parentName == "Axes" && kind == OFX_UI_WIDGET_TEXTINPUT){
       axe = axeToNum(name[name.length()-1]);
        string s =((ofxUITextInput*)e.widget)->getTextString();
        if(s=="")return;
        cout << "minmax" << s << endl;
        ofVec3f mask(axe==0?1:0,axe==1?1:0,axe==2?1:0);
        if(name.substr(0,name.length()-1)=="min"){
            
            Physics::mins = ofToFloat(s)*mask + (-mask+ofVec3f(1))*Physics::mins;

            Physics::orderBy(attr[axe]->getSelected()[0]->getName()+"."+aggr[axe]->getSelected()[0]->getName(), axe, scaleType[axe]->getSelectedIndeces()[0]);
        }
        if(name.substr(0,name.length()-1)=="max"){
            Physics::maxs = ofToFloat(s)*mask + (-mask+ofVec3f(1))*Physics::maxs;
            Physics::orderBy(attr[axe]->getSelected()[0]->getName()+"."+aggr[axe]->getSelected()[0]->getName(), axe, scaleType[axe]->getSelectedIndeces()[0]);
        }
    
}
    
else    if(parentName == "songNames"){
        Container::selectSong(name);
        
    }
        

    
    
    lastFramenum = ofGetFrameNum();
    
}

int GUI::axeToNum(char s){
    switch(s){
        case 'X':
            return 0;
        case 'Y':
            return 1;
        case 'Z':
            return 2;
        default:
            break;
    }
    return -1;
}
string GUI::numToAxe(int i){
    switch(i){
        case 0:
            return "X";
        case 1:
            return "Y";
        case 2:
            return "Z";
        default:
            break;
    }
    return "";
}


void GUI::maxsChanged(ofVec3f &v){
    ofVec3f dif = Physics::maxs.getLast() - v;
    cout << dif << endl;
    for(int i = 0 ; i < 3 ; i++){
        if(dif[i]!=0){
            max[i]->setTextString(ofToString(v[i]));
        }
    }
    
}




void GUI::minsChanged(ofVec3f &v){
    ofVec3f dif = Physics::mins.getLast() - v;
    for(int i = 0 ; i < 3 ; i++){
        if(dif[i]!=0){
            min[i]->setTextString(ofToString(v[i]));
        }
    }
    
}
