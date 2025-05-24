#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pins and Constants
#define RST_PIN         9
#define SS_PIN          10
#define SERVO_PIN       3
#define SLOT_COUNT      4

// IR Sensors on Analog Pins
const int irSensors[SLOT_COUNT] = {A0, A1, A2, A3};

// RFID Valid UID (example)
byte validUID[4] = {0xAB, 0xCD, 0x12, 0x34};  // REPLACE with your card's UID

Servo gateServo;
MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int slotStatus[SLOT_COUNT] = {0, 0, 0, 0};
int availableSlots = SLOT_COUNT;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();

  gateServo.attach(SERVO_PIN);
  gateServo.write(90);  // Closed

  for (int i = 0; i < SLOT_COUNT; i++) {
    pinMode(irSensors[i], INPUT);
  }
}

void loop() {
  readSlotSensors();
  displayLCDStatus();

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    if (isCardValid(rfid.uid.uidByte, rfid.uid.size)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access Granted");
      openGate();
      delay(5000);
      closeGate();
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access Denied");
      triggerESP32Capture();  // Let ESP32-CAM know to capture image
    }
    rfid.PICC_HaltA();
  }

  delay(1000);
}

void readSlotSensors() {
  availableSlots = SLOT_COUNT;
  for (int i = 0; i < SLOT_COUNT; i++) {
    if (digitalRead(irSensors[i]) == LOW) {
      slotStatus[i] = 1;
      availableSlots--;
    } else {
      slotStatus[i] = 0;
    }
  }
}

void displayLCDStatus() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Free Slots: ");
  lcd.print(availableSlots);

  lcd.setCursor(0, 1);
  for (int i = 0; i < SLOT_COUNT; i++) {
    lcd.print("S");
    lcd.print(i + 1);
    lcd.print(slotStatus[i] ? ":F " : ":E ");
  }
}

bool isCardValid(byte *uid, byte length) {
  for (byte i = 0; i < length; i++) {
    if (uid[i] != validUID[i]) return false;
  }
  return true;
}

void openGate() {
  gateServo.write(180);
  lcd.setCursor(0, 1);
  lcd.print("Gate Opening");
}

void closeGate() {
  gateServo.write(90);
  lcd.setCursor(0, 1);
  lcd.print("Gate Closing");
}

void triggerESP32Capture() {
  // Send a HIGH signal to ESP32 via pin (e.g., D7)
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
}
