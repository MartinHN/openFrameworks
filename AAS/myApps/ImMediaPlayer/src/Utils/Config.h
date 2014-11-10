//
//  Config.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#ifndef ImMedia_Config_h
#define ImMedia_Config_h

//projects


typedef enum{PDF,IMAGE,VIDEO} MediaType;

#define MIN_BOX_WIDTH 200
#define MIN_BOX_HEIGHT 200
#define ZOOM_FACTOR 0.001


//Screen
#define TOTALRES_X 5760


#define ANCHOR_WIDTH 300
#define ANCHOR_HEIGHT 200


//Glove
#define SERVERPORT 25000

#define SERVERIP "10.0.1.3"
#define LOCALPORT 25101
#define LOCALIP "10.0.1.2"
#define APPNAME "ImMedia"
#define NUMTOUCH 4
#define NUMFLEX 4

typedef enum{GLOVE_CLICK=0,GLOVE_DRAG,GLOVE_ZOOM,GLOVE_BACK}TouchType;
typedef enum {GLOVE_DOWN=0,GLOVE_UP,GLOVE_SHORTPRESS,GLOVE_LONGPRESS} TouchAction;

#define SYPHON
#define MOUSEDBG



#ifndef SUPPORTED_FORMATS
#define SUPPORTED_FORMATS
static vector<string> supported_formats;
static int dumb_for_formats = (supported_formats.push_back("avi"),
                               supported_formats.push_back("mp4"),
                               supported_formats.push_back("mov"),
                               supported_formats.push_back("pdf"),
                               supported_formats.push_back("png"),
                               supported_formats.push_back("jpg"),
                               supported_formats.push_back("jpeg"),
                               supported_formats.push_back("mp3"),
                               supported_formats.push_back("wav"),0);
#endif

#endif
