#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <DHT.h>
#include <Wire.h>

#define dhtPin 7
#define dhtType DHT11
DHT HumidityTemp(dhtPin, dhtType);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

const int ledPin = 11; // LED PIN setting
const int touchPin = 13; // TOUCH SENSOR setting

int previousTouchValue = 0;
bool isOpen = false; // CLOSEed on start

void setup() {
  Serial.begin(9600);
  myServo.attach(3);

  pinMode(ledPin, OUTPUT); // Set LED pin as output

  pinMode(touchPin, INPUT); //Touch sensor INPUT
  lcd.begin(16,2);
  lcd.backlight();

}

void loop() {
  float Humidity = HumidityTemp.readHumidity();
  float Temp = HumidityTemp.readTemperature();

  lcd.setCursor(0,0);
  lcd.print("Humid: ");
  lcd.print(Humidity);

  lcd.setCursor(0,1);
  lcd.print("Temp: ");
  lcd.print(Temp);

  int touchValue = digitalRead(touchPin);
  if (touchValue != previousTouchValue) {
    // Runs only when touch sensor input changes
    previousTouchValue = touchValue;
    
    if (touchValue == 1) { // open
      isOpen = !isOpen; // toggle
      if (isOpen) {
        digitalWrite(ledPin, HIGH); // light ON
        for (int angle = 100; angle >= 1; angle--) {
          myServo.write(angle); // angle setting
          delay(10); // delay time(0.01sec)
        }
      } else {
        digitalWrite(ledPin, LOW); // light OFF
        for (int angle = 0; angle <= 100; angle++) {
          myServo.write(angle); // angle setting
          delay(10); // delay time(0.01sec)
        }
      }
    }
  }
  delay(500);
}