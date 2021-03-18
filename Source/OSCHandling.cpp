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

    accelXVal(0.0f),
    accelYVal(0.0f),
    accelZVal(0.0f),
    
    gyroXVal(0.0f),
    gyroYVal(0.0f),
    gyroZVal(0.0f),

    distVal(0.0f),

    touchXVal(0.0f),
    touchYVal(0.0f),
    touchZVal(0.0f)
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
    addListener ( this, distanceAddress );
    addListener ( this, touchXAddress   );
    addListener ( this, touchYAddress   );
    addListener ( this, touchZAddress   );
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
