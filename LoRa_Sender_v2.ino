#include <LoRa.h>

String message = '';
int buttonPin = 13;
int redLedPin = 12;
int greenLedPin = 11;


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
  // read in analog signal from sensor and use for LED activation
  digitalWrite(greenLedPin, digitalRead(buttonPin));
  digitalWrite(redLedPin, !digitalRead(buttonPin));

  // print reading to serial monitor
  Serial.print("Reading: ");
  Serial.println(digitalRead(buttonPin));

  
  // initialize packet and send it
  LoRa.beginPacket();
  LoRa.print(digitalRead(buttonPin));
  LoRa.endPacket();
  
  // delay between packet sends
  delay(500);
}
