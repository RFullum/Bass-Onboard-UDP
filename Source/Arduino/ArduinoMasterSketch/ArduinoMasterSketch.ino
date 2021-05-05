/**
 * Sketch communicates between Arduino Nano 33 IoT and Digital Bass Juce App
 * 
 * Sensors: 
 *  - 3D Accelerometer (onboard IMU) 
 *  - 3D Gyroscope (onboard IMU)
 *  - 3D Touch Screen
 *  - Time of Flight Distance
 *  
 *  Interfaces:
 *    - 10 button bank (Sensor on/off)
 *    - 4 button bank (Filter parameter controls)
 *    - 2 Encoder twist + push button (parameter value & function selection)
 *    
 *  Connects to WiFi network and uses UDP to communicate OSC Bundles/Messages to Juce App
 *  
 *  
 */
 
// ~~~ INCLUDE SECTION ~~~ 
#include <WiFiNINA.h>           // Wifi library
#include <WiFiUdp.h>            // UDP library
#include <Arduino_LSM6DS3.h>    // IMU accelerometer library
#include "Adafruit_VL53L0X.h"   // Distance meter library
#include "TouchScreen.h"        // Touch Screen Library
#include <OSCBundle.h>          // OSC Bundle Library
#include <AnalogMultiButton.h>  // Button Press Library  



// ~~~ ROTARY ENCODER SECTION ~~~
int encoderPin1 = 2;
int encoderPin2 = 4;
int encoderPin3 = 3;
int encoderPin4 = 7;

volatile int  lastEncoded1  = 0;
volatile long encoderValue1 = 0;

volatile int  lastEncoded2  = 0;
volatile long encoderValue2 = 0;

long lastencoderValue1 = 0;
long lastencoderValue2 = 0;

int lastMSB1 = 0;
int lastLSB1 = 0;

int lastMSB2 = 0;
int lastLSB2 = 0;



// ~~~ BUTTON SECTION ~~~ 

// define the pin you want to use
const int SENSOR_BUTTONS_PIN = A0;
const int FILTER_BUTTONS_PIN = A1;
const int ENCODER_BUTTONS_PIN = A7;

// set how many buttons you have connected
const int SENSOR_BUTTONS_TOTAL = 10;
const int FILTER_BUTTONS_TOTAL = 4;
const int ENCODER_BUTTONS_TOTAL = 2;

// Red = R, Yellow = Y, Green = G, Brown = B
// The button order goes clockwise from the Vin wire, starting at the Red button.
//                                                      R1, R2, R3,  Y1,  Y2,  Y3,  G1,  G2,  G3,  B1
const int SENSOR_BUTTONS_VALUES[SENSOR_BUTTONS_TOTAL] = {0, 92, 167, 233, 288, 337, 381, 417, 450, 480};
const int FILTER_BUTTONS_VALUES[FILTER_BUTTONS_TOTAL] = {0, 90, 167, 234};
//                                                      LPF BPF HPF POLES
const int ENCODER_BUTTONS_VALUES[ENCODER_BUTTONS_TOTAL] = {0, 90};

// Define constants for each button
// Sensor bank
const int RED_1    = 0;
const int RED_2    = 1;
const int RED_3    = 2;
const int YELLOW_1 = 3;
const int YELLOW_2 = 4;
const int YELLOW_3 = 5;
const int GREEN_1  = 6;
const int GREEN_2  = 7;
const int GREEN_3  = 8;
const int BROWN_1  = 9;

// Filter bank
const int LPF   = 0;
const int BPF   = 1;
const int HPF   = 2;
const int POLES = 3;

// Encoder bank
const int B1 = 0;
const int B2 = 1;


// make an AnalogMultiButton object, pass in the pin, total and values array
// ***
// pass a fourth parameter to set the debounce time in milliseconds
// this defaults to 20 and can be increased if you're working with particularly bouncy buttons
// (didn't do this)
// ***
AnalogMultiButton sensorButtons  ( SENSOR_BUTTONS_PIN,  SENSOR_BUTTONS_TOTAL,  SENSOR_BUTTONS_VALUES  );
AnalogMultiButton filterButtons  ( FILTER_BUTTONS_PIN,  FILTER_BUTTONS_TOTAL,  FILTER_BUTTONS_VALUES  );
AnalogMultiButton encoderButtons ( ENCODER_BUTTONS_PIN, ENCODER_BUTTONS_TOTAL, ENCODER_BUTTONS_VALUES );

// Sensor Button On/Off
float accelXOnOff = -1.0f;  // YELLOW_3
float accelYOnOff = -1.0f;  // YELLOW_2
float accelZOnOff = -1.0f;  // YELLOW_1

float gyroXOnOff = -1.0f;   // RED_3
float gyroYOnOff = -1.0f;   // RED_2
float gyroZOnOff = -1.0f;   // RED_1

float touchScreenXOnOff = -1.0f;  // GREEN_1
float touchScreenYOnOff = -1.0f;  // GREEN_2
float touchScreenZOnOff = -1.0f;  // GREEN_3

float distanceOnOff = -1.0f;  // BROWN_1

// 0 = LPF
// 1 = BPF
// 2 = HPF
float filterType = 0.0f;

// -12dB/-24dB toggle
// -1 = -12dB
// +1 = -24dB
float filterPoles = -1;

// Encoder Buttons
float encButton1 = 0.0f;
float encButton2 = 0.0f;



// ~~~ TOUCHSCREEN SECTION ~~~ 

// Touch Screen Defines
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin



// ~~~ WiFi SECTION ~~~ 
char ssid[] = "Frasier Crane";    // Wifi SSID (network name)
char pass[] = "1974Ducati750ss";  // Wifi password

int status = WL_IDLE_STATUS;      // Status of WiFi connection

WiFiSSLClient client;             // Instantiate the Wifi client



// ~~~ OSC SECTION ~~~
WiFiUDP udp;                          // Instantiate UDP class

unsigned int localPort = 2390;        // local port to listen on
const char* computerIP = "Speed3";

const unsigned int destPort = 9001;

// OSC Bundle
OSCBundle bndl;



// ~~~ SENSOR SECTION ~~~ 

// Sensor Values
float accelX = 0.0f;
float accelY = 0.0f;
float accelZ = 0.0f;

float gyroX = 0.0f;
float gyroY = 0.0f;
float gyroZ = 0.0f;

float touchX = 0.0f;
float touchY = 0.0f;
float touchZ = 0.0f;

float distance = 0.0f;

// Value smoothing
float accelSmoothingFactor = 0.4f;
float gyroSmoothingFactor  = 0.4f;
float touchSmoothingFactor = 0.4f;
float distSmoothingFactor  = 0.4f;

// Distance Sensor instance
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Touchscreen Instance
TouchScreen ts = TouchScreen( XP, YP, XM, YM, 600 );




//
// ***********************************************************
//



void setup() 
{
  // Sketch Functions
  connectToWiFi();
  connectUDP();

  // Sensors Setup
  checkIMU();
  checkDistanceSensor();

  // Rotary Encoder Setup
  rotaryEncoderSetup();
}




void loop() 
{
  // Sensor Reading
  readSensor();
  readTouchScreen();
  readDistance();

  // Button Handling
  sensorButtonController();
  filterButtonController();

  // OSC Send
  sendOSCBundle();
}





//
//  setup() Functions ************************************************
//



// ~~~ WIFI ~~~ 
void connectToWiFi()
{
  // Check for Wifi Module. If no module, don't continue
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("FAILURE");
    while (true);
  }
  else
  {
    Serial.println("Not Failure");
  }

  // Connect to Wifi Access Point
  while ( status != WL_CONNECTED) 
  {
    status = WiFi.begin(ssid, pass);

    // wait 1 second for connection:
    delay(1000);
  }
}



// ~~~ OSC/UDP ~~~ 
void connectUDP()
{
  // Start UDP
  udp.begin(localPort);
}



// ~~~ SENSORS ~~~ 
void checkIMU()
{
  // Stop if IMU doesn't begin.
  if (!IMU.begin())
  {
    while(1);
  }
}

void checkDistanceSensor()
{
  // Stop if Distance Sensor not working
  if (!lox.begin())
  {
    while(1);
  }
}



// ~~~ ROTARY ENCODERS ~~~
void rotaryEncoderSetup()
{
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  pinMode(encoderPin3, INPUT);
  pinMode(encoderPin4, INPUT);
  
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin3, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin4, HIGH); //turn pullup resistor on
  
  //call updateEncoder() when any high/low changed seen
  //on interrupt 2 (pin 2), or interrupt 2 (pin 3)
  attachInterrupt(2, updateEncoder, CHANGE);
  attachInterrupt(3, updateEncoder, CHANGE);
}


//
//  loop() Functions ************************************************
//



// ~~~ SENSORS ~~~ 
void readSensor()
{ 
  if (IMU.accelerationAvailable())
  {
    float accelXRaw, accelYRaw, accelZRaw;
    
    IMU.readAcceleration( accelXRaw, accelYRaw, accelZRaw );

    accelX += ( accelXRaw - accelX ) * accelSmoothingFactor;
    accelY += ( accelYRaw - accelY ) * accelSmoothingFactor;
    accelZ += ( accelZRaw - accelZ ) * accelSmoothingFactor;
  }
  
  if (IMU.gyroscopeAvailable())
  {
    float gyroXRaw, gyroYRaw, gyroZRaw;
    
    IMU.readGyroscope( gyroXRaw, gyroYRaw, gyroZRaw );

    gyroX += ( gyroXRaw - gyroX ) * gyroSmoothingFactor;
    gyroY += ( gyroYRaw - gyroY ) * gyroSmoothingFactor;
    gyroZ += ( gyroZRaw - gyroZ ) * gyroSmoothingFactor;
  }
}


void readTouchScreen()
{
  // A point object holds x, y, and z coords
  TSPoint p = ts.getPoint();

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  // Pressur is inversed (lower number is greater pressure)
  if (p.z > ts.pressureThreshhold)
  {
    float touchXRaw = p.x;
    float touchYRaw = p.y;
    float touchZRaw = p.z;

    touchX += ( touchXRaw - touchX ) * touchSmoothingFactor;
    touchY += ( touchYRaw - touchY ) * touchSmoothingFactor;
    touchZ += ( touchZRaw - touchZ ) * touchSmoothingFactor;
  }
}



void readDistance()
{
  VL53L0X_RangingMeasurementData_t measure;

  float distRaw;

  lox.rangingTest( &measure, false );

  if (measure.RangeStatus != 4)
  {
    distRaw = measure.RangeMilliMeter;
  }
  else
  {
    distRaw = 0.0f;
  }

  distance += ( distRaw - distance ) * distSmoothingFactor;
}



// ~~~ BUTTONS ~~~
void sensorButtonController()
{
  sensorButtons.update();

  // Accelerometer
  if (sensorButtons.onRelease(YELLOW_3))
  {
    accelXOnOff *= -1.0f;
  }

  if (sensorButtons.onRelease(YELLOW_2))
  {
    accelYOnOff *= -1.0f;
  }

  if (sensorButtons.onRelease(YELLOW_1))
  {
    accelZOnOff *= -1.0f;
  }

  // Gyroscope
  if (sensorButtons.onRelease(RED_3))
  {
    gyroXOnOff *= -1.0f;
  }

  if (sensorButtons.onRelease(RED_2))
  {
    gyroYOnOff *= -1.0f;
  }

  if (sensorButtons.onRelease(RED_1))
  {
    gyroZOnOff *= -1.0f;
  }

  // Touch Screen
  if (sensorButtons.onRelease(GREEN_1))
  {
    touchScreenXOnOff *= -1.0f;
  }

  if (sensorButtons.onRelease(GREEN_2))
  {
    touchScreenYOnOff *= -1.0f;
  }

  if (sensorButtons.onRelease(GREEN_3))
  {
    touchScreenZOnOff *= -1.0f;
  }

  // Distance
  if (sensorButtons.onRelease(BROWN_1))
  {
    distanceOnOff *= -1.0f;
  }
}

void filterButtonController()
{
  // update the AnalogMultiButton object every loop
  filterButtons.update();

  // Flip filter poles between -12dB & -24dB
  if (filterButtons.onRelease(POLES))
  {
    filterPoles *= -1.0f;
  }

  // Select between LPF, BPF, and HPF
  if (filterButtons.onRelease(LPF))
  {
    filterType = 0.0f;
  }

  if (filterButtons.onRelease(BPF))
  {
    filterType = 1.0f;
  }

  if (filterButtons.onRelease(HPF))
  {
    filterType = 2.0f;
  }
}

void encoderButtonController()
{
  // Number of mappings the presses cycle through minus 1 (count from 0)
  float enc1Limit = 6.0f;
  float enc2Limit = 2.0f;

  
  // update the AnalogMultiButton object every loop
  encoderButtons.update();

  if (encoderButtons.onRelease(B1))
  {
    encButton1++;

    if (encButton1 < enc1Limit)
    {
      encButton1 = 0.0f;
    }
  }

  if (encoderButtons.onRelease(B2))
  {
    encButton2++;

    if (encButton2 < enc2Limit)
    {
      encButton2 = 0.0f;
    }
  }
  
}



// ~~~ OSC ~~~ 
void sendOSCBundle()
{
  bndl.add( "/juce/accelX" ).add( accelX );
  bndl.add( "/juce/accelY" ).add( accelY );
  bndl.add( "/juce/accelZ" ).add( accelZ );

  bndl.add( "/juce/gyroX" ).add( gyroX );
  bndl.add( "/juce/gyroY" ).add( gyroY );
  bndl.add( "/juce/gyroZ" ).add( gyroZ );

  bndl.add( "/juce/touchX" ).add( touchX );
  bndl.add( "/juce/touchY" ).add( touchY );
  bndl.add( "/juce/touchZ" ).add( touchZ );

  bndl.add( "/juce/dist" ).add( distance );

  bndl.add( "/juce/filtPole" ).add( filterPoles );
  bndl.add( "/juce/filtType" ).add( filterType  );

  bndl.add( "/juce/accelXOnOff" ).add( accelXOnOff );
  bndl.add( "/juce/accelYOnOff" ).add( accelYOnOff );
  bndl.add( "/juce/accelZOnOff" ).add( accelZOnOff );

  bndl.add( "/juce/gyroXOnOff" ).add( gyroXOnOff );
  bndl.add( "/juce/gyroYOnOff" ).add( gyroYOnOff );
  bndl.add( "/juce/gyroZOnOff" ).add( gyroZOnOff );

  bndl.add( "/juce/touchScreenXOnOff" ).add( touchScreenXOnOff );
  bndl.add( "/juce/touchScreenYOnOff" ).add( touchScreenYOnOff );
  bndl.add( "/juce/touchScreenZOnOff" ).add( touchScreenZOnOff );

  bndl.add( "/juce/distanceOnOff" ).add( distanceOnOff );

  bndl.add( "/juce/encoder1" ).add( encoderValue1 );
  bndl.add( "/juce/encoder2" ).add( encoderValue2 );

  bndl.add( "/juce/encButton1" ).add( encButton1 );
  bndl.add( "/juce/encButton2" ).add( encButton2 );

  udp.beginPacket( computerIP, destPort );
  bndl.send( udp );
  udp.endPacket();
  bndl.empty();
}



// Actually called in rotaryEncoderSetup() in setup() but because it's an interrupt
// it happens while the loop() is running
void updateEncoder()
{
  float

  
  int MSB1 = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB1 = digitalRead(encoderPin2); //LSB = least significant bit

  int MSB2 = digitalRead(encoderPin3); //MSB = most significant bit
  int LSB2 = digitalRead(encoderPin4); //LSB = least significant bit
  
  int encoded1 = (MSB1 << 1) |LSB1; //converting the 2 pin value to single number 
  int sum1 = (lastEncoded1 << 2) | encoded1; //adding it to the previous encoded value 

  int encoded2 = (MSB2 << 1) |LSB2; //converting the 2 pin value to single number 
  int sum2 = (lastEncoded2 << 2) | encoded2; //adding it to the previous encoded value 
  
  if (sum1 == 0b1101 || sum1 == 0b0100 || sum1 == 0b0010 || sum1 == 0b1011)
    encoderValue1 ++; 
    
  if(sum1 == 0b1110 || sum1 == 0b0111 || sum1 == 0b0001 || sum1 == 0b1000)
    encoderValue1 --; 
    
  lastEncoded1 = encoded1; //store this value for next time 

  if (sum2 == 0b1101 || sum2 == 0b0100 || sum2 == 0b0010 || sum2 == 0b1011)
    encoderValue2 ++; 
    
  if(sum2 == 0b1110 || sum2 == 0b0111 || sum2 == 0b0001 || sum2 == 0b1000)
    encoderValue2 --; 
    
  lastEncoded2 = encoded2; //store this value for next time 
}
