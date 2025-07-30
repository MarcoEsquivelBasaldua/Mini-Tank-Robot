#include "src/myServo/myServo.h"

//----------------- Defines ----------------//
#define SERVO_PIN       (10u)
#define MIN_ERROR_LIGHT (-50)
#define MAX_ERROR_LIGHT (50)
#define MIN_DEGS        (0u)
#define MAX_DEGS        (180u)

//----------- Servo Heading ------------//
myServo  headingServo(SERVO_PIN);
volatile uint8  prevHeading = 90u;
volatile sint16 heading;
const    float  lpfFactor = 0.25f;

/* LDR reading variables */
volatile uint8 leftLDRlevel;
volatile uint8 rightLDRlevel;
volatile sint8 lightError;
const    uint8 fullLight     = 15u;  // Full light is obtained by lower this value inclusive
const    uint8 minErrorLight = 3u;

void setup()
{
  headingServo.setHeading(90u);
  delay(500);
}

void loop()
{
  /* LDR readings */
  leftLDRlevel = u_mapLight2Percentage(analogRead(0));
  rightLDRlevel = u_mapLight2Percentage(analogRead(1));
  lightError = rightLDRlevel - leftLDRlevel;
  uint8 abs_lightError = u_abs_16to8((sint16)lightError);

  /* Set heading of the robot */
  heading = u_mapLigth2Degs(lightError);
  heading = (sint16)((1.0f - lpfFactor) * (float)heading + lpfFactor * (float)prevHeading);
  headingServo.setHeading((uint8)heading);
  prevHeading = heading;

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
  f_output = f_slope * (f_input);
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
    f_headDeg = f_slope * (f_error - f_minError) + f_minDegs;
    return (uint8)f_headDeg;
  }
}
