#include <config.hpp>
#include <display.hpp>
#include <fingerprint.hpp>
#include <server.hpp>
#include <utils.hpp>

#include <Keypad.h>

#define ROWS 4
#define COLS 4

char keyMap[ROWS][COLS] = {{'1', '2', '3', 'A'},
                           {'4', '5', '6', 'B'},
                           {'7', '8', '9', 'C'},
                           {'*', '0', '#', 'D'}};

uint8_t rowPins[ROWS] = {14, 27, 26, 25}; // GIOP14, GIOP27, GIOP26, GIOP25
uint8_t colPins[COLS] = {33, 32, 18, 19}; // GIOP33, GIOP32, GIOP18, GIOP19
Keypad keypad = Keypad(makeKeymap(keyMap), rowPins, colPins, ROWS, COLS);

const char *Config::ssid = "Akshit-DLINK";
const char *Config::password = "1q2w3e4r5t";
const char *Config::adminPassword = "2245";
const char *Config::fingerPrintPassword = "1234";
const char *Config::events = "/events";

std::shared_ptr<Display> display = std::make_shared<Display>();
FingerPrint finger(Config::SERIAL_PORT_NO, display);
// LockServer server(Config::port, Config::events);

int mode = 0; // 0 for normal mode, 1 for enroll mode, 2 for typing mode
std::string typedPassword = "";

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  delay(100);
  display->init();
  finger.connect();
  // server.listen();

  // initialize Relay digital pin as an output.
  pinMode(Config::RELAY_PIN, OUTPUT);
  digitalWrite(Config::RELAY_PIN, LOW);

  // configure buzzer and led pin
  pinMode(Config::BUZZER_PIN, OUTPUT);
}

void loop() // run over and over again
{
  char key = keypad.getKey();
  if (key) {
    if (key == 'A') {
      display->text("Type Admin Password");
      typedPassword.clear();
      mode = 2;
    } else if (key == 'B') {
      display->text("Type FingerPrint Password");
      typedPassword.clear();
      mode = 3;
    }
  }
  if (mode == 0) {
    finger.listen();
  } else if (mode == 1) {
    display->text("Ready to enroll a fingerprint!");
    display->text("Please type in the ID # (from 1 to 127) you want to save "
                  "this finger as...");
    std::string idString = "";
    while (true) {
      key = keypad.getKey();
      if (key) {
        if (key == 'D')
          break;
        else {
          idString += key;
          display->text(idString.c_str());
        }
      }
    }
    if (idString == "") {
      mode = 0;
      return;
    }
    uint8_t id = std::stoi(idString);
    if (id == 0 || id > 127) { // ID #0 not allowed, try again!
      display->text("Enter value between 0-127");
      return;
    }
    display->text("Enrolling ID " + String(id));
    while (!finger.enroll(id))
      ;
    delay(2000);
    mode = 0;
  } else if (mode == 2) {
    while (true) {
      key = keypad.getKey();
      if (key) {
        if (key == 'D') {
          mode = 0;
          break;
        } else if ((key - '0') >= 0 && (key - '0') <= 9) {
          typedPassword += key;
          display->text(std::string(typedPassword.size(), '*').c_str());
          if (typedPassword == Config::adminPassword) {
            mode = 1;
            break;
          }
        }
      }
    }
  } else if (mode == 3) {
    while (true) {
      key = keypad.getKey();
      if (key) {
        if (key == 'D') {
          mode = 0;
          break;
        } else if ((key - '0') >= 0 && (key - '0') <= 9) {
          typedPassword += key;
          display->text(std::string(typedPassword.size(), '*').c_str());
          if (typedPassword == Config::fingerPrintPassword) {
            mode = 0;
            display->text("Access Granted! Opening Door lock...");
            digitalWrite(Config::RELAY_PIN, HIGH);
            delay(5000);
            display->text("Closing Door lock...");
            digitalWrite(Config::RELAY_PIN, LOW);
            break;
          }
        }
      }
    }
  }
}
