#include <AnalogMultiButton.h>  // Button Press Library  
#include <WiFiNINA.h>           // Wifi library
#include <WiFiUdp.h>            // UDP library
#include <OSCBundle.h>          // OSC Bundle Library
#include <OSCMessage.h>         // OSC Message Library


// Button Press Variables


// define the pin you want to use
const int BUTTONS_PIN = A0;

// set how many buttons you have connected
const int BUTTONS_TOTAL = 10;

// Red = R, Yellow = Y, Green = G, Brown = B
// The button order goes clockwise from the Vin wire, starting at the Red button.
//                                        R1, R2, R3,  Y1,  Y2,  Y3,  G1, G2,  G3,  B1
const int BUTTONS_VALUES[BUTTONS_TOTAL] = {0, 65, 120, 166, 206, 241, 271,298, 323, 343};

// Define constants for each button
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

// make an AnalogMultiButton object, pass in the pin, total and values array
AnalogMultiButton buttons(BUTTONS_PIN, BUTTONS_TOTAL, BUTTONS_VALUES);

// ***
// pass a fourth parameter to set the debounce time in milliseconds
// this defaults to 20 and can be increased if you're working with particularly bouncy buttons
// ***

// Sensor Button On/Off
bool accelX = false;  // YELLOW_3
bool accelY = false;  // YELLOW_2
bool accelZ = false;  // YELLOW_1

bool gyroX = false;   // RED_3
bool gyroY = false;   // RED_2
bool gyroZ = false;   // RED_1

bool touchScreenX = false;  // GREEN_1
bool touchScreenY = false;  // GREEN_2
bool touchScreenZ = false;  // GREEN_3

bool distance = false;  // BROWN_1


// WiFi Variables
char ssid[] = "Frasier Crane";    // Wifi SSID (network name)
char pass[] = "1974Ducati750ss";  // Wifi password

int status = WL_IDLE_STATUS;      // Status of WiFi connection

WiFiSSLClient client;             // Instantiate the Wifi client


// UDP Variables
WiFiUDP udp;                          // Instantiate UDP class

unsigned int localPort = 2390;        // local port to listen on
const char* computerIP = "Speed3";

const unsigned int destPort = 9001;


// OSC
OSCBundle  bndl;
OSCMessage msg;


//
// ===============================================================================================
//


void setup() 
{
  Serial.begin(9600);

  connectToWiFi();
  connectUDP();
}


//
// ===============================================================================================
//


void loop() 
{  
  // update the AnalogMultiButton object every loop
  buttons.update();

  // When pressed button is released, flip On/Off bool and send via OSC:


  // Acceleromter
  if (buttons.onRelease(YELLOW_3))
  {
    accelX = !accelX;
    sendOSCMessage( "/juce/accelXOnOff", accelX );
  }
  
  if (buttons.onRelease(YELLOW_2))
  {
    accelY = !accelY;
    sendOSCMessage( "/juce/accelYOnOff", accelY );
  }
  
  if (buttons.onRelease(YELLOW_1))
  {
    accelZ = !accelZ;
    sendOSCMessage( "/juce/accelZOnOff", accelZ );
  }


  // Gyroscope
  if (buttons.onRelease(RED_3))
  {
    gyroX = !gyroX;
    sendOSCMessage( "/juce/gyroXOnOff", gyroX );
  }
  
  if (buttons.onRelease(RED_2))
  {
    gyroY = !gyroY;
    sendOSCMessage( "/juce/gyroYOnOff", gyroY );
  }
  
  if (buttons.onRelease(RED_1))
  {
    gyroZ = !gyroZ;
    sendOSCMessage( "/juce/gyroZOnOff", gyroZ );
  }


  // Touch Screen
  if (buttons.onRelease(GREEN_1))
  {
    touchScreenX = !touchScreenX;
    sendOSCMessage( "/juce/touchXOnOff", touchScreenX );
  }
  
  if (buttons.onRelease(GREEN_2))
  {
    touchScreenY = !touchScreenY;
    sendOSCMessage( "/juce/touchYOnOff", touchScreenY );
  }
  
  if (buttons.onRelease(GREEN_3))
  {
    touchScreenZ = !touchScreenZ;
    sendOSCMessage( "/juce/touchZOnOff", touchScreenZ );
  }


  // Distance Meter
  if (buttons.onRelease(BROWN_1))
  {
    distance = !distance;
    sendOSCMessage( "/juce/distanceOnOff", distance );
  }
}




//
// ========== setup() Functions ==============================================================
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





//
// ========== loop() Functions ==============================================================
//


void sendOSCMessage(String address, bool buttonVal)
{
  OSCMessage msg( address );
  
  msg.add( butonVal );

  udp.beginPacket( computerIP, destPort );
  msg.send( udp );
  udp.endPacket();
  msg.empty();
}
