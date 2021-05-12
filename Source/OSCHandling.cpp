/*
  ==============================================================================

    OSCHandling.cpp
    Created: 17 Mar 2021 11:13:21pm
    Author:  Robert Fullum
 
    OSC over UDP Arduino, Sensor, Button, and Encoder Notes:
 
    Ardiuno Sketch: ArduinoMasterSketch.ino in src subfolder
    
    Written for the Arduino Nano 33 IoT. The sketch will probably work for other
    Arduino models, but modifications may be needed to account for different pin
    connectors, and onboard/outboard IMUs or WiFi.
 
    Accelerometer: Arduino Nano 33 IoT's onboard accelerometer IMU. Values from
    -4.0f to 4.0f, with gravity equal to 1.0f. Values between -1.0f and 1.0f are
    the orientation. Greater than 1.0f or less than -1.0f are movements with more
    Gs than gravity, for instance, stopping suddenly from speed.
 
    Gyroscope: Arduino Nano 33 IoT's onboard gyroscope IMU. Values from -2000.0f
    to 2000.0f. Gyroscope measures directions of movement only when moving.
 
    Distance Sensor: Adafruit Time of Flight Distance Sensor VL53L0X. Measures
    distance from sensor from 50mm to 1200mm, but can get shorter or longer
    readings depending on the ambient light and reflectivity of the object being
    sensed. Readings out of range return 0.0f.
 
    Touchscreen: Adafruit 3.2" Resistive Touchscreen (part 333) + Adafruit
    Dual 1.0mm 4 connector FPC Touchscreen Breakout. In landscape the X
    values are height and the Y values are width. The Z values are touch pressure,
    inversed: the harder you push, the lower the value. The values range from 0.0f
    to 1024.0f. However, there is reduced sensitivity near the edge of the
    touchscreen causing the actual useable value range to be between ~200.0f
    to ~800.0f.
 
    Rotary Encoders: Generic 3 pin 20 PPR Rotary encoders with push-button
    functionality from Bourns Inc on DigiKey.
 
    Buttons: Adafruit 6MM Rainbow Tactile Button Delux. Completes circuit only
    while pressed. Each bank of buttons can attach to 1 pin on the Arduino, plus
    power & ground pins. 

  ==============================================================================
*/

#include "OSCHandling.h"

OSCHandler::OSCHandler() :
    accelXAddress("/juce/accelX"),
    accelYAddress("/juce/accelY"),
    accelZAddress("/juce/accelZ"),

    gyroXAddress("/juce/gyroX"),
    gyroYAddress("/juce/gyroY"),
    gyroZAddress("/juce/gyroZ"),

    distanceAddress("/juce/dist"),

    touchXAddress("/juce/touchX"),
    touchYAddress("/juce/touchY"),
    touchZAddress("/juce/touchZ"),

    accelXOnOffAddress("/juce/accelXOnOff"),
    accelYOnOffAddress("/juce/accelYOnOff"),
    accelZOnOffAddress("/juce/accelZOnOff"),

    gyroXOnOffAddress("/juce/gyroXOnOff"),
    gyroYOnOffAddress("/juce/gyroYOnOff"),
    gyroZOnOffAddress("/juce/gyroZOnOff"),

    touchXOnOffAddress("/juce/touchScreenXOnOff"),
    touchYOnOffAddress("/juce/touchScreenYOnOff"),
    touchZOnOffAddress("/juce/touchScreenZOnOff"),

    distanceOnOffAddress("/juce/distanceOnOff"),

    filterTypeAddress("/juce/filtType"),
    filterPoleAddress("/juce/filtPole"),

    encoder1Address("/juce/encoder1"),
    encoder2Address("/juce/encoder2"),

    encButton1Address("/juce/encButton1"),
    encButton2Address("/juce/encButton2"),

    accelXVal(0.0f),
    accelYVal(0.0f),
    accelZVal(0.0f),
    
    gyroXVal(0.0f),
    gyroYVal(0.0f),
    gyroZVal(0.0f),

    distVal(0.0f),

    touchXVal(0.0f),
    touchYVal(0.0f),
    touchZVal(0.0f),

    accelXOnOff(-1.0f),
    accelYOnOff(-1.0f),
    accelZOnOff(-1.0f),
    
    gyroXOnOff(-1.0f),
    gyroYOnOff(-1.0f),
    gyroZOnOff(-1.0f),

    touchXOnOff(-1.0f),
    touchYOnOff(-1.0f),
    touchZOnOff(-1.0f),

    distanceOnOff(-1.0f),

    filtType(0.0f),
    filtPole(-1.0f),

    encoder1(0.0f),
    encoder2(0.0f),
    encButton1(0.0f),
    encButton2(0.0f)

{
    // UDP Socket Port
    connect ( 9001 );
    
    // Bundle
    addListener ( this );
    
    // Messages
    addListener ( this, accelXAddress   );
    addListener ( this, accelYAddress   );
    addListener ( this, accelZAddress   );
    
    addListener ( this, gyroXAddress    );
    addListener ( this, gyroYAddress    );
    addListener ( this, gyroZAddress    );
    
    addListener ( this, touchXAddress   );
    addListener ( this, touchYAddress   );
    addListener ( this, touchZAddress   );
    
    addListener ( this, distanceAddress );
    
    addListener( this, accelXOnOffAddress );
    addListener( this, accelYOnOffAddress );
    addListener( this, accelZOnOffAddress );
    
    addListener( this, gyroXOnOffAddress );
    addListener( this, gyroYOnOffAddress );
    addListener( this, gyroZOnOffAddress );
    
    addListener( this, touchXOnOffAddress );
    addListener( this, touchYOnOffAddress );
    addListener( this, touchZOnOffAddress );
    
    addListener( this, distanceOnOffAddress );
    
    addListener( this, filterTypeAddress );
    addListener( this, filterPoleAddress );
    
    addListener( this, encoder1Address   );
    addListener( this, encoder2Address   );
    addListener( this, encButton1Address );
    addListener( this, encButton2Address );
}


OSCHandler::~OSCHandler()
{}


/**
 Automatically called when OSC Bundle comes in, splits bundle into its elements, and calls either
 oscMessageReceived or oscBundleReceived depending on which type the element of the bundle is
 */
void OSCHandler::oscBundleReceived(const OSCBundle &bundle)
{
    if (bundle.size() > 0)
    {
        for (int i = 0; i < bundle.size(); i++)
        {
            auto elem = bundle[i];
            
            if (elem.isMessage())
                oscMessageReceived ( elem.getMessage() );
            else if (elem.isBundle())
                oscBundleReceived ( elem.getBundle() );
        }
    }
}


void OSCHandler::oscMessageReceived(const OSCMessage &message)
{
    String address = message.getAddressPattern().toString();
    
    if (message[0].isFloat32())
    {
        float val = message[0].getFloat32();
        
        if (address == accelXAddress.toString())
            accelXVal = val;
        else if (address == accelYAddress.toString())
            accelYVal = val;
        else if (address == accelZAddress.toString())
            accelZVal = val;
        else if (address == gyroXAddress.toString())
            gyroXVal = val;
        else if (address == gyroYAddress.toString())
            gyroYVal = val;
        else if (address == gyroZAddress.toString())
            gyroZVal = val;
        else if (address == touchXAddress.toString())
            touchXVal = val;
        else if (address == touchYAddress.toString())
            touchYVal = val;
        else if (address == touchZAddress.toString())
            touchZVal = val;
        else if (address == distanceAddress.toString())
            distVal = val;
        else if (address == accelXOnOffAddress.toString())
            accelXOnOff = val;
        else if (address == accelYOnOffAddress.toString())
            accelYOnOff = val;
        else if (address == accelZOnOffAddress.toString())
            accelZOnOff = val;
        else if (address == gyroXOnOffAddress.toString())
            gyroXOnOff = val;
        else if (address == gyroYOnOffAddress.toString())
            gyroYOnOff = val;
        else if (address == gyroZOnOffAddress.toString())
            gyroZOnOff = val;
        else if (address == touchXOnOffAddress.toString())
            touchXOnOff = val;
        else if (address == touchYOnOffAddress.toString())
            touchYOnOff = val;
        else if (address == touchZOnOffAddress.toString())
            touchZOnOff = val;
        else if (address == distanceOnOffAddress.toString())
            distanceOnOff = val;
        else if (address == filterTypeAddress.toString())
            filtType = val;
        else if (address == filterPoleAddress.toString())
            filtPole = val;
        else if (address == encoder1Address.toString())
            encoder1 = val;
        else if (address == encoder2Address.toString())
            encoder2 = val;
        else if (address == encButton1Address.toString())
            encButton1 = val;
        else if (address == encButton2Address.toString())
            encButton2 = val;
    }
}


/// Returns float value of Accelerometer X Axis
float OSCHandler::getAccelX()
{
    return accelXVal;
}

/// Returns float value of Accelerometer Y Axis
float OSCHandler::getAccelY()
{
    return accelYVal;
}

/// Returns float value of Accelerometer Z Axis
float OSCHandler::getAccelZ()
{
    return accelZVal;
}

/// Returns float value of Gyroscope X Axis
float OSCHandler::getGyroX()
{
    return gyroXVal;
}

/// Returns float value of Gyroscope Y Axis
float OSCHandler::getGyroY()
{
    return gyroYVal;
}

/// Returns float value of Gyroscope Z Axis
float OSCHandler::getGyroZ()
{
    return gyroZVal;
}

/// Returns float value of Distance Meter
float OSCHandler::getDistance()
{
    return distVal;
}

/// Returns float value of Toucscreen X Axis
float OSCHandler::getTouchX()
{
    return touchXVal;
}

/// Returns float value of Touchscreen Y Axis
float OSCHandler::getTouchY()
{
    return touchYVal;
}

/// Returns float value of Touchscreen Z Axis (Pressure)
float OSCHandler::getTouchZ()
{
    return touchZVal;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
float OSCHandler::getAccelXOnOff()
{
    return accelXOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
float OSCHandler::getAccelYOnOff()
{
    return accelYOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
float OSCHandler::getAccelZOnOff()
{
    return accelZOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
float OSCHandler::getGyroXOnOff()
{
    return gyroXOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
float OSCHandler::getGyroYOnOff()
{
    return gyroYOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
float OSCHandler::getGyroZOnOff()
{
    return gyroZOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
float OSCHandler::getTouchXOnOff()
{
    return touchXOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
float OSCHandler::getTouchYOnOff()
{
    return touchYOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
float OSCHandler::getTouchZOnOff()
{
    return touchZOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
float OSCHandler::getDistanceOnOff()
{
    return distanceOnOff;
}

/// Returns rotational value of encoder1
float OSCHandler::getEncoder1()
{
    return encoder1;
}

/// Returns rotational value of encoder2
float OSCHandler::getEncoder2()
{
    return encoder2;
}

/// Returns encButton1 value 0 to 6
float OSCHandler::getEncButton1()
{
    return encButton1;
}

/// Returns encButton2 value 0 to 2
float OSCHandler::getEncButton2()
{
    return encButton2;
}

/// Returns filter type 0 = LPF; 1 = BPF; 2 = HPF
float OSCHandler::getFiltType()
{
    return filtType;
}

/// Returns filter pole value -1 or 1
float OSCHandler::getFiltPole()
{
    return filtPole;
}
