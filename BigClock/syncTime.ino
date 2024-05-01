//////////// Функция синхронизации времени
byte test = 0;
void rtcCheck() {

  if (c.rtc_check) {
    WiFi.setAutoReconnect(true);
    ntp.updateNow();
    uint32_t ntpTime = ntp.unix();
    Serial.println(ntpTime);
    Serial.println(ntp.timeString());
    Serial.println(ntp.dateString());
    if (ntpTime > 1609459200) {
      Serial.println("");
      Serial.println("Время записано!");
      DateTime now;
      now.hour = ntp.hour();
      now.minute = ntp.minute();
      now.second = ntp.second();
      now.date = ntp.day();
      now.month = ntp.month();
      now.year = ntp.year();
      rtc.setTime(now);
    } else {
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

  if (!c.rtc_check) {
    WiFi.setAutoReconnect(true);
    ntp.updateNow();
    uint32_t ntpTime = ntp.unix();
    Serial.println(ntp.unix());
    Serial.println(ntp.timeString());
    Serial.println(ntp.dateString());
    if (ntpTime > 1609459200) {
      Serial.println("");
      Serial.println("Время верно!");
    } else {
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
