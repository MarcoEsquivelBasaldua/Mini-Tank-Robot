#include "src/typeDefs/typeDefs.h"
#include "src/tankTrack/tankTrack.h"
#include "src/HCSR04/HCSR04.h"

/**************************************************************************************
*  Wiring
*    ______________      ________________      __________________      _____________
*   |           VCC|<---|5V             4|--->|                A1|--->|             |
*   | HCSR04  TRIGG|<---|12             6|--->|                  |    | RIGHT TRACK |
*   |          ECHO|--->|13   ARDUINO    |    |      TB6612FNG   |    |_____________|
*   |           GND|<---|GND    UNO     2|--->|                  |     _____________
*   |______________|    |               5|--->|                B1|--->|             |
*                       |________________|    |__________________|    | LEFT TRACK  |
*                                                                     |_____________|
*
***************************************************************************************/

//----------------- Tank Tracks ----------------//
TankTrack tankTrack;
//////////////////////////////////////////

//----------- Distance sensor ----------//
uint8 u_trigger = 12u;
uint8 u_echo    = 13u;
HCSR04 distSensor(u_trigger, u_echo);
//////////////////////////////////////////

/**********************************************************
*  setup()
*  Call sequence:
*                -> stop tank
**********************************************************/
void setup() {
  tankTrack.stop();
}

/**********************************************************
*  loop()
*  Call sequence:
*                -> set distance threshold
*                -> set desired distance
*                -> get distance error as
*                   currentDistance - desiredDistance
*                -> get error absolute value 
*                -> if error abs val is greater than threshold
*                   -> get sign of the error
*                   -> map error to speed control with a bounded linear interpolation
*                   -> if sign error is positive, this means tank is far from desired distance
*                      -> move forwards
*                   -> else, tank is too close
*                      -> move backwards
*                -> else
*                   -> stop tank
**********************************************************/
void loop() {
  uint8 u_distThreshold = 2u; // We want the car to stop within a distance range

  uint8 u_minVel = MIN_SPEED;   // Min allowed speed
  uint8 u_maxVel = MAX_SPEED;  // Max allowed spped
  
  uint8  u_keepDist    = 8u;                          // Desired distance
  uint8  u_currentDist = distSensor.measureDistance(); // Current distance
  sint16 s_error       = (sint16)u_currentDist - (sint16)u_keepDist;

  uint8 u_error = (uint8)u_abs_16to8(s_error); // Get absolute value of dist error

  if(u_error > u_distThreshold)
  {
    sint8 s_errorSign   = s_getSign(s_error);
    uint8 u_vel         = u_linBoundInterpol(       u_error      ,
                                             (uint8)MIN_SAFE_DIST, (uint8)MAX_SAFE_DIST,
                                               u_minVel          ,        u_maxVel      );

    if(s_errorSign > 0) // Move forward
    {
      tankTrack.forward(u_vel);
    }
    else               // Move backward
    {
      tankTrack.backward(u_vel);
    }
  }
  else
  {
    tankTrack.stop();
  }

  delay(10);
}
