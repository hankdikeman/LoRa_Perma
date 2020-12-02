#include <LoRa.h>

int ledPin = 13;
String message = '';

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
  // detect any sent packet
  int packetSize = LoRa.parsePacket();

  // read packet if one was received > 0
  if (packetSize) {
    // notify that package was received
    Serial.println("");
    Serial.println("Packet received");

    // read accepted packet
    message = '';
    while(LoRa.available()){
      message += (char)LoRa.read();
    }

    // print out received packet to serial monitor
    Serial.print("Message: ");
    Serial.println(message);
    // print Received Signal Strength Indicator
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());
  }

  if(message.toInt() == 1){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
}
