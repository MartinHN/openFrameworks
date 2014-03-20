#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    zoomfactor = 4.;
    h=40;
    
    slicers.push_back(new ThresholdSlicer());
    analyzers.push_back(new DirectAnalyzer());
    
    
    
    for (int k = 0 ; k < slicers.size() ; k++){
        slicers[k]->registerParams();
    }
    for (int k = 0 ; k < analyzers.size() ; k++){
        analyzers[k]->registerParams();
    }

    ofSetFrameRate(20);


    pool.load("tst");
    slicers[0]->SliceIt(pool["envelope"]);
    analyzers[0]->Analyze(slicers[0]->slices);
    
    
    
    view.link2model(&slicers[0]->slices,&slicers,&analyzers);
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
