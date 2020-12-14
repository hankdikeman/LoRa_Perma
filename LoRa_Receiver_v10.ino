#include <LoRa.h>
#include <LiquidCrystal.h>
#include <ctype.h>
#include <SoftwareSerial.h>

#define bit2400Delay 408  
#define halfBit2400Delay 50
#define bit4800Delay 188
#define halfBit4800Delay 94
#define rx A3
#define tx A4
#define carriage 0x0D
#define msg_begin 0x02

LiquidCrystal lcd(A2,A1,5,4,3,A0);
SoftwareSerial serialRL(rx,tx);

String message = "";
byte ascii_byte;
char ascii_list[16];

// uint8_t const carriage = 0x0D;
// uint8_t const msg_begin = 0x02;

void setup() {
  // define pinmodes for send and receive
  serialRL.begin(2400);
  digitalWrite(tx,HIGH);
  
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

    
    // send reading over serial port
    uint16_t signalx1000 = (uint16_t) message.toInt();
    // msg begin ASCII
    // serialRL.write(msg_begin);
    for(int i = 0; i < 16; i++){
      // generate ASCII byte
      char digit = (char) (signalx1000 % 10);
      ascii_list[i] = (char) digit + 48;
      // decrease number by power of ten
      signalx1000 = (uint16_t) signalx1000 / 10;
    }
    // send bytes
    for(int i = 15; i >= 0; i--){
      char digit = (char)ascii_list[i];
      serialRL.write(digit);
    }
    // print carriage return
    serialRL.write((char)carriage);
  }
}

// printing to serial port
void SWprint(uint8_t data)
{
  byte mask;
  //startbit
  digitalWrite(tx,LOW);
  delayMicroseconds(bit2400Delay);
  for (mask = 0x01; mask>0; mask <<= 1) {
    if (data & mask){ // choose bit
     digitalWrite(tx,HIGH);
    }
    else{
     digitalWrite(tx,LOW);
    }
    delayMicroseconds(bit2400Delay);
  }

  digitalWrite(tx, HIGH);
  delayMicroseconds(bit2400Delay);  
}

// reading from serial port
int SWread()
{
  byte val = 0;
  while (digitalRead(rx));
  //wait for start bit
  if (digitalRead(rx) == LOW) {
    delayMicroseconds(halfBit2400Delay);
    for (int offset = 0; offset < 8; offset++) {
     delayMicroseconds(bit2400Delay);
     val |= digitalRead(rx) << offset;
    }
    //wait for stop bit + extra
    delayMicroseconds(bit2400Delay);
    delayMicroseconds(bit2400Delay);
    return val;
  }
}
