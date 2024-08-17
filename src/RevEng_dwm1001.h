/**
 * 
 * RevEng Qorvo Decawave dwm1001 Arduino Driver
 * 
 * @author Aaron S. Crandall <crandall@gonzaga.edu>
 * @copyright 2024
 * @license GPL 3.0
 * 
 * Simple wrapper to get location data out of the dwm1001 developer board.
 * 
 */

//  https://github.com/verlab/ros_decawave/blob/master/docs/DWM1001_DWM1001-DEV_MDEK1001_Sources_and_Docs_v8/DWM1001/Product%20and%20Design%20Documents/DWM1001-API-Guide.pdf

#ifndef __REVENG_DWM1001_H__
#define __REVENG_DWM1001_H__

#include "Arduino.h"


/**
 * 
 */
struct NodePosition {
  int32_t x;
  int32_t y;
  int32_t z;
  uint8_t quality;
};


/**
 * 
 */
class RevEng_dwm1001
{
    public:
        uint8_t begin(HardwareSerial &serialPort);
        NodePosition getPosition();


    private:
        HardwareSerial *_serialPort;
};




#endif // __REVENG_DWM1001_H__