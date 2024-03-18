#include <utils.hpp>

uint8_t Utils::readnumber(void){
  uint8_t num = 0;

  while (num == 0)
  {
    while (!Serial.available())
      ;
    num = Serial.parseInt();
  }
  return num;
};