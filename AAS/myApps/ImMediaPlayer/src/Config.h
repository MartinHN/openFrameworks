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
#define PROJECTPATH "/Users/Sha/Movies/"
#define PROJECTBHEIGHT 40
#define PROJECTWIDTH 80
#define PROJECTPAD 10

typedef enum{PDF,IMAGE,VIDEO} MediaType;





//Screen


#define ANCHOR_WIDTH 100
#define ANCHOR_HEIGHT 50

//Glove
#define SERVERPORT 25000
#define LOCALPORT 25100
#define APPNAME "ImMedia"
#define NUMTOUCH 4
#define NUMFLEX 4

typedef enum{A_CLICK=0,A_DRAG,A_ZOOM,A_BACK}TouchAction;
typedef enum {T_DOWN=0,T_UP,T_CLICK,T_LONGPRESS} touchType;

//#define SYPHON
#define MOUSEDBG
#endif
