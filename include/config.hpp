#pragma once
#include <stdint.h>

class Config {
public:
  static const uint8_t SERIAL_PORT_NO = 2;
  static const uint8_t RELAY_PIN = 23;
  static const uint8_t buzzerPin = 22;
  static const long readInterval = 1000;
  static const long closeInterval = 5000;
  static const char *ssid;
  static const char *password;
  static const char *ipAddress;
  static const uint16_t port;
  static const char *events;
  static const char *notFound;
};
