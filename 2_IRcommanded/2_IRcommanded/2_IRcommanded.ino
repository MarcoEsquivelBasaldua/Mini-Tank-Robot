#include "src/typeDefs/typeDefs.h"
#include "src/tankTrack/tankTrack.h"
#include "src/IRDecoder/IRDecoder.h"

/**************************************************************************************
*  Wiring
*    _____________      ________________      __________________      _____________
*   |            G|<---|GND           5V|--->|ENA           OUT1|--->|             |
*   |  IR        R|<---|5V            11|--->|IN1               |    | RIGHT WHEEL |
*   | RECEIVER   Y|--->| 2   ARDUINO  10|--->|IN2   L298N   OUT2|--->|_____________|
*   |_____________|    |       UNO     9|--->|IN3               |     _____________
*                      |               6|--->|IN4           OUT3|--->|             |
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
