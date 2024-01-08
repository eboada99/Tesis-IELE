#include <UIPEthernet.h>

// Set your MAC address
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

void setup() {
  Serial.begin(9600);
  
  // Initialize the Ethernet client library
  // with the IP address and port of the server that you want to connect to
  Serial.println("Initializing Ethernet...");
  Ethernet.begin(mac);
  
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
}

void loop() {
  // Do nothing here for now.
}
