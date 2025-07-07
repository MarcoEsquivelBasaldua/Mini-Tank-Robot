#include "src/typeDefs/typeDefs.h"
#include "src/tankTrack/tankTrack.h"
#include "src/IRDecoder/IRDecoder.h"

/**************************************************************************************
*  Wiring
*    ______________      ________________      __________________      _____________
*   |             G|<---|GND            4|--->|                A1|--->|             |
*   |  IR         R|<---|5V             6|--->|                  |    | RIGHT TRACK |
*   | RECEIVER    Y|--->|3   ARDUINO     |    |      TB6612FNG   |    |_____________|
*   |              |    |       UNO     2|--->|                  |     _____________
*   |______________|    |               5|--->|                B1|--->|             |
*                       |________________|    |__________________|    | LEFT TRACK  |
*                                                                     |_____________|
*
***************************************************************************************/

//----------------- Tank Tracks ----------------//
TankTrack tankTrack;
//////////////////////////////////////////

//---------------- IR ------------------//
uint8 u_datPin = 3u;

IRDecoder IR(u_datPin);
//////////////////////////////////////////

void setup() {
  tankTrack.stop();
}

void loop() {
  uint32 u_command = IR.getCommand();

  if (u_command) {
    switch (u_command)
    {
      case IR_STOP:
        tankTrack.stop();
        break;
      case IR_FORWARD:
        tankTrack.forward(MAX_SPEED);
        break;
      case IR_BACKWARD:
        tankTrack.backward(MAX_SPEED);
        break;
      case IR_TURNLEFT:
        tankTrack.turnLeftFast(MEDIUM_SPEED);
        break;
      case IR_TURNRIGHT:
        tankTrack.turnRightFast(MEDIUM_SPEED);
        break;
      default:
        tankTrack.stop();
        break;
    }
  }

}
