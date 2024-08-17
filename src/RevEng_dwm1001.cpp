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


/**
 * @brief Initializes communication with the DWM1001 node using a specified hardware serial port.
 *
 * @param serialPort
 *   A reference to the `HardwareSerial` object that will be used for communication with the DWM1001 node.
 *   This typically refers to one of the hardware serial ports on the microcontroller (e.g., `Serial1`, `Serial2`).
 *   The serial port is expected to be set to 115200 baud rate
 *
 * @return uint8_t
 *   Returns 0 to indicate successful initialization.
 *
 * @note This method only sets the serial port for communication; it does not perform any other
 *       initialization or configuration of the DWM1001 node itself.
 */
bool RevEng_dwm1001::begin(HardwareSerial &serialPort) {
    _serialPort = &serialPort;
    return isNodeConnected();
}

/**
 * @brief Checks if the DWM1001 node is connected and responsive.
 *
 * This method communicates with the DWM1001 UWB node to verify its connection by
 * sending a `dwm_cfg_ver` command, which requests the hardware and software version
 * information from the node.
 *
 * @return bool
 *   - `true` if the node is connected and the response is as expected.
 *   - `false` if the response does not match the expected pattern, indicating a possible connection issue.
 */

bool RevEng_dwm1001::isNodeConnected() {
    uint8_t resp[22];
    uint8_t respLen = 22;
    memset(resp, 0, sizeof(resp));
    // Send dwm_cfg_ver - hardware and software versions
    _serialPort->write(0x15);
    _serialPort->write(0x00);
    delay(20);

    int index = 0;
    while(_serialPort->available() && index < respLen) {
        resp[index++] = _serialPort->read();
    }

    return resp[0] == 0x40 && resp[3] == 0x50;
}


/**
 * @brief Checks if the DWM1001 node is operating in Tag mode.
 *
 * This method communicates with the DWM1001 UWB node to query its current
 * configuration and determine if it is operating in Tag mode. The function
 * sends a configuration request command (`dwm_cfg_get`) to the node and waits
 * for the response, which includes the mode byte. The mode byte is then checked
 * to see if the Tag mode bit is unset, indicating Tag mode operation.
 *
 * The configuration request follows the DWM1001 protocol as outlined in the 
 * documentation, section 4.3.7.3 - dwm_cfg_get - SPI/UART Generic.
 * The response is expected to be 7 bytes long, where the 6th byte contains
 * the mode configuration.
 *
 * @return bool
 *   - `true` if the node is in Tag mode.
 *   - `false` otherwise.
 *
 * @note The delay of 20ms after sending the request is empirically chosen and may need 
 *       adjustment based on the specific application or node behavior.
 */
bool RevEng_dwm1001::isInTagMode() {
    uint8_t resp[7];
    uint8_t respLen = 7;
    memset(resp, 0, sizeof(resp));
    // Send dwm_cfg_get - get configuration, inc mode, 4.3.7.3
    _serialPort->write(0x08);
    _serialPort->write(0x00);
    delay(20);  // Await response (20 not well decided, but works)

    int index = 0;
    while(_serialPort->available() && index < respLen) {
        resp[index++] = _serialPort->read();
    }

    uint8_t modeByte = resp[6] & 0b00100000;
    return modeByte == 0b00000000;
}


/**
 * @brief Checks if the DWM1001 node is operating in Anchor mode.
 *
 * This method communicates with the DWM1001 UWB node to query its current
 * configuration and determine if it is operating in Anchor mode. The function
 * sends a configuration request command (`dwm_cfg_get`) to the node and waits
 * for the response, which includes the mode byte. The mode byte is then checked
 * to see if the Anchor mode bit is set.
 *
 * The configuration request follows the DWM1001 protocol as outlined in the 
 * documentation, section 4.3.7.3 - dwm_cfg_get - SPI/UART Generic.
 * The response is expected to be 7 bytes long, where the 6th byte contains
 * the mode configuration.
 *
 * @return bool
 *   - `true` if the node is in Anchor mode.
 *   - `false` otherwise.
 *
 * @note The delay of 20ms after sending the request is empirically chosen and may need 
 *       adjustment based on the specific application or node behavior.
 */
bool RevEng_dwm1001::isInAnchorMode() {
    uint8_t resp[7];
    uint8_t respLen = 7;
    memset(resp, 0, sizeof(resp));
    // Send dwm_cfg_get - get configuration, inc mode, 4.3.7.3
    _serialPort->write(0x08);
    _serialPort->write(0x00);
    delay(20);  // Await response (20 not well decided, but works)

    int index = 0;
    while(_serialPort->available() && index < respLen) {
        resp[index++] = _serialPort->read();
    }

    uint8_t modeByte = resp[6] & 0b00100000;
    Serial.println(modeByte);
    return modeByte == 0b00100000;
}

bool RevEng_dwm1001::isLocationReady() {
    uint8_t resp[6];
    uint8_t respLen = 6;
    memset(resp, 0, sizeof(resp));
    // Send dwm_status_get - get status, inc position ready, 4.3.19
    _serialPort->write(0x32);
    _serialPort->write(0x00);
    delay(20);

    int index = 0;
    while(_serialPort->available() && index < respLen) {
        resp[index++] = _serialPort->read();
    }

    uint8_t readyByte = resp[5] & 0b00000001;
    return readyByte == 0b00000001;
}


/**
 * @brief Retrieves the 3D position and quality metrics of the node.
 *
 * This function communicates with the DWM1001 UWB node via the serial interface
 * to request the current 3D position (X, Y, Z coordinates) and quality metric.
 * The function sends a position request command to the node, waits for the
 * response, and then extracts the position data from the received bytes.
 *
 * The position request follows the DWM1001 protocol as outlined in the 
 * DW1001 TVL request documentation, section 4.3.2 - dwm_pos_get - 4.3.2.3 - SPI/UART Generic.
 * The response is expected to be 18 bytes long, containing the X, Y, Z coordinates
 * as 32-bit signed integers in little-endian format, and a quality metric as an 8-bit value.
 *
 * @return NodeLocation 
 *   A struct containing the following members:
 *   - x: int32_t - X coordinate of the node.
 *   - y: int32_t - Y coordinate of the node.
 *   - z: int32_t - Z coordinate of the node.
 *   - quality: uint8_t - Quality metric of the position data.
 *
 * @note The delay of 20ms after sending the request is empirically chosen and may need 
 *       adjustment based on the specific application or node behavior.
 */
NodeLocation RevEng_dwm1001::getLocation() {
    uint8_t resp[18];
    uint8_t respLen = 18;
    NodeLocation nodeLocation;
    memset(resp, 0, sizeof(resp));

    // Send "give the position" request (see DW1001 TVL request)
    // 4.3.2 - dwm_pos_get - 4.3.2.3 - SPI/UART Generic
    _serialPort->write(0x02);
    _serialPort->write(0x00);
    delay(20);  // Await response (20 not well decided, but works)

    int index = 0;
    while(_serialPort->available() && index < respLen){
        resp[index++] = _serialPort->read();
    }

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

    nodeLocation.x = coord_x;
    nodeLocation.y = coord_y;
    nodeLocation.z = coord_z;
    nodeLocation.quality = quality;

    return nodeLocation;
}