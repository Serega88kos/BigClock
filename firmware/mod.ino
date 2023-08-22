void mod() {
  static uint32_t tmr;
  static byte mode;
  if ((millis() - tmr) >= (modes.myTime[mode] * 1000)) {
    tmr = millis();
    if (++mode >= modes.counter ) mode = 0;
  }
  if (modes.myMods[mode] == 0) {
    TimeToArray();
    BrightnessCheck();
    timeToDots();
  }
  else if (modes.myMods[mode] == 1)
    TempToArray();
  else if (modes.myMods[mode] == 2)
    TempStreetToArray();
  else if (modes.myMods[mode] == 3)
    PressToArray();
  else if (modes.myMods[mode] == 4)
    HumToArray();
  else if (modes.myMods[mode] == 5)
    DateToArray();
  static uint32_t Ftiming;
  if (millis() - Ftiming > 100) {
    Ftiming = millis();
    FastLED.show();
  }
}

void timeToDots() {
  static unsigned int t_sec;
  static bool showDot = false;
  static uint32_t tmr = millis();
  if ((millis() - tmr) < ((clck.type_sec) ? 500 : 1000)) return;
  tmr = millis();
  showDot = !showDot;
  if (showDot)
    Dots(true);
  else
    Dots(false);
}
