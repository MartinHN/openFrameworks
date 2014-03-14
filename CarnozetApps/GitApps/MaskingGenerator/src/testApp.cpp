#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    
    Screen src;

    
    
    src = Screen(ofPoint(0, 0.494792), ofPoint(0.289844, 0.458333),ofPoint(0.3, 0.936458), ofPoint(0.009375, 0.998958));
    listOfScreen.push_back(src);
    
    src = Screen(ofPoint(0.295362, 0.463659), ofPoint(0.465359, 0.425409),ofPoint(0.459409, 0.856352), ofPoint(0.301312, 0.915001));
    listOfScreen.push_back(src);
    
    src = Screen(ofPoint(0.467909, 0.428809), ofPoint(0.590625, 0.41861),ofPoint(0.583507, 0.830853), ofPoint(0.473859, 0.831703));
    listOfScreen.push_back(src);
    
    src = Screen(ofPoint(0.597957, 0.41946), ofPoint(0.797703, 0.438159),ofPoint(0.787503, 0.876752), ofPoint(0.600507, 0.832553));
    listOfScreen.push_back(src);
    
    
    activeScreen = -1;
    
    gui.setup();
    suppr.addListener(this, &testApp::deleteScreen);
    add.addListener(this, &testApp::addScreen);
    reset.addListener(this, &testApp::resetScreen);
    save.addListener(this, &testApp::savePng);
    
    gui.add(suppr.setup("Suppr", 0));
    gui.add(add.setup("Add", 0));
    gui.add(reset.setup("Reset", 0));
    gui.add(save.setup("Save", 0));
    
    
}

//--------------------------------------------------------------
void testApp::update(){

}



//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(100);
    gui.draw();
    
    for(int i=0; i<listOfScreen.size(); i++){
        
        listOfScreen.at(i).draw();
        ofDrawBitmapString(ofToString(i),listOfScreen.at(i).centroid.x, listOfScreen.at(i).centroid.y);
        for (int j = 0 ;j< 4 ; j++){
        ofDrawBitmapString(ofToString(j),listOfScreen.at(i).listOfPoint[j] );
        }

        
    }
    

    
    
}

//--------------------------------------------------------------
void testApp::deleteScreen(){
    
    if(activeScreen >= 0){
        listOfScreen.erase(listOfScreen.begin()+activeScreen);}
}

//--------------------------------------------------------------
void testApp::addScreen(){
   
    ofPoint p1 = ofPoint( ofGetScreenWidth()/2 - ofRandom(100), ofGetScreenHeight()/2 - ofRandom(100));
    ofPoint p2 = ofPoint( ofGetScreenWidth()/2 + ofRandom(100), ofGetScreenHeight()/2 - ofRandom(100));
    ofPoint p3 = ofPoint( ofGetScreenWidth()/2 + ofRandom(100), ofGetScreenHeight()/2 + ofRandom(100));
    ofPoint p4 = ofPoint( ofGetScreenWidth()/2 - ofRandom(100), ofGetScreenHeight()/2 + ofRandom(100));
    Screen src = Screen(p1, p2, p3, p4);
    
    listOfScreen.push_back(src);
    
}

//--------------------------------------------------------------
void testApp::resetScreen(){
    
    if(activeScreen>=0){
            
        listOfScreen.at(activeScreen).reset();
        
    }
    
}

//--------------------------------------------------------------
void testApp::savePng(){
    
    ofSaveScreen("mask.png");
    
    saveXml();
    
    
    
    
}

//--------------------------------------------------------------
void testApp::saveXml(){
    
    xml.clear();
    xml.addTag("screens");   
    xml.pushTag("screens");
    
    
    for(int i=0; i<listOfScreen.size(); i++)
    {
        
        Screen src = listOfScreen.at(i);
        
        xml.addTag(ofToString("screen"+ofToString(i+1)));
        xml.pushTag(ofToString("screen"+ofToString(i+1)));
        
        xml.addTag(ofToString("vertices"));
        xml.pushTag(ofToString("vertices"));
       
        
        
        for (int j=0; j<src.listOfPoint.size(); j++)
        {
            
            
            xml.addTag("p"+ofToString(j));  
            xml.pushTag("p"+ofToString(j));
            
            
xml.setValue("p"+ofToString(j), ofToString(src.listOfPoint.at(j).x/ofGetWidth())+", "+ofToString(src.listOfPoint.at(j).y/ofGetHeight()));
             
            

            
            xml.popTag();
            
            
        }
        
        xml.popTag();
        xml.popTag();        
       
        
        
        
        
    }
    
    xml.popTag();
    
    ofFileDialogResult filep = ofSystemSaveDialog("pecrans1.xml","save screen map preset file");
    string saveName = filep.getPath();
    xml.save(saveName);
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch (key) {
        case 'x':
            savePng();
            break;
        case 's':
            ofSetFullscreen(true);
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
    
    if(activeScreen>=0){
        
        listOfScreen.at(activeScreen).dragPoint(mouse);
        
    }
    
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
   
    if( button == 0 ){
    
        ofPoint mouse = ofPoint(x,y);
        ofPolyline poly;
        poly.clear();
        bool activeOther = true;

        
        //Be sure the screen is not already active
        if(activeScreen>=0)
        {
        
            for(int j=0; j<(listOfScreen.at(activeScreen).listOfPoint.size()); j++)
            {
                
                poly.lineTo(listOfScreen.at(activeScreen).listOfPoint.at(j));
            }
            
            if(poly.inside(mouse)){
                
                activeOther = false;
                listOfScreen.at(activeScreen).findClosest(mouse);
            }
            else
            {
                activeOther = true;
                
                
            }
            
        }
        
        
        
        if(activeOther)
        {
            activeScreen = -1;
            poly.clear();
            for(int i=0; i<listOfScreen.size(); i++)
            {
                
                for(int j=0; j<(listOfScreen.at(i).listOfPoint.size()); j++)
                {
                    
                    poly.lineTo(listOfScreen.at(i).listOfPoint.at(j));
                }
                poly.close();
                
                if( poly.inside(mouse))
                {
                    activeScreen = i;
                    break;                
                }

                poly.clear();
                
            }
            
            for(int i=0; i<listOfScreen.size(); i++)
            {
            
                if( activeScreen == i)
                {
                    listOfScreen.at(i).isActive = true;
                    listOfScreen.at(i).findClosest(mouse);
                }
                else
                {
                    listOfScreen.at(i).isActive = false;
                    listOfScreen.at(i).activePoint = -1;
                }
                

                
            }
            
        }
        
    }
    

}

void testApp::loadScreens(){
    
    ofXml eS;
    if(eS.load("Xml/ecrans.xml")){
        int nS = eS.getNumChildren();
        vector<ofVec3f> vert;
        
        for (int n = 0 ; n<nS;n++){
            eS.setToChild(n);
            
            int nP = eS.getNumChildren();
            eS.setToChild(0);
            for (int i = 0 ; i<nP;i++){
                ofVec3f curpoint(0,0);
                curpoint.x = eS.getFloatValue("x");
                curpoint.y = eS.getFloatValue("y");
                curpoint.z = eS.getFloatValue("z");
                vert.push_back(curpoint); 
                eS.setToSibling();
            }
            
       
            listOfScreen.push_back(Screen(vert[0], vert[1],vert[2],vert[3]));
            eS.setToParent();
            eS.setToParent();
            vert.clear();
            
        }
    }

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
