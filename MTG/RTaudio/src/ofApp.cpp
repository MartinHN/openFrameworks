#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
//    zoomfactor = 4.;
    h=40;
    
    




    ofSetFrameRate(20);
    sH.setup(&pool);
    aH.setup(&sH);
    pool.load("tst");
    view.setup(&aH);
    
    
    sH.curSlicer = sH.slicers[0];
//    sH.curNovelty = pool[0]->at("envelope");
    sH.sliceIt();
    
    aH.curSlice = &sH.curSlicer->slices;
    aH.curAnalyzer = aH.analyzers[0];
    aH.analyzeIt();
    vector<string> files = pool.getFilePaths();
    for (int i = 0 ; i < files.size() ; i++){
        player.push_back(new Player(i,files[i]));

        
    }
    
//    player.load(json["filepath"][0].asString());
}

//--------------------------------------------------------------
void ofApp::update(){
    view.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    view.draw();
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 122:
            view.cam.enableMouseInput();
            cout<<"enable"<<endl;
            break;
            
        case 102:
            view.displayFeatures=!view.displayFeatures;
            break;
            
        default:
            break;

    }
    cout << key <<endl;


}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){


}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
  
//        for(int i = 0 ; i<slices.size();i++){
//            if(ofRectangle(slices[i].start*zoomfactor,ofGetHeight()/2-h/2 + h * slices[i].type,slices[i].length*zoomfactor,h).inside(x,y)&&i!=lastplayed){
//                player.play(slices[i].start, slices[i].length);
//                lastplayed = i;
//                break;
//            }
//        }
    

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if(view.hoverIdx>-1){
        
        Slice s =aH.curSlice->at(view.hoverIdx);
        
        string msg = "p "+ofToString(s.localid)+" play4 "+ofToString(s.tb)+" " +ofToString(s.te-s.tb);
        ofSendMessage(msg);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
//zoomfactor = ofGetWidth()/(0.9*slices.back().te);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
