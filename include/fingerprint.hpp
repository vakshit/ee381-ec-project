#pragma once

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include <config.hpp>

class FingerPrint {
  private:
    AsyncWebServer server;
    AsyncEventSource events;
    HardwareSerial serialPort;
    Adafruit_Fingerprint finger;
    unsigned long previousMillis = 0;
    unsigned long previousOpenMillis = 0;
    bool isOpen = false;

  public:
    FingerPrint(uint8_t serialPortNo);
    void connect();
    uint8_t getFingerprintID();
    void showNotAllowed();
    uint8_t enroll(uint8_t id);
    void listen();
};
