#include "ROBOT.h"

ROBOT::ROBOT(YETI_YUKON &rYukon) : Yukon(rYukon),
     DriveRight(_DriveRightPWM1, _DriveRightPWM2, &Yukon.PWM, _DriveRightReversed), 
     DriveLeft(_DriveLeftPWM1, _DriveLeftPWM2, &Yukon.PWM, _DriveLeftReversed), 
     LiftMotor(_LiftMotorPWM1, _LiftMotorPWM2, &Yukon.PWM, _LiftMotorReversed), 
     ClawMotor(_ClawMotorPWM1, _ClawMotorPWM2, &Yukon.PWM, _ClawMotorReversed), 
    Drive(*this),
    Lift(*this),
    Claw(*this),
                                   Auton(*this),
    
        Btd(&Usb), PSx(&Btd) //, PAIR)
    

{
}

void ROBOT::Setup()
{
    Usb.Init();
    DriveRight.Init();
    DriveLeft.Init();
    LiftMotor.Init();
    ClawMotor.Init();

    pinMode(_Button0, INPUT_PULLUP);
    pinMode(_LEDBuiltIn, OUTPUT);
    digitalWrite(_LEDBuiltIn, LOW);
}

void ROBOT::Loop()
{
     //Read The Controller
    Usb.Task();
        
            if (PSx.PS3Connected)
            {
        Drive.OISetSpeed(Yukon.JoystickTo255(PSx.getAnalogHat(RightHatY), 10),Yukon.JoystickTo255(PSx.getAnalogHat(LeftHatY), 10));
        Lift.OISetSpeed(PSx.getAnalogButton(R2) - PSx.getAnalogButton(L2));
        Claw.OISetSpeed((PSx.getButtonPress(R1)*255) - (PSx.getButtonPress(L1)*255));
    
        if (PSx.getButtonClick(LEFT))
        Auton.QueuePrev();
        if (PSx.getButtonClick(RIGHT))
        Auton.QueueNext();
        if (PSx.getButtonClick(DOWN))
        Auton.ToggleArmed();


        if (PSx.getButtonClick(X))
        Auton.LaunchQueued();

        if (PSx.getButtonClick(SQUARE))
        Auton.ToggleLockArmed();

        if (PSx.getButtonClick(PS))
        PSx.disconnect();
        }
    

        if(digitalRead(_Button0) == LOW)
        {
            digitalWrite(_LEDBuiltIn, HIGH);
            Yukon.SetupWIFI();
            delay(1000);
        }
     //Read The Sensors
    uint16_t LightSensorVal = Yukon.ADC.readADC(_AutonLightSensor);
    State.AutonLightSensorActive = (LightSensorVal <= _AutonLightSensorThreshold);

    //Write To Motor Controllers
    if (_NextMotorControllerWriteMillis < millis())
    {
        _NextMotorControllerWriteMillis = millis() + 20;
    DriveRight.SetMotorSpeed(State.DriveRightSpeed);
    DriveLeft.SetMotorSpeed(State.DriveLeftSpeed);
    LiftMotor.SetMotorSpeed(State.LiftMotorSpeed);
    ClawMotor.SetMotorSpeed(State.ClawMotorSpeed);
    }

    //Write the Display
    if (_NextDisplayMillis < millis())
    {
        _NextDisplayMillis = millis() + 250;

        if(Auton.IsArmLocked())
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(2);

            Yukon.OLED.println("LOCKED! ");

            Yukon.OLED.setTextSize(1);
            Yukon.OLED.print(Auton.QueuedProgramName());

            Yukon.OLED.display();
        }
        else if (Auton.IsArmed())
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(2);

            Yukon.OLED.print("ARMED ");
            Yukon.OLED.println(LightSensorVal);
            Yukon.OLED.setTextSize(1);
            Yukon.OLED.print(Auton.QueuedProgramName());

            Yukon.OLED.display();
        }
        else if(Auton.QueuedProgramName() != "")
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(2);

            Yukon.OLED.print(Auton.QueuedProgramName());

            Yukon.OLED.display();
        }
        else
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(2);
            Yukon.OLED.println("YUKON!");
            Yukon.OLED.display();
        }
    }
}
