//Автор ASM
//Обсуждение работы на форуме https://community.alexgyver.ru/threads/bolshie-chasy-na-ws2812-esp8266-narodnyj-monitoring.5067
//Поблагодарить за труд, можно на ЮMoney 410014148046232
#include "Constants.h"
#include "RX.h"
#include "ESP8266WiFi.h"
#include <WiFiUdp.h>
WiFiUDP Udp;
#include <LittleFS.h>
#include <Arduino.h>
#include <FileData.h>
#include <GyverNTP.h>
#include <WiFiClient.h>
#define GH_NO_MQTT  // MQTT
#define GH_INCLUDE_PORTAL
#include <GyverHub.h>
GyverHub hub("MyDevices", "BigClock", "f017");
#include <GyverDS3231.h>
GyverDS3231 rtc;
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mp3Serial(MP3_RX_PIN, MP3_TX_PIN);
DFRobotDFPlayerMini mp3;
#include <FastLED.h>
#include <Wire.h>
#include <GyverBME280.h>
GyverBME280 bmp280;
#include <GyverHTU21D.h>
GyverHTU21D htu;
#include "DHT.h"
DHT dht(DHTPIN, DHT22);
#include <AHTxx.h>
AHTxx aht(AHTXX_ADDRESS_X38, AHT1x_SENSOR);
#include <GyverDS18.h>
GyverDS18Single ds(ONE_SENSORS_DS);

CRGB ColorTable[16] = {  // Таблица цветов
  CRGB::Amethyst, CRGB::Aqua, CRGB::Blue, CRGB::Chartreuse, CRGB::DarkGreen, CRGB::DarkMagenta, CRGB::DarkOrange, CRGB::DeepPink,
  CRGB::Fuchsia, CRGB::Gold, CRGB::GreenYellow, CRGB::LightCoral, CRGB::Tomato, CRGB::Salmon, CRGB::Red, CRGB::Orchid
};
CRGB ledColor = ColorTable[c.led_color];

#include "Gradient.h"

FileData _wifi(&LittleFS, "/wifi.dat", 'A', &w, sizeof(w));
FileData _clock(&LittleFS, "/clock.dat", 'A', &c, sizeof(c));
FileData _other(&LittleFS, "/other.dat", 'A', &o, sizeof(o));
FileData _narod(&LittleFS, "/narod.dat", 'A', &nm, sizeof(nm));
FileData _dfp(&LittleFS, "/dfp.dat", 'A', &dfp, sizeof(dfp));
FileData _set(&LittleFS, "/set.dat", 'A', &s, sizeof(s));

bool Dot = true, Utro_flag, Utro_play_flag, Vecher_flag, Vecher_play_flag, Vremy_flag, Vremy_play_flag, flag_kuku, flag_night;
uint8_t last_digit = 0, tempH, hum, hour, minute, second, day, month, tab = 0, segment_1, segment_2, segment_3, segment_4;
int8_t tempS;
uint16_t new_brg, pres, year, NUM_LEDS;
float FtempH, FtempS, Fpres;
CRGB* leds;
unsigned int localPort = 8888;
const uint8_t addrRadDS[] = { 0xAA, 0xBB, 0xEE, 0xCC };

void setup() {
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  NTP.begin(w.gmt);
  LittleFS.begin();
  FDstat_t stat1 = _wifi.read();
  FDstat_t stat2 = _clock.read();
  FDstat_t stat3 = _other.read();
  FDstat_t stat4 = _narod.read();
  FDstat_t stat5 = _dfp.read();
  FDstat_t stat6 = _set.read();
  if (o.dsPrs == 1) bmp280.begin(0x76);
  if (o.dsPrs == 2) bmp280.begin(0x77);
  htu.begin();
  dht.begin();
  aht.begin();
  NUM_LEDS = (s.LEDS_IN_SEGMENT * 28 + s.DOTS_NUM + s.DOT_TEMP);  // вычисляем кол-во светодиодов
  leds = new CRGB[NUM_LEDS];
  if (s.COLOR_ORDER == 0) FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);  // подключение ленты
  if (s.COLOR_ORDER == 1) FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, NUM_LEDS);  // подключение ленты
  FastLED.setBrightness(50);
  segment_4 = (NUM_LEDS - s.DOT_TEMP) - s.LEDS_IN_SEGMENT * 7;
  segment_3 = (NUM_LEDS - s.DOT_TEMP) - s.LEDS_IN_SEGMENT * 14;
  segment_2 = (NUM_LEDS - s.DOT_TEMP) - s.LEDS_IN_SEGMENT * 21 - s.DOTS_NUM;
  segment_1 = (NUM_LEDS - s.DOT_TEMP) - s.LEDS_IN_SEGMENT * 28 - s.DOTS_NUM;
  wifi_connected();
  if (dfp.status_kuku) DFPlayer_setup();
  hub.onBuild(build);  // подключаем билдер
  hub.setVersion(VF);
  initPinRX(PINRX);
}

void loop() {
  _wifi.tick();
  _clock.tick();
  _other.tick();
  _narod.tick();
  _dfp.tick();
  _set.tick();
  NTP.tick();
  hub.tick();
  rtc.tick();
  mod();
  static gh::Timer sec(1000);
  if (sec) {
    hub.update("FtempH").value(FtempH);
    hub.sendUpdate("time");
    hub.update("new_bright").value(new_brg);
    hub.update("FtempS").value(FtempS);
    hub.update("pres").value(pres);
    hub.update("hum").value(hum);
    Brightness();
    ReadingSensors();
  }

  if (dfp.status_kuku) kuku_tick();


  if (nm.Enable) {
    static gh::Timer narMon(nm.delay * 1000);
    if (narMon) narodMonitor();
  }
  if (s.passInput) hub.setPIN(passIn);

  static gh::Timer timerDS(10000);
  if (timerDS) {
    ds.requestTemp();
  }
}
