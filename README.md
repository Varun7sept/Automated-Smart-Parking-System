# Automated-Smart-Parking-System 🚗

This project implements an IoT-based Automated Parking System using Arduino Uno and ESP32-CAM. It detects vehicles using IR sensors and RFID authentication. If RFID is missing, ESP32-CAM captures the image and sends it to Telegram.

## Components Used
- Arduino Uno
- IR Sensors
- RFID Reader (RC522)
- RFID Tag
- 16x2 LCD with I2C
- Servo Motor
- ESP32-CAM
- Breadboard, Wires, Power Supply

## Features
- Real-time parking slot display
- Automated gate control
- RFID-based authentication
- Image capture of unauthorized entries via ESP32-CAM
- Telegram alerts

## Folder Structure
- `Arduino/` – Arduino Uno Code
- `ESP32-CAM/` – Image capture & alert code
- `Docs/` – Project Report
- `README.md` – Project Summary

## Setup Instructions
1. Upload `main_parking_system.ino` to Arduino Uno
2. Upload `esp32cam_capture_send.ino` to ESP32-CAM using FTDI
3. Set up Telegram bot via [@BotFather](https://t.me/botfather)
4. Replace WiFi and Telegram credentials in code

## License
MIT License
