/******************************************************************************
SparkFun_Qwiic_Digital_Capacitor.h
SparkFun Qwiic Digital Capacitor Arduino Library Header File
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

#ifndef SparkFun_Qwiic_Digital_Capacitor_h
#define SparkFun_Qwiic_Digital_Capacitor_h

#define DIGITAL_CAPACITOR_ADDR_DEFAULT 0x60 //The I2C address is not adjustable                           \
                                            //But the user can have up to 2 digital capacitors on one bus \
                                            //The NCD2400M1 has an alternative address of 0x61

#include <Wire.h>
#include <Arduino.h>

class digitalCap
{
private:
    TwoWire *_i2cPort;      //generic connection to user's chosen I2C port
    uint8_t _deviceAddress; //I2C address of the digital capacitor

public:
    //Device status
    bool begin(uint8_t address = DIGITAL_CAPACITOR_ADDR_DEFAULT, TwoWire &wirePort = Wire); //Sets device I2C address to default over whatever port the user specifies
    bool isConnected();                                                                     //Returns true if the digital capacitor acknowledges over I2C

    //Digital capacitor functions
    bool setCapacitance(float capacitance, bool nvm = true); //Function to set the capacitance of the digital capacitor. Parameters are described below:
                                                             //capacitance = float in pF
                                                             //nvm = 1 for non-volatile memory write, default
                                                             //nvm = 0 for volatile memory write
    float getCapacitance(bool nvm = true);                   //Function returns float of the current capacitance. Parameters are described below:
                                                             //nvm = 1 for returning capacitance in non-volatile memory, default
                                                             //nvm = 0 for returning capacitance in volatile memory

    void shuntConfigMode();
    void seriesConfigMode();

    //Helper functions
    //Capacitance code conversion functions
    uint16_t calculateShuntCode(float capacitance);  //From datasheet pg 6
    uint16_t calculateSeriesCode(float capacitance); //From datasheet pg 6
    float calculateShuntCapacitance(uint16_t code);  //From datasheet pg 6
    float calculateSeriesCapacitance(uint16_t code); //From datasheet pg 6

    uint32_t readRegisters(); //Returns the 4 bytes of NCD2400M memory, datasheet pg 17

    //Volatile memory functions
    bool writeVolatileCapacitance(uint16_t code); //Writes the calculated capacitance code to the volatile memory registers
    uint16_t readVolatileCapacitance();           //Returns the capacitance code in the volatile memory registers

    //Non-volatile memory functions
    bool writeNVM(uint8_t byte1, uint8_t byte2);     //Writes the two config bytes and don't cares to fulfill programming time of NVM write
    bool eraseNonVolatileRegisters();                //Clears the non-volatile memory registers. This is needed before performing a NVM write
    bool writeNonVolatileCapacitance(uint16_t code); //Writes the calculated capacitance code to the non-volatile memory registers
    uint16_t readNonVolatileCapacitance();           //Returns the capacitance code in the non-volatile memory registers
    bool setNonVolatileMode();                       //Set the non-volatile memory mode operation. This is the default mode,
                                                     //but this function needs to be called if a write to volatile memory has occured.
};

#endif