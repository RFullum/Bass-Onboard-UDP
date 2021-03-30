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
float accelX, accelY, accelZ;
float gyroX,  gyroY,  gyroZ;
float touchX, touchY, touchZ;
float distance;

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
    IMU.readAcceleration( accelX, accelY, accelZ );
  }

  if (IMU.gyroscopeAvailable())
  {
    IMU.readGyroscope( gyroX, gyroY, gyroZ );
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
    touchX = p.x;
    touchY = p.y;
    touchZ = p.z;
  }
}



void readDistance()
{
  VL53L0X_RangingMeasurementData_t measure;

  float dist;

  lox.rangingTest( &measure, false );

  if (measure.RangeStatus != 4)
  {
    dist = measure.RangeMilliMeter;
  }
  else
  {
    dist = 0.0f;
  }

  distance = dist;
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
