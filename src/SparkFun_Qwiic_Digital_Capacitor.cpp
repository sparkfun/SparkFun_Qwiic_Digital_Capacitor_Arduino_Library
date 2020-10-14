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

bool digitalCap::writeVolatileCapacitance(uint8_t capacitance)
{
    //TODO: figure out how to separate bytes
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(byte2);
    _i2cPort->write(byte1);
    if (_i2cPort->endTransmission() == 0)
        return true;
    return false;
}

uint8_t digitalCap::readVolatileCapacitance()
{
    uint8_t temp = 0;
    uint8_t count = 0;
    _i2cPort->requestFrom(_deviceAddress, 4);
    while(_i2cPort->available())
    {
        temp = temp | (_i2cPort->read() << count*8);    //Try to concatenate all four bytes into one number, datasheet pg 17
        count++;
    }

}

bool digitalCap::setNonVolatileMode()
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(0x90);  //From datasheet, pg 18
    _i2cPort->write(0xFF);  //These are don't care bits
    if (_i2cPort->endTransmission() == 0)
        return true;
    return false;
}