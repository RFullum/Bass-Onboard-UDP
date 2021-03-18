/*
  ==============================================================================

    Accelerometer.h
    Created: 28 Feb 2021 4:09:45pm
    Author:  Robert Fullum
    
    
    The Accelerometer values are from -4.0f to 4.0f, with gravity being 1.0f.
    Values between -1.0f and 1.0f are the orientation. Greater than 1.0f or less than -1.0f
    are movements that more Gs than gravity, for instance, stopping suddenly from speed.
    The Gyroscope measures directions of movement, range -2000.0f to 2000.0f.
 
 
  ==============================================================================
*/

#include <JuceHeader.h>
#include "Accelerometer.h"

//==============================================================================
Accelerometer::Accelerometer() :
    smoothingFactor(0.4f),
    smoothVal(0.0f)
{}

Accelerometer::~Accelerometer() {}


/*
 * These classes need to be updated to use OSCHandler class instead of UDPConnection
 * They need to know if the player has the sensor on or off.
 * If the sensor is on, they need to get the appropriate value from the OSCHandler.
 * They need to know wha parameter they're being mapped to and value-map the sensor values
 *      to the parameter's range.
 * They need to smooth the values.
 * They need to set the parameter value.
 */






//==============================================================================
//==============================================================================
//==============================================================================



Gyroscope::Gyroscope() : smoothingFactor(0.4f), smoothGyroX(0.0f), smoothGyroY(0.0f), smoothGyroZ(0.0f)
{
    udpX.setPortBind ( 65019 );
    udpY.setPortBind ( 65020 );
    udpZ.setPortBind ( 65021 );
}

Gyroscope::~Gyroscope() {}


/**
Sets the gyroscope's axes' valuess
Call in timerCallback()
*/
void Gyroscope::setGyroValue()
{
    float gyroX = 0.0f;
    float gyroY = 0.0f;
    float gyroZ = 0.0f;
    
    gyroX = udpX.getSensorValue();
    gyroY = udpY.getSensorValue();
    gyroZ = udpZ.getSensorValue();
    
    smoothGyroX += ( gyroX - smoothGyroX ) * smoothingFactor;
    smoothGyroY += ( gyroY - smoothGyroY ) * smoothingFactor;
    smoothGyroZ += ( gyroZ - smoothGyroZ ) * smoothingFactor;
}

/// Returns Gyroscope's X value -2000.0f to 2000.0f
float Gyroscope::getGyroX()
{
    return smoothGyroX;
}

/// Returns Gyroscope's Y value -2000.0f to 2000.0f
float Gyroscope::getGyroY()
{
    return smoothGyroY;
}

/// Returns Gyroscope's Z value -2000.0f to 2000.0f
float Gyroscope::getGyroZ()
{
    return smoothGyroZ;
}
