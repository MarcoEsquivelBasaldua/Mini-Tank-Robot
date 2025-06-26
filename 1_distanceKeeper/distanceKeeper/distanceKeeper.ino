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
  tankTrack.forward(MAX_SPEED);
  delay(3000);
  tankTrack.stop();
  delay(3000);
  /*
  uint8 u_distThreshold = 2u; // We want the car to stop within a distance range

  uint8 u_minVel = INDOOR_SPEED_CONTROL;   // Min allowed speed
  uint8 u_maxVel = OUTDOOR_SPEED_CONTROL;  // Max allowed spped
  
  uint8 u_keepDist    = 8u;                          // Desired distance
  uint8 u_currentDist = distSensor.measureDistance(); // Current distance
  sint8 s_error       = (sint8)u_currentDist - (sint8)u_keepDist;

  uint8 u_error = (uint8)s_abs(s_error); // Get absolute value of dist error

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
  */

  delay(100);
}

/**********************************************************
*  Function u_linBoundInterpol
*
*  Brief: Bounded linear interpolation is done.
*
*          u_maxOut .|              .......
*                    |             /
*                    |            /
*                    |           /
*          u_minOut .|........../
*                    |________________________________________
*                               .  .
*                         u_minIn   u_maxIn
*
*  Inputs: [uint8] u_input   : input to be mapped
*          [uint8] u_minDist : minimum allowed Input
*          [uint8] u_maxDist : maximum allowed Input
*          [uint8] u_minOut  : minimum allowed output
*          [uint8] u_maxOut  : maximum allowed output
*
*  Outputs: [uint8] mapped value
*
*  Wire Inputs: None
**********************************************************/
uint8 u_linBoundInterpol(uint8 const u_input  , 
                         uint8 const u_minIn, uint8 const u_maxIn, 
                         uint8 const u_minOut , uint8 const u_maxOut)
{
  if(u_input <= u_minIn)
  {
    return u_minOut;
  }
  else if(u_input >= u_maxIn)
  {
    return u_maxOut;
  }
  else
  {
    /* Convert to float all variables */
    float f_input   = (float)u_input;
    float f_minIn = (float)u_minIn;
    float f_maxIn = (float)u_maxIn;
    float f_minOut  = (float)u_minOut;
    float f_maxOut  = (float)u_maxOut;
    float f_out;

    float f_slope = (f_maxOut - f_minOut)/(f_maxIn - f_minIn);
    f_out = f_slope * (f_input - f_minIn) + f_minOut;
    return (uint8)f_out;
  }
}

/**********************************************************
*  Function s_abs
*
*  Brief: Returns the absolute value. 
*         Datatypes are determined according to the ones used 
*         in the project.
*
*  Inputs: [sint8] s_value : value to get absolute value from
*
*  Outputs: [uint8] absolute value of s_value
*
*  Wire Inputs: None
**********************************************************/
sint8 s_abs(sint8 const s_value)
{
  if(s_value >= 0)
    return s_value;
  else
    return -s_value;
}

/**********************************************************
*  Function s_abs
*
*  Brief: Returns the absolute value. 
*         Datatypes are determined according to the ones used 
*         in the project.
*
*  Inputs: [sint8] s_value : value to get absolute value from
*
*  Outputs: [uint8] absolute value of s_value
*
*  Wire Inputs: None
**********************************************************/
sint8 s_getSign(sint8 const s_value)
{
  if(s_value >= 0)
    return 1;
  else
    return -1;
}

