#pragma once

#include "SPIFFS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class LockServer {
private:
  AsyncWebServer server;
  AsyncEventSource events;
  static void notFound(AsyncWebServerRequest *request);

public:
  LockServer(uint32_t port, const char *events);
  void listen();
};
