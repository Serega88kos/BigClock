//////////// Функция синхронизации времени
void rtcCheck() {
  byte test = 0;
  if (c.rtc_check == 1) {
    WiFi.setAutoReconnect(true);
    NTP.updateNow();
    uint32_t ntpTime = NTP.getUnix();
    Serial.println(ntpTime);
    Serial.println(NTP.toString());
    if (ntpTime > 1609459200) {
      Serial.println("");
      Serial.println("Время записано!");
      Datime now;
      now.hour = NTP.hour();
      now.minute = NTP.minute();
      now.second = NTP.second();
      now.day = NTP.day();
      now.month = NTP.month();
      now.year = NTP.year();
      rtc.setTime(now);
    }
    if (ntpTime < 1609459200) {
      Serial.println("");
      Serial.println("Отказ в записи! Время получено неправильное!");
      test++;
      delay(5000);
      if (test == 4) {
        test = 0;
        return;
      }
      rtcCheck();
    }
  }

  if (c.rtc_check == 0) {
    WiFi.setAutoReconnect(true);
    NTP.updateNow();
    uint32_t ntpTime = NTP.getUnix();
    Serial.println(NTP.getUnix());
    Serial.println(NTP.toString());
    if (ntpTime > 1609459200) {
      Serial.println("");
      Serial.println("Время верно!");
    }
    if (ntpTime < 1609459200) {
      Serial.println("");
      Serial.println("Время получено неправильное!");
      delay(5000);
      if (test == 4) {
        test = 0;
        return;
      }
      rtcCheck();
    }
  }
}