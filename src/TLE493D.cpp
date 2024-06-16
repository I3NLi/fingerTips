#include "TLE493D.h"

TLE493D::TLE493D(int sdaPin, int sclPin, uint8_t address)
  : wire(Wire), sdaPin(sdaPin), sclPin(sclPin), address(address), xOffset(0), yOffset(0), zOffset(0) {}

// TLE493D::TLE493D(TwoWire& wire, int sdaPin, int sclPin, uint8_t address)
//   : wire(wire), sdaPin(sdaPin), sclPin(sclPin), address(address), xOffset(0), yOffset(0), zOffset(0) {}

void TLE493D::begin() {
  wire.begin(sdaPin, sclPin, 400000);
  configureSensor();
  calibrate();
}

void TLE493D::configureSensor() {
  wire.begin(sdaPin, sclPin, 400000);
  wire.beginTransmission(address);
  wire.write(0x10); 
  wire.write(0x21); 
  wire.write(0x15); 
  wire.endTransmission();
  wire.end();
}

void TLE493D::calibrate() {
  wire.begin(sdaPin, sclPin, 400000);

  uint8_t buf[7];
  wire.requestFrom(address, (uint8_t)7);
  for (uint8_t i = 0; i < 7; i++) {
    buf[i] = wire.read();
  }

  xOffset = (int16_t)(((buf[0] << 8) | (buf[4] & 0xF0)) >> 4);
  yOffset = (int16_t)(((buf[1] << 8) | ((buf[4] & 0x0F) << 4)) >> 4);
  zOffset = (int16_t)(((buf[2] << 8) | ((buf[5] & 0x0F) << 4)) >> 4);
  uint16_t T = ((buf[3] << 8) | buf[5]) >> 6;
  Serial.print("Calibration offsets: ");
  Serial.print(xOffset);
  Serial.print(", ");
  Serial.print(yOffset);
  Serial.print(", ");
  Serial.print(zOffset);
  Serial.print(", ");
  Serial.println(T);

  for (int16_t i = 7; i > -1; i--) {
    uint8_t plot = buf[6] >> i;
    Serial.print(plot & 0x01);
    if (i == 4) { Serial.print(" "); }
  }
  Serial.println();
  wire.end();

}

SensorData TLE493D::readData() {
    wire.begin(sdaPin, sclPin, 400000);


  SensorData data;
  wire.requestFrom(address, (uint8_t)7);
  uint8_t buf[7];
  for (uint8_t i = 0; i < 7; i++) {
    buf[i] = wire.read();
  }

  data.X = ((int16_t)(((buf[0] << 8) | (buf[4] & 0xF0)) >> 4)) - xOffset;
  data.Y = ((int16_t)(((buf[1] << 8) | ((buf[4] & 0x0F) << 4)) >> 4)) - yOffset;
  data.Z = ((int16_t)(((buf[2] << 8) | ((buf[5] & 0x0F) << 4)) >> 4)) - zOffset;
  data.T = ((buf[3] << 8) | buf[5]) >> 6;
  wire.end();

  return data;
}

void TLE493D::printData(const SensorData& data) {
  Serial.print(data.X);
  Serial.print(", ");
  Serial.print(data.Y);
  Serial.print(", ");
  Serial.print(data.Z);
  Serial.print(", ");
  Serial.println(data.T);
}

String TLE493D::getDataAsJson(const SensorData& data) {
  String json = "{";
  json += "\"x\": " + String(data.X) + ",";
  json += "\"y\": " + String(data.Y) + ",";
  json += "\"z\": " + String(data.Z) + ",";
  json += "\"t\": " + String(data.T);
  json += "}";
  return json;
}
