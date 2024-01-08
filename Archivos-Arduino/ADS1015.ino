
#include <Adafruit_ADS1X15.h>
#include <Wire.h>

Adafruit_ADS1015 ads1015;

void setup() {
  Serial.begin(9600);
  ads1015.begin();
}

void loop() {
  // Read from channel 0 (AIN0)
  int16_t adcValue = ads1015.readADC_SingleEnded(0);

  // Convert ADC value to voltage
  // Since we're using the default gain (6.144V range), the factor is:
  // voltage = adcValue * (6.144/2047.0)
  float voltage = adcValue * (6.144 / 2047.0);
  float E = 1600 * voltage;
  //Serial.print("Voltage: ");
  //Serial.println(voltage, 3);  // Print voltage to 3 decimal places
  Serial.print("Irradiance: ");
  Serial.print('\t');
  Serial.println(E, 3);  // Print voltage to 3 decimal places
  
  delay(1000);  // Wait for a second
}
