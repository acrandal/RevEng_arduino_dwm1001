/**
 *
 *  Example dwm1001 location reading
 * 
 *  Author: Aaron S. Crandall <crandall@gonzaga.edu>
 *  Copyright: 2024
 *
 *  License: MIT
 */


#include <HardwareSerial.h>
HardwareSerial Serial4(1); // Use UART1 (Serial1)

#include "RevEng_dwm1001.h"

RevEng_dwm1001 rtl;

// ** ************************************************************************
void setup() {
  Serial.begin(115200);
  delay(500); // Let serial settle/connect

  // Example HardwareSerial on ESP32 pins 35,33 configuration
  Serial4.begin(115200, SERIAL_8N1, 35, 33);

  // rtl.begin() takes a HardwareSerial (like Serial, Serial1, etc)
  if( !rtl.begin(Serial4) ) {
    Serial.println("No DWM1001 Node found.");
  }

  Serial.println("dwm1001 connected - starting location polling");
}

// ** ************************************************************************
void loop() {
  // Check if there's a location update/data available
  if( rtl.isLocationReady() ) {
    // Get the node's measured location
    NodeLocation nodePos = rtl.getLocation();

    // Example of printing location out cleanly
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Location: X=%ld, Y=%ld, Z=%ld, Quality=%u",
             nodePos.x, nodePos.y, nodePos.z, nodePos.quality);
    Serial.println(buffer);
  }

  delay(500);
}

