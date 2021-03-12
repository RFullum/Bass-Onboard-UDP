/*
  ==============================================================================

    DistanceMeter.h
    Created: 28 Feb 2021 4:33:09pm
    Author:  Robert Fullum
    
    Written for Adafruit Time of Flight Distance Sensor VL53L0X ->
    Arduino Nano 33 IoT ->
    Serial Connection via USB Port
 
    Used in conjunction with UDPConnection class, it takes the data from the
    UDP Socket and returns the distance as a float in millimeters.
    The sensor is rated from 50mm to 1200mm, but can get shorter or further
    readings depending on ambient light and reflectivity of obect being sensed.
    Readings out of range return 0.0f;
 
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "UDPConnection.h"

//==============================================================================
/*
*/

class DistanceMeter
{
public:
    DistanceMeter();
    ~DistanceMeter();
    
    /**
    Sets the distance value from the sensor
    Call in timerCallback()
    */
    void setDistanceValue();
    
    /// Returns float of distance in millimeters
    float getDistanceValue();
    
    
private:
    // UDP Conection
    UDPConnection udp;
    
    // Member Variables
    float smoothingFactor;
    float smoothedDistVal;
};
