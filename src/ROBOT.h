#ifndef ROBOT_h
#define ROBOT_h

#include <YETI_YUKON.h>
#include <STATE.h>

#include <PS3BT.h>
#include <usbhub.h>

#include <YUKON_DBH12V.h>

#include "Subsystems\Drive.h"
#include "Subsystems\Lift.h"
#include "Subsystems\Claw.h"

#include "Autonomous\AUTONOMOUS.h"

class ROBOT
{
  public:
    ROBOT(YETI_YUKON &rYukon);

    void Setup();
    void Loop();

    YETI_YUKON &Yukon;

    STATE State;

    //Motors
    YUKON_DBH12V DriveRight;
    YUKON_DBH12V DriveLeft;
    YUKON_DBH12V LiftMotor;
    YUKON_DBH12V ClawMotor;

    //Subsystems
    DRIVE Drive;
    LIFT Lift;
    CLAW Claw;
    AUTONOMOUS Auton;

    //USB Items
    USB Usb;
    
    BTD Btd;
	PS3BT PSx;
    
    
   

    //PIN Declarations
    static const int _DriveRightPWM1 = 0;
    static const int _DriveRightPWM2 = 1;
    static const bool _DriveRightReversed = false;
        
    static const int _DriveLeftPWM1 = 2;
    static const int _DriveLeftPWM2 = 3;
    static const bool _DriveLeftReversed = true;
        
    static const int _LiftMotorPWM1 = 4;
    static const int _LiftMotorPWM2 = 5;
    static const bool _LiftMotorReversed = false;
        
    static const int _ClawMotorPWM1 = 6;
    static const int _ClawMotorPWM2 = 7;
    static const bool _ClawMotorReversed = false;
        

    static const uint8_t _Button0 = 0;
    static const uint8_t _LEDBuiltIn = 25;
    static const uint8_t _AutonLightSensor = 0;
    
    static const uint16_t _AutonLightSensorThreshold = 150; //Value 0 - 1024

  private:
      long _NextDisplayMillis = 0;
      long _NextMotorControllerWriteMillis = 0;
};

#endif