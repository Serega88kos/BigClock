static byte mode;
gh::Timer timerFastLED(500);
void mod() {
  static uint32_t tmr;
  if (o.night_mode && o.night_time && flag_night) {
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
        TempHomeToArray();
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
  if (timerFastLED) FastLED.show();
}
gh::Timer timerDot(1000);
void timeToDots() {
  static bool showDot = false;
  if (timerDot) {
    timerDot.setTime(c.mode_sec ? 500 : 1000);
    showDot = !showDot;
    Dots(showDot);
  }
}