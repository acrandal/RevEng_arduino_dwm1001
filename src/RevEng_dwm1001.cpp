/**
 *  RevEng dwm1001 3D RTL developer board wrapper
 * 
 *  @author Aaron S. Crandall <crandall@gonzaga.edu>
 *  @copyright 2024
 * 
 *  @license GPL v3.0
 * 
 *  Device: Qovro Decawave DWM1001 Developer Board
 *  Connection: Serial
 * 
 */




#include "RevEng_dwm1001.h"

uint8_t RevEng_dwm1001::begin(HardwareSerial &serialPort) {
    _serialPort = &serialPort;
    return 0;
}

NodePosition RevEng_dwm1001::getPosition() {
    uint8_t resp[20];
    NodePosition nodePos;
    memset(resp, 0, sizeof(resp));

    // Send "give the position" request (see DW1001 TVL request)
    // 4.3.2 - dwm_pos_get
    // 4.3.2.3 - SPI/UART Generic
    _serialPort->write(0x02);
    _serialPort->write(0x00);
    delay(20);  // Await response (20 not well decided, but works)

    int index = 0;
    while(_serialPort->available()){
        resp[index++] = _serialPort->read();
    }

    // Serial.print("Resp:");
    // for( int i = 0; i < 18; i++ ) {
    //     Serial.print(resp[i]);
    //     Serial.print(" ");
    // }
    // Serial.println();

    int32_t coord_x = (int32_t) resp[5] |
                      ((int32_t) resp[6] << 8) |
                      ((int32_t) resp[7] << 16) |
                      ((int32_t) resp[8] << 24);

    int32_t coord_y = (int32_t) resp[9] |
                      ((int32_t) resp[10] << 8) |
                      ((int32_t) resp[11] << 16) |
                      ((int32_t) resp[12] << 24);

    int32_t coord_z = (int32_t) resp[13] |
                      ((int32_t) resp[14] << 8) |
                      ((int32_t) resp[15] << 16) |
                      ((int32_t) resp[16] << 24);

    uint8_t quality = resp[17];

    nodePos.x = coord_x;
    nodePos.y = coord_y;
    nodePos.z = coord_z;
    nodePos.quality = quality;

    return nodePos;
}