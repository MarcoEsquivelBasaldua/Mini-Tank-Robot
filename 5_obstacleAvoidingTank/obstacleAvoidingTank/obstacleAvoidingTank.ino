#include "src/typeDefs/typeDefs.h"
#include "src/BT_encodedData/BT_encodedData.h"
#include "src/tankTrack/tankTrack.h"
#include "src/HCSR04/HCSR04.h"
#include "src/myServo/myServo.h"

//----------------- Defines ----------------//
#define SERVO_PIN       (9u)

#define MIN_DEGS        (0u)
#define CENTER_DEGS     (90u)
#define MAX_DEGS        (180u)
#define SAFETY_DISTANCE (15u)
#define TURNING_TIME    (700)
#define BACKWARD_TIME   (1000)

#define ONE_DEG_DELAY   (10u)

#define STUCKED_BETWEEN_OBS_TH (5.0f)
//////////////////////////////////////////

//----------------- Enums ----------------//
enum lookDirection {FRONT, RIGHT, LEFT};
//////////////////////////////////////////

//----------------- Tank Tracks ----------------//
TankTrack tankTrack;

//----------- Distance sensor ----------//
uint8 u_trigger = 12u;
uint8 u_echo    = 13u;
HCSR04 distSensor(u_trigger, u_echo);

volatile uint8 u_distance = 0u;
//////////////////////////////////////////

//----------- Servo Heading ------------//
myServo headingServo(SERVO_PIN);

volatile sint16 u_heading;
//////////////////////////////////////////

//--------- Operational Modes ----------//
enum op_Modes{STAND_BY, OBSTACLE_AVOIDANCE, BT_COMMANDED};
op_Modes curr_opMode;
//////////////////////////////////////////

char bt_command = BT_STOP;

void setup() {
  /* INnit operational Mode */
  curr_opMode = STAND_BY;

  /* Robot Motion init */
  tankTrack.stop();
  headingServo.setHeading(CENTER_DEGS);
  delay(1000);

  /* BT init */
  Serial.begin(9600);
}

void loop() {

  if (Serial.available()) 
  {
    char c_command = Serial.read();
    
    /* Obstacle Ovoidance enabled */
    if (c_command == BT_A)
    {
      curr_opMode = OBSTACLE_AVOIDANCE;
    }
    else if (c_command == BT_B)
    {
      curr_opMode = BT_COMMANDED;
    }
    else if (c_command == BT_C)
    {
      curr_opMode = STAND_BY;
    }
    else
    {
      switch (c_command)
      {
        case BT_STOP:
          bt_command = BT_STOP;
          break;
        case BT_FORWARD:
          bt_command = BT_FORWARD;
          break;
        case BT_BACKWARD:
          bt_command = BT_BACKWARD;
          break;
        case BT_LEFT:
          bt_command = BT_LEFT;
          break;
        case BT_RIGHT:
          bt_command = BT_RIGHT;
          break;
        case BT_FORWARD_LEFT:
          bt_command = BT_FORWARD_LEFT;
          break;
        case BT_FORWARD_RIGHT:
          bt_command = BT_FORWARD_RIGHT;
          break;
        case BT_BACKWARD_RIGHT:
          bt_command = BT_BACKWARD_RIGHT;
          break;
        case BT_BACKWARD_LEFT:
          bt_command = BT_BACKWARD_LEFT;
          break;
        default:
          bt_command = BT_STOP;
          break;
      }
    }
  }


  if (curr_opMode == OBSTACLE_AVOIDANCE)
  {
    ObstacleAvoidance();
  }
  else if (curr_opMode == BT_COMMANDED)
  {
    blueToothCommand(bt_command);
  }
  else if (curr_opMode == STAND_BY)
  {
    tankTrack.stop();
  }
  else
  {
    /* Do nothing*/
  }
  
}

/**********************************************************
*  Function ObstacleAvoidance
*
*  Brief: Main function for obstacle avoidance functionality
*
*  Inputs: None
*
*  Outputs: None
*
*  Callsequence:
*
**********************************************************/
void ObstacleAvoidance()
{
  /* Robot going forward */
  tankTrack.forward(MEDIUM_SPEED);

  /* Get current distance */
  u_distance = distSensor.measureDistance();
  delay(10);

  if (u_distance < SAFETY_DISTANCE)
  {
    float f_meanDist2ObstaclesRight, f_meanDist2ObstaclesLeft;
    tankTrack.stop();

    /* Look to the right */
    f_meanDist2ObstaclesRight = getMeanFreeSpace(RIGHT);

    /* Get heading back to middle */
    headingServo.setHeading(CENTER_DEGS);
    delay(1000);

    /* Look to the left */
    f_meanDist2ObstaclesLeft = getMeanFreeSpace(LEFT);

    /* Get heading back to middle */
    headingServo.setHeading(CENTER_DEGS);
    delay(1000);

    /* Change direction due to obstacle */
    if (f_abs_floatTofloat(f_meanDist2ObstaclesRight - f_meanDist2ObstaclesLeft) <= STUCKED_BETWEEN_OBS_TH)
    {
      tankTrack.backward(MEDIUM_SPEED);
      delay(BACKWARD_TIME);
      tankTrack.turnRightFast(MEDIUM_SPEED);
      delay(TURNING_TIME);
    }
    else if (f_meanDist2ObstaclesRight > f_meanDist2ObstaclesLeft)
    {
      tankTrack.turnRightFast(MAX_SPEED);
      delay(TURNING_TIME);
    }
    else
    {
      tankTrack.turnLeftFast(MAX_SPEED);
      delay(TURNING_TIME);
    }
  }
}

/**********************************************************
*  Function getMeanFreeSpace
*
*  Brief: Get mean distances to obstacles in the given direction
*
*  Inputs: [lookDirection] direction : RIGHT or LEFT
*
*  Outputs: [float] : mean distance to obstacles
*
*  Callsequence:
*         start
*           : init mean value with 0;
*           : check which direction to check;
*           : repeat
*             : move heading angle;
*             : update mean value with measured distance;
*           : repeat (unit reaching side direction)
*           : return mean distances;
*         end
**********************************************************/
float getMeanFreeSpace(lookDirection direction)
{
  float f_meanDist2Obstacles = 0.0f;
  sint8 s_headingIncrement = (direction == LEFT) ? (1) : (-1); 

  for (uint8 u_heading = CENTER_DEGS, counter = 1u; 
       u_heading > MIN_SERVO_DEGREES && u_heading < MAX_SERVO_DEGREES; 
       u_heading += s_headingIncrement, counter++)
  {
    headingServo.setHeading(u_heading);
    delay(ONE_DEG_DELAY);
    f_meanDist2Obstacles = ((float)(counter - 1u) * (f_meanDist2Obstacles) + (float)distSensor.measureDistance()) / (float)counter;
  }

  return f_meanDist2Obstacles;
}

void blueToothCommand(char c_command)
{
  switch (c_command)
  {
    case BT_STOP:
      tankTrack.stop();
      break;
    case BT_FORWARD:
      tankTrack.forward(MEDIUM_SPEED);
      break;
    case BT_BACKWARD:
      tankTrack.backward(MEDIUM_SPEED);
      break;
    case BT_LEFT:
      tankTrack.turnLeftFast(MEDIUM_SPEED);
      break;
    case BT_RIGHT:
      tankTrack.turnRightFast(MEDIUM_SPEED);
      break;
    case BT_FORWARD_LEFT:
      tankTrack.turnLeft(MEDIUM_SPEED);
      break;
    case BT_FORWARD_RIGHT:
      tankTrack.turnRight(MEDIUM_SPEED);
      break;
    case BT_BACKWARD_RIGHT:
      tankTrack.setTracksSpeed(-(uint16)(MEDIUM_SPEED), STOP_RPM);
      break;
    case BT_BACKWARD_LEFT:
      tankTrack.setTracksSpeed(STOP_RPM, -(uint16)(MEDIUM_SPEED));
      break;
    default:
      tankTrack.stop();
      break;
  }
}
