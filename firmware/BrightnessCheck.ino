//////////// функция освещенности
void Brightness() {
  static gh::Timer timer(o.brg * 1000);  // как часто проверять изменение по датчику освещенности в сек

  if (timer) {

    if ((hour >= o.start_night && hour < o.stop_night) && (o.night_mode)) {
      FastLED.setBrightness(o.night_brg);  // яркость ночного режима
      ledColor = ColorTable[o.NightColor];
    } else {

      if (o.auto_brg) {  // если включена автояркость

        if (o.type_brg) {  // тип датчика цифра
          new_brg = o.min_max ? map(digitalRead(BRG_PIN), 0, 1023, o.min_brg, o.max_brg) : map(digitalRead(BRG_PIN), 0, 1023, o.max_brg, o.min_brg);
        } else {  // иначе аналог
          new_brg = o.min_max ? map(analogRead(BRG_PIN), 0, 1023, o.min_brg, o.max_brg) : map(analogRead(BRG_PIN), 0, 1023, o.max_brg, o.min_brg);
        }
        FastLED.setBrightness(new_brg);  // устанавливаем яркость
        ledColor = ColorTable[c.Ledcolor];
      }

      if (!o.auto_brg) FastLED.setBrightness(o.min_brg);
    }
  }
}
