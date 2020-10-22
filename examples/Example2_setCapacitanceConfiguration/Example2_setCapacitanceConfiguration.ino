/**************************************************************************************
 * Set and get the capacitance of NCD2400M on the Qwiic Digital Capacitor breakout board
 * and the configuration of the capacitor. Series configuration is when the capacitor (CP
 * and CN) is connected in series within a system and shunt configuration is when CN is 
 * connected to ground.
 * The set and getCapacitance() functions default to shunt capacitor configuration and
 * non-volatile memory write.
 * The range of the capacitor is:
 *  Shunt = 12.5 pF to 194 pF in discrete 355 fF steps
 *  Series = 1.7 pF to 194 pF in discrete 376 fF steps
 * 
 * Priyanka Makin @ SparkFun Electronics
 * Original Creation Date: October 20, 2020
 * 
 * SparkFun labored with love to create this code. Feel like supporting open source hardware?
 * Buy a board from SparkFun! https://www.sparkfun.com/products/17182
 * 
 * This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
 * local, and you've found our code helpful, please buy us a round!
 * 
 * Hardware Connections:
 * Attach Red Board to computer using micro-B USB cable.
 * Solder Qwiic Digital Capacitor board into system of choice.
 * Attach Qwiic Digital Capacitor board to Red Board using Qwiic cable.
 * 
 * Distributed as-is; no warranty is given.
 **************************************************************************************/

#include "SparkFun_Qwiic_Digital_Capacitor.h"

digitalCap myCapacitor;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  if (myCapacitor.begin() == false) {
    Serial.println("Digital capacitor did not acknowledge. Freezing.");
    while(1);
  }
  Serial.println("Digital capacitor acknowledged!");

  //Choose the capacitor configuration mode below
  //Only uncomment up to ONE of the following lines
  //If the configuration mode is not set, the library defaults to shunt configuration
//  myCapacitor.shuntConfigMode();
  myCapacitor.seriesConfigMode();

  myCapacitor.setCapacitance(100);
  Serial.println("Capacitance set: ");
  Serial.println(myCapacitor.getCapacitance());
}

void loop() {
  // put your main code here, to run repeatedly:

}
