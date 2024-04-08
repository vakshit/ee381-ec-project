#pragma once

#include "SPIFFS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <config.hpp>
#include <display.hpp>

class LockServer {
private:
  AsyncWebServer server;
  static void notFound(AsyncWebServerRequest *request);
  std::shared_ptr<Display> display;

public:
  AsyncEventSource events;
  LockServer(uint32_t port, const char *events,
             std::shared_ptr<Display> &display);
  void init();
  void listen();
};
