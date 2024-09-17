#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "index.h"


const char* ssid = "SSID_HERE"; // CHANGE IT
const char* password = "PASS_HERE"; // CHANGE IT

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    //data[len] = 0;
    //String message = (char*)data;
    // Check if the message is "getReadings"
    if (strncmp((char*)data, "JST:", 4) == 0) {
      //if it is, send current sensor readings
      float x, y;
      int isValidString = sscanf((char*)data, "JST: %f, %f", &x, &y);
      if (isValidString == 2){
        Serial.printf("JOYSTICK: %f, %f\n", x, y);
      } else {
        Serial.println("INVALID JOYSTICK DATA");
      }
    } else if (strncmp((char*)data, "BTN:", 4) == 0) {
      char btn;
      int isValidString = sscanf((char*)data, "BTN: %c", &btn);
      if (isValidString == 1){
        Serial.printf("BUTTON: %c\n", btn);
      } else {
        Serial.println("INVALID BUTTON DATA");
      }
    }
  }
}



void webSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}




void initWebSocket() {
  ws.onEvent(webSocketEvent);
  server.addHandler(&ws);
}


void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print the ESP32's IP address
  Serial.print("ESP32 Web Server's IP address: ");
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Define a route to serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("ESP32 Web Server: New request received:");  // for debugging
    Serial.println("GET /");        // for debugging
    request->send(200, "text/html", webpage);
  });

  // Start the server
  server.begin();
}


void loop() {

}
