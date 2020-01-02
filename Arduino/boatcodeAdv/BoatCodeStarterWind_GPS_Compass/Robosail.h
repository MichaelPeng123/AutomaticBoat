// ****** Find the values for the transmitter (using program RCReader) 
// ****** and WindSensor (using WindSensorTest)
// ****** and change the values below as needed
#define RUDDER_HIGH 1905  //nominal 2000
#define RUDDER_LOW 1193   //nominal 1000
#define SAIL_HIGH 2000   //nominal 2000
#define SAIL_LOW 1000    //nominal 1000
#define WIND_HIGH 1023   //nominal 1023

// Pin assignments
#define WIND_PIN 7
//input pins from receiver
#define RUDDER_RC_PIN 2
#define SAIL_RC_PIN 3
// Output pins to the servos
#define RUDDER_SERVO_PIN 5
// variables to hold input and output values
int rudderPulseWidth;
int rudderServoOut;
int sailPulseWidth;
int sailServoOut;
//variables for WindSensor
int windAngle = 0;
int windPulseWidth = 0;
int rudderPosition = 90;
int sailPosition = 45;

//create servo objects
Servo rudderServo;

// Compass (magnetometer and accelerometer variables
// These values will need to be adjusted based on your particular compass.
// Use compassCalibration (in the Orientation library) to determine the correct hard iron calibration.

//X: -50.09 < -3.64 < 42.82 : -22.27 (-25.91) Y: -44.73 < -5.55 < 33.64 : 15.45 (9.91) Z: -76.94 < -33.37 < 10.20 

float hardiron_x = -3.64;
float hardiron_y = -5.55;
float hardiron_z = -33.37;

float pitchAccel = 0; //raw accelerometer value
float rollAccel = 0;  //raw accelerometer value
float yawAccel = 0;   //raw accelerometer value
float robosailRollAccel = 0;  // converted so Port lean is positive
float pitch = 0;  //value in degrees, converted from raw data
float roll = 0;   //value in degrees, converted from raw data
float yaw = 0;    //value in degrees, converted from raw data

float heading;  //value in degrees
float robosailHeading;  //value in degrees, converted to RoboSail FrameofRef
float robosailRoll;   // converted so Port lean is positive

// Source: http://www.ngdc.noaa.gov/geomag-web/#igrfwmm
float declination = 14.35;
/* Assign a ID to Accelerometer and Magnetometer and define eventt */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
sensors_event_t event;

//variables for GPS
// this will be false until GPS fix is found and starting position saved
bool start_pos_found = false;
int GPSfix;
int GPSqual;
int GPSsat;
// once GPS fix is found, these variables will be updated
float startPositionX = 0;
float startPositionY = 0;
float relPositionX = 0;
float relPositionY = 0;
float XYpos[2]; // this is xy position - it is in a 2 place array for x, y
float angleFromStart = 0;
