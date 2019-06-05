// Height Measurement Tool
// 2019.06.02
// Bret Wagner

#include "SR04.h"
#include <LiquidCrystal.h>
#define TRIG_PIN 14
#define ECHO_PIN 15
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long distance;
long start;
long height;
long inches;
int feet;
int resetButton = 3;
int measureButton = 4;
int convertButton = 5;
int dataUnit = 0;

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(measureButton, INPUT_PULLUP);
  pinMode(convertButton, INPUT_PULLUP);
  lcd.begin(16,2);
  lcd.print("Measure your");
  lcd.setCursor(0,1);
  lcd.print("height?");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(resetButton) == LOW) {
    Serial.println("Tare button Pressed");
    tareMeasurement();
    delay(300);
  }

  if (digitalRead(measureButton) == LOW) {
    distance = sr04.Distance();
    height = start-distance;
    Serial.println("Measure button Pressed");
    displayData(height, dataUnit);
    delay(300);
  }

  if (digitalRead(convertButton) == LOW) {
    Serial.println("Convert button Pressed");
    dataUnit = switchUnit(dataUnit);
    displayData(height, dataUnit);
    delay(300);
  }
}

int switchUnit(int dataUnit) {
  if (dataUnit == 2) {
    dataUnit = 0;
  } else {
    dataUnit = dataUnit + 1;
  }
  Serial.print("data type ");
  Serial.println(dataUnit);
  return dataUnit;
}

void displayData(long height, int dataUnit) {
  clearLcd();
  lcd.print("You are");
  if (dataUnit == 0) {
    lcd.setCursor(0,1);
    lcd.print(height);
    lcd.print("cm tall");
    Serial.print(height);
    Serial.println("cm");
  } else if (dataUnit == 1) {
    inches = getInches(height);
    lcd.setCursor(0,1);
    lcd.print(inches);
    lcd.print(" inches tall");
    Serial.print(height);
    Serial.println("cm");
  } else if (dataUnit == 2) {
    feet = getFeet(height);
    inches = getInches(height);
    inches = inches % 12;
    lcd.setCursor(0,1);
    lcd.print(feet);
    lcd.print("'");
    lcd.print(inches);
    lcd.print("\" tall");
    Serial.print(height);
    Serial.println("cm");
  } else {
    lcd.setCursor(0,0);
    lcd.print("\0");
    lcd.setCursor(0,1);
    lcd.print("\0");  
    Serial.println("Error");
  }
}

void clearLcd() {
  lcd.setCursor(0,0);
  lcd.print("                   ");
  lcd.setCursor(0,1);
  lcd.print("                   ");
  lcd.setCursor(0,0);
}

int getFeet(long height) {
  inches = height/2.54;
  feet = inches / 12;
  return feet;
}

int getInches(long height) {
  inches = height/2.54;
  return inches;
}


long tareMeasurement() {
  start = sr04.Distance();
  Serial.print(start);
  Serial.println("cm");
  return start;
}
