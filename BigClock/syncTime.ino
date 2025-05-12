void syncTime() {
  const byte maxAttempts = 4;      // 4 попытки
  static bool timeSynced = false;  // Флаг успешной синхронизации

  if (timeSynced) return;  // Пропускаем если уже синхронизировано

  for (byte attempt = 1; attempt <= maxAttempts; attempt++) {
    Serial.println("Попытка синхронизации " + String(attempt));

    if (NTP.updateNow() && NTP.getUnix() > 0) {  // Если время корректно
      timeSynced = true;
      Serial.println("Синхронизация успешна!");
      Serial.println(NTP.toString());

      if (o.rtc_check == 1) {  // Обновляем RTC
        Datime now;
        now.hour = NTP.hour();
        now.minute = NTP.minute();
        now.second = NTP.second();
        now.day = NTP.day();
        now.month = NTP.month();
        now.year = NTP.year();

        if (rtc.setTime(now)) Serial.println("RTC обновлён!");
        else Serial.println("Ошибка записи в RTC!");
      }
      return;  // Успех - выходим
    }
    if (attempt < maxAttempts) {
      delay(5000);  // Пауза 5 сек между попытками
    }
  }

  Serial.println("Ошибка синхронизации! Перезагрузка...");
  delay(2000);
  ESP.restart();
}