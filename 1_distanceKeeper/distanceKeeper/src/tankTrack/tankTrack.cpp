/******************************************************************************
*						Tank Track
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Commands used for mini Tank Tracking system using the TB6612FNG module.
*
*  Wire Inputs: None
*
*  Wire Outputs: L298n Module -> IN1, IN2, IN3, IN4
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
*  Brief: Set each ddr wheel to desired speed
*
*  Inputs: [sint16] leftVel: left wheel velocity control on the PWM cycle-duty range [-255, 255]
*          [sint16] rightVel: right wheel velocity control on the PWM cycle-duty range [-255, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to vel
*                right wheel IN2 to 0
*                left wheel  IN1 to vel
*                left wheel IN2 to 0
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
*  Brief: DDR wheels are set to move forward
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to vel
*                right wheel IN2 to 0
*                left wheel  IN1 to vel
*                left wheel IN2 to 0
**********************************************************/
void TankTrack::forward(uint8 const vel)
{
	setTracksSpeed(vel, vel);
}

/**********************************************************
*  Function TankTrack::turnRight()
*
*  Brief: DDR wheels are set to turn right
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to 0
*                right wheel IN2 to 0
*                left wheel IN1 to vel
*                left wheel IN2 to 0
**********************************************************/
void TankTrack::turnRight(uint8 const vel)
{
	setTracksSpeed(vel, STOP_RPM);
}

/**********************************************************
*  Function TankTrack::turnLeft()
*
*  Brief: DDR wheels are set to turn left
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to vel
*                right wheel IN2 to 0
*                left wheel IN1 to 0
*                left wheel IN2 to 0
**********************************************************/
void TankTrack::turnLeft(uint8 const vel)
{
	setTracksSpeed(STOP_RPM, vel);
}

/**********************************************************
*  Function TankTrack::turnRightFast()
*
*  Brief: DDR wheels are set to turn right fast
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to 0
*                right wheel IN2 to vel
*                left wheel IN1 to vel
*                left wheel IN2 to 0
**********************************************************/
void TankTrack::turnRightFast(uint8 const vel)
{
	setTracksSpeed(vel, -vel);
}

/**********************************************************
*  Function TankTrack::turnLeftFast()
*
*  Brief: DDR wheels are set to turn left
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to vel
*                right wheel IN2 to 0
*                left wheel IN1 to 0
*                left wheel IN2 to 0
**********************************************************/
void TankTrack::turnLeftFast(uint8 const vel)
{
	setTracksSpeed(-vel, vel);
}

/**********************************************************
*  Function TankTrack::backward()
*
*  Brief: DDR wheels are set to move backward
*
*  Inputs: [uint8] vel: velocity control on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to 0
*                right wheel IN2 to vel
*                left wheel IN1 to 0
*                left wheel IN2 to vel
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
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to 0
*                right wheel IN2 to 0
*                left wheel IN1 to 0
*                left wheel IN2 to 0
**********************************************************/
void TankTrack::stop()
{
	setTracksSpeed(0, 0);
}
