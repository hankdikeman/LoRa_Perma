#include <LoRa.h>
#include <LiquidCrystal.h>
#include <ctype.h>

#define bit9600Delay 100  
#define halfBit9600Delay 50
#define bit4800Delay 188
#define halfBit4800Delay 94
#define rx A3
#define tx A4
#define carriage 0x0D
#define msg_begin 0x02

LiquidCrystal lcd(A2,A1,5,4,3,A0);

String message = "";
byte ascii_byte;
uint8_t ascii_list[16];
boolean rev_byte[8];

// uint8_t const carriage = 0x0D;
// uint8_t const msg_begin = 0x02;

void setup() {
  // define pinmodes for send and receive
  pinMode(rx,INPUT);
  pinMode(tx,OUTPUT);
  digitalWrite(tx,LOW);
  delay(5000);
  
  // initialize LCD screen
  lcd.begin(16, 2);
  lcd.clear();
  // initialize the LoRa sender protocol, kill program if not
  if(!LoRa.begin(915E6)){
    while(1);
  }
  // print out connection affirm
  lcd.setCursor(0,0);
  lcd.print("CONNECTED");

  Serial.begin(9600);
}

void loop() {
  // detect any sent packet
  int packetSize = LoRa.parsePacket();

  // read packet if one was received > 0
  if (packetSize) {
    
    // read accepted packet
    message = "";
    while(LoRa.available()){
      message += (char)LoRa.read();
    }

    // print out RSSI to LCD
    lcd.setCursor(0,1);
    lcd.print("RSSI: ");
    lcd.print(LoRa.packetRssi());

    // print out message to LCD
    lcd.setCursor(0,0);
    lcd.print("MSG: ");
    lcd.print(message.toFloat()/1000);
    lcd.print("   ");

    Serial.println(message);
    
    // send reading over serial port
    uint16_t signalx1000 = (uint16_t) message.toInt();
    // msg begin ASCII
    SWprint(msg_begin);
    Serial.print(msg_begin);
    Serial.print(" ");
    for(int i = 0; i < 16; i++){
      // generate ASCII byte
      uint8_t digit = signalx1000 % 10;
      ascii_list[i] = (uint8_t) digit + 48;
      // decrease number by power of ten
      signalx1000 = (uint16_t) signalx1000 / 10;
    }
    // send bytes
    for(int i = 15; i >= 0; i--){
      uint8_t digit = ascii_list[i];
      SWprint(digit);
      Serial.print(digit);
      Serial.print(" ");
    }
    // print carriage return
    SWprint(carriage);
    Serial.println(carriage);
  }
}

// printing to serial port
void SWprint(uint8_t data)
{
  byte mask;
  //Serial.println("");
  int count = 0;
  for (mask = 0x01; mask>0; mask <<= 1) {
    //Serial.print(bitRead(data,7-count));
    if (data & mask){ // choose bit
     rev_byte[count] = true;
     //Serial.print("1");
    }
    else{
     rev_byte[count] = false;
     //Serial.print("0");
    }
    count = count + 1;
  }
  
  //Serial.println("");
  //startbit
  digitalWrite(tx,HIGH);
  //Serial.print("START");
  delayMicroseconds(bit9600Delay);
  
  count = 7;
  while(count >= 0){
    if(rev_byte[count]){
      digitalWrite(tx,LOW);
      //Serial.print(1);
    }
    if(!rev_byte[count]){
      digitalWrite(tx,HIGH);
      //Serial.print(0);
    }
    delayMicroseconds(bit9600Delay);
    count = count-1;
  }
  //stop bit
  digitalWrite(tx, LOW);
  //Serial.println("STOP");
  delayMicroseconds(bit9600Delay);
}

// reading from serial port
int SWread()
{
  byte val = 0;
  while (digitalRead(rx));
  //wait for start bit
  if (digitalRead(rx) == LOW) {
    delayMicroseconds(halfBit9600Delay);
    for (int offset = 0; offset < 8; offset++) {
     delayMicroseconds(bit9600Delay);
     val |= digitalRead(rx) << offset;
    }
    //wait for stop bit + extra
    delayMicroseconds(bit9600Delay);
    delayMicroseconds(bit9600Delay);
    return val;
  }
}
