//////////// функция освещенности
void Brightness() {
  static gh::Timer timer(o.brg * 1000);  // как часто проверять изменение по датчику освещенности в сек
  if (timer) {
    if (o.night_mode && ((o.start_night > o.stop_night && (hour >= o.start_night || hour < o.stop_night)) || (o.start_night < o.stop_night && hour >= o.start_night && hour >= o.start_night && hour < o.stop_night))) {
      FastLED.setBrightness(o.night_brg); // яркость ночного режима
      ledColor = ColorTable[o.night_color];
      flag_night = true;
    } else {
      if (o.auto_brg) {                  // если включена автояркость
        if (o.type_brg) new_brg = o.min_max ? map(digitalRead(BRG_PIN), 0, 1023, o.min_brg, o.max_brg) : map(digitalRead(BRG_PIN), 0, 1023, o.max_brg, o.min_brg);  // тип датчика цифра
        if (!o.type_brg) new_brg = o.min_max ? map(analogRead(BRG_PIN), 0, 1023, o.min_brg, o.max_brg) : map(analogRead(BRG_PIN), 0, 1023, o.max_brg, o.min_brg);   // тип датчика аналог
        FastLED.setBrightness(new_brg);  // устанавливаем яркость
        if (c.change_color == 0) ledColor = ColorTable[c.led_color];
        flag_night = false;
      }
      if (!o.auto_brg) FastLED.setBrightness(o.min_brg);
    }
  }
}