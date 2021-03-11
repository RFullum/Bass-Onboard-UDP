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
    
    
    
    
private:
    DatagramSocket imuUDP;
};
