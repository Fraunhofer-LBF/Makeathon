#include "UserTypes.h"
// User data functions.  Modify these functions for your data items.

// Start time for data
static uint32_t startMicros;

const uint8_t LIS3D_CS = 10;

const uint8_t LIS3DH_REG_CTRL1 = 0x20;
const uint8_t LIS3DH_REG_CTRL4 = 0x23;

const uint8_t LIS3DH_REG_OUT_X_L = 0x28; //X-Axis Data 0
const uint8_t LIS3DH_REG_OUT_X_H = 0x29; //X-Axis Data 1
const uint8_t LIS3DH_REG_OUT_Y_L = 0x2A; //Y-Axis Data 0
const uint8_t LIS3DH_REG_OUT_Y_H = 0x2B; //Y-Axis Data 1
const uint8_t LIS3DH_REG_OUT_Z_L = 0x2C; //Z-Axis Data 0
const uint8_t LIS3DH_REG_OUT_Z_H = 0x2D; //Z-Axis Data 1

void writeADXL345Register(const uint8_t registerAddress, const uint8_t value) {
  // Max SPI clock frequency is 5 MHz with CPOL = 1 and CPHA = 1.
  SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));  
  digitalWrite(LIS3D_CS, LOW);
  SPI.transfer(registerAddress);
  SPI.transfer(value);
  digitalWrite(LIS3D_CS, HIGH);
  SPI.endTransaction();  
}

void userSetup() {
  SPI.begin();
  pinMode(LIS3D_CS, OUTPUT);
  digitalWrite(LIS3D_CS, HIGH);

  // enable all axes, high res, 400 Hz
  // ODR3 | ODR2  | ODR1 | ODR0 | LPen | Zen  | Yen | Xen  |
  //   0  |   1   |   1  |   1  |   0  |  1   |  1  |   1  |
  writeADXL345Register(LIS3DH_REG_CTRL1, 0x77);
 
  // High res, BDU enabled, +/- 16G
  // BDU  |  BLE  |  FS1 |  FS0 |   HR | ST1  | ST0 | SIM  |
  //   1  |   0   |   1  |   1  |   1  |  0   |  0  |   0  |
  writeADXL345Register(LIS3DH_REG_CTRL4, 0xB8);  
 
}

// Acquire a data record.
void acquireData(data_t* data) {
  // Max SPI clock frequency is 5 MHz with CPOL = 1 and CPHA = 1.
  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE3));
  data->time = micros();
  digitalWrite(LIS3D_CS, LOW);
  // Read multiple bytes so or 0XC0 with address.
  SPI.transfer(LIS3DH_REG_OUT_X_L | 0XC0);
  data->accel[0] = SPI.transfer(0) | (SPI.transfer(0) << 8);
  data->accel[1] = SPI.transfer(0) | (SPI.transfer(0) << 8);
  data->accel[2] = SPI.transfer(0) | (SPI.transfer(0) << 8); 
  digitalWrite(LIS3D_CS, HIGH);
  SPI.endTransaction();
}

// Print a data record.
void printData(Print* pr, data_t* data) {
  if (startMicros == 0) {
    startMicros = data->time;
  }
  pr->print(data->time - startMicros);
  for (int i = 0; i < ACCEL_DIM; i++) {
    pr->write(',');
    pr->print(data->accel[i]);
  }
  pr->println();
}

// Print data header.
void printHeader(Print* pr) {
  startMicros = 0;
  pr->println(F("micros,ax,ay,az"));
}
