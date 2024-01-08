int bpw21 = A0;
int sensorValue_bpw21 = 0;
int sensorPin1 = A1;
int sensorValue1 = 0;
int lm35 = A2;
int sensorValue_lm35 = 0;

void setup(void) {
   Serial.begin(9600);
   pinMode(bpw21, INPUT); 
   pinMode(sensorPin1, INPUT); 
   pinMode(lm35, INPUT); 
}
 
void loop(void) {
   sensorValue_bpw21 = analogRead(bpw21);
   sensorValue1 = analogRead(sensorPin1);
   sensorValue_lm35 = analogRead(lm35);
   float voltage = sensorValue_lm35 * (5000 / 1024.0);
   float temperature = voltage / 10;
   Serial.println("BPW21");
   Serial.println(sensorValue_bpw21);
   Serial.println("Sensor2");
   Serial.println(sensorValue1);
   Serial.println("LM35");
   Serial.println(temperature);
   Serial.print(" \xC2\xB0"); // shows degree symbol
   Serial.println("C");
   delay(1000);
}  
