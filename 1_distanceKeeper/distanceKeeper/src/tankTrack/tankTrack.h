/******************************************************************************
*						DDR
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Commands used for DDR (Differential Driven Robot) using the L298N module.
*
*  Wire Inputs: None
*
*  Wire Outputs: L298n Module -> IN1, IN2, IN3, IN4
******************************************************************************/
#ifndef TANKTRACK_h
#define TANKTRACK_h

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"
#include "../commonAlgo/commonAlgo.h"

/******************* DEFINES *********************/
#define  TOP_VEL_OFFSET         (  1u)
#define  BOTTOM_VEL_OFFSET      (  9u)
#define  STOP_RPM               (  0u)                   /* RPM when stop                                            */
#define  MIN_SPPED_CONTROL      ( 50u)                   /* Minimum allowed wheel output (determined experimentally) */
#define  INDOOR_SPEED_CONTROL   (200u)                   /* Desired control for indoor usage                         */
#define  OUTDOOR_SPEED_CONTROL  (255u)                   /* Desired control for outdoor usage                        */
#define  MAX_SPPED_CONTROL      (255u - TOP_VEL_OFFSET)  /* Maximum allowed wheel output (full PWM)                  */
#define  ONE_F                  (1.0f)                   /* Constant 1 float                                         */
#define  THREE_QUARTERS         (0.75f)                  /* Constant 0.75 float                                      */
#define  LEFT_IR_SENSOR         (3u)
#define  RIGHT_IR_SENSOR        (2u)
#define  LEFT_VEL_COMP          (100u)
#define  RIGHT_VEL_COMP         (100u)

/*************************************************/

/****************** CONSTANTS ********************/
const uint8 leftTrackCommandPin  = 2;
const uint8 leftVelCommandPin    = 5;
const uint8 rightTrackCommandPin = 4;
const uint8 rightVelCommandPin   = 6;
/*************************************************/

class TankTrack
{
	public:
		TankTrack();
		void setTracksSpeed(sint16 const leftVel, sint16 const rightVel);
		void forward(uint8 const vel);
		void backward(uint8 const vel);
		void turnRight(uint8 const vel);
		void turnLeft(uint8 const vel);
		void turnRightFast(uint8 const vel);
		void turnLeftFast(uint8 const vel);
		void stop();
};

uint8 getVelOffset(uint8 vel);

#endif
