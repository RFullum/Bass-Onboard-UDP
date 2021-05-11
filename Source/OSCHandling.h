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
    
    OSCAddress accelXOnOffAddress;
    OSCAddress accelYOnOffAddress;
    OSCAddress accelZOnOffAddress;
    
    OSCAddress gyroXOnOffAddress;
    OSCAddress gyroYOnOffAddress;
    OSCAddress gyroZOnOffAddress;
    
    OSCAddress touchXOnOffAddress;
    OSCAddress touchYOnOffAddress;
    OSCAddress touchZOnOffAddress;
    
    OSCAddress distanceOnOffAddress;
    
    OSCAddress filterTypeAddress;
    OSCAddress filterPoleAddress;
    
    OSCAddress encoder1Address;
    OSCAddress encoder2Address;
    
    OSCAddress encButton1Address;
    OSCAddress encButton2Address;
    
    
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
