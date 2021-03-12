/*
  ==============================================================================

    Accelerometer.h
    Created: 28 Feb 2021 4:09:45pm
    Author:  Robert Fullum
    
    
    Written for Arduino Nano 33 IoT internal IMU Accelerometer and Gyroscope ->
    UDP Socket
 
    Used in conjunction with UDPConnection class, it takes the data from the
    UDP Sockets. The Accelerometer values are from -4.0f to 4.0f, with gravity being 1.0f.
    Values between -1.0f and 1.0f are the orientation. Greater than 1.0f or less than -1.0f
    are movements that more Gs than gravity, for instance, stopping suddenly from speed.
    The Gyroscope measures directions of movement, range -2000.0f to 2000.0f.
 
 
  ==============================================================================
*/

#include <JuceHeader.h>
#include "Accelerometer.h"

//==============================================================================
Accelerometer::Accelerometer() : smoothingFactor(0.4f), smoothValX(0.0f), smoothValY(0.0f), smoothValZ(0.0f)
{
    udpX.setPortBind ( 65016 );
    udpY.setPortBind ( 65017 );
    udpZ.setPortBind ( 65018 );
}

Accelerometer::~Accelerometer() {}


/**
Sets acceleromteter axes' values
Call in timerCallback()
*/
void Accelerometer::setAccelValue()
{
    float accelX = 0.0f;
    float accelY = 0.0f;
    float accelZ = 0.0f;
    
    accelX = udpX.getSensorValue();
    accelY = udpY.getSensorValue();
    accelZ = udpZ.getSensorValue();
    
    smoothValX += ( accelX - smoothValX ) * smoothingFactor;
    smoothValY += ( accelY - smoothValY ) * smoothingFactor;
    smoothValZ += ( accelZ - smoothValZ ) * smoothingFactor;
}


/// Returns the X value from the Accelerometer, -4.0f to 4.0f
float Accelerometer::getAccelX()
{
    return smoothValX;
}

/// Returns the Y value from the Accelerometer, -4.0f to 4.0f
float Accelerometer::getAccelY()
{
    return smoothValY;
}

/// Returns the Z value from the Accelerometer, -4.0f to 4.0f
float Accelerometer::getAccelZ()
{
    return smoothValZ;
}




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
