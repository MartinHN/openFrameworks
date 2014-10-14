//
//  Container.cpp
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#include "Container.h"
//#include "ofxTweener.h"





vector<Container> Container::containers;
map<string,vector<Container*> > Container::songs;
string Container::selectedSong;




float Container::radius = 10;
ofFloatColor Container::stateColor[]= {ofFloatColor::black, ofFloatColor::white, ofFloatColor::whiteSmoke };


void Container::registerListener(){
    for(int i = 0 ; i < containers.size() ; i++){
        containers[i].state = ofParameter<float>();
        
        containers[i].state.addListener(&containers[i], &Container::setState);
        containers[i].state = 0;
    }
    songs.clear();
    for(vector<Container>::iterator it = containers.begin() ; it!= containers.end() ; ++it){
        songs[it->filename].push_back(&(*it));
    }
}


void Container::setState(float & s){
    
    Physics::updateOneColor(index,stateColor[(int)s]);
    if(s<=1)AudioPlayer::instance()->Play(*this,(int)s);
//    AudioPlayer::instance()->Play(index,path,begin,end,state);
    
}

void Container::selectSong(string name){
    if(selectedSong!=""){
        vector<Container*> conts = Container::songs[selectedSong];
        for(vector<Container*>::iterator it = conts.begin() ; it!=conts.end() ; ++it){
            Physics::updateOneColor((*it)->index,stateColor[0]);
        }
    }
    selectedSong = name;
    if(selectedSong!=""){
        vector<Container*> conts = Container::songs[selectedSong];
        for(vector<Container*>::iterator it = conts.begin() ; it!=conts.end() ; ++it){
            Physics::updateOneColor((*it)->index,stateColor[2]);
        }
    }
}








