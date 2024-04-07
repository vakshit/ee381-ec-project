#pragma once

#include "SPIFFS.h"
#include <Adafruit_Fingerprint.h>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <config.hpp>
#include <display.hpp>
#include <utils.hpp>

class FingerPrint {
private:
  AsyncWebServer server;
  AsyncEventSource events;
  HardwareSerial serialPort;
  Adafruit_Fingerprint finger;
  std::shared_ptr<Display> display;
  unsigned long previousMillis = 0;
  unsigned long previousOpenMillis = 0;
  bool isOpen = false;

public:
  FingerPrint(uint8_t serialPortNo);
  FingerPrint(uint8_t serialPortNo, std::shared_ptr<Display> &display);
  void connect();
  uint8_t getFingerprintID();
  void showNotAllowed();
  uint8_t enroll(uint8_t id);
  void listen();
};
