#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    zoomfactor = 4.;
    h=40;
    
    




    ofSetFrameRate(20);
    sH.setup(&pool);
    aH.setup(&sH);
    pool.load("tst");
    view.setup(&aH);
    sH.sliceIt("threshold", "envelope");
    aH.analyzeIt("Direct", "threshold");
    vector<string> files = pool.getFilePaths();
    for (vector<string>::iterator it = files.begin();it!= files.end() ; ++it){
        player.push_back(Player());
        player.back().load(*it);
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
        case 'r':
            
            break;
            
        default:
            break;

    }
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if(view.hoverIdx>-1){
        
        Slice s =aH.curslice->at(view.hoverIdx);

        string msg = "p "+ofToString(s.localid)+" play4 "+ofToString(s.tb)+" " +ofToString(s.te-s.tb);
        ofSendMessage(msg);
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
zoomfactor = ofGetWidth()/(0.9*slices.back().te);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
