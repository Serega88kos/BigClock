void mod() {
  static uint32_t tmr;
  static byte mode;
  if (millis() - tmr >= modes.myTime[mode] * 1000) {
    tmr = millis();
    if (++mode >= modes.counter )mode = 0;
    /*   switch (modes.myMods[mode]) {
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
           BrightnessCheck();
           timeToDots();
           break;
       }
    */

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

  } static uint32_t Ftiming;
  if (millis() - Ftiming > 100) {
    Ftiming = millis();
    FastLED.show();
  }
}

void timeToDots() {
  static unsigned int t_sec;
  static bool showDot = false;
  static uint32_t tmr = millis();
  //  if (clck.type_sec)
  //    t_sec = 500;
  //  else
  //    t_sec = 1000;

  t_sec =  (clck.type_sec) ? 500 : 100;


  if ((millis() - tmr) < t_sec) return;
  tmr = millis();
  showDot = !showDot;
  if (showDot)
    Dots(Dot);
  else
    Dots(!Dot);
}
