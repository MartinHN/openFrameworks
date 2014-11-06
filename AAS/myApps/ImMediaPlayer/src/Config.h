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
//#ifdef DEBUG
#define PROJECTPATH "/Users/Sha/Movies/"
//#endif
#ifndef DEBUG
//#define PROJECTPATH "/Users/immersif/Documents/GOSS/ImMedia/"
#endif
#define PROJECTBHEIGHT 40
#define PROJECTWIDTH 400
#define PROJECTPAD 10

typedef enum{PDF,IMAGE,VIDEO} MediaType;





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
typedef enum {GLOVE_DOWN=0,GLOVE_UP,GLOVE_SHORTPRESS,GLOVE_LONGPRESS} TouchState;

#define SYPHON
#define MOUSEDBG
#endif
