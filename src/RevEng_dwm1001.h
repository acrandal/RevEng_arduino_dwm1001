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
 *  Stores one position sample with x,y,z measurements
 *  Distance measurements are in mm
 *  Quality is a 0..100 value of the sample's estimated quality/trustworthiness
 */
struct NodeLocation {
  int32_t x;
  int32_t y;
  int32_t z;
  uint8_t quality;
};


/**
 *  Main hardware driver class for the dwm1001
 */
class RevEng_dwm1001
{
    public:
        bool begin(HardwareSerial &serialPort);
        bool isNodeConnected();
        NodeLocation getLocation();
        bool isInTagMode();
        bool isInAnchorMode();
        bool isLocationReady();


    private:
        HardwareSerial *_serialPort;
};




#endif // __REVENG_DWM1001_H__