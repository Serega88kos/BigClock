//////////// Функция WiFi и RTC
void wifi_connected() {
  WiFi.mode(WIFI_STA);
  WiFi.hostname("BigClock");
  WiFi.begin(w.ssid, w.pass);

  for (byte tries = 60; tries > 0; tries--) {  // Обратный отсчёт 60 секунд
    if (WiFi.status() == WL_CONNECTED) break;
    Serial.print F(".");

    // Отображение обратного отсчёта
    int digit = tries % 10;
    Dots(!Dot);
    Digit(digits[digit], segment_4);  // 4 сегмент
    digit = tries / 10;
    Digit(digits[digit], segment_3);  // 3 сегмент
    Digit(digits[12], segment_2);     // 2 сегмент
    Digit(digits[12], segment_1);     // 1 сегмент
    FastLED.show();
    delay(1000);
  }

  if (WiFi.status() != WL_CONNECTED) {
    // Если не удалось подключиться запускаем в режиме AP
    Serial.print F("\nWiFi up ACCESS POINT: ");
    Serial.println(ssidAP);
    WiFi.disconnect();   // Отключаем WIFI
    WiFi.mode(WIFI_AP);  // Меняем режим на режим точки доступа
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssidAP, passAP);  // Включаем WIFI в режиме точки доступа с именем и паролем
    Serial.print F("Start Settings IP: 192.168.4.1");

    hub.onBuild(build);  // подключаем билдер
    hub.begin();         // запускаем систему
    // Индикация AP на часах, как точки
    Digit(digits[18], segment_4);
    Digit(digits[17], segment_3);
    Digit(digits[12], segment_2);
    Digit(digits[12], segment_1);
    FastLED.show();
    delay(3000);
    return;
  }

  Serial.print F("\nWiFi запущен! IP адрес: ");
  Serial.println(WiFi.localIP());

  if (s.mode_udp != 0) {
    Serial.printf("UDP server on port %d\n", localPort);
    Udp.begin(localPort);
  }
  NTP.setGMT(w.gmt);
  NTP.setHost(w.host);
  NTP.begin();
  hub.onBuild(build);  // подключаем билдер
  hub.begin();         // запускаем систему
  delay(2000);
  syncTime();
}
