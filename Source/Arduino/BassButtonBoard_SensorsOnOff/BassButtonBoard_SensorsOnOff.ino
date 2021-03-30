#include <AnalogMultiButton.h>  // Button Press Library  
#include <WiFiNINA.h>           // Wifi library
#include <WiFiUdp.h>            // UDP library
#include <OSCBundle.h>          // OSC Bundle Library


// Button Press Variables


// define the pin you want to use
const int BUTTONS_PIN = A0;

// set how many buttons you have connected
const int BUTTONS_TOTAL = 10;

// Red = R, Yellow = Y, Green = G, Brown = B
// The button order goes clockwise from the Vin wire, starting at the Red button.
//                                        R1, R2, R3,  Y1,  Y2,  Y3,  G1,  G2,  G3,  B1
const int BUTTONS_VALUES[BUTTONS_TOTAL] = {0, 92, 167, 233, 288, 337, 381, 417, 450, 480};

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
  float on  = 1.0f;
  float off = 0.0f;
  
  // update the AnalogMultiButton object every loop
  buttons.update();

  // When pressed button is released, flip On/Off bool and send via OSC:

  // Tell me Button Values (If you add more devices to the Arduino's power, check values because they'll change.)
  //Serial.println(analogRead(BUTTONS_PIN));

  // Acceleromter
  if (buttons.onRelease(YELLOW_3))
  {
    accelX = !accelX;
    
    if (accelX)
      sendOSCMessage( "/juce/accelXOnOff", on );
    else
      sendOSCMessage( "/juce/accelXOnOff", off );
  }
  
  if (buttons.onRelease(YELLOW_2))
  {
    accelY = !accelY;

    if (accelY)
      sendOSCMessage( "/juce/accelYOnOff", on );
    else
      sendOSCMessage( "/juce/accelYOnOff", off );
  }
  
  if (buttons.onRelease(YELLOW_1))
  {
    accelZ = !accelZ;
    
    if (accelZ)
      sendOSCMessage( "/juce/accelZOnOff", on );
    else
      sendOSCMessage( "/juce/accelZOnOff", off );
  }


  // Gyroscope
  if (buttons.onRelease(RED_3))
  {
    gyroX = !gyroX;

    if (gyroX)
      sendOSCMessage( "/juce/gyroXOnOff", on );
    else
      sendOSCMessage( "/juce/gyroXOnOff", off );
  }
  
  if (buttons.onRelease(RED_2))
  {
    gyroY = !gyroY;

    if (gyroY)
      sendOSCMessage( "/juce/gyroYOnOff", on );
    else
      sendOSCMessage( "/juce/gyroYOnOff", off );
  }
  
  if (buttons.onRelease(RED_1))
  {
    gyroZ = !gyroZ;

    if (gyroZ)
      sendOSCMessage( "/juce/gyroZOnOff", on );
    else
      sendOSCMessage( "/juce/gyroZOnOff", off );
  }


  // Touch Screen
  if (buttons.onRelease(GREEN_1))
  {
    touchScreenX = !touchScreenX;

    if (touchScreenX)
      sendOSCMessage( "/juce/touchXOnOff", on );
    else
      sendOSCMessage( "/juce/touchXOnOff", off );
  }
  
  if (buttons.onRelease(GREEN_2))
  {
    touchScreenY = !touchScreenY;

    if (touchScreenY)
      sendOSCMessage( "/juce/touchYOnOff", on );
    else
      sendOSCMessage( "/juce/touchYOnOff", off ); 
  }
  
  if (buttons.onRelease(GREEN_3))
  {
    touchScreenZ = !touchScreenZ;

    if (touchScreenZ)
      sendOSCMessage( "/juce/touchZOnOff", on );
    else
      sendOSCMessage( "/juce/touchZOnOff", off );
  }


  // Distance Meter
  if (buttons.onRelease(BROWN_1))
  {
    distance = !distance;

    if (distance)
      sendOSCMessage( "/juce/distanceOnOff", distance );
    else
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


void sendOSCMessage(const char *address, float buttonVal)
{
  bndl.add( address ).add( buttonVal );

  udp.beginPacket( computerIP, destPort );
  bndl.send( udp );
  udp.endPacket();
  bndl.empty();
}
