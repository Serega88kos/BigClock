void mod() {
  static uint32_t tmr;
  static byte mode;
  if (o.night_mode && o.night_time && (hour >= o.start_night && hour < o.stop_night)) {
    TimeToArray();
    Brightness();
    timeToDots();
  } else {

    if ((millis() - tmr) >= (c.myTime[mode] * 1000)) {
      tmr = millis();
      if (++mode >= c.counter) mode = 0;
    }
    switch (c.myMods[mode]) {
      case 1:
        TempToArray();
        break;
      case 2:
        TempStreetToArray();
        break;
      case 3:
        PressToArray();
        break;
      case 4:
        HumToArray();
        break;
      case 5:
        DateToArray();
        break;
      default:
        TimeToArray();
        Brightness();
        timeToDots();
        break;
    }
  }

  static uint32_t Ftiming;
  if (millis() - Ftiming > 500) {
    Ftiming = millis();
    FastLED.show();
  }
}

void timeToDots() {
  static bool showDot = false;
  static gh::Timer timer(c.mode_sec ? 500 : 1000);
  if (timer) { showDot = !showDot; }
  showDot ? Dots(Dot) : Dots(!Dot);
}
