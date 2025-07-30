#include "src/myServo/myServo.h"

//----------------- Defines ----------------//
#define SERVO_PIN       (10u)
#define MIN_DEGS        (0u)
#define MAX_DEGS        (180u)

//----------- Servo Heading ------------//
myServo headingServo(SERVO_PIN);

void setup()
{
  headingServo.setHeading(90u);
  delay(500);
}

void loop()
{
  headingServo.setHeading(90u);

}
