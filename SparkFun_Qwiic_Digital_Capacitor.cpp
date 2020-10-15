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

//"capacitance" parameter is in pF
uint16_t digitalCap::calculateShuntCode(float capacitance)
{
    //Edge-case gaurds
    if (capacitance > 194)
        capacitance = 194;
    else if (capacitance < 12.5)
        capacitance = 12.5;

    float code = (capacitance - 12.5) / 0.355; //Eq from datasheet pg 6
    Serial.println((uint16_t)code);
    return (uint16_t)code;
}

//"capacitance" parameter is in pF
uint16_t digitalCap::calculateSeriesCode(float capacitance)
{
    //Edge-case gaurds
    if (capacitance > 194)
        capacitance = 194;
    else if (capacitance < 1.7)
        capacitance = 1.7;

    float code = (capacitance - 1.7) / 0.376; //Eq from datasheet pg 6
    Serial.println((uint16_t)code);
    return (uint16_t)code;
}

float digitalCap::calculateShuntCapacitance(uint16_t code)
{
    //Edge-case gaurds
    if (code > 511)
        code = 511;

    float capacitance = 12.5 + 0.355 * code;
    Serial.println(capacitance);
    return capacitance;
}

float digitalCap::calculateSeriesCapacitance(uint16_t code)
{
    //Edge-case gaurds
    if (code > 511)
        code = 511;

    float capacitance = 1.7 + 0.376 * code;
    Serial.println(capacitance);
    return capacitance;
}

//For now, assume the user enters two bytes
bool digitalCap::writeVolatileCapacitance(uint16_t code)
{
    uint16_t byte2 = code & ~0xFF00;        //LSB
    uint16_t byte1 = (code & ~0x00FF) >> 8; //MSB

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