#include <Adafruit_ADS1X15.h>
#include <Wire.h>

Adafruit_ADS1015 ads;  // Use this for the ADS1015

// Calibration factor - adjust as necessary based on your calibration measurements
const float calibrationFactor = 1.84; // Start with no correction

void setup(void) {
  Serial.begin(9600);
  ads.begin();  // Initialize the ADS1015
  ads.setGain(GAIN_TWO);  // 2x gain +/- 1.024V
}

void loop(void) {
  int16_t adc0;  // Variable to hold the ADC result
  float voltage0;

  adc0 = ads.readADC_SingleEnded(0);  // Read from channel 0
  voltage0 = (adc0 * 0.0005) * calibrationFactor;  // Convert to voltage and apply calibration

  Serial.print("AIN0: "); 
  Serial.print(adc0);
  Serial.print(" (");
  Serial.print(voltage0,3);
  Serial.println("V)");

  delay(1000);  // Delay for a second
}
