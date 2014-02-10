#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    numEcran = 3;
    ofPolyline poly;
    
    //Ecran carré jardin
    poly.lineTo(100, 600);
    poly.lineTo(200, 600);
    poly.lineTo(200, 700);
    poly.lineTo(100, 700);
    poly.close();
    listOfPoly.push_back(poly);
    poly.clear();
    
    
    //Ecran triangle toit
    poly.lineTo(100, 100);
    poly.lineTo(200, 100);
    poly.lineTo(200, 200);
    poly.lineTo(100, 200);
    poly.close();
    listOfPoly.push_back(poly);
    poly.clear();
    
    //Ecran rectangle cour
    poly.lineTo(900, 100);
    poly.lineTo(950, 100);
    poly.lineTo(950, 700);
    poly.lineTo(900, 700);
    poly.close();
    listOfPoly.push_back(poly);
    poly.clear();
    
    
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(0);
    
    ofSetColor(255);
    ofFill();
    
    pa.clear();
    
    
    for(int i=0; i<listOfPoly.size(); i++)
    {
        
        ofPolyline poly = listOfPoly.at(i);
        
        for(int j=0; j<poly.size(); j++)
        {
            
            ofPoint p = poly[j];
            
            pa.lineTo(p);
            
            
        }
        
        pa.setFillColor(255);
        pa.close();
        
        
        
        
        
        
        
    }
    pa.draw();
    
    
}

//--------------------------------------------------------------
void testApp::savePng(){
    
    ofSaveScreen("mask.png");
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch (key) {
        case 'x':
            savePng();
            break;
            
        default:
            break;
    }
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    ofPoint mouse = ofPoint(x,y);
    float thresh = 45.0f;
    
    for(int i=0;i<listOfPoly.size(); i++)
    {
        
        
        ofPolyline poly = listOfPoly.at(i);
        
        for(int j=0; j<poly.size(); j++)
        {
            
            ofPoint p = poly[j];
            
            float dist = p.distance(mouse);
            
            if(dist < thresh)
            {
                listOfPoly.at(i)[j] = mouse;
                break;
            }
            
            
            
        }
        
        
        
        
    }
    
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
   

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

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
