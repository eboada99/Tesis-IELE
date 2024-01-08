
void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bits per second
}

void loop() {
  float sensorValue = analogRead(A0); // read the value from the sensor
  float voltage = sensorValue *  (5 / 1023.0);
  float E = voltage * 1600.0 ; 
  
  Serial.println(voltage); // print out the value you read
  
  delay(1000); // delay in between reads for stability
}
