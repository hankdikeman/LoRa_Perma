#include <LoRa.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(A2,A1,5,4,3,A0);

String message = "";

void setup() {
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
  }
}
