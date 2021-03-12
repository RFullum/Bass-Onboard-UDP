/*
  ==============================================================================

    TouchScreen.cpp
    Created: 28 Feb 2021 5:05:52pm
    Author:  Robert Fullum
 
 
    Written for Adafruit 3.2" Resistive Touch Screen (part 333) ->
    Adafruit Dual 1.0mm 4 connector FPC Touchscreen Breakout ->
    Arduino Nano 33 IoT ->
    UDP Socket

    Used in conjunction with UDPConnection class, it takes the data from the
    UDP Socket, and returns the Touch Screen's X Coordinate, Y Coordinate,
    and Touch Pressure Value as floats between 0.0f and 1024.0f.

    Actual touch response tends to range 200.0f to 800.0f.
    Pressure readings are inverse: Lower is more pressure. Adjust as needed.

  ==============================================================================
*/

#include "TouchScreen.h"


TouchScreen::TouchScreen() : smoothingFactor(0.4f), smoothedValX(0.0f), smoothedValY(0.0f), smoothedValZ(0.0f)
{
    udpX.setPortBind ( 65024 );
    udpY.setPortBind ( 65025 );
    udpZ.setPortBind ( 65026 );
}

TouchScreen::~TouchScreen() {}


/**
 Sets the distance value from the sensor
 Call in timerCallback()
 */
void TouchScreen::setTouchScreenCoords()
{
    float xCoord = 0.0f;
    float yCoord = 0.0f;
    float zCoord = 0.0f;
    
    xCoord = udpX.getSensorValue();
    yCoord = udpY.getSensorValue();
    zCoord = udpZ.getSensorValue();
    
    smoothedValX += ( xCoord - smoothedValX ) * smoothingFactor;
    smoothedValY += ( yCoord - smoothedValY ) * smoothingFactor;
    smoothedValZ += ( zCoord - smoothedValZ ) * smoothingFactor;
}


/// Returns Touchscreen X Coordinate 0.0f to 1024 (useable surface approx 200.0f to 800.0f
float TouchScreen::getXCoord()
{
    return smoothedValX;
}

/// Returns Touchscreen Y Coordinate 0.0f to 1024 (useable surface approx 200.0f to 800.0f
float TouchScreen::getYCoord()
{
    return smoothedValY;
}

/// Returns Touchscreen Touch Pressure from 0.0f to 1024.0f (lower is more pressure)
float TouchScreen::getPressure()
{
    return smoothedValZ;
}
