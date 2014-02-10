#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    //OSC Connection
    sender.setup("192.168.0.15", 12345);
    
    //Temp variable
    currValue = "Hello";
    isModified = 0;
    
    //set some sketch parameters
    //Background Color
    red = 60;
    blue = 120;
    green = 80;
    alpha = 200;
    radius = 150;
    noiseScale = .1;
    drawFill = true;
    backgroundColor = ofColor(red, green,blue);
    resolution = 60;
    position = ofPoint(ofGetWidth()*.5, ofGetHeight()*.5);
    ofSetCircleResolution(resolution);
    
    //FBO
    fboPart.allocate(200, 100, GL_RGBA);
    fboPart.begin();
    ofClear(255,255,255, 0);
    fboPart.end();
    fboBg.allocate(200, 100, GL_RGBA);
    fboBg.begin();
    ofClear(255,255,255, 0);
    fboBg.end();
    
    //Height of main gui
    menuHeight = 40;
    
    //You can easily re-order these to change where they are shown
    setupGUI1();
    setupGUI2();
    setupGUI3();
    setupGUI4();
    setupGUI5();
    setupGUI6();
    
    
    /* open the last controls */
    //loadGUISettings();
    
    /* open an empty set controls*/
    openPreset(true);
    
    
    chooseCurrentGui(2);
    
}

void testApp::setupGUI1()    // SAVE AND RECORD GUI
{
    ofxUIScrollableCanvas *gui = new ofxUIScrollableCanvas;
    gui->setName("gui1");
    gui->addLabel("MENU");
    
    gui->setFontSize(OFX_UI_FONT_LARGE, 25);
    textArea = gui->addTextArea("Current Value", currValue);
    
    springTypes.clear();
    springTypes.push_back("open/save");
    springTypes.push_back("controls");
    
    
    
    ofxUIRadio* menu = new ofxUIRadio(120, 1, 30, 30, "onglet", springTypes, 1);
    menu->getToggles().back()->ofxUIButton::setValue(true);
    gui->addWidget(menu);
    springTypes.clear();
    
    
    gui->setWidth(ofGetWidth());
    gui->setHeight(menuHeight);
    //add gui to a c++ stl vector
    guis.push_back(gui);
    //add gui to a c++ stl map
    guihash["menu"] = gui;
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    
    
}

void testApp::setupGUI2()  // Parameters GUI
{
    ofxUIScrollableCanvas *gui = new ofxUIScrollableCanvas;
    gui->setName("gui2");
    
    gui->setScrollAreaToScreen();
    gui->setScrollableDirections(false, false);
    gui->setScrollAreaToScreenWidth();
    
    //Temp variables
    float zero = 0.0f;


    //--------------------------------------Global--------------------------------
    float globall1 = 10;
    float globall2 = globall1 + 15;
    float globall3 = globall2 + 15;
    float globall4 = globall3 + 15;
    
    float globalCol2 = 600;

    addWidgetToggle(gui, false,12, "origin", 2, globall1);
    addWidgetLabel(gui, "Origine", 20, globall1);
    addWidgetLabel(gui, "K:", 70,globall1);
    addWidgetSlider(gui, 0, 255, 0.0f, 150, "korigin", 90, globall1);
    addWidgetLabel(gui, "Z:", 250, globall1);
    addWidgetSlider(gui, 0, 255, 0.0f, 150, "zorigin", 260, globall1);
    
    addWidgetToggle(gui, false,12, "gravity", 600, globall1);
    addWidgetLabel(gui, "Gravity", 620, globall1);
    addWidgetLabel(gui, "X", 680, globall1 );
    addWidgetSlider(gui, 0, 255, 0.0f, 100, "Gx",700, globall1);
    addWidgetLabel(gui, "Y:", 800, globall1);
    addWidgetSlider(gui, 0, 255, 0.0f, 100, "Gy", 810, globall1);
    addWidgetLabel(gui, "Z:", 910, globall1 );
    addWidgetSlider(gui, 0, 255, 0.0f, 100, "Gz", 930, globall1);
    
    addWidgetToggle(gui, false,12, "border", 2, globall2);
    addWidgetLabel(gui, "Border", 20, globall2);
    addWidgetLabel(gui, "K:", 70, globall2);
    addWidgetSlider(gui, 0, 255, 0.0f, 175, "kborder", 90, globall2);
    addWidgetLabel(gui, "Z:", 250, globall2);
    addWidgetSlider(gui, 0, 255, 0.0f, 175, "zborder", 260, globall2);
    
    addWidgetLabel(gui, "Friction", 620, globall2);
    addWidgetSlider(gui, 0, 255, 0.0f, 75, "fr", 670, globall2);
    addWidgetLabel(gui, "Force:", 760, globall2);
    addWidgetSlider(gui, 0, 255, 0.0f, 75, "frmin", 800, globall2);
    addWidgetLabel(gui, "Vit Min:", 875, globall2);
    addWidgetSlider(gui, 0, 255, 0.0f, 75, "minv", 930, globall2);
    addWidgetLabel(gui, "Vit Max:" ,1005, globall2);
    addWidgetSlider(gui, 0, 255, 0.0f, 75, "maxv", 1065, globall2);
    
    
    addWidgetToggle(gui, false,12, "netw", 2, globall3);
    addWidgetLabel(gui, "NetW", 20, globall3);
    addWidgetLabel(gui, "L:",80, globall3);
    addWidgetSlider(gui, 0, 255, 0.0f, 100, "lnetw", 100, globall3);
    addWidgetLabel(gui, "K:",200, globall3);
    addWidgetSlider(gui, 0, 255, 0.0f, 100, "knetw", 210, globall3);
    addWidgetLabel(gui, "Z:", 310, globall3);
    addWidgetSlider(gui, 0, 255, 0.0f, 100, "znetw", 330, globall3);
    
    addWidgetToggle(gui, false,12, "neth", 600, globall3);
    addWidgetLabel(gui, "NetH", 620, globall3);
    addWidgetLabel(gui, "L:", 680, globall3);
    addWidgetSlider(gui, 0, 255, 0.0f, 100, "lneth", 700, globall3);
    addWidgetLabel(gui, "K:", 800, globall3);
    addWidgetSlider(gui, 0, 255, 0.0f, 100, "kneth", 810, globall3);
    addWidgetLabel(gui, "Z:", 910, globall3);
    addWidgetSlider(gui, 0, 255, 0.0f, 100, "zneth", 930, globall3);
    
    
    //-------------------------------FORCE --------------------------------------
    float forceCol1 = 10;
    float forceCol2 = 410;
    float forceCol3 = 810;
    float forceH = 100;
    float force_l1 = 106;
    float force_l1_ = force_l1 + 8;

    //Family title
    label = new ofxUILabel(10.0f, 76, "Family1", "Family1", OFX_UI_FONT_MEDIUM);
    gui->addWidget(label);
    
    label = new ofxUILabel(forceCol2, 76, "Family2", "Family2", OFX_UI_FONT_MEDIUM);
    gui->addWidget(label);
    
    label = new ofxUILabel(forceCol3, 76, "Family3", "Family3", OFX_UI_FONT_MEDIUM);
    gui->addWidget(label);
    
    //toggle for mouse
    addWidgetLabel(gui, "use mouse", 930, 76);
    addWidgetToggle(gui, false, 13, "usemouse", 910, 76);
    
    
    addWidgetToggle(gui, false, 25, "attrspring0", 10,force_l1);
    addWidgetLabel(gui, "Spring", 30 , force_l1_);
    springTypes.clear();
    springTypes.push_back("normal");
    springTypes.push_back("interieur");
    radio = new ofxUIRadio(90, force_l1, 12, 12, "attrspringstype0", springTypes, 0);
    gui->addWidget(radio);
    addWidgetLabel(gui, "R:", 170 , force_l1_);
    addWidgetRotSlider(gui, 0, 600, 0, 35, "raspring0", 185, force_l1);
    addWidgetLabel(gui, "K:", 225, force_l1_);
    addWidgetRotSlider(gui, 0, 1000, 0, 35, "kaspring0", 245, force_l1);
    addWidgetLabel(gui, "Z:", 285, force_l1_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "zaspring0", 305, force_l1);
    
    addWidgetToggle(gui, false, 25, "attrspring1", 410,force_l1);
    addWidgetLabel(gui, "Spring", 430 , force_l1_);
    radio = new ofxUIRadio(490, force_l1, 12, 12, "attrspringtype1", springTypes, 0);
    gui->addWidget(radio);
    addWidgetLabel(gui, "R:",560,force_l1_);
    addWidgetRotSlider(gui, 0, 600, 0, 35, "raspring1",580,force_l1);
    addWidgetLabel(gui, "K:",620,force_l1_);
    addWidgetRotSlider(gui, 0, 10000, 0, 35, "kaspring1",640,force_l1);
    addWidgetLabel(gui, "Z:",680,force_l1_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "zaspring1",700,force_l1);
    
    addWidgetToggle(gui, false, 25, "attrspring2", 810,force_l1);
    addWidgetLabel(gui, "Spring", 830 , force_l1_);
    radio = new ofxUIRadio(880, force_l1, 12, 12, "attrspringtype2", springTypes, 0);
    gui->addWidget(radio);
    addWidgetLabel(gui, "R:",975,force_l1_);
    addWidgetRotSlider(gui, 0, 600, 0, 35, "raspring2",995,force_l1);
    addWidgetLabel(gui, "K:",1035,force_l1_);
    addWidgetRotSlider(gui, 0, 1000, 0, 35, "kaspring2",1055,force_l1);
    addWidgetLabel(gui, "Z:",1095,force_l1_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "zaspring2",1115,force_l1);
    
    //FORCE -> ROTATION
    float force_l2 = force_l1 + 40;
    float force_l2_ = force_l2 + 8;
    addWidgetToggle(gui, false, 25, "attrrotation0", 10,force_l2);
    addWidgetLabel(gui, "Rotation", 30 , force_l2_);
    
    addWidgetLabel(gui, "R:", 170 , force_l2_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "rarotation0", 185, force_l2);
    addWidgetLabel(gui, "S:", 225, force_l2_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "sarotation0", 245, force_l2);
    
    addWidgetToggle(gui, false, 25, "attrrotation1",  410,force_l2);
    addWidgetLabel(gui, "Rotation", 430 , force_l2_);
    addWidgetLabel(gui, "R:",560,force_l2_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "rarotation1",580,force_l2);
    addWidgetLabel(gui, "S:",620,force_l2_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "sarotation1",640,force_l2);

    
    addWidgetToggle(gui, false, 25, "attrrotation2",810,force_l2);
    addWidgetLabel(gui, "Rotation", 830 , force_l2_);
    addWidgetLabel(gui, "R:",975,force_l2_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "rarotation2",995,force_l2);
    addWidgetLabel(gui, "S:",1035,force_l2_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "sarotation2",1055,force_l2);


    //FORCE -> GRAVITY
    float force_l3 = force_l2 + 40;
    float force_l3_ = force_l3 + 8;
    addWidgetToggle(gui, false, 25, "attrgravity0", 10,force_l3);
    addWidgetLabel(gui, "Gravity", 30 , force_l3_);
    addWidgetLabel(gui, "M:", 170 , force_l3_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "magravity0", 185, force_l3);
    addWidgetLabel(gui, "R:", 225, force_l3_);
    addWidgetRotSlider(gui, 0, 600, 0, 35, "rinagravity0", 245, force_l3);
    addWidgetLabel(gui, "min:", 285, force_l3_);
    addWidgetRotSlider(gui, -255, 255, 0, 35, "minagravity0", 305, force_l3);
    
    addWidgetToggle(gui, false, 25, "attrgravity1", 410,force_l3);
    addWidgetLabel(gui, "Gravity", 430 , force_l3_);
    addWidgetLabel(gui, "M:",560,force_l3_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "magravity1",580,force_l3);
    addWidgetLabel(gui, "R:",600,force_l3_);
    addWidgetRotSlider(gui, 0, 600, 0, 35, "Rragravity1",640,force_l3);
    addWidgetLabel(gui, "min:",680,force_l3_);
    addWidgetRotSlider(gui, -255, 255, 0, 35, "minagravity1",700,force_l3);
    
    addWidgetToggle(gui, false, 25, "attrgravity2", 810,force_l3);
    addWidgetLabel(gui, "Gravity", 830 , force_l3_);
    addWidgetLabel(gui, "M:",975,force_l3_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "magravity2",995,force_l3);
    addWidgetLabel(gui, "R:",1035,force_l3_);
    addWidgetRotSlider(gui, 0, 600, 0, 35, "ragravity2",1055,force_l3);
    addWidgetLabel(gui, "min:",1095,force_l3_);
    addWidgetRotSlider(gui, -255, 255, 0, 35, "minagravity2",1115,force_l3);

    //FORCE -> DAMP
    float force_l4 = force_l3 + 40;
    float force_l4_ = force_l4 + 8;
    addWidgetToggle(gui, false, 25, "attrdamp0", 10,force_l4);
    addWidgetLabel(gui, "Damp", 30 , force_l4_);
    addWidgetLabel(gui, "R:", 170 , force_l4_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "radamp0", 185, force_l4);
    addWidgetLabel(gui, "Coef:", 225, force_l4_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "cadamp0", 245, force_l4);
    
    addWidgetToggle(gui, false, 25, "attrdamp1",  410,force_l4);
    addWidgetLabel(gui, "Damp", 430 , force_l4_);
    addWidgetLabel(gui, "R:",560,force_l4_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "radamp1",580,force_l4);
    addWidgetLabel(gui, "Coef",620,force_l4_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "cadamp1",640,force_l4);
    
    
    addWidgetToggle(gui, false, 25, "attrdamp3",810,force_l4);
    addWidgetLabel(gui, "Damp", 830 , force_l4_);
    addWidgetLabel(gui, "R:",975,force_l4_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "radamp2",995,force_l4);
    addWidgetLabel(gui, "Coef:",1035,force_l4_);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "cadamp2",1055,force_l4);
    
    //Color mirror x/y
    float force_l5 = force_l4 + 40;
    addWidgetToggle(gui, false, 14, "attrmirrorx0", 10, force_l5);
    addWidgetLabel(gui, "mirror x",30, force_l5);
    addWidgetToggle(gui, false, 14, "attrmirrory0", 90, force_l5);
    addWidgetLabel(gui, "mirror y",110, force_l5);
        
    addWidgetToggle(gui, false, 14, "attrmirrorx1", 410, force_l5);
    addWidgetLabel(gui, "mirror x",430, force_l5);
    addWidgetToggle(gui, false, 14, "attrmirrory1", 490, force_l5);
    addWidgetLabel(gui, "mirror y",510, force_l5);
    
    addWidgetToggle(gui, false, 14, "attrmirrorx2", 810, force_l5);
    addWidgetLabel(gui, "mirror x",830, force_l5);
    addWidgetToggle(gui, false, 14, "attrmirrory2", 890, force_l5);
    addWidgetLabel(gui, "mirror y",910, force_l5);
    
    
    //------------------------------COLOR------------------------------------
    colorl1 = force_l5 + 100;
    int colorl2 = colorl1 + 20;
    int colorl3 = colorl2 + 20;
    int colorl4 = colorl3 + 20;

    
    //pad couleur
    label = new ofxUILabel(2, colorl1, "Part", "Part", OFX_UI_FONT_MEDIUM);
    gui->addWidget(label);
    ofPoint colorPoint = ofPoint(partS,partV);
    pad = new ofxUI2DPad(60, colorl1, 200, 100, ofPoint(0,255), ofPoint(0,255), ofPoint(120,120),"partSV");
    pad->getLabel()->setVisible(false);
    gui->addWidget(pad);
    slider = new ofxUISlider(60, colorl1+100, 200, 20, 0, 254, 120, "partHue");
    gui->addWidget(slider);
    
    //pad bg
    label = new ofxUILabel(1080, colorl1, "BackGround", "BackGround", OFX_UI_FONT_MEDIUM);
    gui->addWidget(label);
    colorPoint = ofPoint(bgS,bgV);
    pad = new ofxUI2DPad(860, colorl1, 200, 100, ofPoint(0,255), ofPoint(0,255), ofPoint(120,120),"bgSV");
    pad->getLabel()->setVisible(false);
    gui->addWidget(pad);
    slider = new ofxUISlider(860, colorl1+100, 200, 20, 0, 254, 120, "bgHue");
    gui->addWidget(slider);
    
    //Radio
    springTypes.clear();
    springTypes.push_back("Fixed_Color");
    springTypes.push_back("Z_Gradient");
    springTypes.push_back("Speed_Gradient");
    radio = new ofxUIRadio(300, colorl1  , 24, 24, "gradienttype", springTypes, 0);
    gui->addWidget(radio);
    
    //Alpha settings
    addWidgetLabel(gui, "Alphablur", 450, colorl1);
    addWidgetSlider(gui, 0, 255, 125, 100, "alphablur", 520, colorl1);
    addWidgetLabel(gui, "Finalblur", 450, colorl2);
    addWidgetSlider(gui, 0, 255, 125, 100, "finalblur", 520, colorl2);
    addWidgetLabel(gui, "Size", 650, colorl1);
    addWidgetSlider(gui, 0, 20, 125, 100, "partsize", 720, colorl1);
    addWidgetLabel(gui, "AlphaPart", 650, colorl2);
    addWidgetSlider(gui, 0, 255, 125, 100, "alphaparticle", 720, colorl2);
    
    //-------Gradient list from files
//    string path = "/my/path/file";
//    ofDirectory dir(path);
//    dir.allowExt("png");
//    dir.listDir();
//    springTypes.clear();
//    for(int i = 0; i < dir.numFiles(); i++){
//        springTypes.push_back(dir.getPath(i));
//    }
    
    //---- Gradient list coded
    springTypes.clear();
    springTypes.push_back("portugal");
    springTypes.push_back("polymaroon");
    springTypes.push_back("BW");
    springTypes.push_back("magenta");
    springTypes.push_back("lagoon");
    springTypes.push_back("fire");
    springTypes.push_back("rasta");
    springTypes.push_back("Burnigsmoke");
    springTypes.push_back("Bhorizon");
    springTypes.push_back("Gflash");
    springTypes.push_back("GOneon");
    springTypes.push_back("Oflash");
    springTypes.push_back("Ycontrast");
    
    ofxUIDropDownList* dropdown = new ofxUIDropDownList(450, colorl4, 150, "gradname", springTypes, 1);
    dropdown->setAutoClose(true);
    gui->addWidget(dropdown);
    addWidgetLabel(gui, "Range", 600, colorl4);
    ofxUIRangeSlider* rangeSlider = new ofxUIRangeSlider(640, colorl4, 150, 20, 0, 10000, 0, 1000, "maxgrad");
    gui->addWidget(rangeSlider);
    
    
    //------------------------------ADDONS : camera,  blob------------------------------------
    int addonsl1 = colorl4 + 110;
    int addonsl2 = addonsl1 + 20;
    int addonsl3 = addonsl2 + 40;
    
    
    // camera
    label = new ofxUILabel(2, addonsl1, "Camera", "Camera", OFX_UI_FONT_MEDIUM);
    gui->addWidget(label);
        
    addWidgetLabel(gui, "Rot x", 20 , addonsl2);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "camrotx", 60, addonsl2);
    addWidgetLabel(gui, "Rot y ", 95, addonsl2);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "camroty", 135, addonsl2);
    addWidgetLabel(gui, "Rot y ", 170, addonsl2);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "camrotz", 210, addonsl2);
    addWidgetLabel(gui, "Dist", 245, addonsl2);
    addWidgetSlider(gui, 0, 255, 128, 100, "camdist", 285, addonsl2 );
    
    //blob symetrie
    label = new ofxUILabel(600, addonsl1, "Blob", "Blob", OFX_UI_FONT_MEDIUM);
    gui->addWidget(label);
    
    
    //Blob forces
    addWidgetToggle(gui, true, 14, "blobin", 600, addonsl2);
    addWidgetLabel(gui, "repuls/attr", 620, addonsl2);
    addWidgetLabel(gui, " Force ", 720 , addonsl2);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "blobforce", 760, addonsl2);
    addWidgetLabel(gui, " Z: ", 805, addonsl2);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "blobz", 825, addonsl2);
    addWidgetLabel(gui, " Out Vel ", 860, addonsl2);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "bloboutvel", 900, addonsl2);
    addWidgetLabel(gui, " Blur ", 935, addonsl2);
    addWidgetRotSlider(gui, 0, 255, 0, 35, "blobblur", 960, addonsl2);
    
    addWidgetToggle(gui, true, 14, "blobmirrorx", 600, addonsl3);
    addWidgetLabel(gui, "Mirror x", 620, addonsl3);
    addWidgetToggle(gui, true, 14, "blobmirrory", 680, addonsl3);
    addWidgetLabel(gui, "Mirror y", 700, addonsl3);
    addWidgetToggle(gui, true, 14, "blobxinvert", 780, addonsl3);
    addWidgetLabel(gui, "Invert x", 800, addonsl3);
    addWidgetToggle(gui, true, 14, "blobyinvert", 880, addonsl3);
    addWidgetLabel(gui, "Invert y", 910, addonsl3);
    

    

    


    gui->autoSizeToFitWidgets();
    gui->setPosition(0, menuHeight+10);

    //add gui to a c++ stl vector
    guis.push_back(gui);
    //add gui to a c++ stl map
    guihash["preset"] = gui;
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setupGUI3()
{

    ofxUIScrollableCanvas *gui = new ofxUIScrollableCanvas;
    gui->setName("gui1");
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);

    
    int menul1 = 80;
    int menul2 = menul1 + 80;
    
    // list of saved files
    string path = "save/";
    ofDirectory dir(path);
    dir.allowExt("xml");
    dir.listDir();
    springTypes.clear();
    string temp;
    for(int i = 0; i < dir.numFiles(); i++){
        temp = dir.getName(i);
        springTypes.push_back(temp);
    }
    
    loadWidget = new ofxUIDropDownList(100, menul1 , 150, "load list", springTypes, 2);
    loadWidget->setAutoClose(true);
    gui->addWidget(loadWidget);
    
    addWidgetLabel(gui, "OPEN", 2, menul1);
    addWidgetButton(gui, "open", 40, menul1);
    
    addWidgetLabel(gui, "SAVE", 2, menul2);
    addWidgetButton(gui, "save", 40, menul2);
    recWidget = new ofxUITextInput(100, menul2, 150, "savename", "nom", 2);
    gui->addWidget(recWidget);
    
    
    gui->autoSizeToFitWidgets();
    gui->setPosition(0, menuHeight+10);
    gui->setHeight(550);
    //add gui to a c++ stl vector
    guis.push_back(gui);
    //add gui to a c++ stl map
    guihash["menu"] = gui;
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    
    
}

void testApp::setupGUI4()
{

}

void testApp::setupGUI5()
{

}

void testApp::setupGUI6()
{

}

//--------------------------------------------------------------
void testApp::update()
{
    
}

//--------------------------------------------------------------
void testApp::draw()
{
	
    bool displayText = false;
    
    if(isModified>0){
    isModified--;
    displayText = true;
    }
    
    
    ofBackground(backgroundColor);
	ofPushStyle();
    
    
    if(currentGui==2){
    
       // Draw a small popup with the modified value
        if(isModified)
        {
            ofFill();
            ofSetColor(ofColor::white);
            ofRect(ofGetMouseX()+10, ofGetMouseY()+20, 100, 100);
            ofRect(ofGetMouseX()+10, ofGetMouseY()+20, 100, 100);
            
            ofSetColor(ofColor::black);
            ofDrawBitmapString(currValue, ofPoint(ofGetMouseX()+10, ofGetMouseY()+40));
            ofDrawBitmapString(ofToString(ofGetMouseX()), ofPoint(ofGetMouseX()+10, ofGetMouseY()+50));
            ofDrawBitmapString(ofToString(ofGetMouseY()), ofPoint(ofGetMouseX()+10, ofGetMouseY()+70));
            ofDrawBitmapString(currValue, ofPoint(ofGetMouseX()+10, ofGetMouseY()+40));
    //        ofDrawBitmapStringHighlight(currValue, ofGetMouseX()+10, ofGetMouseY()+20,ofColor(255,255,255,255),ofColor(0,0,0,255));
        }
        
        
        
        // Display colors of particules && background
        
        
        ofSetColor(255,255,255); 
        fboPart.draw(60, 10 + colorl1 + menuHeight);
        fboBg.draw(860,10 + colorl1 + menuHeight);
        ofSetColor(partColor);
        ofRect(2, 40 + colorl1 + menuHeight, 50, 70);
        ofSetColor(bgColor);
        ofRect(1080, 40 + colorl1 + menuHeight, 50, 70);
        
        
    }
   
    
    
    
	
	ofPopStyle();
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e) 
{
	ofxOscMessage msg;
    string address ="/";
    msg.setAddress(address);
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    
    // NORMAL PROCESS ----------------------------
    
    switch (kind) {
        case 2: // Toggle
        {
            toggle = ( ofxUIToggle *)e.widget;
            address += toggle->getLabel()->getLabel();
            msg.setAddress(address);
            msg.addIntArg(toggle->getValue());
            
            if(name=="open/save") chooseCurrentGui(3);
            if(name=="controls") chooseCurrentGui(2);
        }
            break;
        case 22: // Dropdown list
        {
            dropDownList = ( ofxUIDropDownList *)e.widget;
            address += "gradname";

            string gradientName;

            
            // Coin qui pose problme
            
            if(  dropDownList->isOpen())
            {gradientName = dropDownList->getSelected().front()->getName();}
            else{ gradientName="gradname"; }
            
            if(gradientName != "gradname"){
            msg.setAddress(address);
            msg.addStringArg(gradientName);
            }

        }
            break;
        case 4: // Slider
        {
            slider = (ofxUISlider *) e.widget;
            address += slider->getName();
            msg.setAddress(address);
            msg.addFloatArg(slider->getValue()*slider->getMax());
            currValue = ofToString( slider->getValue()*slider->getMax() );
            int valueChar = (int)(slider->getValue()*slider->getMax());
            isModified = 120;
            
          

        }
            break;
        case 21: // Rotary slider
        {
            rotSlider = (ofxUIRotarySlider *) e.widget;
            address += rotSlider->getName();
            msg.setAddress(address);
            msg.addFloatArg(rotSlider->getValue()*rotSlider->getMax());
            currValue = ofToString( rotSlider->getValue()*rotSlider->getMax() );
            isModified = 120;

        }
            break;
        case 3: // Button   -> this means : from the menu
        {
            
            if(name=="save"){
                savePreset();
            }
            if(name=="open"){
                openPreset(false);
            }
            
        }
            
        default:
            break;
    }
    
    
    // EXEPTIONS ---------------
    
    if(name == "partHue"){
        partHue =  static_cast<ofxUISlider*>(e.widget)->getValue()*static_cast<ofxUISlider*>(e.widget)->getMax();
        setPadBg( (int) partHue, true);
    }
    if(name == "bgHue"){
        bgHue =  static_cast<ofxUISlider*>(e.widget)->getValue()*static_cast<ofxUISlider*>(e.widget)->getMax();
        setPadBg( (int) bgHue, false);
    }
    
    if(name == "partSV"){
        
        ofxUI2DPad* myPad = ( ofxUI2DPad *) e.widget;
        ofPoint value = myPad->getValue();
        partS = value.x * 255.0f;
        partV = value.y * 255.0f;
        partColor = ofColor::fromHsb(partHue, partS, partV);
        
        address += "rpart";
        msg.setAddress(address);
        msg.addFloatArg(partColor.r);
        //msg.addIntArg((int)partColor.r);
        sender.sendMessage(msg);

        msg.clear();
        address = "/gpart";
        msg.setAddress(address);
        msg.addIntArg((int)partColor.g);
        sender.sendMessage(msg);
        
        msg.clear();
        address = "/bpart";
        msg.setAddress(address);
        msg.addIntArg((int)partColor.r);
        sender.sendMessage(msg);
        
        
        
    }
    if(name == "bgSV"){
        
        ofxUI2DPad* myPad = ( ofxUI2DPad *) e.widget;
        ofPoint value = myPad->getValue();
        bgS = value.x * 255.0f;
        bgV = value.y * 255.0f;
        bgColor = ofColor::fromHsb(bgHue, bgS, bgV);
        
        address += "rback";
        msg.setAddress(address);
        msg.addFloatArg(bgColor.r);
        //msg.addIntArg((int)partColor.r);
        sender.sendMessage(msg);
        
        msg.clear();
        address = "/gback";
        msg.setAddress(address);
        msg.addIntArg((int)bgColor.g);
        sender.sendMessage(msg);
        
        msg.clear();
        address = "/bback";
        msg.setAddress(address);
        msg.addIntArg((int)bgColor.r);
        sender.sendMessage(msg);
        msg.clear();
        
    }


    
 
    if(msg.getAddress().length()>4);
        sender.sendMessage(msg);
    
    //Update texteArea
    textArea->setTextString(currValue);
    
 

    
    
}
//--------------------------------------------------------------
void testApp::loadGUISettings()
{
    vector<ofxUIScrollableCanvas *>::iterator it;
    for(it = guis.begin(); it != guis.end(); it++)
    {
        (*it)->loadSettings("GUI/"+(*it)->getName()+"Settings.xml");
    }
}
//--------------------------------------------------------------
void testApp::saveGUISettings()
{
    vector<ofxUIScrollableCanvas *>::iterator it;
    for(it = guis.begin(); it != guis.end(); it++)
    {
        (*it)->saveSettings("GUI/"+(*it)->getName()+"Settings.xml");
    }
}
//--------------------------------------------------------------
void testApp::exit()
{
    saveGUISettings();
    
    for(int i = 0; i < guis.size(); i++)
    {
        ofxUICanvas *gui = guis[i];
        delete gui;
    }
    guis.clear();
    
//    delete rotSlider;
//    delete label;
//    delete circleSlider;
//    delete toggle;
//    delete radio;
//    delete slider;
//    delete pad;
//    delete button;
}
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void testApp::chooseCurrentGui(int num){
    
    currentGui = num;
    vector<ofxUIScrollableCanvas *>::iterator it;
    int i=1;
    for(it = guis.begin(); it != guis.end(); it++)
    {
        if(i==num || i==1  ) (*it)->setVisible(true);
        else(*it)->setVisible(false);
        
        i++;
    }
    
}
//--------------------------------------------------------------
void testApp::addWidgetLabel(ofxUIScrollableCanvas * gui,string name){
    

        label = new ofxUILabel(name, OFX_UI_FONT_SMALL);
        gui->addWidgetRight(label);

    
}
void testApp::addWidgetLabel(ofxUIScrollableCanvas * gui,string name, float x, float y){
    
    
    label = new ofxUILabel(x, y, name, name, OFX_UI_FONT_SMALL);
    gui->addWidget(label);
    
    
}

//--------------------------------------------------------------
void testApp::addWidgetSlider(ofxUIScrollableCanvas * gui,float min, float max, float value , float size, string name){
    

    slider = new ofxUISlider(name, min, max, value, size, 10);
    slider->getLabel()->setVisible(false);
    gui->addWidgetRight(slider);

    
}

//--------------------------------------------------------------
void testApp::addWidgetSlider(ofxUIScrollableCanvas * gui,float min, float max, float value , float size, string name, float x, float y ){
    
    
    slider = new ofxUISlider(x, y, size, 10, min, max, value, name);
    slider->getLabel()->setVisible(false);
    gui->addWidget(slider);
    
}

void testApp::addWidgetSlider(ofxUIScrollableCanvas * gui,float min, float max, float value , float size, string name, float x, float y, ofColor fill ){
    
    ofColor col = ofColor::blue;
    
    slider = new ofxUISlider(x, y, size, 10, min, max, value, name);
    slider->getLabel()->setVisible(false);
    slider->setDrawFill(true);
    slider->setColorBack(col);
    slider->setColorFill(col);
    slider->setColorPadded(col);
    gui->addWidget(slider);
    
}

//--------------------------------------------------------------
void testApp::addWidgetRotSlider(ofxUIScrollableCanvas * gui,float min, float max, float value , float size, string name){
    
    rotSlider = new ofxUIRotarySlider(size, min, max, value, name);
    rotSlider->getLabel()->setVisible(false);
    gui->addWidgetRight(rotSlider);
    
}

//--------------------------------------------------------------
void testApp::addWidgetRotSlider(ofxUIScrollableCanvas * gui,float min, float max, float value , float size, string name, float x , float y ){
    
    rotSlider = new ofxUIRotarySlider(x, y, size, min, max, value, name);
    rotSlider->getLabel()->setVisible(false);
    gui->addWidget(rotSlider);
    
}

//--------------------------------------------------------------
void testApp::addWidgetToggle(ofxUIScrollableCanvas * gui,bool value,int size, string name, string northName){
    
    toggle = new ofxUIToggle(name, value, 12, size);
    toggle->getLabel()->setVisible(false);
    gui->addWidgetSouthOf(toggle, northName);
    
    
}

//--------------------------------------------------------------
void testApp::addWidgetToggle(ofxUIScrollableCanvas * gui,bool value,int size, string name, float x, float y){
    
    toggle = new ofxUIToggle(x, y, 12, size, value, name);
    toggle->setValue(value);
    toggle->getLabel()->setVisible(false);
    gui->addWidget(toggle);
    
    
}

//--------------------------------------------------------------
void testApp::addWidgetButton(ofxUIScrollableCanvas *gui, string name, float x, float y){
    
    button = new ofxUIButton(x, y, 30, 30, false, name);
    button->getLabel()->setVisible(false);
    gui->addWidget(button);
    
    
}

//--------------------------------------------------------------
void testApp::setPadBg(int hue, bool isPart){
    
    ofColor rgbColor;
    float s,v;
    
    ofFbo* fbo;
    if(isPart) fbo = &fboPart;
    else fbo = & fboBg;
    
    fbo->begin();
    ofClear(255,255,255, 0);
        
    for(int i=0; i<200; i++){
    for(int j=0; j<100; j++){
        
        s= (i/(250.0))*255.0f;
        v = (j/150.0)*255.0f;
        
        rgbColor= ofColor::fromHsb(hue, s, v);
        rgbColor.a = 255;
        ofSetColor(rgbColor);
    
        ofCircle(i, j, 1.0f);

        
    }
    }
    


    fbo->end();
    
}

//--------------------------------------------------------------
void testApp::savePreset(){

    ofxUIScrollableCanvas * gui = guihash.at("preset");
    string name = "save/"+recWidget->getTextString()+".xml";
    gui->saveSettings(name);
    
    //refreshing the open file list
    vector<ofxUILabelToggle *> list =  loadWidget->getToggles();
    ofxUIWidget* wid = list.back();
    string lastname = wid->getName();
    
    if(lastname != recWidget->getTextString()+".xml")
    {
    loadWidget->addToggle(recWidget->getTextString()+".xml");
    }
    
    ofSleepMillis(200);
    
    
}

//--------------------------------------------------------------
void testApp::openPreset(bool init){
  
    ofxUIScrollableCanvas * gui = guihash.at("preset");
    
    
    if(init) // at the beggining , open the default preset file
    {
     
        gui->loadSettings("save/empty.xml");
        
    }
    else  // open the xml file from the selected name of loadWidget
    {
    
        vector<ofxUIWidget *> list =  loadWidget->getSelected();
        ofxUIWidget* wid = list.front();
        string name = "save/"+wid->getName();
        gui->loadSettings(name);
        
        
        
    }
    
    ofSleepMillis(200);
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}