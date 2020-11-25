#include <LoRa.h>
#define inputpin_1 A0
int sensorvalue = -10;

void setup() {
  // start serial and set Baud rate to 9600 bps
  Serial.begin(9600);
  while(!Serial);
  Serial.println("LoRa Sender Node Test");

  // initialize the LoRa sender protocol, kill program if not
  if(!LoRa.begin(915E6)){
    Serial.println("LoRa Shield Connection Failed");
    while(1);
  }
}

void loop() {
  // read in analog signal from sensor
  sensorvalue = analogRead(inputpin_1);
  Serial.print("Reading: ");
  Serial.println(sensorvalue);
  
  // initialize packet and send it
  LoRa.beginPacket();
  LoRa.print(sensorvalue);
  LoRa.endPacket();
  
  // delay between packet sends
  delay(5000);
}
