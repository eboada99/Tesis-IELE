#include <UIPEthernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192, 168, 0, 190);  // Replace with the IP address of your Python server
EthernetClient client;

const float calibrationFactor = 1.84; // Start with no correction
const float R1 = 6.75 * 1000.0;
const float R2 = 1.984 * 1000.0;
const float ACS712_SENSITIVITY = -0.185;


void setup() {
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (true)
      ;
  }
  // Additional delay after setting up the network might be helpful
  
  delay(1000);
}

void loop() {
  float Samples_PYR = 0;
  float Samples_T = 0;
  float Samples_V = 0;
  float Samples_I = 0;
  //SENSORES
  float BPW21 = analogRead(A0); // Valor sensor de luz BPW21
  float LM35 = analogRead(A1); // Valor sensor de temperatura LM35
  float voltageDivisor = analogRead(A2); // Valor voltaje del panel solar 
  float corriente = analogRead(A3); // Valor corriente del panel solar
  for (int x = 0; x < 1000; x++){ //Get 1000 samples
       //Read current sensor values  
    Samples_PYR = Samples_PYR + BPW21;  //Add samples together
    Samples_T = Samples_T + LM35;
    Samples_V = Samples_V + voltageDivisor;
    Samples_I = Samples_I + corriente;
    delay(10);
   }
  float AvgPYR = Samples_PYR / 1000.0;//Taking Average of Samples
  float AvgT = Samples_T / 1000.0;//Taking Average of Samples
  float AvgV = Samples_V / 1000.0;//Taking Average of Samples
  float AvgI = Samples_I / 1000.0;//Taking Average of Samples
  //Conversor a arduino
  float irradiancia_con = AvgPYR / (1023.0); 
  float temperatura_con = AvgT * (4.653 / 1023.0);
  float divisor_con = AvgV *  (4.653 / 1023.0);
  float corriente_con = AvgI *  (4.653 / 1023.0);
  //valores finales
  float irradiancia = irradiancia_con * 1600;
  float temperatura = temperatura_con * 100.0;
  float v_out = divisor_con * ( (R1+R2) / R2 );
  float i_out = (corriente_con - 2.3265) / ACS712_SENSITIVITY;
  if (i_out < 0){
    i_out=0;
  }
  //Potencia
  float P_out = v_out * i_out;

  if (client.connect(server, 1234)) {
    client.print("PYR: ");
    client.print(irradiancia);
    client.print(", Temperatura: ");
    client.print(temperatura);
    client.print(", Voltaje: ");
    client.println(v_out);
    client.print(", Corriente: ");
    client.println(i_out,3);
    client.print(", Potencia: ");
    client.println(P_out);
    client.stop();
  }
  else {
    Serial.println("connection failed");
  }

  delay(30000);  // Wait for a while before sending the next data
}
