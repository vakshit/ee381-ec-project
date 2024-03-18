#include <server.hpp>

LockServer::LockServer(uint32_t port, const char* events) : server(port), events(events) {
  return;
}

void LockServer::notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void LockServer::listen() {
  this->events.onConnect([](AsyncEventSourceClient *client){
    if (client->lastId())
    {
      Serial.printf("Client reconnected! Last message ID that it gat is: %u\n", client->lastId());
    } 
  });

  // attach AsyncEventSource
  server.addHandler(&this->events);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      Serial.println("Requesting index page...");
      request->send(SPIFFS, "/index.html", "text/html", false); 
    });
  server.serveStatic("/css", SPIFFS, "/css/");
  server.serveStatic("/js", SPIFFS, "/js/");

  // On Not Found
  server.onNotFound(this->notFound);

  // Start server
  server.begin();
}
