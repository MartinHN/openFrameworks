#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){
//    ofSetDataPathRoot("../Resources/data/");
    
    #ifndef GUIMODE
    
 
    isFPS = true;

    
    ofSetLogLevel(OF_LOG_VERBOSE);

    isFullScreen=false;



    ofBackground(0);
    glDisable(GL_DEPTH_TEST);
    ofEnableAlphaBlending();
    glBlendEquation(GL_FUNC_ADD_EXT);
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    ofSetBackgroundAuto(false);
    ofEnableArbTex();
    glEnable(GL_POINT_SPRITE);
    glPointParameteri(	GL_POINT_SPRITE_COORD_ORIGIN,GL_UPPER_LEFT);

ofSetVerticalSync(false);
     ofSetFrameRate(FPS);
   
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    inw=320;
    inh=240;
    videow=320;
    videoh=240;

    //Zdepth
    zdepth=1000;    
    camera.setFov(ofRadToDeg(atan(scrw*1.0/(zdepth))));//50
    camera.setAspectRatio(scrw*1.0/scrh);
    camrot.x=camdest.x=1.0;
    camrot.y=camrot.z=camrot.w=0.0;
    camdest.y=camdest.z=camdest.w=0.0;
    camera.disableOrtho();
    
#ifdef testvid
    vidplay.loadMovie("bien.mov");
    vidplay.play();
    threshold=55;
#endif



    

    receiver.setup(12345);
    
    
    
    blurX.load("","shaders/blurX.frag");
    blurY.load("","shaders/blurY.frag");
    colorMod.load("","shaders/colorMod.frag");
    bloom.load("","shaders/bloom.frag");
    gloom.load("","shaders/gloom.frag");
    
    finalRender.allocate(scrw,scrh,GL_RGB);
        

    drawcam=true;
    isAtt=false;

    
    
#if defined  localcamera || defined testvid || defined blobcomp

    colorImg.allocate(inw,inh);
   grayImage.allocate(inw,inh);
    grayDiff.allocate(inw,inh);

    bLearnBakground=true;
 
#endif  
    


    finalblur=0;



    attrctl = AttrCtl();  


#endif  

    visuHandler.setup(&attrctl,inw,inh,zdepth,&scrw,&scrh);

#ifndef GUIMODE 
    visuHandler.setupSyphon(&blurX,&blurY);
#ifndef LIVEBLUR
    foreground.push_back(visuHandler.sH.globalMask);
#endif
    
#ifdef blobosc
    drawBlob = false;
#endif
    
    polyBlob.allocate(inw,inh,GL_RGB32F);
//    Shadow.load("","shaders/Shadow.frag");
    
#endif
    camera2.setup(&scrw,&scrh, &zdepth);
    
    
    globalParam.setName("OF");
    settings.setName("global");
    MYPARAM(loadName, "", "", "");
    MYPARAM(saveName, "", "", "");
    saveName.addListener(this, &testApp::saveState);
    loadName.addListener(this, &testApp::loadState);
    
    MYPARAM(finalblur, 0.f, 0.f, 10.f);
    MYPARAM(saturation, 1.f, 0.f, 2.f);
    MYPARAM(contrast, 1.f, 0.f, 2.f);
    MYPARAM(brightness, 1.f, 0.f, 2.f);
    MYPARAM(rback, 255, 0, 255);
    MYPARAM(gback, 255, 0, 255);
    MYPARAM(bback, 255, 0, 255);
    MYPARAM(alphablur, 100, 0, 255);
    MYPARAM(isBloom,false,false,true);
    MYPARAM(isGloom,false,false,true);

    settings.add(camera2.settings);
    
    
//    settings.add(finalblur);
    
    visuHandler.addVisu(new background(&visuHandler));
    visuHandler.addVisu(new boule2gomme(&visuHandler));
    visuHandler.addVisu(new Particles(&visuHandler));
    
    
    
    

    visuHandler.registerParams();
    visuHandler.sH.registerParams();
    


    globalParam.add(settings);
    globalParam.add(visuHandler.settings);
    globalParam.add(visuHandler.attr->settings);
    globalParam.add(visuHandler.sH.screensParam);
    globalParam.add(visuHandler.allParams);
    
    

#ifdef GUIMODE
    paramSync.setup(globalParam,VISU_OSC_IN,VISU_OSC_IP_OUT,VISU_OSC_OUT);
#else
    paramSync.setup(globalParam,VISU_OSC_OUT,"0",VISU_OSC_IN);
#endif
//    string savename = "lolo";
//    visuHandler.saveState(savename);

#ifdef GUIMODE
         ofSetFrameRate(8);
//    gui.load(visuHandler.allParams);
//    gui.loadOne(settings);
    gui.load(globalParam);
    
    
#endif

}




//--------------------------------------------------------------

///// UPDATE

//---------------------------------------------------------------



void testApp::update(){
#ifndef GUIMODE
    visuHandler.update();
    for (int ncom = 0 ; ncom< computeRatio;ncom++){
        visuHandler.updateHighFPS();
    }
paramSync.update();
        
    oscUpdate();
  
   
    attrctl.smooth();
    attrctl.addpoints();
//    attrctl.setattr();
#endif 

    
} 


//--------------------------------------------------------------
//     
//     DRAW
//
//--------------------------------------------------------------











void testApp::draw(){
    
#ifdef GUIMODE
    ofSetBackgroundAuto(true);
    gui.draw();
    
#else
    
    
    glBlendEquation(GL_FUNC_ADD);
    
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);




    finalRender.dst->begin();
    
    ofSetColor(rback,gback,bback,alphablur);
    ofRect(0,0,scrw,scrh);
    
    
        if(isAtt){
            for( int k=0;k<attrctl.addpoint.size() ; k++){
                ofSetColor(attrctl.addfamilly[k]==0?255:0,attrctl.addfamilly[k]==1?255:0,attrctl.addfamilly[k]==2?255:0);
                ofCircle(attrctl.addpoint[k].x*width,attrctl.addpoint[k].y*height,20);   
            }
            
        }
         
    
    ofPushMatrix();
    ofPushView();
    
          camera2.begin();
        

        //NEW POSITION for VISU
        visuHandler.draw();
          
  camera2.end();
    ofPopMatrix();
    ofPopView();
    




finalRender.dst->end();
    
    finalRender.swap();
    
    
    
    glBlendEquation(GL_FUNC_ADD_EXT);
    
    glBlendFunc(GL_ONE,GL_ZERO);
    

            finalRender.dst->begin();
            blurX.begin();
            blurX.setUniform1f("blurAmnt",finalblur);
            finalRender.src->draw(0,0); 
            blurX.end();
            finalRender.dst->end();
            
            finalRender.swap();

            finalRender.dst->begin();
            blurY.begin();
            blurY.setUniform1f("blurAmnt",finalblur);            
            finalRender.src->draw(0,0);
            blurY.end();        
            finalRender.dst->end();
            
            finalRender.swap();

            finalRender.dst->begin();
            colorMod.begin();
            colorMod.setUniform1f("contrast",contrast); 
            colorMod.setUniform1f("saturation",saturation); 
            colorMod.setUniform1f("brightness",brightness); 
            finalRender.src->draw(0,0);
//    ofRect(0,0,ofGetHeight()/2,300);
            colorMod.end();
            finalRender.dst->end();

            finalRender.swap();
            
    if(isBloom){
        finalRender.dst->begin();
        bloom.begin();
        finalRender.src->draw(0,0);
        bloom.end();
        finalRender.dst->end();
        
        finalRender.swap();
    }
    if(isGloom){
        finalRender.dst->begin();
        gloom.begin();
        finalRender.src->draw(0,0);
        gloom.end();
        finalRender.dst->end();
        
        finalRender.swap();

    
    }
    
    


   
        
//        glFinish();
    
       
                  

    
    ofSetColor(255);
    

    

    if(visuHandler.sH.isMasking){
        visuHandler.sH.drawMask();
        ofSetColor(0);
        glBlendEquation(GL_ADD);
        glBlendFunc(GL_DST_COLOR,GL_ZERO);

    }


    ofSetColor(255);
    finalRender.src->draw(0,0,ofGetWidth(),ofGetHeight());
    

    
    if(isFPS){
        ofSetColor(0,0,0,!isFPS?0:255);
        ofRect(0, 0, 300, 25);
        ofSetColor(255, 255, 255,!isFPS?0:255);
        ofDrawBitmapString("Fps: " + ofToString( ofGetFrameRate()) + "thr :" + ofToString(threshold), 15,15);
    }
    else{
        ofSetColor(0,0,0);
        ofRect(0, 00, 1000, 25);
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("Un Des Sens : www.undessens.org",50,40);
        ofDrawBitmapString(" www.facebook.com/AssoUnDesSens",500,40);
    }
    
    
 #endif
    
    
}
#ifdef GUIMODE
void testApp::keyPressed(int key){
     switch (key){
         case 's':
         {ofFileDialogResult filep = ofSystemSaveDialog("preset","save preset file");
             saveName = filep.getPath();}
             break;
         case 'l':
         {ofFileDialogResult filep = ofSystemLoadDialog("load preset");
             loadName = filep.getPath();}
             break;
     }
}
#endif


#ifndef GUIMODE
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    int k = 0;

    switch (key){


#ifdef blobcomp
            case 'b':
            bLearnBakground=true;
            break;
#endif

        case 's':
            isFullScreen=!isFullScreen;
            ofSetFullscreen(isFullScreen);
            break;
            
            
            
            case'a':
            isAtt=!isAtt;
            break;
            
            case'+':
            threshold+=1;
            break;
            
            case'-':
            threshold-=1;
            break;
            
            case 356:
            camera2.ypr+=ofVec2f(5,0);
            break;

        case 357:
            camera2.ypr=ofVec2f(camera2.ypr.get().x,ofClamp(camera2.ypr.get().y+5,-90,90));
            break;
            
        case 358:
            camera2.ypr-=ofVec2f(5,0);
            break;
            
        case 359:
            camera2.ypr=ofVec2f(camera2.ypr.get().x,ofClamp(camera2.ypr.get().y-5,-90,90));
            break;
        
        case ' ':
#if defined testvid || defined localcamera
            vidplay.setPaused(!vidplay.isPaused());
#endif
            break;
        case 'c':
            drawcam=!drawcam;
            break;   
        case 'f':
            isFPS=!isFPS ;
            break;
        case 'x':
            if( visuHandler.get("particles")){
                Particles * ptmp =(Particles *)visuHandler.get("particles") ;
                ptmp->initFbo();
            };

        default :
            break;
    }
    
    
    
}



//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
#ifndef syphonout
    scrw=width=w;
    scrh=height=h;
    

 
    visuHandler.updateScreenSize();
    finalRender.allocate(width,height,GL_RGB);
#endif
}



void testApp::oscUpdate(){
    ofxOscMessage m;
    while(receiver.getNextMessage(&m)){
 
        
#ifndef PVISU
        if(m.getAddress() == "/polyline"){
            
    #ifdef blobosc  
       // polylastt = ofGetElapsedMillis();
            polyline.clear();
                      
            int n=m.getArgAsInt32(0);
           
            if(n>0){
            for(int k=1;k<n+1;k++){
                ofPoint inp=ofPoint(1-m.getArgAsFloat(2*k-1),1-m.getArgAsFloat(2*k));
                
                if(f.blobxinvert)inp.x=1- inp.x;
                if(f.blobyinvert)inp.y=1- inp.y;
                
                
                polyline.lineTo(inw/2.0+inw*(inp.x-.5)/camrot.x,inh/2+inh*(inp.y-.5)/camrot.x);
            }
            polyline.close();
           // printf("%f\n",ofGetElapsedTimef());
            }
            else{
                polyline. lineTo(-1,-1);
                polyline.lineTo(-1.5,-1.5);
                polyline.close();
                //polyline.clear();
            }
            
           #endif
        }
        

        else
            #endif
            if(m.getAddress() == "/attractors"){
            vector<ofPoint> points;
            vector<int> familly;
            for(int i=0;i<(m.getNumArgs()-1)/3;i++){
                familly.push_back(m.getArgAsInt32(1+i*3));
                
                points.push_back(ofPoint(m.getArgAsFloat(2+i*3),m.getArgAsFloat(3+i*3)));
            }
            attrctl.update(points,familly);
        } 

//        else if(m.getAddress() == "/gravity"){
//            f.gravity=m.getArgAsFloat(0)==1;
//        }
        
        
        
        
        else if(m.getAddress()=="/finalblur"){
            finalblur=m.getArgAsFloat(0);
            
        }
        
        //CAMERA
        
        else if(m.getAddress()=="/camrotx"){
            camdest.y=m.getArgAsFloat(0);
        }
        else if(m.getAddress()=="/camroty"){
            camdest.z=m.getArgAsFloat(0);
        }
        else if(m.getAddress()=="/camrotz"){
            camdest.w=m.getArgAsFloat(0);
        }
        
        else if(m.getAddress()=="/camdist"){
            camdest.x=m.getArgAsFloat(0)/100.0;
        }
        else if(m.getAddress()=="/alphacam"){
            alphacamrot=m.getArgAsFloat(0)/100.0;
        }
        
        else if(m.getAddress()=="/brightness"){
            brightness=m.getArgAsFloat(0)/100.0;
        }
        else if(m.getAddress()=="/saturation"){
            saturation=m.getArgAsFloat(0)/100.0;
        }
        else if(m.getAddress()=="/contrast"){
           contrast=m.getArgAsFloat(0)/100.0;
        }
        
               
        
//        
//        
//        
//        else if (m.getAddress()=="/MyLiveSet/y"){
//            f.yori=scrh/2-m.getArgAsFloat(0)*300.0;
//        }
        
//        else if(m.getAddress()=="/drawBlob"){
//            drawBlob=m.getArgAsFloat(0)==1;
//        }
    
        
        else if(m.getAddress()=="/beat"){
            visuHandler.beat=m.getArgAsFloat(0);
        }
        else if(m.getAddress()=="/visufollowcam"){
            visuHandler.visufollowcam=m.getArgAsFloat(0)==1;
        }

        else if(m.getAddress()=="/audioenv"){
            visuHandler.audioenv=m.getArgAsFloat(0);
        }

        
        
    }


}
    




void testApp::exit(){
    
    
    
    // videoClient.~ofxSyphonClient();
    //    blobClient.~ofxSyphonClient();
    
}



//--------------------------------------------------------------

#ifndef PVISU
void testApp::readObj(const char * path){
    int maxmesh=numParticles;
   float* oritab= new float[3*maxmesh];
    for(int k = 0 ; k<3*maxmesh;k++){
        oritab[k] = 0;
    }
    
    
    ofFile f(path);
    ofBuffer buf = f.readToBuffer();
    buf.getFirstLine();
    //Boolean initvertex=true;

    
    //ofVec3f minpoint=ofVec3f(999999,9999999,9999999);
    

    
    
    
    int numvertex = 0;
//    while(!buf.isLastLine()&&numvertex<maxmesh){
//        
//        string sbuf=buf.getNextLine();
//        if(sbuf[0]=='v'){
//            
//            //if(initvertex){initvertex=false;}
//            char vbuf[20];
//            int i =2;
//            for (int k=0 ,z=0;k<3;k++,z=0){
//                
//                while((char)sbuf[i]!='\0'&&(char)sbuf[i]!=' '){
//                    vbuf[z]=(char)sbuf[i];
//                    z++;
//                    i++;
//                }
//                vbuf[z]='\0';
//                i++;
//                float mult=0.31;
//                switch (k) {
//                        
//                    case 0:
//                        oritab[numvertex*3]=(float)(ofClamp(atof(vbuf)*mult,0.0,1.0));
//                       // minpoint.x=oritab[numvertex*3]<minpoint.x?oritab[numvertex*3]:minpoint.x;
//                        break;
//                    case 1:
//                        oritab[numvertex*3+1]=(float)(ofClamp(atof(vbuf)*mult,0.0,1.0));
//                       // minpoint.y=oritab[numvertex*3+1]<minpoint.y?oritab[numvertex*3+1]:minpoint.y;
//                        break;
//                    case 2:
//                        oritab[numvertex*3+2]=(float)(ofClamp(atof(vbuf)*mult,0.0,1.0)+0.5);
//                        //minpoint.z=oritab[numvertex*3+2]<minpoint.z?oritab[numvertex*3+2]:minpoint.z;
//                        break;
//                        
//                    default:
//                        break;
//                }
//            } 
//           // if(numvertex%100==0)ofSleepMillis(1);
//             particleStates[numvertex]=true;
//            numvertex++;
//        }
//        
//    }
    
 
    f.close();
    for(int k=numvertex;k<numParticles;k++){
               oritab[3*k]=0.5;
                oritab[3*k+1]=0.5;
                oritab[3*k+2]=0.5;
        
    }
//    for(int k=0;k<numvertex;k++){
//        oritab[3*k]-=minpoint.x;
//        oritab[3*k+1]-=minpoint.y;
//        oritab[3*k+2]-=minpoint.z;
//    }
    
    origins.getTextureReference().loadData(oritab,textureRes,textureRes,GL_RGB);
    
       
       
       
    delete [] oritab;
}








void testApp::reset(){
    posPingPong.src= &origins;
    
    
    float * velinit = new float[numParticles*3];
    for (int i = 0; i < numParticles; i++){
        velinit[i*3 + 0] = 0.;//+ofRandom(-.1,.1);
        velinit[i*3 + 1] = 0.;//+ofRandom(-.1,.1);
        velinit[i*3 + 2] = 0.;
    }
    velPingPong.src->getTextureReference().loadData(velinit, textureRes, textureRes, GL_RGB);
    
    delete [] velinit;
}








 



void testApp::changeOrigin(int type){
    Float32* oritab= new Float32[3*numParticles];
    
    switch (type) {
            
        case 0:
        {
            for (int x = 0; x < textureRes; x++){
                for (int y = 0; y < textureRes; y++){
                    int i = textureRes * y + x;
                    oritab[i*3 + 0] = x*1.0/textureRes;
                    oritab[i*3 + 1] = y*1.0/textureRes;
                    oritab[i*3 + 2] = 0.5;
                }
            }
               origins.getTextureReference().loadData(oritab,textureRes,textureRes,GL_RGB);
            
            
        }
               
            break;
            
        case 1:
        {
            int numcircles=textureRes;
            int numangl=textureRes;
            float ray=0.4;
            for(int k=0;k<numcircles;k++){
                float y=0.5-ray+2.0*ray*k/numcircles;
                
                for(int j=0;j<numangl;j++){
                    float x =  0.5+sqrtf(ray*ray-1.0*(y-0.5)*(y-0.5))*cos(j*1.0/numangl*2.0*PI);
                    float z = 0.5+sqrtf(ray*ray-1.0*(y-0.5)*(y-0.5))*sin(j*1.0/numangl*2.0*PI);
                    
                    oritab[3*(k*numangl+j)+0]=x;
                    oritab[3*(k*numangl+j)+1]=y;
                    oritab[3*(k*numangl+j)+2]=z;
                    
 
                    
                }
                             }
            origins.getTextureReference().loadData(oritab,textureRes,textureRes,GL_RGB);


            
           
        }   
             break;
        case 2:
        {
 

            for (int x = 0; x < textureRes; x++){
                for (int y = 0; y < textureRes; y++){
                    int i = textureRes * y + x;
                    
                    oritab[i*3 + 0] = 0;
                    oritab[i*3 + 1] = y*1.0/textureRes;
                    oritab[i*3 + 2] = 0.5;
                }
            }
            

            origins.getTextureReference().loadData(oritab, textureRes, textureRes, GL_RGB);
            
          
        }
            break;
        case 3:
        {
//            ofxAssimpModelLoader model;
//            model.loadModel("obj/text-100170.obj",false);
//            ofMesh mesh  = model.getMesh(0);
////            mesh.getBoundingBox()
//               model.clear();
//            float scale = 0.1;
//            ofVec3f pos = ofVec3f(0.5,0,0.5);
//            ofVec3f min, max;
//            min = ofVec3f(99999,99999,99999);
//            max = ofVec3f(-9999,-9999,-9999);
//            
//            for (int i = 0 ; i< mesh.getNumVertices();i++){
//                min.x = mesh.getVertex(i).x<min.x?mesh.getVertex(i).x:min.x;
//                min.y = mesh.getVertex(i).y<min.y?mesh.getVertex(i).y:min.y;
//                min.z = mesh.getVertex(i).z<min.z?mesh.getVertex(i).z:min.z;
//                
//                max.x = mesh.getVertex(i).x>max.x?mesh.getVertex(i).x:max.x;
//                max.y = mesh.getVertex(i).y>max.y?mesh.getVertex(i).y:max.y;
//                max.z = mesh.getVertex(i).z>max.z?mesh.getVertex(i).z:max.z;
//            }
//            
//            
//            pos = -(min+max)/2.;
//            scale = 1./(max.x-min.x);
//            for (int i = 0 ; i<(textureRes-1)*(textureRes-1);i++){
//
//                if(i< mesh.getNumVertices()){
//                    oritab[i*3 + 0] = (Float32)CLAMP(0.5+(mesh.getVertex(i).x+ pos.x)*scale ,0,1);
//                    oritab[i*3 + 1] = (Float32)CLAMP(0.5+(mesh.getVertex(i).y+ pos.y)*scale ,0,1);
//                    oritab[i*3 + 2] = (Float32)CLAMP(0.5+(mesh.getVertex(i).z+ pos.z)*scale ,0,1);
//
//                    
//                }
//                else{
//                    
//                    oritab[i*3 + 0] = 0.5;
//                    oritab[i*3 + 1] = 0.5;
//                    oritab[i*3 + 2] = 0.5;
//                }
//                
//                                    
//            }
//            mesh.clear();
//         
//            
//            
//            origins.getTextureReference().loadData(oritab, textureRes, textureRes, GL_RGB);
//
//            
//            
//            
        }
            break;
        case 4:
        {
            
            
            for (int x = 0; x < textureRes; x++){
                for (int y = 0; y < textureRes; y++){
                    int i = textureRes * y + x;
                    
                    oritab[i*3 + 0] = x*1.0/textureRes;
                    oritab[i*3 + 1] = y*1.0/textureRes;
                    oritab[i*3 + 2] = sqrt((x*1.0/textureRes-0.5)*(x*1.0/textureRes-0.5))+sqrt((y*1.0/textureRes-0.5)*(y*1.0/textureRes-0.5));

                }
            }
            
            
            origins.getTextureReference().loadData(oritab, textureRes, textureRes, GL_RGB);
            
            
        }
            break;
        default:
            break;
            
            
            
    }
       delete [] oritab; 
    
 
}






void testApp::oscBlobUpdate(){
   // blobHandler.update();
#ifdef blobosc 
    
    
    vector<ofPolyline> & polys = polyline.getOutline();
    for(int i=0;i<(int)polys.size();i++)
        if(polys[0].size()>0){
            ofPolyline cp=polys[0];
            if(f.blobmirrorx){
                for(int k = 0;k<cp.size();k++){
                    polyline.lineTo(inw-cp[k].x,cp[k].y) ;
                }
                polyline.close();
            }
            if(f.blobmirrory){
                for(int k = 0;k<cp.size();k++){
                    polyline.lineTo(cp[k].x,inh-cp[k].y) ;
                }
                polyline.close();   
            }
        }
    
    
    
    
    ofPushStyle();

    polyBlob.src->begin();
    ofSetColor(ofColor(0,0,0,ablob));
    ofFill();
    ofRect(0,0,inw,inh);

    ofSetColor(255);
    // ofNoFill();
    
    //printf("s %d",(polyline.getOutline()).size());
    polyline.setFillColor(ofColor(255));
    polyline.draw();
    visuHandler.drawFbo();
    
    polyBlob.src->end();
   // if( polyline.getOutline().size()>0){
        
            if(f.blobblur>0){
        polyBlob.dst->begin();
        Shadow.begin();
        Shadow.setUniformTexture("texture",polyBlob.src->getTextureReference(),0);
        Shadow.setUniform1f("offset",f.blobblur);
        Shadow.setUniform1f("direction_x",1.0);
            Shadow.setUniform1f("brightness",brightness2);
            Shadow.setUniform1f("saturation",saturation2);
            Shadow.setUniform1f("contrast",contrast2);

        ///Just a frame to put pixels on
        ofSetColor(255,255,255,255);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(inw, 0); glVertex3f(inw, 0, 0);
        glTexCoord2f(inw,inh); glVertex3f( inw,inh, 0);
        glTexCoord2f(0, inh);  glVertex3f(0, inh, 0);
        glEnd();
        
        
           Shadow.end();
        polyBlob.dst->end();
        polyBlob.swap();
        
        polyBlob.dst->begin();
        Shadow.begin();
        Shadow.setUniformTexture("texture",polyBlob.src->getTextureReference(),0);
        Shadow.setUniform1f("offset",f.blobblur);
        Shadow.setUniform1f("direction_x",0.0);
            Shadow.setUniform1f("contrast",contrast2);
            Shadow.setUniform1f("saturation",saturation2);    
            Shadow.setUniform1f("brightness",brightness2);
        ///Just a frame to put pixels on
        ofSetColor(255,255,255,255);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(inw, 0); glVertex3f(inw, 0, 0);
        glTexCoord2f(inw,inh); glVertex3f( inw,inh, 0);
        glTexCoord2f(0, inh);  glVertex3f(0, inh, 0);
        glEnd();
        
        
        Shadow.end();
        polyBlob.dst->end();
        polyBlob.swap();
            }

    
   // }

    
  
    ofPopStyle();
   
#endif 
    
    
}

#endif


void testApp::mouseDragged(int x, int y, int button){
    vector<ofPoint> points;
    vector<int> familly;
  
        familly.push_back(0);
        points.push_back(ofPoint(1.0-x*1.0/scrw,y*1.0/scrh,0));

    if(points.size()>0)attrctl.update(points,familly);
//    printf("%i",button);
}
void testApp::mouseReleased(int x, int y, int button){
    vector<ofPoint> points;
    vector<int> familly;
    familly.clear();
    points.clear();
    
    attrctl.update(points,familly);
    
}

#endif



void testApp::saveState(string & s){
    if(s!=""){
        string abspath = ofToDataPath("presets/"+ofToString(loadName));
        if(s.find("/")!=string::npos) {abspath = s;}
        else{ofLogWarning("saving to local : " + abspath);}
        cout<<"saving to " + abspath<<endl;
        ofXml xml;
        xml.serialize(globalParam);
        cout<<xml.save(abspath)<<endl;
    }
    else{
        ofLogWarning("no argument for save state");
    }
    
}





void testApp::loadState(string & s){
    if(s!=""){
        string abspath = ofToDataPath("presets/"+ofToString(loadName));
        if(s.find("/")!=string::npos) {abspath = s;}
        else{ofLogWarning("loading from local : " + abspath);}
        ofXml xml;
        
        xml.load(abspath);
        xml.deserialize(globalParam);
    }
    else{
        ofLogWarning("no argument for load state");
    }
    
}

