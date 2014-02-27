#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    zoomfactor = 4.;
    h=40;
    
    loadSlices("tst");
    player.load(json["filepath"][0].asString());
    ofSetFrameRate(20);
    zoomfactor = ofGetWidth()/(0.9*slices.back().start);

    
}

//--------------------------------------------------------------
void ofApp::update(){
   
}

//--------------------------------------------------------------
void ofApp::draw(){
    drawFromSlice();
}

void ofApp::loadSlices(string filename){
    json.open(filename);
    classesN = json["slice"].getMemberNames().size();
    slices.clear();
    
    
    for (int i  = 0 ; i < classesN ; i++){

        Json::Value * curslice  = &json["slice"][ofToString(i)][0];
        for (int j = 0 ; j < curslice->size();j++){
            slices.push_back(Slice((*curslice)[j][0].asDouble(),(*curslice)[j][1].asDouble()*zoomfactor,i));
           
        }
        //
    }
    cout <<slices.size()<<endl;

}


void ofApp::drawFromSlice(){
    
//    ofSetColor(0);
    for(int i = 0 ; i<slices.size();i++){
        ofSetColor(ofColor().fromHsb(slices[i].type*255.0/classesN,255,255));
        ofFill();
        ofSetLineWidth(.1);


        ofRect(slices[i].start*zoomfactor,ofGetHeight()/2-h/2 + h * slices[i].type,slices[i].length*zoomfactor,h);
                ofNoFill();
        ofSetColor(0);
        ofRect(slices[i].start*zoomfactor,ofGetHeight()/2-h/2 + h * slices[i].type,slices[i].length*zoomfactor,h);
    }

    //         cout<<json["slice"][ofToString(i)][0][0]<<"\n"<<endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'r':
            loadSlices("tst");
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
    
  
        for(int i = 0 ; i<slices.size();i++){
            if(ofRectangle(slices[i].start*zoomfactor,ofGetHeight()/2-h/2 + h * slices[i].type,slices[i].length*zoomfactor,h).inside(x,y)&&i!=lastplayed){
                player.play(slices[i].start, slices[i].length);
                lastplayed = i;
                break;
            }
        }
    

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
zoomfactor = ofGetWidth()/(0.9*slices.back().start);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
