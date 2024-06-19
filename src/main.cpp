#include <Arduino.h>
#include "TLE493D.h"
#include <WiFi.h>

/** Wifi */
const char* ssid = "FingerTips";
const char* password = "FingerTips";

WiFiServer server(80); // Set web server port number to 80

/** Sensor */
const int SDA_PINS[] = {26,27,14}; // 根据 ESP32 的引脚定义
const int NUM_SENSORS = sizeof(SDA_PINS) / sizeof(SDA_PINS[0]);
const int SCL_PIN = 22; // 例如使用的 SCL 引脚
TLE493D* sensors[NUM_SENSORS];

void setup() {
  Serial.begin(115200);
  Serial.println("Fingertips demo");

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // 初始化每个传感器
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensors[i] = new TLE493D(SDA_PINS[i], SCL_PIN);
    // sensors[i]->begin();
  }

  Serial.println("Bx, By, Bz, T, debug");
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    String header = "";                            // Clear header at the beginning
    String requestPath = "";   
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n' ) {                    // if the byte is a newline character
          if (currentLine.length() == 0) {  // if the current line is blank, you got two newline characters in a row
            if (header.indexOf("GET /sse") >= 0) {
              // Send HTTP headers for SSE
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/event-stream");
              client.println("Cache-Control: no-cache");
              client.println("Connection: keep-alive");
              client.println("Access-Control-Allow-Origin: *");
              client.println();
              requestPath = "/sse";
            } else if (header.indexOf("GET /data") >= 0) {
              // Send HTTP headers for JSON
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: application/json");
              client.println("Connection: close");
              client.println("Access-Control-Allow-Origin: *");
              client.println();
              requestPath = "/data";
            } else {
              // Handle 404 Not Found
              client.println("HTTP/1.1 404 Not Found");
              client.println("Connection: close");
              client.println();
              requestPath = "/404";
            }
            break;
          } else {
            currentLine = ""; // Clear currentLine for new data
          }
        } else if (c != '\r') {
          currentLine += c;  // Add character to currentLine if it's not a carriage return
        }
      }
    }

    if (requestPath == "/sse") {
      // Start sending data as Server-Sent Events (SSE)
      while (client.connected()) {
        String json;
        json.reserve(256);  // Reserve memory to avoid fragmentation
        json = "{\"timestamp\":" + String(millis()) + ","; // Add timestamp
        for (int i = 0; i < NUM_SENSORS; i++) {
          SensorData data = sensors[i]->readMockData();
          json += "\"sensor" + String(i) + "\":" + sensors[i]->getDataAsJson(data);
          if (i < NUM_SENSORS - 1) json += ",";
        }
        json += "}";

        // Send SSE event
        client.print("data: ");
        client.println(json);
        client.print("\n\n");  // Each SSE message is terminated with a double newline

        // Print the JSON data to serial monitor (optional for debugging)
        Serial.println(json);

        delay(100); // Send data every second, adjust as needed
      }
    } else if (requestPath == "/data") {
      // Send one-time JSON data response
      String json;
      json.reserve(256);  // Reserve memory to avoid fragmentation
      json = "{\"timestamp\":" + String(millis()) + ","; // Add timestamp
      for (int i = 0; i < NUM_SENSORS; i++) {
        SensorData data = sensors[i]->readMockData();
        json += "\"sensor" + String(i) + "\":" + sensors[i]->getDataAsJson(data);
        if (i < NUM_SENSORS - 1) json += ",";
      }
      json += "}";

      // Send JSON response
      client.println(json);
      Serial.println(json);

      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
  }
}
