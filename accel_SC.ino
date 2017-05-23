// Basic demo for accelerometer readings from Adafruit LIS3DH

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// Used for software I2C
//#define LIS3DH_CLK 9 
//#define LIS3DH_MOSI 10 

// Used for software SPI
//#define LIS3DH_CLK 13
//#define LIS3DH_MISO 12
//#define LIS3DH_MOSI 11

// Used for hardware & software SPI
#define LIS3DH_CS 5 ////Board-Pin "5"(choose any I/O-Pin from the Feather M0) connected to Sensor-Pin (LIS3DH) "CS"


// software SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);

// hardware SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS); //Used for SPI
//Board-Pin 24 "SCK" (Feather M0) connected to Sensor-Pin (LIS3DH) "SCL"
//Board-Pin 22 "MISO" (Feather M0) connected to Sensor-Pin (LIS3DH) "SDO"
//Board-Pin 23 "MOSI" (Feather M0) connected to Sensor-Pin (LIS3DH) "SDA"

// Used for hardware & software I2C
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(); //Used for I2C
//Board-Pin 21 "SCL" (Feather M0) connected to Sensor-Pin (LIS3DH) "SCL"
//Board-Pin 20 "SDA" (Feather M0) connected to Sensor-Pin (LIS3DH) "SDA"

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial Serial
#endif

void setup(void) {
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif

  Serial.begin(9600);
  Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");
}

void loop() {
  lis.read();      // get X Y and Z data at once
  // Then print out the raw data
  Serial.print("X:  "); Serial.print(lis.x); 
  Serial.print("  \tY:  "); Serial.print(lis.y); 
  Serial.print("  \tZ:  "); Serial.print(lis.z); 

  /* Or....get a new sensor event, normalized */ 
  sensors_event_t event; 
  lis.getEvent(&event);
  
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  Serial.print(" \tY: "); Serial.print(event.acceleration.y); 
  Serial.print(" \tZ: "); Serial.print(event.acceleration.z); 
  Serial.println(" m/s^2 ");

  Serial.println();
 
  delay(200); 
}
