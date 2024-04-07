#pragma once
#include <stdint.h>

class Config {
public:
  static const uint8_t SERIAL_PORT_NO = 2;
  static const uint8_t RELAY_PIN = 2;
  static const uint8_t BUZZER_PIN = 22;
  static const long READ_INTERVAL = 1000;
  static const long CLOSE_INTERVAL = 5000;
  static const char *ssid;
  static const char *password;
  static const char *ipAddress;
  static const uint16_t port = 80;
  static const char *events;
  static const char *notFound;
  static const char *adminPassword;
  static const char *fingerPrintPassword;
};
