/*ACS712 Current Sensor Demo
   By miliohm.com
*/

#include "ACS712.h"

ACS712 sensor(ACS712ELC_20A, A3);
//ACS712_20A for 20 Amp type
//ACS712_30A for 30 Amp type

void setup() {
  Serial.begin(9600);
}

void loop() {
  float I = sensor.getCurrentAC();
//ignoring the value below 0.09
  if (I < 0.09) {
    I = 0;
  }
  Serial.println(I);
  delay(300);
}
