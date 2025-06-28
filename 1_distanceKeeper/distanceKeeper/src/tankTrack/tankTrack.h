/******************************************************************************
*						Tank Track
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Commands used for mini Tank Tracking system using the TB6612FNG module.
*
*  Wire Inputs: LEFT_IR_SENSOR -> PIN6
*               RIGHT_IR_SENSOR -> PIN7
*
*  Wire Outputs: leftTrackPin -> PIN2
*                leftVelPin -> PIN5
*                rightTrackPin -> PIN4
*                rightVelPin -> PIN6
******************************************************************************/
#ifndef TANKTRACK_h
#define TANKTRACK_h

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"
#include "../commonAlgo/commonAlgo.h"

/******************* DEFINES *********************/
#define  STOP_RPM               (  0u)                   /* RPM when stop                                            */
#define  MIN_SPEED              (150u)                   /* Desired control for indoor usage                         */
#define  MAX_SPEED              (255u)                   /* Desired control for outdoor usage                        */
#define  LEFT_IR_SENSOR         (6u)  
#define  RIGHT_IR_SENSOR        (7u)
#define  MIN_VEL_COMP           (10u)
#define  MAX_VEL_COMP           (22u)

/*************************************************/

/****************** CONSTANTS ********************/
const uint8 leftTrackPin  = 2;
const uint8 leftVelPin    = 5;
const uint8 rightTrackPin = 4;
const uint8 rightVelPin   = 6;
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
