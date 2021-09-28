#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
signature BLUEGOAL = signature (1, -2759, -857, -1808, 4095, 11947, 8021, 1.6, 0);
signature REDGOAL = signature (2, 4059, 9261, 6660, -1291, -209, -750, 1.4, 0);
signature YELLOWGOAL = signature (3, 307, 2377, 1342, -3777, -1619, -2698, 1.1, 0);

/*vex-vision-config:begin*/
vision centerVision = vision (PORT13, 50, BLUEGOAL, REDGOAL, YELLOWGOAL);
/*vex-vision-config:end*/

/*vex-vision-config:begin*/
vision rightVision = vision (PORT15, 50, BLUEGOAL, REDGOAL, YELLOWGOAL);
/*vex-vision-config:end*/

/*vex-vision-config:begin*/
vision leftVision = vision (PORT5, 50, BLUEGOAL, REDGOAL, YELLOWGOAL);
/*vex-vision-config:end*/

controller Controller1 = controller(primary);
motor rightMotor = motor(PORT11, ratio18_1, true);
motor leftMotor = motor(PORT14, ratio18_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}