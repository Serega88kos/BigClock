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
#define GH_NO_MQTT      // MQTT
#define GH_INCLUDE_PORTAL
#include <GyverHub.h>
GyverHub hub("MyDevices", "BigClock 1.3", "");
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

#define NUM_LEDS (LEDS_IN_SEGMENT * 28 + DOTS_NUM + DOT_TEMP) // вычисляем кол-во светодиодов
CRGB leds [NUM_LEDS];                                         // определение СД ленты

GHcolor color1(0, 0, 255);

FileData wifi_ (&LittleFS, "/wifi.dat",  'A', &wifi, sizeof(wifi));
FileData clock_(&LittleFS, "/clock.dat", 'A', &clck, sizeof(clck));
FileData other_(&LittleFS, "/other.dat", 'A', &other, sizeof(other));
FileData narod_(&LittleFS, "/narod.dat", 'A', &narod, sizeof(narod));
FileData dfp_  (&LittleFS, "/dfp.dat",   'A', &dfp, sizeof(dfp));
FileData modes_(&LittleFS, "/modes.dat", 'A', &modes, sizeof(modes));
FileData color1_(&LittleFS, "/color1.dat", 'A', &color1, sizeof(color1));

bool Utro_flag, Utro_play_flag, Vecher_flag, Vecher_play_flag, Vremy_flag, Vremy_play_flag, flag_kuku;
bool Dot = true;                    // переменная для точек
uint8_t last_digit = 0;             // последний символ равен нулю
byte set_light, brightness;         // переменная для освещенности
uint16_t new_bright, new_bright_f;  // переменная для освещенности
uint32_t bright_timer, off_timer;   // переменная для освещенности
float FtempH, FtempS, Fpres;
int8_t tempH, tempS;
uint16_t pres;
uint8_t hum, hour, minute, second, day, month;
int year;
uint8_t tab = 0;

byte segment_1, segment_2, segment_3, segment_4;
GHcolor col(clck.r, clck.g, clck.b);
//CRGB ledColor = CRGB(col.r, col.g, col.b);
CRGB ledColor = CRGB(color1.r, color1.g, color1.b);

CRGB ColorTable[NUM_COLORS] = {  // Таблица цветов
  CRGB::Amethyst, CRGB::Aqua, CRGB::Blue, CRGB::Chartreuse, CRGB::DarkGreen, CRGB::DarkMagenta, CRGB::DarkOrange, CRGB::DeepPink,
  CRGB::Fuchsia, CRGB::Gold, CRGB::GreenYellow, CRGB::LightCoral, CRGB::Tomato, CRGB::Salmon, CRGB::Red, CRGB::Orchid
};
/////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  LittleFS.begin();
  FDstat_t stat1 = wifi_.read();   FDstat_t stat2 = clock_.read(); FDstat_t stat3 = other_.read();
  FDstat_t stat4 = narod_.read();  FDstat_t stat5 = dfp_.read();   FDstat_t stat6 = modes_.read();
  FDstat_t stat7 = color1_.read();
  if (clck.htu21d)htu.begin();
  bmp280.begin();
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // подключение ленты
  FastLED.setMaxPowerInVoltsAndMilliamps(5, milliamp);
  segment_4 = (NUM_LEDS - DOT_TEMP) - LEDS_IN_SEGMENT * 7;
  segment_3 = (NUM_LEDS - DOT_TEMP) - LEDS_IN_SEGMENT * 14;
  segment_2 = (NUM_LEDS - DOT_TEMP) - LEDS_IN_SEGMENT * 21 - DOTS_NUM;
  segment_1 = (NUM_LEDS - DOT_TEMP) - LEDS_IN_SEGMENT * 28 - DOTS_NUM;
  wifi_connected();
  rtcCheck();
  if (dfp.status_kuku)DFPlayer_setup();
  hub.setVersion(VF);
}
/////////////////////////////////////////////
void loop() {
  if (clck.htu21d) {
    htu.readTick();
  }
  kuku_tick();
  wifi_.tick();  clock_.tick(); other_.tick();
  narod_.tick(); dfp_.tick();   modes_.tick();
  color1_.tick();
  hub.sendUpdate("n1,new_bright");
  hub.tick();
  ntp.tick();
  mod();
  if (dfp.status_kuku) {
    kuku_tick();
  }
  static uint32_t timing;
  if (narod.Monitoring) {
    if ((millis() - timing) > (narod.delay_narod * 1000)) {
      timing = millis();
      narodMonitor();
    }
  }
}
