void mod() {
  static uint32_t tmr;
  static byte mode;
  if (millis() - tmr >= modes.myTime[mode] * 1000) {
    tmr = millis();
    if (++mode >= modes.counter )mode = 0;
    switch (modes.myMods[mode]) {
      case 0:
        timeToDotsOLD();
        timeTo();
        BrightnessCheck();
        Serial.println("Часы");
        break;
      case 1:
        TempToArray();
        Serial.println("Дом");
        break;
      case 2:
        TempStreetToArray();
        Serial.println("Ул");
        break;
      case 3:
        PressToArray();
        Serial.println("Давл");
        break;
      case 4:
        HumToArray();
        Serial.println("Влажн");
        break;
      case 5:
        DateToArray();
        Serial.println("Дата");
        break;
      default:
        break;
    }
  } static uint32_t Ftiming;
  if (millis() - Ftiming > 100) {
    Ftiming = millis();
    FastLED.show();
  }
}

void timeTo() {
  static uint32_t tmr1 = millis();
  if (millis() - tmr1 < 500) return;
  tmr1 = millis();
  TimeToArray();
}
//bool hasChange = false;
/*
  void timeToDots() {
  static unsigned int t_ms;
  static uint32_t tmr1;
  bool showDot = false;
  bool hasChange = false;
  if (clck.type_sec) {
    t_ms = 500;
  } else {
    t_ms = 1000;
  }

  if ((millis() - tmr1) < t_ms)return;
  tmr1 = millis();
  showDot = !showDot;
  hasChange = true;
  if (showDot)
    //Dots(Dot);
    Serial.println("Dot");
  else
    //Dots(!Dot);
    Serial.println("!Dot");
  }*/

void timeToDotsOLD() {
  static unsigned int t_sec;
  static bool showDot = false;
  static uint32_t tmr1 = millis();
  if (clck.type_sec)
    t_sec = 500;
  else
    t_sec = 1000;

  if ((millis() - tmr1) < t_sec) return;
  tmr1 = millis();
  showDot = !showDot;
  //hasChange = true;
  if (showDot)
    Dots(Dot);
  //Serial.println("Dot");
  else
    Dots(!Dot);
  //Serial.println("!Dot");
}
