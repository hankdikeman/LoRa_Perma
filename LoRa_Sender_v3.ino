#include <LoRa.h>

int buttonPin = 5;
int redLedPin = 4;
int greenLedPin = 3;
float RSMT = 0.00;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(A0, INPUT);

  // initialize the LoRa sender protocol, kill program if not
  if(!LoRa.begin(915E6)){
    while(1);
  }
}

void loop() {
  // read in analog signal from sensor and use for LED activation
  digitalWrite(greenLedPin, digitalRead(buttonPin));
  digitalWrite(redLedPin, !digitalRead(buttonPin));

  // read and convert 4-20 signal
  RSMT = (int) (1000*convertVoltage((float) analogRead(A0)));

  // initialize packet and send 4-20 signal x1000
  LoRa.beginPacket();
  LoRa.print((RSMT));
  LoRa.endPacket();
  
  // delay between packet sends
  delay(500);
}

float convertVoltage(float reading) {
  // change for given resistance used, here is 243 ohms
  float resistor = 243.0;
  float voltage = (reading + 0.5)/1023.0*5.0;

  return voltage*1000/resistor;
}
