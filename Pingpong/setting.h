#ifndef SETTING_H
#define SETTING_H

const int CXINSIDE = 480;
const int CYINSIDE = 270;
const int BASEFPS = 40;
const double ST_RACKETSPEEDMULTIPY = 0.5;

const int GAMEWINDOW_LOGICALWIDTH = 240;
const int GAMEWINDOW_LOGICALHEIGHT = 270;

const bool DEBUGMODE = true;
const unsigned int OPRT_DELAY_TOLERANCE = 2500;

const std::string RESPATH= "res\\";

#define WM_SHOWCURSOR 0x401
#define WM_HIDECURSOR 0x402
#define WM_OBJUNLOAD  0x403
#define WM_BRICKHIT 0x404

#endif
