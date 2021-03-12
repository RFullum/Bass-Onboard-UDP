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

#pragma once

#include <JuceHeader.h>
#include "UDPConnection.h"

//==============================================================================
/*
*/
class Accelerometer
{
public:
    Accelerometer();
    ~Accelerometer();

    
    /**
    Sets the Accelerometer axes' values
    Call in timerCallback()
    */
    void setAccelValue();
    
    /// Returns the X value from the Accelerometer, -4.0f to 4.0f
    float getAccelX();
    
    /// Returns the Y value from the Accelerometer, -4.0f to 4.0f
    float getAccelY();
    
    /// Returns the Z value from the Accelerometer, -4.0f to 4.0f
    float getAccelZ();

private:
    // UDP Connection
    UDPConnection udpX;
    UDPConnection udpY;
    UDPConnection udpZ;
    
    // Member Variables
    float smoothingFactor;
    float smoothValX;
    float smoothValY;
    float smoothValZ;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Accelerometer)
};



//==============================================================================
//==============================================================================
//==============================================================================

class Gyroscope
{
public:
    Gyroscope();
    ~Gyroscope();
    
    /**
    Takes a String in the form of "[TAG][VALUE]\n". Uses the TAG to route the value to the appropriate variable and converts to float
    Call in timerCallback()
    */
    void setGyroValue();
    
    /// Returns Gyroscope's X value -2000.0f to 2000.0f
    float getGyroX();
    
    /// Returns Gyroscope's Y value -2000.0f to 2000.0f
    float getGyroY();
    
    /// Returns Gyroscope's Z value -2000.0f to 2000.0f
    float getGyroZ();
    
private:
    // UDP Connection
    UDPConnection udpX;
    UDPConnection udpY;
    UDPConnection udpZ;
    
    // Member variables
    float smoothingFactor;
    float smoothGyroX;
    float smoothGyroY;
    float smoothGyroZ;
};
