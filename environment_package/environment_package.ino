#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define SCREEN_W 128
#define SCREEN_H 64
#define OLED_ADDR 0x3C

const char* ssid = "CMCC-C555";
const char* password = "lucky987";
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* topic = "esp32/env";

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}
void reconnect_mqtt() {
  while (!client.connected()) {
    String clientId = "esp32-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) break;
    delay(5000);
  }
}
void setup() {
  Serial.begin(115200);
  dht.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED 失败");
    while (1);
  }
  display.clearDisplay();
  display.display();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}
void loop() {
  if (WiFi.status() != WL_CONNECTED) setup_wifi();
  if (!client.connected()) reconnect_mqtt();
  client.loop();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (isnan(h) || isnan(t)) {
    display.println("Sensor Error");
    Serial.println("DHT 读取失败");
  } else {
    display.print("Temp: "); display.print(t); display.println(" C");
    display.print("Humi: "); display.print(h); display.println(" %");
    String payload = "{\"temp\":" + String(t) + ",\"humi\":" + String(h) + "}";
    client.publish(topic, payload.c_str());
    Serial.println("发布: " + payload);
  }
  display.display();
  delay(2000);
}

