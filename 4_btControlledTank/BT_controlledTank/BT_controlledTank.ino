#include "src/typeDefs/typeDefs.h"
#include "src/BT_encodedData/BT_encodedData.h"
#include "src/tankTrack/tankTrack.h"

/**************************************************************************************
*  Wiring
*    ___________      ________________      __________________      _____________
*   |        VCC|<---|5V             4|--->|                A1|--->|             |
*   | HC-06  GND|<---|GND            6|--->|                  |    | RIGHT TRACK |
*   |         Tx|--->|Rx   ARDUINO    |    |      TB6612FNG   |    |_____________|
*   |         Rx|<---|Tx     UNO     2|--->|                  |     _____________
*   |___________|    |               5|--->|                B1|--->|             |
*                    |                |    |__________________|    | LEFT TRACK  |
*                    |________________|                            |_____________|
*                       
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
