const float R1 = 6.75 * 1000.0;
const float R2 = 1.984 * 1000.0;
const float ACS712_SENSITIVITY = 0.1;

void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bits per second
}
void loop() {
  float BPW21 = analogRead(A0); // Valor medido sensor de luz BPW21
  float LM35 = analogRead(A1); // Valor medido sensor de temperatura LM35
  float voltageDivisor = analogRead(A2); // Valor voltaje del panel solar 
  float ACS712 = analogRead(A3); // Valor voltaje del panel solar 
  float Divisor_con = voltageDivisor *  (5.0 / 1023.0);
  float temperatura = (LM35 * 5.0 / 1023.0 ) * 100.0;
  float v_out = Divisor_con * ( (R1+R2) / R2 );
  float i_con = ACS712 * (5.0 / 1023.0 );
  float i_out = (i_con - 2.495) / ACS712_SENSITIVITY ;
  if (i_out < 0.16){
    i_out=0;
    }
  
  
  Serial.print("Intensidad de la luz: ");
  Serial.println(BPW21);
  Serial.print("Temperature: ");
  Serial.print(temperatura);
  Serial.println(" C");
  Serial.print("Voltaje: ");
  Serial.print(v_out);
  Serial.println("V");
  Serial.print("Corriente: ");
  Serial.print(i_out);
  Serial.println("A");
  delay(1000);
}
