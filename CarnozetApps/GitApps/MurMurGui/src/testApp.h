#ifndef TESTAPP
#define TESTAPP

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOsc.h"

class testApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void exit();
    
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
    void setupGUI1();
    void setupGUI2();
    void setupGUI3();
    void setupGUI4();
    void setupGUI5();
    void setupGUI6();
    
    void saveGUISettings();
    void loadGUISettings();
    
    void chooseCurrentGui(int num);
    int currentGui;
    
    void addWidgetLabel(ofxUIScrollableCanvas*, string name );
    void addWidgetLabel(ofxUIScrollableCanvas * gui,string name, float x, float y);
    void addWidgetRotSlider(ofxUIScrollableCanvas * gui,float min, float max, float value , float size, string name);
    void addWidgetSlider(ofxUIScrollableCanvas * gui,float min, float max, float value , float size, string name);
    void addWidgetSlider(ofxUIScrollableCanvas * gui,float min, float max, float value , float size, string name, float x ,float y );
    void addWidgetSlider(ofxUIScrollableCanvas * gui,float min, float max, float value , float size, string name, float x, float y, ofColor fill );
    void addWidgetToggle(ofxUIScrollableCanvas * gui,bool value,int size, string name, string northName);
    void addWidgetToggle(ofxUIScrollableCanvas * gui,bool value,int size, string name, float x, float y);
    void addWidgetRotSlider(ofxUIScrollableCanvas * gui,float min, float max, float value , float size, string name, float x , float y );
    void addWidgetButton(ofxUIScrollableCanvas * gui,string name, float x, float y);
    
    vector<ofxUIScrollableCanvas *> guis;
	map<string, ofxUIScrollableCanvas *> guihash;
    void guiEvent(ofxUIEventArgs &e);  // C'est ici que les msg osc sont envoyés
    bool drawFill;
	float red, green, blue, alpha;
    float menuHeight;
    
    //Fix widget
    ofxUITextArea* textArea;
    ofRectangle popup;
    int isModified;
    
    //Color Variables
    int colorl1;
    ofColor partColor;
    ofColor bgColor;
    ofFbo fboPart;
    ofFbo fboBg;
    void setPadBg( int hue, bool isPart);
    float partHue;
    float partS;
    float partV;
    float bgHue;
    float bgS;
    float bgV;
    
    //Save - Open xml files
    ofxUIDropDownList* loadWidget;
    ofxUITextInput* recWidget;
    void savePreset();
    void openPreset(bool init);
    
    //Temp variable to use everywhere
    ofxUIRotarySlider* rotSlider;
    ofxUILabel* label;
    ofxUICircleSlider* circleSlider;
    ofxUIToggle* toggle;
    ofxUIDropDownList * dropDownList;
    ofxUIRadio* radio;
    ofxUISlider* slider;
    ofxUI2DPad* pad;
    ofxUIButton* button;
    
    
    vector<string> springTypes;
    string currValue;
    
    ofColor backgroundColor;
    float radius;
    int resolution;
    ofPoint position;
    
    float noiseScale;
    float *buffer;
    vector<float> xPos;
    vector<float> yPos;
    
    ofxUIMovingGraph *xGraph;
    ofxUIMovingGraph *yGraph;
    int bufferSize;
    ofImage *image;
    
    //OSC Message
    ofxOscSender sender;
    
    
    
    
};

#endif