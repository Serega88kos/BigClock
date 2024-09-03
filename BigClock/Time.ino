//////////// Функция времени
int GetTime() {
  if (c.rtc_check) {
    Datime now = rtc.getTime();
    hour = now.hour;
    minute = now.minute;
    second = now.second;
    day = now.day;
    month = now.month;
    year = now.year;
    //Serial.println((String)hour + ":" + minute + ":" + second);
    //Serial.println((String)day + "." + month + "." + year);
    return (hour * 100 + minute);
  } else {
    hour = NTP.hour();
    minute = NTP.minute();
    second = NTP.second();
    day = NTP.day();
    month = NTP.month();
    year = NTP.year();
    //Serial.println((String)hour + ":" + minute + ":" + second);
    return (hour * 100 + minute);
  }
}

void TimeToArray() {    // вывод времени на экран
  int Now = GetTime();  // получаем время
  if (c.DOT_TEMP == 1) {
    leds[NUM_LEDS - 1] = CRGB::Black;
  }
  boolean change_color_flag = false;
  for (int i = 1; i <= 4; i++) {  // 4 сегмента
    int digit = Now % 10;         // получаем последнюю цифру в времени
    int cursor = NUM_LEDS - i * c.LEDS_IN_SEGMENT * 7;
    if (i > 2) {
      cursor -= c.DOTS_NUM;
    }
    if ((i == 4) & (digit == 0)) {
      if (!c.symbol) {           // проверка, включен ли ноль
        Digit(digits[12], cursor);  // если впереди ноль, то выключаем его, например 01:23 будет как 1:23
      } else {
        Digit(digits[0], cursor);  // иначе будет как 01:23
      }
    } else {
      Digit(digits[digit], cursor);  // иначе показываем символ
    }

    if (i == c.change_color) {  // как часто менять цвет
      if (digit != last_digit) {
        change_color_flag = true;
        last_digit = digit;
      }
    }
    Now /= 10;
  };
  if (change_color_flag) ledColor = ColorTable[random(16)];  // случайный цвет из таблицы
};
