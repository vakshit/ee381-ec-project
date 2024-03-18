#include <fingerprint.hpp>
#include <config.hpp>
#include <utils.hpp>

FingerPrint finger(Config::SERIAL_PORT_NO);

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
  finger.connect();
}

void loop() // run over and over again
{
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  uint8_t id = Utils::readnumber();
  if (id == 0)
  { // ID #0 not allowed, try again!
    return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  while (!finger.enroll(id));
}

