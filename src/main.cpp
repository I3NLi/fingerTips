#include <Arduino.h>
#include "TLE493D.h"
#include <WiFi.h>

/** Wifi */
const char* ssid = "FingerTips";
const char* password = "FingerTips";

WiFiServer server(80); // Set web server port number to 80
String header; // Variable to store the HTTP request
unsigned long currentTime; // Current time
unsigned long previousTime = 0; // Previous time
const long timeoutTime = 2000; // Define timeout time in milliseconds (example: 2000ms = 2s)

/** Sensor */
const int SDA_PINS[] = {21, 19, 18}; // 根据 ESP32 的引脚定义
const int NUM_SENSORS = sizeof(SDA_PINS) / sizeof(SDA_PINS[0]);
const int SCL_PIN = 22; // 例如使用的 SCL 引脚
TLE493D* sensors[NUM_SENSORS];

void setup() {
  Serial.begin(115200);
  Serial.println("Fingertips demo");

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // 初始化每个传感器
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensors[i] = new TLE493D(SDA_PINS[i], SCL_PIN);
    sensors[i]->begin();
  }

  Serial.println("Bx, By, Bz, T, debug");
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    header = "";                            // Clear header at the beginning

    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {  // if the current line is blank, you got two newline characters in a row
            // Send HTTP response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type: application/json");
            client.println("Connection: close");
            client.println();

            // 读取每个传感器的数据并打印
            String json;
            json.reserve(256);  // Reserve memory to avoid fragmentation
            json = "{";
            for (int i = 0; i < NUM_SENSORS; i++) {
              SensorData data = sensors[i]->readData();
              json += "\"sensor" + String(i) + "\":" + sensors[i]->getDataAsJson(data);
              if (i < NUM_SENSORS - 1) json += ",";
            }
            json += "}";

            client.println(json);
            Serial.println(json);

            break;
          } else {
            currentLine = ""; // Clear currentLine for new data
          }
        } else if (c != '\r') {
          currentLine += c;  // Add character to currentLine if it's not a carriage return
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
