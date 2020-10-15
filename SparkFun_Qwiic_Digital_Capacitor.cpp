#include "SparkFun_Qwiic_Digital_Capacitor.h"

//Initialize the I2C port
bool digitalCap::begin(uint8_t address, TwoWire &wirePort)
{
    _deviceAddress = address; //grab the address that the IC is on
    _i2cPort = &wirePort;     //grab whichever port the user wants to use

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

//For now, assume the user enters two bytes
bool digitalCap::writeVolatileCapacitance(uint16_t capacitance)
{
    uint8_t byte2 = capacitance | 0xFF00; //LSB
    uint8_t byte1 = capacitance | 0x00FF; //MSB
    byte1 = byte1 >> 7;

    // Serial.println(byte1, HEX);
    // Serial.println(byte2, HEX);

    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(byte1);
    _i2cPort->write(byte2);
    if (_i2cPort->endTransmission() == 0)
    {
        // Serial.println("Write successful!");
        return true;
    }
    return false;
}

uint32_t digitalCap::readRegisters()
{
    uint8_t count = 3;
    uint32_t readBytes = 0;
    _i2cPort->requestFrom(_deviceAddress, 4);
    while (_i2cPort->available())
    {
        readBytes = readBytes | (_i2cPort->read() << count * 8); //Concatenate all four bytes into one number, datasheet pg 17
        count--;
    }
    // Serial.println(readBytes, HEX);
    return readBytes;
}

uint16_t digitalCap::readVolatileCapacitance()
{
    uint32_t readBytes = readRegisters();
    readBytes = readBytes & ~0xFE00FFFF;
    uint16_t volCap = readBytes >> 16;
    // Serial.print("0x");
    // Serial.println(volCap, HEX);
    return volCap;
}

//TODO: have not tested yet...
bool digitalCap::setNonVolatileMode()
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(0x90); //From datasheet, pg 18
    _i2cPort->write(0xFF); //These are don't care bits
    if (_i2cPort->endTransmission() == 0)
        return true;
    return false;
}