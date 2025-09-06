*******************************************************
 * @file Basic.ino
 *
 * @brief Basic example demonstrating usage of the 7Semi ADS1xx5 library.
 *
 * This example initializes an ADS1015 (12-bit) or ADS1115 (16-bit) ADC over I2C
 * and reads voltages from all 4 channels in both single-ended and differential modes.
 *
 * Key features demonstrated:
 * - Reading single-ended input voltages from AIN0–AIN3
 * - Reading differential voltages (AIN0–AIN1 and AIN2–AIN3)
 * - Configuring programmable gain amplifier (PGA)
 *
 * @note This example requires the 7Semi ADS1xx5 library to be installed.
 *       It supports both 7Semi ADS1015 and ADS1115 modules.
 *
 * @section author Author
 * Written by 7Semi
 *
 * @section license License
 * @license MIT
 * Copyright (c) 2025 7Semi
 *******************************************************/

#include <7semi_ADS1xx5.h>

// Create ADS1xx5_7semi object 
// default address 0x48
ADS1xx5_7semi adc;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("ADS1xx5 Test Begin");

  if (!adc.begin()) {
    Serial.println("Failed to connect to ADS1xx5!");
    while (1)
      ;
  }
  Serial.println("ADS1xx5 Connected");

  // Optional: Set PGA (reference voltage)
  // Default: ±2.048V
  // To change: use
  // PGA_6_144V  6.144V
  // PGA_4_096V  4.096V
  // PGA_1_024V  1.024V
  // PGA_0_512V  0.512V
  // PGA_0_256V  0.256V
  adc.setRefV(PGA_2_048V);  // ±2.048V full scale
}

void loop() {
  Serial.println("Reading Single-Ended Channels:");
  for (int ch = 0; ch < 4; ch++) {
    float voltage = adc.readCH(ch);
    Serial.print("AIN");
    Serial.print(ch);
    Serial.print(": ");
    Serial.print(voltage, 6);
    Serial.println(" V");
    delay(50);
  }
  //  MUX_AIN0_AIN1 AIN0 - AIN1
  //  MUX_AIN0_AIN3 AIN0 - AIN3
  //  MUX_AIN1_AIN3 AIN1 - AIN3
  //  MUX_AIN2_AIN3 AIN2 - AIN3
  Serial.println("\n Reading Differential Voltages:");
  float diff0_1 = adc.readDiff(0);  // AIN0 - AIN1
  float diff2_3 = adc.readDiff(3);  // AIN2 - AIN3

  Serial.print("AIN0 - AIN1: ");
  Serial.print(diff0_1, 6);
  Serial.println(" V");

  Serial.print("AIN2 - AIN3: ");
  Serial.print(diff2_3, 6);
  Serial.println(" V");

  Serial.println("------------------------------------------------");
  delay(2000);
}
