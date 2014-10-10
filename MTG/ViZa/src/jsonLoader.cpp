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
            vector<string> attrs = json.getMemberNames();
            for(vector<string>::iterator it = attrs.begin();it!=attrs.end();++it ){
                string cur = *it;
                vector<string> subnames=json[cur].getMemberNames();
                for(vector<string>::iterator itt = subnames.begin();itt!=subnames.end();++itt ){
                    if(json[cur][*itt].getMemberNames()[0]=="aggregate"){
                        
                    }
                }
            }
        
        }
        
        if(p->second.getExtension() =="yml"){
//            yaml_parser_t parser;
//            yaml_event_t event;
//            FILE *input = fopen(p->second.path().c_str(), "rb");
//            yaml_parser_set_input_file(&parser, input);
//            int done = 0;
//            /* Read the event sequence. */
//            while (!done) {
//                
//                /* Get the next event. */
//                if (!yaml_parser_parse(&parser, &event))
//                    break;
//                
//                /*
//                 ...
//                 Process the event.
//                 ...
//                 */
//                
//                /* Are we finished? */
//                done = (event.type == YAML_STREAM_END_EVENT);
//                
//                /* The application is responsible for destroying the event object. */
//                yaml_event_delete(&event);
//                
//            }
//            
//             if(!done)ofLogError("fuckYaml");
//            
//            /* Destroy the Parser object. */
//            yaml_parser_delete(&parser);
//            
//            
//            yaml_parser_set_input_file(&parser, input);
        }
       
        cout << j << endl;
        globalCount++;
       
    }
    
}







jsonLoader * jsonLoader::instance(){
    if(inst==NULL){ inst =new  jsonLoader;
       }
        return inst;
    
}
