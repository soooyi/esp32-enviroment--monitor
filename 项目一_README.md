# ESP32 环境监测站（DHT11 + OLED + WiFi/MQTT）

## 一、项目简介
基于 ESP32 的端到端环境监测系统：用 DHT11 采集温湿度，0.96 寸 OLED 本地显示，同时通过 WiFi 连接 EMQX 公共 MQTT Broker 定时上报，手机 / 电脑可实时订阅查看。具备**传感器断线容错**——拔掉 DHT11 后程序不崩溃，屏幕提示 `Sensor Error`。

## 二、硬件清单
- ESP32 开发板（DOIT DevKit V1 / ESP32 Dev Module）
- DHT11 温湿度模块（带 4.7k 上拉，建议买"模块"非裸 sensor）×1
- 0.96 寸 OLED（I2C 接口，4 线）×1
- 面包板 + 杜邦线（公对母 / 公对公 / 母对母）

## 三、接线图
| DHT11 | ESP32 | 说明 |
|-------|-------|------|
| VCC | 3.3V | 供电 |
| GND | GND | 共地 |
| DATA | GPIO 4 | 数据脚（模块已带上拉，直接接） |

| OLED | ESP32 | 说明 |
|------|-------|------|
| VCC | 5V / VIN | **实测 3.3V 供电不足，改接 5V 才亮**（见踩坑） |
| GND | GND | 共地 |
| SDA | GPIO 21 | ESP32 默认 I2C 数据线 |
| SCL | GPIO 22 | ESP32 默认 I2C 时钟线 |

## 四、通信参数
- WiFi：2.4GHz（ESP32 不支持 5G），Station 模式
- MQTT Broker：broker.emqx.io
- 端口：1883
- 主题：esp32/env
- 上报频率：每 2 秒一条 JSON，如 `{"temp":26.5,"humi":65.0}`
- 容错：DHT 读取返回 NaN → 屏幕显示 `Sensor Error`，不重启

## 五、软件依赖（Arduino 库管理器）
- `DHT sensor library`（Adafruit）+ `Adafruit Unified Sensor`
- `Adafruit SSD1306` + `Adafruit GFX Library`
- `PubSubClient`（Nick O'Leary）
- `WiFi.h`（ESP32 自带，不用装）

## 六、使用步骤
1. 按接线图接好 DHT11 与 OLED，OLED 的 VCC 接 5V。
2. 安装上述库。
3. 修改代码中的 `ssid` / `password` 为你自己的 2.4G WiFi。
4. 烧录，打开串口监视器（115200）。
5. OLED 显示温湿度；手机用 MQTTX 订阅 `esp32/env` 实时查看。
6. 拔掉 DHT11 的 DATA 线，验证屏幕显示 `Sensor Error` 且程序不崩。

## 七、踩坑记录（请补全你实际遇到的）
- [ ] OLED 不显示 → VCC 从 3.3V 改到 5V/VIN 后正常（面板 3.3V 供电不足）
- [ ] OLED 初始化失败 → I2C 地址用 0x3C（少数 0x3D，用 Wire.scan 扫描确认）
- [ ] DHT 读失败 → DATA 接 GPIO4、接触不良或模块坏；采样间隔 ≥ 2 秒
- [ ] WiFi 连不上 → 仅支持 2.4G、SSID/密码大小写、中文 SSID 易错
- [ ] MQTT 收不到 → 主题大小写敏感、broker/端口正确、WiFi 已真连上
- [ ] （你补充：_______________）

## 八、验证标志
上电后 OLED 显示 `Temp: xx C` / `Humi: xx %`；MQTTX 每 2 秒收到 JSON；拔掉 DHT11 显示 `Sensor Error` 不重启。
