/* ManAutoSailSwitch - Sample code 1/19/2016
© 2014-2016 RoboSail
This program contains starter code for a boat to have automatic 
sail control using information from the Wind Sensor.
For detailed information about getting input from the WindSensor,
input from the RC receiver, and sending output to the Servos,
see these programs: WindSensor and RCPassThroughSimple
The variable "autosail" sets the sail to automatic.
Program values are displayed to the Serial Monitor when the 
variable "verbose" is set to true.  otherwise set verbose to false.
*/

#include <Servo.h>

boolean autoSail = true; // set to tru for automatic sail
boolean verbose = true;  //true calls function for values to be printed to monitor


//determine these values using the hardware test programs and fill in the appropriate values
int RUDDER_HIGH = 1900;   //nominal 2000
int RUDDER_LOW = 1100;    //nominal 1000
int SAIL_HIGH = 2000;    //nominal 2000
int SAIL_LOW = 1000;     //nominal 1000
int WIND_HIGH = 1023;    //nominal 1023

// Pin assignments
int WIND_PIN = 7;         // Wind sensor pin
int RUDDER_RC_PIN = 2;    //input pins from receiver
int SAIL_RC_PIN = 3;
int RUDDER_SERVO_PIN = 8; // Output pins to the servos
int SAIL_SERVO_PIN = 9;
//************************//create servo objects
Servo rudderServo;
Servo sailServo;
//************************// variables to hold input and output values
int rudderPulseWidth;
int rudderServoOut;
int sailPulseWidth;
int sailServoOut;
//************************//variables for WindSensor
int windAngle = 0;
int windPulseWidth = 0;
int rudderPosition = 0;
int sailPosition = 45;

/****************************** SETUP ******************/
void setup() {
  Serial.begin(115200);
  Serial.println("\nRoboSail BoatCode - ManAutoSailSwitch");  //write program name here
  // Set RC receiver and WindSensor on digital input pins
  pinMode(RUDDER_RC_PIN, INPUT);
  pinMode(SAIL_RC_PIN, INPUT);
  pinMode(WIND_PIN, INPUT);

  // attach the servos to the proper pins
  rudderServo.attach(RUDDER_SERVO_PIN);
  sailServo.attach(SAIL_SERVO_PIN);
}

void loop() {
//*********** Read in data from the RC receiver and sensors *********
  // Read the command pulse from the RC receiver
  rudderPulseWidth = pulseIn(RUDDER_RC_PIN, HIGH);
  sailPulseWidth = pulseIn(SAIL_RC_PIN, HIGH);
  // Calculate the servo position in degrees.
  sailPosition = map(sailPulseWidth, SAIL_LOW, SAIL_HIGH, 0, 90);
  rudderPosition = map(rudderPulseWidth, RUDDER_LOW, RUDDER_HIGH, -60, 60);
  
  // Read values from the WindSensor
  windPulseWidth = pulseIn(WIND_PIN, HIGH);
  // Convert the wind angle to degrees from PWM.  Range -180 to +180
  windAngle = map(windPulseWidth, 0, WIND_HIGH, 180, -180);
  windAngle = constrain(windAngle, -180, 180);  // gets rid of noisy readings

//**************** your code here ******************
// calculate your values for rudderPosition and sailPosition in degrees 
// and set those variables to the new values.
// If you do not set the values, it will use the values from the Transmitter
// For example, to make the rudder follow the wind angle you would have:
// rudderPosition = windAngle;

//  Automatic sail switch
if (sailPulseWidth > 1850){autoSail=true;}
else {autoSail=false;}

if (autoSail)
  {
  windAngle = abs(windAngle);           // sail position can only be positive
  if (windAngle < 45) sailPosition = 0;
  if ((windAngle >= 45) && (windAngle < 135)) sailPosition = windAngle - 45;
  if (windAngle >= 135) sailPosition = 90;
  // sailPosition = (180-abs(windAngle))/2;  // a different algorithm for automatic sail 
  }

// if autoSail = false sailPosition will be set by RC transmitter signal  
//  rudderPosition will always be set by RC transmitter signal


/********************* send commands to motors *************************/
  driveSailServo(sailPosition);
  driveRudderServo(rudderPosition);
  
  if (verbose) {printToMonitor();}
} //end of loop()

/************Functions to drive Sail and Rudder servos ******************/
 // This code takes in the desired postions for the servos in degrees (as 
 // defined in RoboSail) then calculates appropriate values for the servo commands, 
 // making sure not to send the servos to impossible positions, which could 
 // damage the servo motors.
 // The Rudder servo motor ranges from 0 to 180 with 90 deg in the center
 // The Sailwinch servo is at ~55 deg when full-in, which we think of as 0 deg,
 // and ~125 deg when full out, which we think of as 90 deg

void driveSailServo(int sailPos)
{
  if ((sailPos >= 0) && (sailPos <= 90))  // the command in degrees is valid
  {
    sailServoOut = map(sailPos, 0, 90, 55, 125);
    sailServo.write(sailServoOut);
  }
  else 
  {
    Serial.print("ERROR - sail position out of range: ");
    Serial.println(sailPos);
    }
}

void driveRudderServo(int rudderPos)
{
  if ((rudderPos >= -60) && (rudderPos <= 60))
  {
    rudderServoOut = map(rudderPos, -90, 90, 0, 180);
    rudderServo.write(rudderServoOut);
  }
  else 
    {
    Serial.print("ERROR - rudder position out of range: ");
    Serial.println(rudderPos);
    }
}

// Function to Print out all values for debug.
void printToMonitor()
{
   Serial.print("Mode: ");
  if (autoSail) Serial.println("automatic sail");
  else Serial.println("manual sail");
  Serial.print("Wind Angle: ");
  Serial.print(windAngle);
 
//  Serial.print("  Rudder, RC: ");
//  Serial.print(rudderPulseWidth);
   Serial.print("  desired Rudder angle: ");
  Serial.print(rudderPosition);
  Serial.print("  to servo: ");
  Serial.print(rudderServoOut);

//  Serial.print("     Sail, RC: ");
//  Serial.print(sailPulseWidth);
  Serial.print("  desired Sail angle: ");
  Serial.print(sailPosition);
  Serial.print("  to servo: ");
  Serial.print(sailServoOut);

  Serial.print("\n"); // Print a new line
}

