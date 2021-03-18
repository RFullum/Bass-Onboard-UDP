/*
  ==============================================================================

    OSCHandling.h
    Created: 17 Mar 2021 11:13:21pm
    Author:  Robert Fullum

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OSCHandler : private OSCReceiver,
                   private OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback>,
                   private OSCReceiver::Listener<juce::OSCReceiver::MessageLoopCallback>
{
public:
    OSCHandler();
    ~OSCHandler();
    
    /// Returns float value of Accelerometer X Axis
    float getAccelX();
    
    /// Returns float value of Accelerometer Y Axis
    float getAccelY();
    
    /// Returns float value of Accelerometer Z Axis
    float getAccelZ();
    
    /// Returns float value of Gyroscope X Axis
    float getGyroX();
    
    /// Returns float value of Gyroscope Y Axis
    float getGyroY();
    
    /// Returns float value of Gyroscope Z Axis
    float getGyroZ();
    
    /// Returns float value of Distance Meter
    float getDistance();
    
    /// Returns float value of Toucscreen X Axis
    float getTouchX();
    
    /// Returns float value of Touchscreen Y Axis
    float getTouchY();
    
    /// Returns float value of Touchscreen Z Axis (Pressure)
    float getTouchZ();
    
private:
    /// Automatically called when an OSC Message comes in. Checks the address, and sets appropriate variable with incoming float
    void oscMessageReceived (const OSCMessage& message) override;

    /**
     Automatically called when OSC Bundle comes in, splits bundle into its elements, and calls either
     oscMessageReceived or oscBundleReceived depending on which type the element of the bundle is
     */
    void oscBundleReceived (const OSCBundle& bundle) override;
    
    OSCAddress accelXAddress;
    OSCAddress accelYAddress;
    OSCAddress accelZAddress;
    
    OSCAddress gyroXAddress;
    OSCAddress gyroYAddress;
    OSCAddress gyroZAddress;
    
    OSCAddress distanceAddress;
    
    OSCAddress touchXAddress;
    OSCAddress touchYAddress;
    OSCAddress touchZAddress;
    
    float accelXVal;
    float accelYVal;
    float accelZVal;
    
    float gyroXVal;
    float gyroYVal;
    float gyroZVal;
    
    float distVal;
    
    float touchXVal;
    float touchYVal;
    float touchZVal;
};
