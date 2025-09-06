/***********************************************************************/
//  Arduino Library for ADS1115 16-bit ADC
//  Developed by 7semi
//  Device: ADS1115 (Texas Instruments)
//  Features: Differential and single-ended voltage reading, PGA control, threshold config
//  Communication: I2C
/***********************************************************************/

#ifndef _7SEMI_ADS1XX5_H_
#define _7SEMI_ADS1XX5_H_

#include <Arduino.h>
#include <Wire.h>

// Register addresses
#define REG_CONVERSION   0x00  // Conversion result
#define REG_CONFIG       0x01  // Configuration register
#define REG_LO_THRESH    0x02  // Comparator low threshold
#define REG_HI_THRESH    0x03  // Comparator high threshold

// MUX[2:0] - input selection (config bits 14:12)
#define MUX_AIN0_AIN1 (0x0)  // AIN0 - AIN1
#define MUX_AIN0_AIN3 (0x1)  // AIN0 - AIN3
#define MUX_AIN1_AIN3 (0x2)  // AIN1 - AIN3
#define MUX_AIN2_AIN3 (0x3)  // AIN2 - AIN3

#define MUX_AIN0_GND  (0x4 << 12)  // AIN0 - GND
#define MUX_AIN1_GND  (0x5 << 12)  // AIN1 - GND
#define MUX_AIN2_GND  (0x6 << 12)  // AIN2 - GND
#define MUX_AIN3_GND  (0x7 << 12)  // AIN3 - GND

// PGA[2:0] - gain settings (config bits 11:9)
#define PGA_6_144V     (0x0 << 9)  // ±6.144V
#define PGA_4_096V     (0x1 << 9)  // ±4.096V
#define PGA_2_048V     (0x2 << 9)  // ±2.048V (default)
#define PGA_1_024V     (0x3 << 9)  // ±1.024V
#define PGA_0_512V     (0x4 << 9)  // ±0.512V
#define PGA_0_256V     (0x5 << 9)  // ±0.256V

// MODE bit - operating mode (config bit 8)
#define MODE_CONTINUOUS  (0x0 << 8)  // Continuous conversion
#define MODE_SINGLESHOT  (0x1 << 8)  // Single-shot (default)

// DR[2:0] - data rate (config bits 7:5)
#define DR_128SPS        (0x4 << 5)  // 128 samples per second (default)
#define DR_860SPS        (0x7 << 5)  // 860 SPS

// Comparator settings (bits 4:0)
#define COMP_MODE_TRAD   (0x0 << 4)  // Traditional comparator
#define COMP_MODE_WINDOW (0x1 << 4)  // Window comparator

#define COMP_POL_LOW     (0x0 << 3)  // ALERT/RDY active low (default)
#define COMP_POL_HIGH    (0x1 << 3)  // ALERT/RDY active high

#define COMP_LAT_NON     (0x0 << 2)  // Non-latching comparator
#define COMP_LAT_LATCH   (0x1 << 2)  // Latching comparator

#define COMP_QUE_DISABLE (0x3 << 0)  // Disable comparator (default)
#define COMP_QUE_1CONV   (0x0 << 0)  // Trigger after 1 conversion
#define COMP_QUE_2CONV   (0x1 << 0)  // Trigger after 2 conversions
#define COMP_QUE_4CONV   (0x2 << 0)  // Trigger after 4 conversions

// OS bit - start single-shot conversion
#define OS_SINGLE        (0x1 << 15)  // Writing 1 triggers conversion

// Alternative readable aliases for differential and single-ended input MUX
#define MUX_DIFF_AIN0_AIN1  (0x0 << 12)  // 000b : AIN0 - AIN1 (default)
#define MUX_DIFF_AIN0_AIN3  (0x1 << 12)  // 001b : AIN0 - AIN3
#define MUX_DIFF_AIN1_AIN3  (0x2 << 12)  // 010b : AIN1 - AIN3
#define MUX_DIFF_AIN2_AIN3  (0x3 << 12)  // 011b : AIN2 - AIN3

#define MUX_SINGLE_AIN0     (0x4 << 12)  // 100b : AIN0 - GND
#define MUX_SINGLE_AIN1     (0x5 << 12)  // 101b : AIN1 - GND
#define MUX_SINGLE_AIN2     (0x6 << 12)  // 110b : AIN2 - GND
#define MUX_SINGLE_AIN3     (0x7 << 12)  // 111b : AIN3 - GND

class ADS1xx5_7semi {
public:
  // Constructor (default address: 0x48, default wire: Wire)
  ADS1xx5_7semi(uint8_t address = 0x48, TwoWire &wire = Wire);

  // Initialize I2C and set default config
  bool begin();

  // Comparator threshold controls
  void setLowThreshold(int16_t value);     // Set comparator low threshold
  void setHighThreshold(int16_t value);    // Set comparator high threshold
  int16_t getLowThreshold();               // Read low threshold
  int16_t getHighThreshold();              // Read high threshold

  // Reference voltage setting via PGA gain
  bool setRefV(uint16_t pga_bits);         // Set PGA to adjust ref voltage
  float getRefV();                         // Return current reference voltage

  // Single-ended voltage read (AINx vs GND)
  float readCH(uint8_t channel);           // Read in volts
  int16_t readRawCH(uint8_t channel);      // Read raw ADC value

  // Differential read between AINx and AINy (0–3 mux setting)
  float readDiff(uint8_t mux_setting);     // Read differential voltage

private:
  // Internal register read/write helpers
  bool writeReg(uint8_t reg, uint16_t value);
  uint16_t readReg(uint8_t reg);

  uint8_t _address;        // I2C address
  TwoWire *_wire;          // Pointer to I2C object
};

#endif  // _7SEMI_ADS1XX5_H_
