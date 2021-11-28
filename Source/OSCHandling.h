/*
  ==============================================================================

    OSCHandling.h
    Created: 17 Mar 2021 11:13:21pm
    Author:  Robert Fullum
 
 Arduino Pinout Connections:
 
    Touchscreen -X to A3
    Touchscreen +Y to A2
    Touchscreen +X to D9
    Touchscreen -Y to D8
    
    Distance VIN to 3.3V
    Distance GND to GND (Between VIN and RST)
    Distance SCL to A5
    Distance SDA to A4
 
    Sensor Button Board:
    Ground to GND
    Data to A0
    VIn to 3.3V
 
    Filter Button Board:
    Ground to GND
    Data to A1
    VIn to 3.3V

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OSCHandler : private juce::OSCReceiver,
                   private juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::MessageLoopCallback>,
                   private juce::OSCReceiver::Listener<juce::OSCReceiver::MessageLoopCallback>
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
    
    /// Returns 0 or 1 if this sensor is Off or On respectively
    float getAccelXOnOff();
    
    /// Returns 0 or 1 if this sensor is Off or On respectively
    float getAccelYOnOff();
    
    /// Returns 0 or 1 if this sensor is Off or On respectively
    float getAccelZOnOff();
    
    /// Returns 0 or 1 if this sensor is Off or On respectively
    float getGyroXOnOff();
    
    /// Returns 0 or 1 if this sensor is Off or On respectively
    float getGyroYOnOff();
    
    /// Returns 0 or 1 if this sensor is Off or On respectively
    float getGyroZOnOff();
    
    /// Returns 0 or 1 if this sensor is Off or On respectively
    float getTouchXOnOff();
    
    /// Returns 0 or 1 if this sensor is Off or On respectively
    float getTouchYOnOff();
    
    /// Returns 0 or 1 if this sensor is Off or On respectively
    float getTouchZOnOff();
    
    /// Returns 0 or 1 if this sensor is Off or On respectively
    float getDistanceOnOff();
    
    /// Returns rotational value of encoder1
    float getEncoder1();
    
    /// Returns rotational value of encoder2
    float getEncoder2();
    
    /// Returns encButton1 value 0 to 6
    float getEncButton1();
    
    /// Returns encButton2 value 0 to 2
    float getEncButton2();
    
    /// Returns filter type 0 = LPF; 1 = BPF; 2 = HPF
    float getFiltType();
    
    /// Returns filter pole value -1 or 1
    float getFiltPole();
    
    /// Calls arduino for OSC Bundle
    void callArduino();
    
private:
    /// Automatically called when an OSC Message comes in. Checks the address, and sets appropriate variable with incoming float
    void oscMessageReceived (const juce::OSCMessage& message) override;

    /**
     Automatically called when OSC Bundle comes in, splits bundle into its elements, and calls either
     oscMessageReceived or oscBundleReceived depending on which type the element of the bundle is
     */
    void oscBundleReceived (const juce::OSCBundle& bundle) override;
    
    juce::OSCAddress accelXAddress;
    juce::OSCAddress accelYAddress;
    juce::OSCAddress accelZAddress;
    
    juce::OSCAddress gyroXAddress;
    juce::OSCAddress gyroYAddress;
    juce::OSCAddress gyroZAddress;
    
    juce::OSCAddress distanceAddress;
    
    juce::OSCAddress touchXAddress;
    juce::OSCAddress touchYAddress;
    juce::OSCAddress touchZAddress;
    
    juce::OSCAddress accelXOnOffAddress;
    juce::OSCAddress accelYOnOffAddress;
    juce::OSCAddress accelZOnOffAddress;
    
    juce::OSCAddress gyroXOnOffAddress;
    juce::OSCAddress gyroYOnOffAddress;
    juce::OSCAddress gyroZOnOffAddress;
    
    juce::OSCAddress touchXOnOffAddress;
    juce::OSCAddress touchYOnOffAddress;
    juce::OSCAddress touchZOnOffAddress;
    
    juce::OSCAddress distanceOnOffAddress;
    
    juce::OSCAddress filterTypeAddress;
    juce::OSCAddress filterPoleAddress;
    
    juce::OSCAddress encoder1Address;
    juce::OSCAddress encoder2Address;
    
    juce::OSCAddress encButton1Address;
    juce::OSCAddress encButton2Address;
    
    
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
    
    float accelXOnOff;
    float accelYOnOff;
    float accelZOnOff;
    
    float gyroXOnOff;
    float gyroYOnOff;
    float gyroZOnOff;
    
    float touchXOnOff;
    float touchYOnOff;
    float touchZOnOff;
    
    float distanceOnOff;
    
    float filtType;
    float filtPole;
    
    float encoder1;
    float encoder2;
    float encButton1;
    float encButton2;
};
