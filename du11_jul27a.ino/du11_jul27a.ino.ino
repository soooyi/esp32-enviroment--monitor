#include "DHT.h"
#define DHTPIN 17
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Sal.eriprintln("DHT11 测试启动");
  dht.begin();
}

void loop() {
  delay(2000);  // DHT11 采样间隔至少 1 秒，建议 2 秒

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("读取失败：请检查接线或换 GPIO");
    return;
  }

  Serial.print("湿度: ");
  Serial.print(h);
  Serial.print(" %\t温度: ");
  Serial.print(t);
  Serial.println(" °C");
}