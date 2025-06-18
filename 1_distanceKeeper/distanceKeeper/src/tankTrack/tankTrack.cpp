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
#include "tankTrack.h"

volatile uint8 leftVelObsComp = 0u;
volatile uint8 rightVelObsComp = 0u;

TankTrack::TankTrack()
{
	/* Set Left Track outputs */
	pinMode(leftTrackCommandPin, OUTPUT);
	pinMode(leftVelCommandPin  , OUTPUT);

	/* Set Rigth Track outputs */
	pinMode(rightTrackCommandPin, OUTPUT);
	pinMode(rightVelCommandPin  , OUTPUT);

	/* Stop Tank */
	stop();

	/* IR sensors */
	//pinMode(LEFT_IR_SENSOR , INPUT);
	//pinMode(RIGHT_IR_SENSOR, INPUT);

	/* Init Vel compensations */
	leftVelObsComp  = 0u;
	rightVelObsComp = 0u;
}

/**********************************************************
*  Function TankTrack::setTracksSpeed()
*
*  Brief: Set each tank track to desired speed
*
*  Inputs: [sint16] leftVel: left track velocity control on the PWM cycle-duty range [-255, 255]
*          [sint16] rightVel: right track velocity control on the PWM cycle-duty range [-255, 255]
*
*  Outputs: void
**********************************************************/
void TankTrack::setTracksSpeed(sint16 const leftVel, sint16 const rightVel)
{
	/* Compensate velocity if IR sensors are set */
	//leftVelObsComp = (digitalRead(LEFT_IR_SENSOR) == HIGH) ? 0u : LEFT_VEL_COMP;
	//rightVelObsComp = (digitalRead(RIGHT_IR_SENSOR) == HIGH) ? 0u : RIGHT_VEL_COMP;
	
	/* Left Track */
	uint8 abs_leftVel = u_abs_16to8(leftVel);

	if (leftVel >= 0)
	{
		digitalWrite(leftTrackCommandPin, HIGH);
	}
	else
	{
		digitalWrite(leftTrackCommandPin, LOW);
	}
	//analogWrite(leftVelCommandPin, abs_leftVel + leftVelObsComp);
	analogWrite(leftVelCommandPin, abs_leftVel);

	/* Right Wheel */
	// Get vel offset for right wheel
	uint8 abs_rightVel = u_abs_16to8(rightVel);

	if (rightVel >= 0)
	{
 		digitalWrite(rightTrackCommandPin, HIGH);
	}
	else
	{
		digitalWrite(rightTrackCommandPin, LOW);
	}
	//analogWrite(rightVelCommandPin, abs_rightVel + rightVelObsComp);
	analogWrite(rightVelCommandPin, abs_rightVel);
}

/**********************************************************
*  Function TankTrack::forward()
*
*  Brief: Tank tracks are set to move forward
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
**********************************************************/
void TankTrack::forward(uint8 const vel)
{
	setTracksSpeed(vel, vel);
}

/**********************************************************
*  Function TankTrack::turnRight()
*
*  Brief: Tank tracks are set to turn right
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
**********************************************************/
void TankTrack::turnRight(uint8 const vel)
{
	setTracksSpeed(vel, STOP_RPM);
}

/**********************************************************
*  Function TankTrack::turnLeft()
*
*  Brief: Tank tracks are set to turn left
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
**********************************************************/
void TankTrack::turnLeft(uint8 const vel)
{
	setTracksSpeed(STOP_RPM, vel);
}

/**********************************************************
*  Function TankTrack::turnRightFast()
*
*  Brief: Tank tracks are set to turn right fast
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
**********************************************************/
void TankTrack::turnRightFast(uint8 const vel)
{
	setTracksSpeed(vel, -vel);
}

/**********************************************************
*  Function TankTrack::turnLeftFast()
*
*  Brief: Tank tracks are set to turn left
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
**********************************************************/
void TankTrack::turnLeftFast(uint8 const vel)
{
	setTracksSpeed(-vel, vel);
}

/**********************************************************
*  Function TankTrack::backward()
*
*  Brief: Tank tracks are set to move backward
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
**********************************************************/
void TankTrack::backward(uint8 const vel)
{
	setTracksSpeed(-vel, -vel);
}

/**********************************************************
*  Function TankTrack::stop()
*
*  Brief: Both tracks are stopped
*
*  Inputs: None
*
*  Outputs: void
**********************************************************/
void TankTrack::stop()
{
	setTracksSpeed(0, 0);
}
