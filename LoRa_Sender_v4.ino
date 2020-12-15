#include <LoRa.h>

#define serialCode 12345678
#define level A1
#define pressure A2

float RSMT_LVL = 0.00;
float RSMT_PRESS = 0.00;

void setup() {
  pinMode(level, INPUT);
  pinMode(pressure, INPUT);

  // initialize the LoRa sender protocol, kill program if not
  if(!LoRa.begin(915E6)){
    while(1);
  }
}

void loop() {
  // read and convert 4-20 signal
  RSMT_LVL = (int) (1000*convertVoltage((float) analogRead(level)));
  RSMT_PRESS = (int) (1000*convertVoltage((float) analogRead(pressure)));
  
  // initialize packet and send 4-20 signal x1000
  LoRa.beginPacket();
  LoRa.print((RSMT_LVL));
  LoRa.print('%');
  LoRa.print((RSMT_PRESS));
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
