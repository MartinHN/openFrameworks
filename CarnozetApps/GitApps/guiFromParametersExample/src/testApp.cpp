#include "testApp.h"

	
//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);

    ofXml eS;
    if(eS.load(ofToDataPath("tst.xml"))){
        
        deserialize(&eS, &allParams);
    }
        
        
    gui = ofxPanel(allParams);
        
}
//

void testApp::deserialize(ofXml * x,ofParameterGroup * parent){
    int numC = x->getNumChildren();
    string name = x->getPocoElement()->tagName();
    
    if(numC>0){
        cout<<name+":"<<endl;
        pL.push_back(new ofParameterGroup());
        pL.back()->setName(name);
        parent->add(*pL.back());
        for(int i = 0;i<numC;i++){
            x->setToChild(i);
            deserialize(x,(ofParameterGroup *)pL.back());
        }
        x->setToParent();
    }
    else {
    
        float f = x->getValue<float>("");
        cout<<name +ofToString(f)<<endl;
        pL.push_back(new ofParameter<float>(f));
         pL.back()->setName(name);
        parent->add(*pL.back());
        x->setToParent();
        
    }


    
}

template <class C> void FreeClear( C & cntr ) {
    for ( typename C::iterator it = cntr.begin(); 
         it != cntr.end(); ++it ) {
    	delete * it;
    }
    cntr.clear();
}

//--------------------------------------------------------------
void testApp::exit(){
    FreeClear(pL);
//	ringButton.removeListener(this,&testApp::ringButtonPressed);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    
//	if( filled ){
//		ofFill();
//	}else{
//		ofNoFill();
//	}
//
//	ofSetColor(color);
//	if(twoCircles){
//		ofCircle(center->x-radius*.5, center->y, radius );
//		ofCircle(center->x+radius*.5, center->y, radius );
//	}else{
//		ofCircle((ofVec2f)center, radius );
//	}
	
	if( bHide ){
		gui.draw();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if( key == 'h' ){
		bHide = !bHide;
	}
	if(key == 's') {
		gui.saveToFile("settings.xml");
	}
	if(key == 'l') {
		gui.loadFromFile("settings.xml");
	}
//	if(key == ' '){
//		color = ofColor(255);
//	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
//    screenSize = ofToString(w) + "x" + ofToString(h);
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}
