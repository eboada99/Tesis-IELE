int PYR = A0;
int V = 0;
float R=220.0;
void setup() {
  Serial.begin(9600);
  pinMode(PYR, INPUT); 
}

void loop() {
  V = analogRead(PYR);
  float v = V*(5.0/1023.0);
  float I=v/R;
  Serial.println(I);
  delay(1000);
}
