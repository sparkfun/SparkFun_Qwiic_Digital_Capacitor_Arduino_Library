/******************************************************************************
SparkFun_Qwiic_Digital_Capacitor.cpp
SparkFun Qwiic Digital Capacitor Arduino Library Source File
Priyanka Makin @ SparkFun Electronics
Original Creation Date: October 22, 2020
https://github.com/sparkfun/SparkFun_Qwiic_Digital_Capacitor_Arduino_Library

Pickup a board here: https://www.sparkfun.com/products/17182

This file prototypes the digitalCap class, implemented in SparkFun_Qwiic_Digital_Capacitor.cpp.
Based on the user's input, there are 9 bits that are set in the NCD2400M memory to determine the 
output capacitance. In setting the capacitance, there are a few things to be mindful of:
First, the configuration of the capacitor. Is the capacitor in series (CP and CN inserted in series 
into a system) or shunt configuration (connecting CN to GND)? It's necessary to distinguish between
the two because they have different conversion equations to their 9-bit code.
Second, the user can determine wheter the capacitance change can be made to volatile (not retained 
after power cycle) or non-volatile memory (retained after power cycle). The non-volatile memory write
is the default but takes more total I2C write commands.
The range of the capacitor is:
    Shunt = 12.5 pF to 194 pF in discrete 355 fF steps
    Series = 1.7 pF to 194 pF in discrete 376 fF steps

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno
	Qwiic Digital Capacitor Breakout Version: x02

This code is lemonade-ware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include "SparkFun_Qwiic_Digital_Capacitor.h"

bool config = 1; //Global bool to define the configuration of the capacitor \
                    //1 = shunt configuration (CN connected to GND)            \
                    //2 = series configuration (capacitor in series with system)

//Initialize the I2C port
bool digitalCap::begin(uint8_t address, TwoWire &wirePort)
{
    _deviceAddress = address; //grab the address that the IC is on
    _i2cPort = &wirePort;     //grab whichever port the user wants to use

    //return true if the device is connected
    return (isConnected());
}

//Check that the NCD2400M acknowledges on the I2C bus
bool digitalCap::isConnected()
{
    _i2cPort->beginTransmission(_deviceAddress);
    if (_i2cPort->endTransmission() == 0)
        return true;
    return false;
}

//This function sets the non-volatile capacitance by default
//capacitance float in pF
//nvm = 1 for non-volatile memory write, default
//nvm = 0 for volatile memory write
bool digitalCap::setCapacitance(float capacitance, bool nvm)
{
    //First, figure out the code to write
    uint16_t code;
    if (config == true)
    {
        code = calculateShuntCode(capacitance);
    }
    else
    {
        code = calculateSeriesCode(capacitance);
    }

    //Next, figure out what kind of write needs to be made
    if (nvm == true)
    {
        eraseNonVolatileRegisters();
        writeNonVolatileCapacitance(code);
        return setNonVolatileMode();
    }
    else
    { //Do a write to volatile registers
        return writeVolatileCapacitance(code);
    }
}

//This function returns the capacitance, converted to pF, in the non-volatile register by default
//nvm = 1 for non-volatile memory write, default
//nvm = 0 for volatile memory write
float digitalCap::getCapacitance(bool nvm)
{
    //First, read the capacitance code from the register
    uint16_t code;
    if (nvm == true)
    {
        code = readNonVolatileCapacitance();
    }
    else
    {
        code = readVolatileCapacitance();
    }

    float cap;
    if (config == true)
    {
        return cap = calculateShuntCapacitance(code);
    }
    else
    {
        return cap = calculateSeriesCapacitance(code);
    }
}

//Set to shunt configuration mode
void digitalCap::shuntConfigMode()
{
    config = 1;
}

//Set to series configuration mode
void digitalCap::seriesConfigMode()
{
    config = 0;
}

//Returns the 4 bytes of NCD2400M memory, datasheet pg 17
uint32_t digitalCap::readRegisters()
{
    uint8_t count = 3;
    uint32_t readBytes = 0;
    _i2cPort->requestFrom(_deviceAddress, 4);
    while (_i2cPort->available())
    {
        uint32_t temp = _i2cPort->read();
        temp = temp << count * 8;
        readBytes = readBytes | temp; //Concatenate all four bytes into one number, datasheet pg 17
        count--;
    }

    // Serial.println(readBytes, HEX);
    return readBytes;
}

//Returns the capacitance code for shunt configuration
//"capacitance" parameter is in pF
//Capacitance must be between 12.5 and 194 pF
uint16_t digitalCap::calculateShuntCode(float capacitance)
{
    //Edge-case guards
    if (capacitance > 194)
        capacitance = 194;
    else if (capacitance < 12.5)
        capacitance = 12.5;

    float code = (capacitance - 12.5) / 0.355; //Eq from datasheet pg 6
    // Serial.println((uint16_t)code);
    return (uint16_t)code;
}

//Returns the capacitance code for series configuration
//"capacitance" parameter is in pF
//Capacitance must be between 1.7 and 194 pF
uint16_t digitalCap::calculateSeriesCode(float capacitance)
{
    //Edge-case guards
    if (capacitance > 194)
        capacitance = 194;
    else if (capacitance < 1.7)
        capacitance = 1.7;

    float code = (capacitance - 1.7) / 0.376; //Eq from datasheet pg 6
    // Serial.println((uint16_t)code);
    return (uint16_t)code;
}

//Converts the shunt capacitance code to a capacitance in pF
//Code must be between 0 and 511 (9-bits max)
float digitalCap::calculateShuntCapacitance(uint16_t code)
{
    //Edge-case guards
    if (code > 511)
        code = 511;

    float capacitance = 12.5 + 0.355 * code; //Eq from datasheet pg 6
    // Serial.println(capacitance);
    return capacitance;
}

//Converts the series capacitance code to a capacitance in pF
//Code must be between 0 and 511 (9-bits max)
float digitalCap::calculateSeriesCapacitance(uint16_t code)
{
    //Edge-case guards
    if (code > 511)
        code = 511;

    float capacitance = 1.7 + 0.376 * code; //Eq from datasheet pg 6
    // Serial.println(capacitance);
    return capacitance;
}

//Writes the capacitance code to the volatile registers
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
        return true; //Success
    }
    return false;
}

//Reads and returns the capacitance code in the volatile registers
uint16_t digitalCap::readVolatileCapacitance()
{
    uint32_t readBytes = readRegisters();
    readBytes = readBytes & ~0xFE00FFFF;
    uint16_t volCap = readBytes >> 16;

    // Serial.print("0x");
    // Serial.println(volCap, HEX);
    return volCap;
}

//Writes the two config bytes and don't care to fulfill programming time of a NVM write
bool digitalCap::writeNVM(uint8_t byte1, uint8_t byte2)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(byte1);
    _i2cPort->write(byte2);

    //Datasheet pg 19 mentions the non-volatile programming time is 5 ms
    //Write don't care values, i2c buffer is only 32 bytes long
    //This write takes 2.8ms to complete on ATMega328
    for (uint8_t i = 0; i < 30; i++)
    {
        _i2cPort->write(0xFF);
    }

    if (_i2cPort->endTransmission() != 0)
        return false;

    //This delay is to round out the overall delay to 5 ms
    delay(3);
    return true;
}

//Clears the non-volatile memory registers. This is needed before performing a NVM write
bool digitalCap::eraseNonVolatileRegisters()
{
    if (writeNVM(0xC0, 0x00) == false)
        return false;
    if (writeNVM(0xA0, 0x00) == false)
        return false;
    if (writeNVM(0x80, 0x00) == false)
        return false;

    return true; //Success
}

//Writes the capacitance code to the non-volatile memory registers
bool digitalCap::writeNonVolatileCapacitance(uint16_t code)
{
    if (code > 511)
        code = 511;
    if (code < 0)
        code = 0;

    //First, separate into bytes
    uint16_t byte2 = code & ~0xFF00;      //LSB
    uint16_t byte1 = (code & ~0xFF) >> 8; //MSB

    //Then, separate into nibbles
    uint8_t nib3 = (uint8_t)byte2 & ~0xF0;
    uint8_t nib2 = ((uint8_t)byte2 & ~0x0F) >> 4;
    uint8_t nib1 = byte1;

    // Serial.println(nib1, HEX);
    // Serial.println(nib2, HEX);
    // Serial.println(nib3, HEX);

    //Now, write one nibble at a time
    if (writeNVM(0xC0, nib1) == false)
        return false;
    if (writeNVM(0xA0, nib2) == false)
        return false;
    if (writeNVM(0x80, nib3) == false)
        return false;

    return true; //Success
}

//Reads and returns the capacitance code in the non-volatile memory registers
uint16_t digitalCap::readNonVolatileCapacitance()
{
    uint32_t readBytes = readRegisters();
    readBytes = readBytes & ~0xFFFFFE00;
    uint16_t nonVolCap = readBytes;

    // Serial.print("0x");
    // Serial.println(nonVolCap, HEX);
    return nonVolCap;
}

//Set the non-volatile memory mode operation. This is the default mode but
//this function needs to be called if a write to volatile memory has occured.
bool digitalCap::setNonVolatileMode()
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(0x90); //From datasheet, pg 18
    _i2cPort->write(0xFF); //These are don't care bits
    if (_i2cPort->endTransmission() == 0)
        return true; //Success
    return false;
}