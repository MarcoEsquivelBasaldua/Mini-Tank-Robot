#include "src/myServo/myServo.h"
#include "src/tankTrack/tankTrack.h"

/**************************************************************************************
*  Wiring
*    ______________      ________________      __________________      _____________
*   |           VCC|<---|5V             4|--->|                A1|--->|             |
*   | LDR_left   A0|--->|A3             6|--->|                  |    | RIGHT TRACK |
*   |           GND|<---|GND  ARDUINO    |    |      TB6612FNG   |    |_____________|
*   |              |    |       UNO     2|--->|                  |     _____________
*   |______________|    |               5|--->|                B1|--->|             |
*    ______________     |                |    |__________________|    | LEFT TRACK  |
*   |           VCC|<---|5V              |     __________________     |_____________|
*   | LDR_right  A0|--->|A2           D10|--->|                  |
*   |           GND|<---|GND           5V|--->|  SERVO HEADING   |
*   |______________|    |             GND|--->|__________________|
*                       |________________|
*
***************************************************************************************/

//----------------- Defines ----------------//
#define SERVO_PIN       (10u)
#define MIN_ERROR_LIGHT (-50)
#define MAX_ERROR_LIGHT (50)
#define MIN_DEGS        (0u)
#define MAX_DEGS        (180u)

//----------------- Tank Tracks ----------------//
TankTrack tankTrack;

//----------- Servo Heading ------------//
myServo  headingServo(SERVO_PIN);
volatile uint8  prevHeading = 90u;
volatile sint16 heading;
const    float  lpfFactor = 0.25f;

/* LDR reading variables */
const    uint8 leftLDRInput      = 1u;
const    uint8 rightDRInput      = 2u;
volatile uint8 leftLDRlevel;
volatile uint8 rightLDRlevel;
volatile sint8 lightError;
const    uint8 fullLight         = 15u;  // Full light is obtained by lower this value inclusive
const    uint8 minErrorLight     = 15u;
const    sint8 minDiagMove2Left  = 15;
const    sint8 minMove2Left      = 30;
const    sint8 minDiagMove2Right = -15;
const    sint8 minMove2Right     = -30;

void setup()
{
  tankTrack.stop();
  headingServo.setHeading(90u);
  Serial.begin(9600);
  delay(500);
}

void loop()
{
  /* LDR readings */
  leftLDRlevel         = u_mapLight2Percentage(analogRead(leftLDRInput));
  rightLDRlevel        = u_mapLight2Percentage(analogRead(rightDRInput));
  lightError           = rightLDRlevel - leftLDRlevel;
  uint8 abs_lightError = u_abs_16to8((sint16)lightError);

  /* Set heading of the robot */
  heading = u_mapLigth2Degs(lightError);
  heading = (sint16)((1.0f - lpfFactor) * (float)heading + lpfFactor * (float)prevHeading);
  headingServo.setHeading((uint8)heading);
  prevHeading = heading;

  Serial.println(lightError);

  /* Choose correct operational mode */
  if ( (leftLDRlevel <= fullLight) && (rightLDRlevel <= fullLight) )
  {
    OP_MODE_0();
  }
  else if(abs_lightError <=  minErrorLight)
  {
    OP_MODE_1();
  }
  else
  {
    OP_MODE_2();
  }

  delay(50);
}

/**********************************************************
*  Function OP_MODE_0
*
*  Brief: Operational Mode 0. Tank is stopped.
*
*  Inputs: None
*
*  Outputs: None
**********************************************************/
void OP_MODE_0()
{
  tankTrack.stop();
}

/**********************************************************
*  Function OP_MODE_1
*
*  Brief: Operational Mode 1. Tank moves forward in a straight line.
*         Tracks velocities are proportional to average value on both
*         light readings.
*
*  Inputs: None
*
*  Outputs: None
**********************************************************/
void OP_MODE_1()
{
  /* Get average value from light readings*/
  uint8 u_ldrLevelMean = (leftLDRlevel + rightLDRlevel) >> 1;

  /* Interpolate light level to valid speed */
  uint8 u_controlSpeed = u_linBoundInterpol(u_ldrLevelMean, 0u, 100, (uint8)MEDIUM_SPEED, (uint8)MAX_SPEED);

  /* Set Motor speed to computed control */
  tankTrack.setTracksSpeed((sint16)u_controlSpeed, (sint16)u_controlSpeed);
}

/**********************************************************
*  Function OP_MODE_2
*
*  Brief: Operational Mode 2. Tank moves forward. Tracks
*         velocities are proportional to ligth readings,
*         on each side.
*
*  Inputs: None
*
*  Outputs: None
**********************************************************/
void OP_MODE_2()
{
  if (lightError >= minMove2Left)
  {
    tankTrack.turnLeftFast(MAX_SPEED);
  }
  else if (lightError >= minDiagMove2Left)
  {
    tankTrack.turnLeft(MAX_SPEED);
  }

  if (lightError <= minMove2Right)
  {
    tankTrack.turnRightFast(MAX_SPEED);
  }
  else if (lightError <= minDiagMove2Right)
  {
    tankTrack.turnRight(MAX_SPEED);
  }
}

/**********************************************************
*  Function OP_MODE_3
*
*  Brief: Operational Mode 3. Tank moves backward in a straight line.
*         Tracks velocities are proportional to average value on both
*         light readings.
*
*  Inputs: None
*
*  Outputs: None
**********************************************************/
void OP_MODE_3()
{
  /* Get average value from light readings*/
  uint8 u_ldrLevelMean = (leftLDRlevel + rightLDRlevel) >> 1;

  /* Interpolate light level to valid speed */
  uint8 u_controlSpeed = u_linBoundInterpol(u_ldrLevelMean, 0u, 100, (uint8)MIN_SPEED, (uint8)MAX_SPEED);

  /* Set Motor speed to computed control */
  tankTrack.setTracksSpeed(-((sint16)u_controlSpeed), -((sint16)u_controlSpeed));
}

/**********************************************************
*  Function s_mapLight2Percentage
*
*  Brief: Maps the light read in the analog input from [0, 1024]
*         to [0, 100]
*
*        100 .|    .......
*             |   /
*             |  /
*             | /
*          0 .|/__________
*             .    .
*             0   1024
*
*  Inputs: [uint16] u_sensorInput : analog read on the light sensor
*
*  Outputs: [uint8] mapped light intensity
*
*  Wire Inputs: None
**********************************************************/
uint8 u_mapLight2Percentage(uint16 const u_sensorInput)
{
  /* Convert to float all variables */
  float f_input   = (float)u_sensorInput;
  float f_output;

  float f_slope = 0.098f;
  f_output      = f_slope * (f_input);

  return (uint8)f_output;
}

/**********************************************************
*  Function u_mapLigth2Degs
*
*  Brief: Determines the heading degrees based on the error on
*         light sensors
*
*          MAX_DEGS .|              .......
*                    |             /
*                    |            /
*                    |           /
*          MIN_DEGS .|........../
*                    |________________________________________
*                               .  .
*                 MIN_ERROR_LIGHT  MAX_ERROR_LIGHT
*
*  Inputs: [uint8] s_error  : error between light sensors
*
*  Outputs: [uint8] heading degrees
*
*  Wire Inputs: None
**********************************************************/
uint8 u_mapLigth2Degs(sint8 const s_error)
{
  if(s_error <= MIN_ERROR_LIGHT)
  {
    return MIN_DEGS;
  }
  else if(s_error >= MAX_ERROR_LIGHT)
  {
    return MAX_DEGS;
  }
  else
  {
    /* Convert to float all variables */
    float f_error    = (float)s_error;
    float f_minError = (float)MIN_ERROR_LIGHT;
    float f_maxError = (float)MAX_ERROR_LIGHT;
    float f_minDegs  = (float)MIN_DEGS;
    float f_maxDegs  = (float)MAX_DEGS;
    float f_headDeg;

    float f_slope = (f_maxDegs - f_minDegs)/(f_maxError - f_minError);
    f_headDeg     = f_slope * (f_error - f_minError) + f_minDegs;
    return (uint8)f_headDeg;
  }
  
  return 0;
}
