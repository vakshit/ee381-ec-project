#include <fingerprint.hpp>
#include <config.hpp>
#include <utils.hpp>
#include <server.hpp>

const char* Config::ssid = "Akshit-DLINK";
const char* Config::password = "1q2w3e4r5t";
const char* Config::events = "/events";
FingerPrint finger(Config::SERIAL_PORT_NO);
LockServer server(Config::port, Config::events);
int mode = 0; // 0 for normal mode, 1 for enroll mode

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
  finger.connect();
  server.listen();


  // initialize Relay digital pin as an output.
  pinMode(Config::RELAY_PIN, OUTPUT);
  digitalWrite(Config::RELAY_PIN, HIGH);

  // configure buzzer and led pin
  pinMode(Config::BUZZER_PIN, OUTPUT);
}

void loop() // run over and over again
{
  if (mode) {
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
  } else {
    finger.listen();
  }
}

