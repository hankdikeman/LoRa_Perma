int buttonPin = 13;
int redLedPin = 12;
int greenLedPin = 11;


void setup() {
  // Serial.begin(9600);
  // while(!Serial);
  // Serial.println("LoRa Sender Node Test");
  
  // set pinmode of led and button pins
  pinMode(buttonPin, INPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

}

void loop() {
  // read in analog signal from sensor and use to turn on/off LED
  digitalWrite(greenLedPin, digitalRead(buttonPin));
  digitalWrite(redLedPin, !digitalRead(buttonPin));


  // Serial.print("Reading: ");
  // Serial.println(digitalRead(buttonPin));
  // delay between packet sends
  delay(100);
}
