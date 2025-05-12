//////////// функция освещенности
void Brightness() {
  static gh::Timer timer(o.brg * 1000);  // как часто проверять изменение по датчику освещенности в сек
  if (timer) {
    if (o.auto_brg) {                                                                                                                                             // если включена автояркость
      if (o.type_brg) new_brg = o.min_max ? map(digitalRead(BRG_PIN), 7, 1023, o.min_brg, o.max_brg) : map(digitalRead(BRG_PIN), 7, 1023, o.max_brg, o.min_brg);  // тип датчика цифра
      if (!o.type_brg) new_brg = o.min_max ? map(analogRead(BRG_PIN), 7, 1023, o.min_brg, o.max_brg) : map(analogRead(BRG_PIN), 7, 1023, o.max_brg, o.min_brg);   // тип датчика аналог
      FastLED.setBrightness(new_brg);                                                                                                                             // устанавливаем яркость
      if (c.change_color == 0) ledColor = ColorTable[c.led_color];
      flag_night = false;
    }
    if (!o.auto_brg) FastLED.setBrightness(o.min_brg);
    if (new_brg < c.light_night && c.night_mode && ((c.start_night > c.stop_night && (hour >= c.start_night || hour < c.stop_night)) || (c.start_night < c.stop_night && hour >= c.start_night && hour >= c.start_night && hour < c.stop_night))) {
      FastLED.setBrightness(c.night_brg);  // яркость ночного режима
      ledColor = ColorTable[c.night_color];
      flag_night = true;
    }
  }
}