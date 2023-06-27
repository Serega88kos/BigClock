//////////// Функции опросов датчиков и их преобразования
void TempToArray() {  // вывод температуры с датчика BMP/BME280 на экран
  if (wifi.DOT_TEMP == 1) {
    leds[NUM_LEDS] = 0x000000;
  }
  FtempH = (bmp280.readTemperature()) + other.cor_tempH;
  tempH = FtempH;
  //Serial.println((String)tempH + " | " + FtempH);
  Dots(!Dot);
  Digit(digits[10], segment_4);  // символ градуса
  int digit = abs(tempH % 10);
  Digit(digits[digit], segment_3);
  digit = tempH / 10;
  if (digit == 0) Digit(digits[12], segment_2);  // если впереди ноль, то выключаем его
  else
    Digit(digits[digit], segment_2);  // иначе показываем как есть
  Digit(digits[12], segment_1);       // отключаем 1 сегмент
}

void TempStreetToArray() {  // вывод уличной температуры на экран
  sensors.requestTemp();    // опрос датчика уличной температуры
  if (sensors.readTemp())
    FtempS = sensors.getTemp() + other.cor_tempS;  // чтение уличной температуры с датчика 0, аналогично следующий будет 1
  tempS = FtempS;
  //Serial.println((String)tempS + " | " + FtempS);
  Dots(!Dot);
  if (wifi.DOT_TEMP == 1) {
    if (clck.new_god) {
      leds[NUM_LEDS] = ColorTable[rand() % NUM_COLORS - 1];
    } else {
      leds[NUM_LEDS] = ledColor;
    }
    int a = FtempS * 10;              //25.43 -> 254
    int digit = abs(a % 10);          //254 -> 4
    Digit(digits[digit], segment_4);  // символ градуса
    digit = abs((a % 100) / 10);      // 254 -> 54 -> 5
    Digit(digits[digit], segment_3);
    digit = abs(a / 100);                          // 254 -> 2
    if (digit == 0) Digit(digits[12], segment_2);  // если впереди ноль, то выключаем его
    else
      Digit(digits[digit], segment_2);              // иначе показываем как есть
    if (tempS <= -1) Digit(digits[13], segment_1);  // если < или = -1, то показываем -
    else
      Digit(digits[12], segment_1);  // иначе выключаем 1 сегмент
  } else {
    Digit(digits[10], segment_4);  // символ градуса
    int digit = abs(tempS % 10);
    Digit(digits[digit], segment_3);
    digit = abs(tempS / 10);
    if (digit == 0) Digit(digits[12], segment_2);  // если впереди ноль, то выключаем его
    else
      Digit(digits[digit], segment_2);              // иначе показываем как есть
    if (tempS <= -1) Digit(digits[13], segment_1);  // если < или = -1, то показываем -
    else
      Digit(digits[12], segment_1);  // иначе выключаем 1 сегмент
  }
}

void PressToArray() {  // вывод давления на экран с датчика BMP/BME280
  if (wifi.DOT_TEMP == 1) {
    leds[NUM_LEDS] = 0x000000;
  }
  float pressure = bmp280.readPressure();
  Fpres = pressureToMmHg(pressure) + other.cor_pres;
  pres = Fpres;
  //Serial.println(pres);
  Dots(!Dot);
  int digit = pres % 10;
  Digit(digits[digit], segment_4);
  digit = pres % 100 / 10;
  Digit(digits[digit], segment_3);
  digit = pres / 100;
  Digit(digits[digit], segment_2);
  if (clck.prs) {
    Digit(digits[14], segment_1);  // показываем символ P
  } else {
    Digit(digits[12], segment_1);  // отключаем первый сегмент
  }
}

void HumToArray() {  // вывод влажности с датчика BME280 на экран
  if (wifi.DOT_TEMP == 1) {
    leds[NUM_LEDS] = 0x000000;
  }
  if (clck.htu21d) {
    hum = htu.getHumidity() + other.cor_hum;
  } else {
    hum = bmp280.readHumidity() + other.cor_hum;
  }
  //Serial.println(hum);
  Dots(!Dot);
  if (clck.hmd) {
    Digit(digits[16], segment_4);  // символ %  включен
    Digit(digits[15], segment_3);
  } else {
    Digit(digits[12], segment_4);  // символ % выключен
    Digit(digits[12], segment_3);
  }
  int digit = hum % 10;
  Digit(digits[digit], segment_2);  // 2 сегмент
  digit = hum / 10;
  Digit(digits[digit], segment_1);  // 1 сегмент
}

void DateToArray() {
  Dots(!Dot);
  int digit = day % 10;
  Digit(digits[digit], segment_2);  // 2 сегмент
  digit = day / 10;
  Digit(digits[digit], segment_1);  // 1 сегмент
  digit = month % 10;
  Digit(digits[digit], segment_4);  // 4 сегмент
  digit = month / 10;
  Digit(digits[digit], segment_3);  // 3 сегмент
}
