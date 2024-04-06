#include <config.hpp>
#include <display.hpp>
#include <fingerprint.hpp>
#include <server.hpp>
#include <utils.hpp>

const char *Config::ssid = "Akshit-DLINK";
const char *Config::password = "1q2w3e4r5t";
const char *Config::events = "/events";
std::shared_ptr<Display> display = std::make_shared<Display>();
FingerPrint finger(Config::SERIAL_PORT_NO, display);
// LockServer server(Config::port, Config::events);

int mode = 0; // 0 for normal mode, 1 for enroll mode

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  delay(100);
  display->init();
  display->text("Adafruit Fingerprint sensor enrollment");
  finger.connect();
  // server.listen();

  // initialize Relay digital pin as an output.
  pinMode(Config::RELAY_PIN, OUTPUT);
  digitalWrite(Config::RELAY_PIN, HIGH);

  // configure buzzer and led pin
  pinMode(Config::BUZZER_PIN, OUTPUT);
}

void loop() // run over and over again
{
  if (mode) {
    display->text("Ready to enroll a fingerprint!");
    display->text("Please type in the ID # (from 1 to 127) you want to save"
                  "this finger as...");
    uint8_t id = Utils::readnumber();
    if (id == 0) { // ID #0 not allowed, try again!
      return;
    }
    Serial.print("Enrolling ID #");
    Serial.println(id);
    while (!finger.enroll(id))
      ;
  } else {
    finger.listen();
  }
}
