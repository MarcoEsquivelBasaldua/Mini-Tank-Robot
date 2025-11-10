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

TankTrack::TankTrack()
{
	/* Set Left Track outputs */
	pinMode(leftTrackPin, OUTPUT);
	pinMode(leftVelPin  , OUTPUT);

	/* Set Rigth Track outputs */
	pinMode(rightTrackPin, OUTPUT);
	pinMode(rightVelPin  , OUTPUT);

	/* Stop Tank */
	stop();

	/* IR sensors */
	pinMode(LEFT_IR_SENSOR , INPUT);
	pinMode(RIGHT_IR_SENSOR, INPUT);
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
	uint8 rightVelObsComp; // Compensate right track velocity for a more straight movement
	uint8 abs_leftVel;    // Left Track
	uint8 abs_rightVel;   // Right Wheel
	bool  leftIRdetected  = digitalRead(LEFT_IR_SENSOR) == LOW;
	bool  rightIRdetected = digitalRead(RIGHT_IR_SENSOR) == LOW;

	if (leftIRdetected)
	{
		analogWrite(rightVelPin, STOP_RPM);
	}
	else
	{
		if (rightVel != 0)
		{
			if (rightVel > 0)
			{
				digitalWrite(rightTrackPin, HIGH);
			}
			else
			{
				digitalWrite(rightTrackPin, LOW);
			}
			abs_rightVel = u_abs_16to8(rightVel);
			rightVelObsComp = u_linBoundInterpol(rightVel, MIN_SPEED, MAX_SPEED, MIN_VEL_COMP, MAX_VEL_COMP);
			analogWrite(rightVelPin, (abs_rightVel - rightVelObsComp));
		}
		else
		{
			analogWrite(rightVelPin, STOP_RPM);
		}
	}

	if (rightIRdetected)
	{
		analogWrite(leftVelPin, STOP_RPM);
	}
	else
	{
		if (leftVel != 0)
		{
			if (leftVel > 0)
			{
				digitalWrite(leftTrackPin, HIGH);
			}
			else
			{
				digitalWrite(leftTrackPin, LOW);
			}
			abs_leftVel = u_abs_16to8(leftVel);
			analogWrite(leftVelPin, abs_leftVel);
		}
		else
		{
			analogWrite(leftVelPin, STOP_RPM);
		}
	}
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
	setTracksSpeed(STOP_RPM, STOP_RPM);
}
