#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#ifdef ESP32
#include <WiFi.h>
#endif // ESP32
#endif // ESP8266

#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>

#define ENCRYPTED false

constexpr char WIFI_SSID[] = "RD-SEAI_2.4G";
constexpr char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";
constexpr char TOKEN[] = "wipfx1rt8orrlpte60p2";
constexpr char THINGSBOARD_SERVER[] = "app.coreiot.io";

#if ENCRYPTED
constexpr uint16_t THINGSBOARD_PORT = 8883U;
#else
constexpr uint16_t THINGSBOARD_PORT = 1883U;
#endif

constexpr uint16_t MAX_MESSAGE_SEND_SIZE = 256U;
constexpr uint16_t MAX_MESSAGE_RECEIVE_SIZE = 256U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

#if ENCRYPTED
WiFiClientSecure espClient;
#else
WiFiClient espClient;
#endif

Arduino_MQTT_Client mqttClient(espClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_RECEIVE_SIZE, MAX_MESSAGE_SEND_SIZE);

void InitWiFi() {
  Serial.println("Connecting to AP ...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

bool reconnect() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  InitWiFi();
  return true;
}

void setup() {
  Serial.begin(SERIAL_DEBUG_BAUD);
  delay(1000);
  InitWiFi();
}

void loop() {
  delay(1000);

  if (!reconnect()) {
    return;
  }

  if (!tb.connected()) {
    Serial.printf("Connecting to: (%s) with token (%s)\n", THINGSBOARD_SERVER, TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  // Create a JSON document
  StaticJsonDocument<256> doc;
  doc["temperature"] = random(20, 30);  // Example: Simulating a temperature reading

  // Serialize the JSON document
  char buffer[256];
  size_t len = serializeJson(doc, buffer, sizeof(buffer));

  // Send telemetry data
  if (!tb.sendTelemetryJson(doc, len)) {
    Serial.println("Failed to send telemetry");
  }

  tb.loop();
}
