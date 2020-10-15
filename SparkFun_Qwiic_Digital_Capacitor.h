#ifndef SparkFun_Qwiic_Digital_Capacitor_h
#define SparkFun_Qwiic_Digital_Capacitor_h

#define DIGITAL_CAPACITOR_ADDR_DEFAULT 0x60

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
    uint32_t readRegisters();

    uint16_t calculateShuntCode(float capacitance);
    uint16_t calculateSeriesCode(float capacitance);
    float calculateShuntCapacitance(uint16_t code);
    float calculateSeriesCapacitance(uint16_t code);

    bool writeVolatileCapacitance(uint16_t code);
    uint16_t readVolatileCapacitance();

    bool eraseNonVolatileRegisters();
    bool writeNonVolatileCapacitance(uint16_t code);
    uint16_t readNonVolatileCapacitance();
    bool setNonVolatileMode();
};

#endif