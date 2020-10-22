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

  //The parameters for the setCapacitance() function are as follows:
  //First, capacitance to set in pF
  //Then configuration of capacitor: 1 for shunt, 0 for series
  //Optional parameter for type of write
  myCapacitor.setCapacitance(86, 1);
  Serial.print("Capacitance set: ");
  Serial.println(myCapacitor.getCapacitance(1));
}

void loop() {
  // put your main code here, to run repeatedly:

}
