/**************************************************************************************
 * Set and get the capacitance of NCD2400M on the Qwiic Digital Capacitor breakout board.
 * Please take into consideration the configuration of the capacitor. Is the capacitor 
 * in series (CP and CN inserted in series into a system) or shunt configuration (connecting 
 * CN to GND)? It's necessary to distinguish between the two because they have different 
 * conversion equations. The range of the capacitor is:
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

  //The parameters for the setCapacitance() function are described below:
  //First, capacitance to set in pF
  //Then configuration of capacitor: 1 for shunt, 0 for series
  myCapacitor.setCapacitance(86, 1);
  Serial.print("Capacitance set: ");
  //The parameter for getCapacitance() describes the configuration of the capacitor
  //1 for shunt, 0 for series
  Serial.println(myCapacitor.getCapacitance(1));
}

void loop() {
  // put your main code here, to run repeatedly:

}
