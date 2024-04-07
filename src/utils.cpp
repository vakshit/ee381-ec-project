#include <utils.hpp>

uint8_t Utils::readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (!Serial.available())
      ;
    num = Serial.parseInt();
  }
  return num;
};

void Utils::flashNotAllowed() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(Config::RELAY_PIN, HIGH);
    delay(100);
    digitalWrite(Config::RELAY_PIN, LOW);
    delay(100);
  }
}

void Utils::checkPassword(const char *password) {}