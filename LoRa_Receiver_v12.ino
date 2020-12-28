#include <LoRa.h>
#include <SoftwareSerial.h>

#define rx A3
#define tx A4
#define carriage 0x0D

SoftwareSerial serialRL(rx,tx);

String message = "";
byte ascii_byte;
char ascii_list[16];

void setup() {
  // define pinmodes for send and receive
  serialRL.begin(2400);
  Serial.begin(9600);
  digitalWrite(tx,HIGH);
  
  // initialize the LoRa sender protocol, kill program if not
  if(!LoRa.begin(915E6)){
    while(1);
  }
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
    
    // parse string into level and pressure
    uint8_t delim_index = message.indexOf('%');
    String level = message.substring(0,delim_index);
    String pressure = message.substring(delim_index+1);

    // send reading over serial port
    uint16_t levelx1000 = (uint16_t) level.toInt();
    uint16_t pressurex1000 = (uint16_t) pressure.toInt();

    // print out readings with delimiter
    serialRL.write((char)0x03);
    RLPrint(levelx1000);
    serialRL.write((char)0x25);
    RLPrint(pressurex1000);
    serialRL.write((char)carriage);
  }
}

void RLPrint(uint16_t reading){
      for(int i = 0; i < 16; i++){
      // generate ASCII byte
      char digit = (char) (reading % 10);
      ascii_list[i] = (char) digit + 48;
      // decrease number by power of ten
      reading = (uint16_t) reading / 10;
    }
    // send bytes
    for(int i = 15; i >= 0; i--){
      char digit = (char)ascii_list[i];
      serialRL.write(digit);
    }
}
