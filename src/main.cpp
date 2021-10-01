/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       jameschoe                                                 */
/*    Created:      Mon Sep 27 2021                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]         
// Controller1          controller                    
// rightMotor           motor         11              
// leftMotor            motor         14                         
// highVision           vision        5   
// lowVision            vision        13                 
// ---- END VEXCODE CONFIGURED DEVICES ----

// |-------------------- Library Imports --------------------|

#include "vex.h"
using namespace vex;

// |-------------------- Initialize Global Variables --------------------|

int highCenterX = 158;          // half the number of pixels of the vision sensor
int highBottomY = 175;          // bottom of vision sensor field of view
int lowBottomY = 190;

double speedMultiplierX = 0.35;  // multiplier used when calculating speed based on distance from object on x-axis
double speedMultiplierY = 0.75;  // multiplier used when calculating speed based on distance from object on y-axis

// |-------------------- Function Definitions --------------------|

void snap(int sig, int* lowObjects, int* highObjects)
{
  // take snapshot with vision camera and return number of signature objects in frame
  switch (sig)
  {
    case 0:
      *lowObjects = lowVision.takeSnapshot(LOWREDGOAL);
      *highObjects = highVision.takeSnapshot(HIGHREDGOAL);
      break;
    case 1:
      *lowObjects = lowVision.takeSnapshot(LOWBLUEGOAL);
      *highObjects = highVision.takeSnapshot(HIGHBLUEGOAL);
      break;
    case 2:
      *lowObjects = lowVision.takeSnapshot(LOWYELLOWGOAL);
      *highObjects = highVision.takeSnapshot(HIGHYELLOWGOAL);
      break;
  }
}

// find(object): function that returns a boolean of whether the object is in vision
void scan(int sig, bool* lowState, bool* highState) // sig parameter is the object to detect
{
  int lowObjects;
  int highObjects;

  snap(sig, &lowObjects, &highObjects);

  if (lowObjects != 0) *lowState = true;
  else *lowState = false;
  if (highObjects != 0) *highState = true;
  else *highState = false;
}

void chooseVisionSensor(int sig, int* x)
{
  bool highState, lowState;

  int highX, highY, lowX, lowY, lowWidth;

  while (true)
  {
    scan(sig, &lowState, &highState);

    highX = highVision.largestObject.centerX;
    highY = highVision.largestObject.centerY;
    lowX = lowVision.largestObject.centerX;
    lowY = lowVision.largestObject.centerY;
    lowWidth = lowVision.largestObject.width;

    if (!highState && !lowState)
    {
      Controller1.Screen.clearLine();
      Controller1.Screen.print("no object");
      return;
    }
    else if (highState && highY < highBottomY)
    {
      Controller1.Screen.clearLine();
      Controller1.Screen.print("high cam");
      *x = highX;
      break;
    }
    else if (lowState && lowY < lowBottomY)
    {
      Controller1.Screen.clearLine();
      Controller1.Screen.print("low cam");
      *x = lowX;
      break;
    }
    else
    {
      Controller1.Screen.clearLine();
      Controller1.Screen.print("BACKUP");

      leftMotor.setVelocity(-50, velocityUnits::pct);
      rightMotor.setVelocity(-50, velocityUnits::pct);

      while(lowY > lowBottomY)
      {
        scan(sig, &lowState, &highState);
        lowY = lowVision.largestObject.centerY;

        leftMotor.spin(forward);
        rightMotor.spin(forward);
      }
      leftMotor.stop();
      rightMotor.stop();
    }
    wait(50, msec);
  }
}

// focus(object): function that rotates the robot until the object in vision is centered on the x-axis
void focus(int sig) // sig parameter is the object to detect
{
  // initialize variables
  int x;

  double speed = 0;
  bool linedUp = false;

  // while loop until robot is lined up with object in vision on the x-axis
  while (!linedUp)
  {
    // take snapshot with vision camera and return number of signature objects in frame

    chooseVisionSensor(sig, &x);

    // if the object's x-coordinate is to the left of the center of vision
    if (x > highCenterX + 10)       // 10 is added to the center x value to give a 10 pixel band to the target
    {  
      // calculate speed by multiplying the speedMultiplier to the distance of object from the center on the x-axis
      speed = speedMultiplierX * (x - highCenterX);

      // turn off right motor and set left motor velocity to the calculated speed to turn right
      leftMotor.setVelocity(speed, velocityUnits::pct);
      rightMotor.setVelocity(0, velocityUnits::pct);
    }
    // if the object's x-coordinate is to the right of the center of vision
    else if (x < highCenterX - 10)  // 10 is subtracted to the center x value to give a 10 pixel band to the target
    {      
      // calculate speed by multiplying the x-speedMultiplier to the distance of object from the center on the x-axis
      speed = speedMultiplierX * (highCenterX - x);

      // turn off left motor and set right motor velocity to the calculated speed to turn left
      leftMotor.setVelocity(0, velocityUnits::pct);
      rightMotor.setVelocity(speed, velocityUnits::pct);
    }
    // if object is in vision and centered on the x-axis
    else
    {
      // set motor velocities to 0 (stop motors)
      leftMotor.setVelocity(0, velocityUnits::pct);
      rightMotor.setVelocity(0, velocityUnits::pct);

      // end while loop
      linedUp = true;
    }
    
    // spin both motors at set velocities
    rightMotor.spin(forward);
    leftMotor.spin(forward);

    // while loop delay (50 milliseconds)
    wait(50, msec);
  }
}

// |------------------- Main --------------------|

int main()
{
  // Initializing Robot Configuration
  vexcodeInit();
  
  while (true)
  {
    if (Controller1.ButtonA.pressing())
    {
      focus(1);
    }
  }
}