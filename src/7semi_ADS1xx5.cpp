/***********************************************************************/
//  Arduino Library for ADS1115 16-bit ADC
//  Developed by 7semi
//  Device: ADS1115 (Texas Instruments)
//  Features: Differential and single-ended voltage reading, PGA control, threshold config
//  Communication: I2C
/***********************************************************************/

#include "7semi_ADS1xx5.h"

// Constructor
ADS1xx5_7semi::ADS1xx5_7semi(uint8_t address, TwoWire &wire) {
  _address = address;
  _wire = &wire;
}

// Begin communication and configure default settings
bool ADS1xx5_7semi::begin() {
  _wire->begin();
  _wire->beginTransmission(_address);
  if (_wire->endTransmission() != 0)
    return false;

  // Default config: AIN0-GND, ±2.048V, single-shot, 128 SPS
  uint16_t config =
    OS_SINGLE |
    MUX_SINGLE_AIN0 |
    PGA_2_048V |
    MODE_SINGLESHOT |
    DR_128SPS |
    COMP_MODE_TRAD |
    COMP_POL_LOW |
    COMP_LAT_NON |
    COMP_QUE_DISABLE;

  return writeReg(REG_CONFIG, config);
}

// Write 16-bit value to register
bool ADS1xx5_7semi::writeReg(uint8_t reg, uint16_t value) {
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->write(value >> 8);
  _wire->write(value & 0xFF);
  return (_wire->endTransmission() == 0);
}

// Read 16-bit value from register
uint16_t ADS1xx5_7semi::readReg(uint8_t reg) {
  _wire->beginTransmission(_address);
  _wire->write(reg);
  if (_wire->endTransmission() != 0) return 0xFFFF;

  if (_wire->requestFrom(_address, (uint8_t)2) != 2) return 0xFFFF;

  uint16_t value = _wire->read() << 8;
  value |= _wire->read();
  return value;
}

// Set comparator low threshold
void ADS1xx5_7semi::setLowThreshold(int16_t value) {
  writeReg(REG_LO_THRESH, value);
}

// Set comparator high threshold
void ADS1xx5_7semi::setHighThreshold(int16_t value) {
  writeReg(REG_HI_THRESH, value);
}

// Get comparator low threshold
int16_t ADS1xx5_7semi::getLowThreshold() {
  return (int16_t)readReg(REG_LO_THRESH);
}

// Get comparator high threshold
int16_t ADS1xx5_7semi::getHighThreshold() {
  return (int16_t)readReg(REG_HI_THRESH);
}

// Set reference voltage via PGA gain bits
bool ADS1xx5_7semi::setRefV(uint16_t pga_bits) {
  uint16_t config = readReg(REG_CONFIG);
  config &= ~(0x07 << 9);  // Clear PGA[2:0]
  config |= pga_bits;      // Set new PGA bits
  return writeReg(REG_CONFIG, config);
}

// Get reference voltage based on current PGA setting
float ADS1xx5_7semi::getRefV() {
  uint16_t config = readReg(REG_CONFIG);
  uint16_t pga = (config >> 9) & 0x07;

  switch (pga) {
    case 0: return 6.144;
    case 1: return 4.096;
    case 2: return 2.048;
    case 3: return 1.024;
    case 4: return 0.512;
    default: return 0.256;
  }
}

// Read single-ended voltage from specified channel (0-3)
float ADS1xx5_7semi::readCH(uint8_t channel) {
  int16_t raw = readRawCH(channel);
  float vref = getRefV();
  float lsb = vref / 32768.0;
  return raw * lsb;
}

// Read raw ADC value from single-ended input
int16_t ADS1xx5_7semi::readRawCH(uint8_t channel) {
  if (channel > 3) return 0;

  uint16_t mux = (0x4 + channel) << 12;  // MUX for AINx vs GND
  uint16_t config =
    OS_SINGLE |
    mux |
    (readReg(REG_CONFIG) & (0x07 << 9)) |
    MODE_SINGLESHOT |
    DR_128SPS |
    COMP_MODE_TRAD |
    COMP_POL_LOW |
    COMP_LAT_NON |
    COMP_QUE_DISABLE;

  writeReg(REG_CONFIG, config);

  // Wait for conversion to complete
  while ((readReg(REG_CONFIG) & OS_SINGLE) == 0);

  return (int16_t)readReg(REG_CONVERSION);
}

// Read differential voltage between pairs based on mux setting (0–3)
float ADS1xx5_7semi::readDiff(uint8_t mux_setting) {
  if (mux_setting > 3) return 0.0;

  uint16_t mux = (mux_setting & 0x07) << 12;
  uint16_t config =
    OS_SINGLE |
    mux |
    (readReg(REG_CONFIG) & (0x07 << 9)) |
    MODE_SINGLESHOT |
    DR_128SPS |
    COMP_MODE_TRAD |
    COMP_POL_LOW |
    COMP_LAT_NON |
    COMP_QUE_DISABLE;

  writeReg(REG_CONFIG, config);

  // Wait for conversion
  while ((readReg(REG_CONFIG) & OS_SINGLE) == 0);

  int16_t raw = (int16_t)readReg(REG_CONVERSION);
  float vref = getRefV();
  float lsb = vref / 32768.0;

  return raw * lsb;
}
