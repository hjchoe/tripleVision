using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern signature BLUEGOAL;
extern signature REDGOAL;
extern vision rightVision;

extern vision leftVision;
extern signature YELLOWGOAL;
extern vision centerVision;
extern controller Controller1;
extern motor rightMotor;
extern motor leftMotor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );