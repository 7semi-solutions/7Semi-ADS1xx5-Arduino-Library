# 7Semi-ADS1xx5-Arduino-Library

This Arduino library provides unified support for the **7Semi ADS1015 (12-bit)** and **ADS1115 (16-bit)** 4-channel I2C analog-to-digital converter (ADC) boards. It enables high-precision analog voltage measurements in both single-ended and differential modes.

![Arduino](https://img.shields.io/badge/platform-arduino-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Status](https://img.shields.io/badge/status-active-brightgreen.svg)

---

## Hardware Supported

- 7Semi ADS1015 12-bit 4-Channel ADC  
- 7Semi ADS1115 16-bit 4-Channel ADC  

---

## Getting Started

### 1. Installation via Arduino Library Manager

1. Open the **Arduino IDE**
2. Go to:
   - `Sketch > Include Library > Manage Libraries…` (IDE 1.x)  
   - Or use 📚 icon in sidebar (IDE 2.x)
3. Search for:
   -7Semi ADS1xx5
4. Click **Install**

Then include the library in your sketch:

#include <7semi_ads1xx5.h>

### 2. Wiring

| ADS1xx5 Pin     | Arduino Pin                |
| --------------- | -------------------------- |
| VCC             | 3.3V or 5V                 |
| GND             | GND                        |
| SDA             | A4 (Uno)                   |
| SCL             | A5 (Uno)                   |
| ADDR (optional) | GND (default address 0x48) |
