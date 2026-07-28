#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "CMCC-C555";
const char* password = "lucky987";
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  Serial.print("连接 WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 已连接,IP: " + WiFi.localIP().toString());
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "esp32-";
    clientId += String(random(0xffff), HEX);
    Serial.print("连接 MQTT...");
    if (client.connect(clientId.c_str())) {
      Serial.println("已连接");
    } else {
      Serial.print("失败, rc=");
      Serial.print(client.state());
      Serial.println(" 5秒后重试");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  String payload = "{\"temp\":25.0,\"humi\":60.0}";
  client.publish("esp32/env", payload.c_str());
  Serial.println("已发布: " + payload);
  delay(3000);
}
