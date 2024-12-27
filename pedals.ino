#include <HX711.h>
#include <Joystick.h>

#define DOUT  3
#define CLK  2
#define calibration_factor 5000  //  Brake Loadcell: change this value to increase or decrease the range of the brake
HX711 scale;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 
                   0, 0,                 // Button Count, Hat Switch Count
                   false, false, false,  // X, Y, Z
                   true, true, true,     // Rx (potentiometer), Ry (Brake loadcell), Rz (potentiometers)
                   false, false,         // Rudder, throttle
                   false, false, false); // Akcelerator, brake, steering

int throttleValue = 0;
int clutchValue = 0;
int brakeValue = 0;
int lastThrottleValue = -1;
int lastClutchValue = -1;
int lastBrakeValue = -1;

void setup() {
  Joystick.setRyAxisRange(0, 1023); 
  Joystick.setRxAxisRange(0, 1023); 
  Joystick.setRzAxisRange(0, 1023); 

  Joystick.begin();
  Serial.begin(38400);

  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare();
}

void loop() {
  throttleValue = analogRead(A1);
  if (abs(throttleValue - lastThrottleValue) > 1) {  
    Joystick.setRxAxis(throttleValue);
    lastThrottleValue = throttleValue;
  }

  clutchValue = analogRead(A0); 
  if (abs(clutchValue - lastClutchValue) > 1) {  
    Joystick.setRzAxis(clutchValue);
    lastClutchValue = clutchValue;
  }

// Brake loadcell
  brakeValue = scale.get_units(); // If the Brake pedal value is inverted put a - sign in front like -scale.get
  if (brakeValue < 0 or brakeValue < 1) { 
    brakeValue = 0;
  }
  if (abs(brakeValue - lastBrakeValue) > 1) { 
   Joystick.setRyAxis(brakeValue);
   lastBrakeValue = brakeValue;
  }

}
