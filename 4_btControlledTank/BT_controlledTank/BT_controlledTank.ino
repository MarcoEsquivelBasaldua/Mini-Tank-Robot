#include "src/typeDefs/typeDefs.h"
#include "src/BT_encodedData/BT_encodedData.h"
#include "src/tankTrack/tankTrack.h"

/**************************************************************************************
*  Wiring
*                       ________________ 
*    _____________     |                |     __________________      _____________
*   |          VCC|<---|GND           5V|--->|ENA           OUT1|--->|             |
*   |          GND|<---|5V  ARDUINO   11|--->|IN1               |    | RIGHT WHEEL |
*   |  HC-06    Tx|--->|Rx    UNO     10|--->|IN2   L298N   OUT2|--->|_____________|
*   |           Rx|<---|Tx             9|--->|IN3               |     _____________
*   |_____________|    |               6|--->|IN4           OUT3|--->|             |
*                      |              5V|--->|ENB               |    | LEFT WHEEL  |
*                      |________________|    |              OUT4|--->|_____________|
*                                            |      JUMPER      |
*                      _________________     |       .-.        |
*                     |  BATTERY 7.4V  +|--->|VIN               |
*                     |                -|--->|GND               |
*                     |_________________|    |__________________|
*
***************************************************************************************/

//----------------- Tank Tracks ----------------//
TankTrack tankTrack;

void setup() {
  Serial.begin(9600);
  tankTrack.stop();
}

void loop() {

  if (Serial.available()) {
    char c_command = Serial.read();

    switch (c_command)
    {
      case BT_STOP:
        tankTrack.stop();
        break;
      case BT_FORWARD:
        tankTrack.forward(MEDIUM_SPEED);
        break;
      case BT_BACKWARD:
        tankTrack.backward(MEDIUM_SPEED);
        break;
      case BT_LEFT:
        tankTrack.turnLeftFast(MEDIUM_SPEED);
        break;
      case BT_RIGHT:
        tankTrack.turnRightFast(MEDIUM_SPEED);
        break;
      case BT_FORWARD_LEFT:
        tankTrack.turnLeft(MEDIUM_SPEED);
        break;
      case BT_FORWARD_RIGHT:
        tankTrack.turnRight(MEDIUM_SPEED);
        break;
      case BT_BACKWARD_RIGHT:
        tankTrack.setTracksSpeed(-(uint16)(MEDIUM_SPEED), STOP_RPM);
        break;
      case BT_BACKWARD_LEFT:
        tankTrack.setTracksSpeed(STOP_RPM, -(uint16)(MEDIUM_SPEED));
        break;
      default:
        tankTrack.stop();
        break;
    }
  }
}
