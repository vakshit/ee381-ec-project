#pragma once

#include <Arduino.h>
#include <config.hpp>
#include <stdint.h>

class Utils {
public:
  static uint8_t readnumber(void);
  static void flashNotAllowed();
  static void checkPassword(const char *password);
};
