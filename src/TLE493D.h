#ifndef TLE493D_H
#define TLE493D_H

#include <Arduino.h>
#include <Wire.h>

struct SensorData {
  int16_t X;
  int16_t Y;
  int16_t Z;
  uint16_t T;
};

class TLE493D {
public:
  TLE493D(int sdaPin, int sclPin, uint8_t address = 0x35);
  TLE493D(TwoWire& wire, int sdaPin, int sclPin, uint8_t address = 0x35);
  void begin();
  void configureSensor();
  void calibrate();
  SensorData readData();
  void printData(const SensorData& data);
  String getDataAsJson(const SensorData& data);
  void end(); // Method to stop the I2C interface

private:
  uint8_t address;
  TwoWire& wire;
  int16_t xOffset, yOffset, zOffset;
  int sdaPin, sclPin;
};

#endif // TLE493D_H
