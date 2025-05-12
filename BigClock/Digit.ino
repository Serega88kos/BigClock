//////////// Настройка точек
void Dots(boolean Dot_On) {
  for (int i = 0; i < s.DOTS_NUM; i++) {
    switch (c.mode_color) {
      case 1:
        leds[(s.LEDS_IN_SEGMENT * 14) + i] = (Dot_On) ? ColorTable[rand() % 16] : CRGB::Black;
        break;
      default:
        leds[(s.LEDS_IN_SEGMENT * 14) + i] = (Dot_On) ? ledColor : CRGB::Black;
        break;
    }
  }
}

//////////// Настройка символов
uint8_t digits[] = {
  // определяем символы для отображения
  // код начинается с 0b0, далее идут 7 цифр, каждая цифра это номер фрагмента, 1 - включен, 0- отключен
  // далее указан получающийся символ и порядковый номер в массиве
  0b00111111,  // Символ 0          0
  0b00100001,  // Символ 1          1
  0b01110110,  // Символ 2          2
  0b01110011,  // Символ 3          3
  0b01101001,  // Символ 4          4
  0b01011011,  // Символ 5          5
  0b01011111,  // Символ 6          6
  0b00110001,  // Символ 7          7
  0b01111111,  // Символ 8          8
  0b01111011,  // Символ 9          9
  0b01111000,  // Символ * градус  10
  0b00011110,  // Символ C         11
  0b00000000,  // Без символа      12
  0b01000000,  // Символ -         13
  0b01111100,  // Символ P         14
  0b01111000,  // Символ % из двух частей 15
  0b01000111,  // Символ % из двух частей 16
  0b01111101,  // Символ A         17
  0b01111100   // Символ P         18

};
/////////////////////////////////////////////
int d;
void Digit(uint8_t digit, uint8_t cursor) {  // функция отображения символов
  for (uint8_t mask = 0b01000000; mask > 0; mask = mask >> 1) {
    for (uint8_t i = 0; i < s.LEDS_IN_SEGMENT; i++) {
      switch (c.mode_color) {
        case 1:  //Новый год
          leds[cursor] = (digit & mask) ? ColorTable[rand() % 16] : CRGB::Black;
          break;
        default:
          leds[cursor] = (digit & mask) ? ledColor : CRGB::Black;
          break;
      }
      cursor++;
    }
  }
}

void DigitTempS(uint8_t digit, uint8_t cursor) {  // функция отображения символов
  for (uint8_t mask = 0b01000000; mask > 0; mask = mask >> 1) {
    for (uint8_t i = 0; i < s.LEDS_IN_SEGMENT; i++) {
      switch (c.mode_color) {
        case 1:  //Новый год
          leds[cursor] = (digit & mask) ? ColorTable[rand() % 16] : CRGB::Black;
          break;
        case 2:
          leds[cursor] = (digit & mask) ? ColorFromPalette(myPalette1, 128 + tempS, 255, NOBLEND) : CRGB::Black;
          break;
        case 3:
          leds[cursor] = (digit & mask) ? ColorFromPalette(myPalette2, 128 + tempS, 255, NOBLEND) : CRGB::Black;
          break;
        case 4:
          leds[cursor] = (digit & mask) ? ColorFromPalette(myPalette3, 128 + tempS, 255, NOBLEND) : CRGB::Black;
          break;
        case 5:
          leds[cursor] = (digit & mask) ? ColorFromPalette(myPalette4, 128 + tempS, 255, NOBLEND) : CRGB::Black;
          break;
        default:
          leds[cursor] = (digit & mask) ? ledColor : CRGB::Black;
          break;
      }
      cursor++;
    }
  }
}

void DigitTempH(uint8_t digit, uint8_t cursor) {  // функция отображения символов
  for (uint8_t mask = 0b01000000; mask > 0; mask = mask >> 1) {
    for (uint8_t i = 0; i < s.LEDS_IN_SEGMENT; i++) {
      switch (c.mode_color) {
        case 1:  //Новый год
          leds[cursor] = (digit & mask) ? ColorTable[rand() % 16] : CRGB::Black;
          break;
        case 2:
          leds[cursor] = (digit & mask) ? ColorFromPalette(myPalette1, 128 + tempH, 255, NOBLEND) : CRGB::Black;
          break;
        default:
          leds[cursor] = (digit & mask) ? ledColor : CRGB::Black;
          break;
      }
      cursor++;
    }
  }
}