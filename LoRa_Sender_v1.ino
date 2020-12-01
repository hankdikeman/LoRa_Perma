#include <LoRa.h>

int buttonPin = 12;
int ledPin = 13;
int buttonValue = 0;


void setup() {
  // initialize the LoRa sender protocol, kill program if not
  if(!LoRa.begin(915E6)){
    while(1);
  }

  // set pinmode of led and button pins
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read in analog signal from sensor and use to turn on/off LED
  buttonValue = digitalRead(buttonPin);
  digitalWrite(ledPin, buttonValue);
  
  // initialize packet and send it
  LoRa.beginPacket();
  LoRa.print(buttonValue);
  LoRa.endPacket();
  
  // delay between packet sends
  delay(500);
}
