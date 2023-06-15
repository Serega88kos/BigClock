//////////// функция освещенности
void BrightnessCheck() {
  static uint32_t last_br = millis();
  if ((millis() - last_br) < other.brg) return;
  last_br = millis();

  if (other.night_mode && (hour >= other.start_night && hour <= other.stop_night)) {
    LEDS.setBrightness(other.night_bright);
  } else {

    if (other.auto_bright) {                                                                  // если включена автояркость
      if (millis() - bright_timer > 100) {                                                // каждые 100 мс
        bright_timer = millis();                                                          // сбросить таймер
        if (other.type_brg) {                                                                 // тип датчика цифра
          if (other.min_max) {                                                                // проверка инверсии
            new_bright = map(digitalRead(BRI_PIN), 0, 1023, other.min_bright, other.max_bright);  // считать показания с фоторезистора, перевести диапазон прямой
          } else {
            new_bright = map(digitalRead(BRI_PIN), 0, 1023, other.max_bright, other.min_bright);  // считать показания с фоторезистора, перевести диапазон обратный
          }
        } else {                                                                         // иначе тип датчика аналог
          if (other.min_max) {                                                               // проверка инверсии
            new_bright = map(analogRead(BRI_PIN), 0, 1023, other.min_bright, other.max_bright);  // считать показания с фоторезистора, перевести диапазон прямой
          } else {
            new_bright = map(analogRead(BRI_PIN), 0, 1023, other.max_bright, other.min_bright);  // считать показания с фоторезистора, перевести диапазон обратный
          }
        }
        LEDS.setBrightness(new_bright);  // устанавливаем яркость
      }
    } else LEDS.setBrightness(other.min_bright);  // если автояркость выключена, то в расчете минимальная

  }
}
