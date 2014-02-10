#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

        
    //Listener
    gain.addListener(this, &testApp::gainChanged);
    
    //GUI
    gui.setup(); // most of the time you don't need a name    
    gui.add(blobMin.setup("BlobMin", 10, 2.0, 4000.0f));
    gui.add(thres.setup("Threshold", 30, 0, 255));
    gui.add(gain.setup("Gain", 0.2, 0.01, 1.0));
    gui.add(isSending.setup("Send OSC", true));
    
    //SIZE
    globalW = 640;
    globalH = 480;
    float coeff = 0.43;
    drawH = globalH*coeff;
    drawW = globalW*coeff;
    
    

    //PSEYE
    ps3eye.listDevices();
    ps3eye.setDesiredFrameRate(60);
    ps3eye.initGrabber(globalW,globalH);
    ps3eye.setAutoGainAndShutter(false); // otherwise we can't set gain or shutter
	ps3eye.setGain(gain);
	ps3eye.setShutter(0.5);
	ps3eye.setGamma(0.9);
	ps3eye.setBrightness(0.6);
	ps3eye.setContrast(1.0);
	ps3eye.setHue(0.5);
    ps3eye.setFlicker(1);
    
    ofSetFrameRate(61);

    

    
    //Image
    cam.allocate(globalW, globalH);
    grey.allocate(globalW, globalH);
    
    //OSC
    sender.setup("localhost", 12345);
    
    
    
}

//--------------------------------------------------------------
void testApp::update(){

    ps3eye.update();
    
    if(ps3eye.isFrameNew())
    {
        
        cam.setFromPixels(ps3eye.getPixels(), globalW, globalH);
        
        grey = cam;
        
        grey.threshold(thres);
        
        float min = blobMin;
        
        contourFinder.findContours(grey, min, (globalH*globalW)/2, 10, false,false);
        
        updateContourFinder();
        
        sendAttractors();
        
        
    }

    
}

//--------------------------------------------------------------
void testApp::draw(){

    gui.draw();
    
    ofTranslate( gui.getWidth()+30, 0);
    
    ofSetColor(255);
    cam.draw(0, 0, drawW, drawH);
    ofFill();
    ofSetColor(ofColor::blue);
    if(is1) ofCircle(5 + (att1.x * drawW), 5 + (att1.y * drawH), 5);
    if(is2) ofCircle(5 + (att2.x *  drawW), 5 + (att2.y * drawH), 5);
    
    ofTranslate(10 + drawW, 0);
    ofSetColor(255);
    
    grey.draw(0,0,drawW, drawH);
    contourFinder.draw(0, 0, drawW, drawH);
    

}

//-------------------------------------------------------------
void testApp::sendAttractors(){
    
    if( isSending)
    {
        
        // Send Polyline
        ofxOscMessage mes;
        int count = 0;
        if(is1) count++;
        if(is2) count++;
        
        mes.setAddress("/attractors");
        mes.addIntArg(count);
        
        if(is1 )
        {
            mes.addIntArg(1);
            mes.addFloatArg(att1.x);
            mes.addFloatArg(att1.y);
        }
        
        if(is2 )
        {
            mes.addIntArg(1);
            mes.addFloatArg(att2.x);
            mes.addFloatArg(att2.y);
        }
        
        sender.sendMessage(mes);
        
    }
    
    return 1;
    
    
}

//--------------------------------------------------------------
void testApp::updateContourFinder(){
    
    
    if( contourFinder.blobs.size()>0){
        
        int num = contourFinder.blobs.size();
        int actualNum = 0;
        if(is1) actualNum++;
        if(is2) actualNum++;
        
        // ONE blob found
        if( num ==1 )
        {
        
            ofxCvBlob blob = contourFinder.blobs.at(0);
            ofPoint att = ofPoint( blob.centroid.x / globalW, blob.centroid.y / globalH );
            
            if(actualNum == 0)
            {
                is1 = true;
                att1 = att;
            }
            else if( actualNum == 1)
            {
                
                if(is1) att1 = att;
                if(is2) att2 = att;
                
            }else if( actualNum == 2){
                
                float dist1_1 = att1.distance(att);
                float dist1_2 = att2.distance(att);
                
                if(dist1_1 < dist1_2)
                {
                    is2 = false;
                    is1 = true;
                    att1 = att;
                }else
                {
                    is1 = false;
                    is2 = true;
                    att2 = att;
                }
                
                
            }
        
        }
        // TWO OR MORE blobs found
        else if( num > 1)
        {
            ofxCvBlob blob1in = contourFinder.blobs.at(0);
            ofxCvBlob blob2in = contourFinder.blobs.at(1);
            
            ofPoint att1in = ofPoint( blob1in.centroid.x / globalW, blob1in.centroid.y / globalH );
            ofPoint att2in = ofPoint( blob2in.centroid.x / globalW, blob2in.centroid.y / globalH );
            
            if(actualNum == 0)
            {
                is1 = true;
                is2 = true;
                
                if( att1in.x < att2in.x){
                    
                    att1 = att1in;
                    att2 = att2in;
                }else{
                    
                    att1 = att2in;
                    att2 = att1in;
                }
                
            }
            else if( actualNum > 0)
            {
                
                ofPoint att;
                if(is1)att = att1;
                else att = att2;
                
                float dist1_1 = att1in.distance(att);
                float dist2_1 = att2in.distance(att);
                
                if( (dist1_1 < dist2_1 && is1) || (dist1_1 > dist2_1 && is2) ){
                    
                    att1 = att1in;
                    att2 = att2in;
                }
                if((dist1_1 < dist2_1 && is2) || (dist1_1 > dist2_1 && is1 )){
                    
                    att2 = att1in;
                    att1 = att2in;
                }
                
                is1 = is2 = true;
 

            }
            
            
        }
        
        
    
        
        
    }
    else
    {
        is1 = false;
        is2 = false;
    }
    
    
    
}

//--------------------------------------------------------------
void testApp::gainChanged(float &gain){
    
    ps3eye.setGain(gain);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
