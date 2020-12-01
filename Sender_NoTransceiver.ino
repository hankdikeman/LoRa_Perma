int buttonPin = 12;
int ledPin = 13;


void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("LoRa Sender Node Test");
  
  // set pinmode of led and button pins
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read in analog signal from sensor and use to turn on/off LED
  digitalWrite(ledPin, digitalRead(buttonPin));

  Serial.print("Reading: ");
  Serial.println(digitalRead(buttonPin));
  // delay between packet sends
  delay(200)
}
