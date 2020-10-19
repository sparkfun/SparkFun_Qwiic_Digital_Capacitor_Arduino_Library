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

//  myCapacitor.writeVolatileCapacitance(0x5);
//  myCapacitor.writeVolatileCapacitance(0x6A);
//  myCapacitor.writeVolatileCapacitance(0xF6);
//  myCapacitor.writeVolatileCapacitance(0x183);
  myCapacitor.writeNonVolatileCapacitance(0x123);
  myCapacitor.readVolatileCapacitance();
}

void loop() {
  // put your main code here, to run repeatedly:

}
