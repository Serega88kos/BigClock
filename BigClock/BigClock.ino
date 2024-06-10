//Автор ASM
//Обсуждение работы на форуме https://community.alexgyver.ru/threads/bolshie-chasy-na-ws2812-esp8266-narodnyj-monitoring.5067
//Поблагодарить за труд, можно на Яндекс 410014148046232
#include "Constants.h"
#include "ESP8266WiFi.h"
#include <LittleFS.h>
#include <Arduino.h>
#include <FileData.h>
#include <GyverNTP.h>
GyverNTP ntp(3);
#include <WiFiClient.h>
#define GH_NO_MQTT  // MQTT
#define GH_INCLUDE_PORTAL
#include <GyverHub.h>
GyverHub hub("MyDevices", "BigClock", "f017");
#include <microDS3231.h>
MicroDS3231 rtc;
#include <SoftwareSerial.h>
#include "DFPlayer.h"
SoftwareSerial mp3Serial;
DFPlayer mp3;
#include <FastLED.h>
#include <Wire.h>
#include <GyverBME280.h>
GyverBME280 bmp280;
#include <GyverHTU21D.h>
GyverHTU21D htu;
#include <microDS18B20.h>
MicroDS18B20<ONE_SENSORS_DS> sensors;



CRGB ColorTable[16] = {  // Таблица цветов
  CRGB::Amethyst, CRGB::Aqua, CRGB::Blue, CRGB::Chartreuse, CRGB::DarkGreen, CRGB::DarkMagenta, CRGB::DarkOrange, CRGB::DeepPink,
  CRGB::Fuchsia, CRGB::Gold, CRGB::GreenYellow, CRGB::LightCoral, CRGB::Tomato, CRGB::Salmon, CRGB::Red, CRGB::Orchid
};
CRGB ledColor = ColorTable[c.Ledcolor];

DEFINE_GRADIENT_PALETTE(Temperature){
  0, 0, 0, 139,                //DarkBlue
  128 - 30 * 2, 0, 0, 255,     //Blue
  128 - 20 * 2, 30, 144, 255,  //DodgerBlue
  128 - 10 * 2, 0, 191, 255,   //DeepSkyBlue
  128 + 0, 176, 224, 230,      //PowderBlue
  128 + 10, 238, 232, 170,     //PaleGoldenrod
  128 + 20, 255, 215, 0,       //Gold
  128 + 30, 255, 165, 0,       //Orange
  255, 255, 0, 0               //Red
};
CRGBPalette256 myPalette = Temperature;

FileData _wifi(&LittleFS, "/wifi.dat", 'A', &w, sizeof(w));
FileData _clock(&LittleFS, "/clock.dat", 'A', &c, sizeof(c));
FileData _other(&LittleFS, "/other.dat", 'A', &o, sizeof(o));
FileData _narod(&LittleFS, "/narod.dat", 'A', &nm, sizeof(nm));
FileData _dfp(&LittleFS, "/dfp.dat", 'A', &dfp, sizeof(dfp));

bool Dot = true, Utro_flag, Utro_play_flag, Vecher_flag, Vecher_play_flag, Vremy_flag, Vremy_play_flag, flag_kuku;
uint8_t last_digit = 0, tempH, hum, hour, minute, second, day, month, tab = 0, segment_1, segment_2, segment_3, segment_4;
int8_t tempS;
uint16_t new_brg, pres, year, NUM_LEDS;
float FtempH, FtempS, Fpres;
CRGB* leds;

void setup() {
  Serial.begin(115200);
  LittleFS.begin();
  FDstat_t stat1 = _wifi.read();
  FDstat_t stat2 = _clock.read();
  FDstat_t stat3 = _other.read();
  FDstat_t stat4 = _narod.read();
  FDstat_t stat5 = _dfp.read();
  if (c.htu21d) { htu.begin(); }
  bmp280.begin();
  NUM_LEDS = (c.LEDS_IN_SEGMENT * 28 + c.DOTS_NUM + c.DOT_TEMP);  // вычисляем кол-во светодиодов
  leds = new CRGB[NUM_LEDS];
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // подключение ленты
  FastLED.setBrightness(50);
  segment_4 = (NUM_LEDS - c.DOT_TEMP) - c.LEDS_IN_SEGMENT * 7;
  segment_3 = (NUM_LEDS - c.DOT_TEMP) - c.LEDS_IN_SEGMENT * 14;
  segment_2 = (NUM_LEDS - c.DOT_TEMP) - c.LEDS_IN_SEGMENT * 21 - c.DOTS_NUM;
  segment_1 = (NUM_LEDS - c.DOT_TEMP) - c.LEDS_IN_SEGMENT * 28 - c.DOTS_NUM;
  wifi_connected();
  if (dfp.status_kuku) DFPlayer_setup();
  hub.onBuild(build);  // подключаем билдер
  hub.setVersion(VF);
  ReadingSensors();
}

void loop() {
  if (c.htu21d) { htu.readTick(); }
  _wifi.tick();
  _clock.tick();
  _other.tick();
  _narod.tick();
  _dfp.tick();
  hub.sendUpdate("FtempH");
  hub.sendUpdate("n1");
  hub.sendUpdate("n2");
  hub.sendUpdate("new_bright");
  hub.sendUpdate("FtempS");
  hub.sendUpdate("pres");
  hub.sendUpdate("hum");
  hub.tick();
  ntp.tick();
  Brightness();
  mod();
  if (dfp.status_kuku) {
    kuku_tick();
  }
  if (nm.Enable) {
    static gh::Timer narMon(nm.delay * 1000);
    if (narMon) narodMonitor();
  }
  // static gh::Timer readingSensors(60000);
  // if (readingSensors) ReadingSensors();
  ReadingSensors();
}
