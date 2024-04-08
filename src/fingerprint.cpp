#include <fingerprint.hpp>

FingerPrint::FingerPrint(uint8_t serialPortNo)
    : serialPort(serialPortNo), finger(&serialPort) {
  return;
}

FingerPrint::FingerPrint(uint8_t serialPortNo,
                         std::shared_ptr<Display> &display,
                         std::shared_ptr<LockServer> &server)
    : serialPort(serialPortNo), finger(&serialPort), display(display),
      server(server) {
  return;
}

void FingerPrint::connect() {
  this->finger.begin(57600);

  if (this->finger.verifyPassword()) {
    this->display->text("Found fingerprint sensor!");
  } else {
    this->display->text("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  Serial.println(F("Reading sensor parameters"));
  this->finger.getParameters();
  Serial.print(F("Status: 0x"));
  Serial.println(this->finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x"));
  Serial.println(this->finger.system_id, HEX);
  Serial.print(F("Capacity: "));
  Serial.println(this->finger.capacity);
  Serial.print(F("Security level: "));
  Serial.println(this->finger.security_level);
  Serial.print(F("Device address: "));
  Serial.println(this->finger.device_addr, HEX);
  Serial.print(F("Packet len: "));
  Serial.println(this->finger.packet_len);
  Serial.print(F("Baud rate: "));
  Serial.println(this->finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    this->display->text(
        "Sensor doesn't contain any fingerprint data. Please enroll "
        "some fingerprints.");
  } else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
  }
}

uint8_t FingerPrint::enroll(uint8_t id) {
  int p = -1;
  this->display->text("Waiting for valid finger to enroll as #");
  this->display->text(String(id));
  while (p != FINGERPRINT_OK) {
    p = this->finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = this->finger.image2Tz(1);
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  Serial.println("Remove finger");
  display->text("Remove Finger");
  server->events.send("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = this->finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  display->text("Remove Finger");
  server->events.send("Remove Finger");
  while (p != FINGERPRINT_OK) {
    p = this->finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = this->finger.image2Tz(2);
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  // OK converted!
  Serial.print("Creating model for #");
  Serial.println(id);

  p = this->finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID ");
  Serial.println(id);
  p = this->finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}

uint8_t FingerPrint::getFingerprintID() {
  uint8_t p = this->finger.getImage();
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image taken");
    break;
  case FINGERPRINT_NOFINGER:
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_IMAGEFAIL:
    Serial.println("Imaging error");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  // OK success!

  p = this->finger.image2Tz();
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  // OK converted!
  p = this->finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    // found a match!
    Serial.print("Found ID #");
    Serial.print(this->finger.fingerID);
    Serial.print(" with confidence of ");
    Serial.println(this->finger.confidence);

    return p;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}

void FingerPrint::listen() {
  unsigned long currentMillis = millis();
  // Close the door lock after 5 seconds
  if (this->isOpen &&
      currentMillis - this->previousOpenMillis >= Config::CLOSE_INTERVAL) {
    this->isOpen = false;
    this->display->text("Closing the door lock!");
    Serial.println("Closing the door lock!");
    digitalWrite(Config::RELAY_PIN, HIGH);
  }

  // save the last time we read the fingerprint sensor
  if (!this->isOpen &&
      currentMillis - this->previousMillis >= Config::READ_INTERVAL) {
    this->previousMillis = currentMillis;
    uint8_t result = getFingerprintID();
    switch (result) {
    case FINGERPRINT_NOFINGER:
      this->display->text("Scan your fingerprint");
      server->events.send("Scan your fingerprint", "noFingerprint", millis());
      break;
    case FINGERPRINT_OK:
      Serial.println("Access Granted..opening door lock!");
      this->display->text("Access Granted..opening door lock!");
      server->events.send("Access Granted", "accessGranted", millis());
      this->previousOpenMillis = millis();
      this->isOpen = true;
      digitalWrite(Config::RELAY_PIN, LOW);
      break;
    case FINGERPRINT_NOTFOUND:
      Serial.println("Access Denied");
      this->display->text("Access Denied");
      server->events.send("Access Denied", "accessDenied", millis());
      Utils::flashNotAllowed();
      delay(2000);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
    case FINGERPRINT_IMAGEFAIL:
    case FINGERPRINT_IMAGEMESS:
    case FINGERPRINT_FEATUREFAIL:
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Error in Fingerprint Scan!");
      this->display->text("Error in Fingerprint Scan!");
      server->events.send("Unknown Error", "unknownError", millis());
      break;
    default:
      Serial.println("Unknown Error!");
      this->display->text("Unknown Error!");
      break;
    }
  }
}