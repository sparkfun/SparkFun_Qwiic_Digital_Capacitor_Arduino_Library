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

//  myCapacitor.writeVolatileCapacitance(511);
//  myCapacitor.readVolatileCapacitance();

  myCapacitor.writeNonVolatileCapacitance(0x6);
  myCapacitor.setNonVolatileMode();
  myCapacitor.readRegisters();
}

void loop() {
  // put your main code here, to run repeatedly:

}
