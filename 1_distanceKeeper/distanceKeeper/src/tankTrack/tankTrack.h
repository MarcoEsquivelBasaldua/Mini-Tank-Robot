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
#define  STOP_RPM               (  0u)                   /* RPM when stop                                            */
#define  INDOOR_SPEED_CONTROL   (200u)                   /* Desired control for indoor usage                         */
#define  OUTDOOR_SPEED_CONTROL  (255u)                   /* Desired control for outdoor usage                        */
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

#endif
