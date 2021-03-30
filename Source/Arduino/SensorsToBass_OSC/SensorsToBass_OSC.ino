/**
 * Sketch for Arduino Nano 33 IoT
 * 
 * Connects to WiFi network and uses UDP to send 
 * onboard sensor data as OSC Bundles.
 * 
 * Sensors: 3D Gyro and 3D accelerometer (6 values), 
 * 3D touch screen (3 values), and distance (1 value).
 * Sensor values are Floats. 
 *  
 * This is to connect with a JUCE application via UDP/OSC.
 */

#include <WiFiNINA.h>         // Wifi library
#include <WiFiUdp.h>          // UDP library
#include <Arduino_LSM6DS3.h>  // IMU accelerometer library
#include "Adafruit_VL53L0X.h" // Distance meter library
#include "TouchScreen.h"      // Touch Screen Library
#include <OSCBundle.h>        // OSC Bundle Library


// Touch Screen Defines
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin


// WiFi variables
char ssid[] = "Frasier Crane";    // Wifi SSID (network name)
char pass[] = "1974Ducati750ss";  // Wifi password

int status = WL_IDLE_STATUS;      // Status of WiFi connection

WiFiSSLClient client;             // Instantiate the Wifi client


// UDP Variables
WiFiUDP udp;                          // Instantiate UDP class

unsigned int localPort = 2390;        // local port to listen on
const char* computerIP = "Speed3";

const unsigned int destPort = 9001;

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

// OSC Bundle
OSCBundle bndl;

//
// ***********************************************************
//


void setup() 
{
  connectToWiFi();
  connectUDP();
  checkIMU();
  checkDistanceSensor();
}


void loop() 
{
  readSensor();
  readTouchScreen();
  readDistance();
  
  sendOSCBundle();
}


//
//  setup() Functions ************************************************
//


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


void connectUDP()
{
  // Start UDP
  udp.begin(localPort);
}


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


//
//  loop() Functions ************************************************
//


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

  udp.beginPacket( computerIP, destPort );
  bndl.send( udp );
  udp.endPacket();
  bndl.empty();
}
