#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>
#include <Keypad.h>

char auth[] = "TMPL6_b4Oo0T7";
char ssid[] = "Kakapo";
char pass[] = "Kakapo1212";
Adafruit_LiquidCrystal lcd(0x20);
const int buzzerPin = D5;
const int lockPin = D6;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {D1, D2, D3, D4};
byte colPins[COLS] = {D5, D6, D7, D8}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  lcd.begin(12, 2); 
  pinMode(buzzerPin, OUTPUT);
  pinMode(lockPin, OUTPUT);
  digitalWrite(lockPin, LOW);
  lcd.print("Enter PIN:");
}
void loop() {
  Blynk.run();
  char key = keypad.getKey();
  if (key) {
    lcd.clear();
    if (key == '*') {
      // Logic for unlocking
      digitalWrite(lockPin, HIGH);
      Blynk.virtualWrite(V0, "Unlocked");
      lcd.print("Door Unlocked");
      delay(2000);
      digitalWrite(lockPin, LOW);
      Blynk.virtualWrite(V0, "Locked");
      lcd.clear();
      lcd.print("Enter PIN:");
    } else {
      lcd.print("Wrong PIN");
      tone(buzzerPin, 1000, 200);
      delay(1000);
      lcd.clear();
      lcd.print("Enter PIN:");
    }
  }
  int smokeValue = analogRead(A0);
  if (smokeValue > 400) {
    digitalWrite(lockPin, HIGH);
    Blynk.virtualWrite(V0, "Unlocked - Smoke Detected");
    lcd.print("Door Unlocked");
    delay(2000);
    digitalWrite(lockPin, LOW);
  }
}
