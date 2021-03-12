/*
  ==============================================================================

    UDPConnection.h
    Created: 11 Mar 2021 4:11:44pm
    Author:  Robert Fullum

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

class UDPConnection
{
public:
    UDPConnection();
    ~UDPConnection();
    
    /// Sets the port binding for the UDP socket
    void setPortBind(int portNum);
    
    /// Reads the value from the UDP socket and converts to and returns float value. Call this from timerCallback()
    float getSensorValue();
    
private:
    // UDP Connections
    DatagramSocket udp;
    
    // Member Variables
    unsigned char* udpInBuffer = new unsigned char[4];
};
