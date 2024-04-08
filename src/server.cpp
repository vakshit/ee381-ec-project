#include <server.hpp>

LockServer::LockServer(uint32_t port, const char *events,
                       std::shared_ptr<Display> &display)
    : server(port), events(events), display(display) {
  return;
}

void LockServer::notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void LockServer::listen() {
  this->events.onConnect([](AsyncEventSourceClient *client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it gat is: %u\n",
                    client->lastId());
    }
  });

  // attach AsyncEventSource
  server.addHandler(&this->events);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
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

void LockServer::init() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(Config::ssid, Config::password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    display->text("WiFi Failed!\n");
    return;
  }
  display->text("IP Address: " + WiFi.localIP().toString());

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    display->text("An Error has occurred while mounting SPIFFS");
    return;
  }
}