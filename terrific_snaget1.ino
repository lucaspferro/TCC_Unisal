#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int analogPin = A1; 
const float referenceVoltage = 5.0; 

void setup() {
  lcd.begin(16, 2);
  lcd.print("Resistencia:");
}

void loop() {
  
  int rawValue = analogRead(analogPin);
  
  float voltage = (rawValue / 1023.0) * referenceVoltage;

  float current = 0.02; 
  

  float resistance = voltage / current;

  lcd.clear();
  

  lcd.setCursor(0, 0);
  lcd.print("Tensao: ");
  lcd.print(voltage, 2);
  lcd.print(" V");

  
  lcd.setCursor(0, 1);
  lcd.print("Resistencia: ");
  lcd.print(resistance, 2);
  lcd.print(" ohms");

  delay(1000);
}
