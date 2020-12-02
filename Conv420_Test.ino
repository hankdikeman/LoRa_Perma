#include <LiquidCrystal.h>

float input420 = -1.0;
LiquidCrystal lcd(12,11,5,4,3,2);

void setup() {
  // initialize LCD screen
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {
  // convert input value to voltage
  input420 = ((float) analogRead(A0)+0.5)/1023.0*5.0;
  
  // print out raw voltage reading on first line
  lcd.setCursor(0, 0);          
  lcd.print("Volts: ");
  lcd.print(input420,3);
  lcd.print("  ");
  
  // convert voltage to 4-20 signal
  input420 = input420*1000.0/243.0;
  
  // print 4-20 value on second line
  lcd.setCursor(0, 1);        
  lcd.print("4-20: ");
  lcd.print(input420,3);
  lcd.print("  ");

  // delay between measurements
  delay(400);
}
