#include "LIDARLite_v4LED.h"

//Initialize the I2C port
bool digitalCap::begin(uint8_t address, TwoWire &wirePort)
{
    _deviceAddress = address;   //grab the address that the IC is on
    _i2cPort = &wirePort;       //grab whichever port the user wants to use

    //return true if the device is connected
    return (isConnected());
}

bool digitalCap::isConnected()
{
    _i2cPort->beginTransmission(_deviceAddress);
    if (_i2cPort->endTransmission() == 0)
        return true;
    return false;
}