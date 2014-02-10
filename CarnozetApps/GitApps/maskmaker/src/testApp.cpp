#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	snapCounter = 0;
	bSnapshot = false;
	cooper.loadFont("cooperBlack.ttf", 50);
	phase = 0;
	memset(snapString, 0, 255);		// clear the string by setting all chars to 0
}


//--------------------------------------------------------------
void testApp::update(){
	ofBackground(255,255,255);
}


//--------------------------------------------------------------
void testApp::draw(){
    ofBeginShape();
    for(int i=0;i< shapes.length();i++){
        shapes[i].draw();
    }
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	if (key == 'x'){
		bSnapshot = true;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//------------- -------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	for (int i = 0; i < nCurveVertices; i++){
		float diffx = x - curveVertices[i].x;
		float diffy = y - curveVertices[i].y;
		float dist = sqrt(diffx*diffx + diffy*diffy);
		if (dist < curveVertices[i].radius){
			curveVertices[i].bOver = true;
		} else {
			curveVertices[i].bOver = false;
		}	
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	for (int i = 0; i < nCurveVertices; i++){
		if (curveVertices[i].bBeingDragged == true){
			curveVertices[i].x = x;
			curveVertices[i].y = y;
		}
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	for (int i = 0; i < nCurveVertices; i++){
		float diffx = x - curveVertices[i].x;
		float diffy = y - curveVertices[i].y;
		float dist = sqrt(diffx*diffx + diffy*diffy);
		if (dist < curveVertices[i].radius){
			curveVertices[i].bBeingDragged = true;
		} else {
			curveVertices[i].bBeingDragged = false;
		}	
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
	for (int i = 0; i < nCurveVertices; i++){
		curveVertices[i].bBeingDragged = false;	
	}
}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
