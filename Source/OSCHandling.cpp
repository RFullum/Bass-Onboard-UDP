/*
  ==============================================================================

    OSCHandling.cpp
    Created: 17 Mar 2021 11:13:21pm
    Author:  Robert Fullum

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

    touchXOnOffAddress("/juce/touchXOnOff"),
    touchYOnOffAddress("/juce/touchYOnOff"),
    touchZOnOffAddress("/juce/touchZOnOff"),

    distanceOnOffAddress("/juce/distanceOnOff"),

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

    accelXOnOff(false),
    accelYOnOff(false),
    accelZOnOff(false),
    
    gyroXOnOff(false),
    gyroYOnOff(false),
    gyroZOnOff(false),

    touchXOnOff(false),
    touchYOnOff(false),
    touchZOnOff(false),

    distanceOnOff(false)

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
    }
    
    DBG(accelXVal);
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
int OSCHandler::getAccelXOnOff()
{
    return (int)accelXOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
int OSCHandler::getAccelYOnOff()
{
    return (int)accelYOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
int OSCHandler::getAccelZOnOff()
{
    return (int)accelZOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
int OSCHandler::getGyroXOnOff()
{
    return (int)gyroXOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
int OSCHandler::getGyroYOnOff()
{
    return (int)gyroYOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
int OSCHandler::getGyroZOnOff()
{
    return (int)gyroZOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
int OSCHandler::getTouchXOnOff()
{
    return (int)touchXOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
int OSCHandler::getTouchYOnOff()
{
    return (int)touchYOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
int OSCHandler::getTouchZOnOff()
{
    return (int)touchZOnOff;
}

/// Returns 0 or 1 if this sensor is Off or On respectively
int OSCHandler::getDistanceOnOff()
{
    return (int)distanceOnOff;
}
