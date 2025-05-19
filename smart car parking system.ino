#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Initialize LCD at I2C address 0x27 with 16 columns and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gate;

const int entrySensor = 2;  // IR sensor at entry
const int exitSensor = 3;   // IR sensor at exit
const int servoPin = 9;     // Servo connected to pin 9

int totalSlots = 4;
int occupiedSlots = 0;

void setup() {
  pinMode(entrySensor, INPUT);
  pinMode(exitSensor, INPUT);
  
  gate.attach(servoPin);
  gate.write(0); // Initial closed position
  
  lcd.begin(16, 2);
  lcd.backlight();
  updateLCD();
}

void loop() {
  // Entry Detection
  if (digitalRead(entrySensor) == LOW) { // Assuming IR returns LOW when detects object
    if (occupiedSlots < totalSlots) {
      openGate();
      occupiedSlots++;
      updateLCD();
      delay(2000); // Wait for car to enter
      closeGate();
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Parking Full");
      delay(2000);
      updateLCD();
    }
  }

  // Exit Detection
  if (digitalRead(exitSensor) == LOW) {
    if (occupiedSlots > 0) {
      openGate();
      occupiedSlots--;
      updateLCD();
      delay(2000); // Wait for car to exit
      closeGate();
    }
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Free: ");
  lcd.print(totalSlots - occupiedSlots);
  lcd.setCursor(0, 1);
  lcd.print("Occupied: ");
  lcd.print(occupiedSlots);
}

void openGate() {
  gate.write(90); // Open position
  delay(1000);
}

void closeGate() {
  gate.write(0); // Close position
  delay(1000);
}