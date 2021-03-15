/*
  ==============================================================================

    UDPConnection.cpp
    Created: 11 Mar 2021 4:11:44pm
    Author:  Robert Fullum

  ==============================================================================
*/

#include "UDPConnection.h"


UDPConnection::UDPConnection() {}

UDPConnection::~UDPConnection()
{
    delete[] udpInBuffer;
}


/// Sets the port binding for the UDP socket
void UDPConnection::setPortBind(int portNum)
{
    udp.bindToPort( portNum );
}


/// Reads the value from the UDP socket and converts to and returns float value. Call this from timerCallback()
float UDPConnection::getSensorValue()
{
    udp.read ( udpInBuffer, 4, false );
    
    float* floatBuffer = (float*)udpInBuffer;
    
    return *floatBuffer;
}



// ====================================================================================
// ====================================================================================
// ====================================================================================


UDPSend::UDPSend()
{
    udp.bindToPort ( 2390 );    // Arduino's localPort
}


UDPSend::~UDPSend()
{
    //if (*sendByte)
    //    delete[] sendByte;
}

void UDPSend::callArduinoUDP()
{
    String arduinoName = "ArduinoSensors";
    bool callArduino   = true;
    sendByte           = (unsigned char*)callArduino;
    
    if ( udp.waitUntilReady ( false, 60 ) )
         udp.write( arduinoName, udp.getBoundPort(), sendByte, 1 );
}
