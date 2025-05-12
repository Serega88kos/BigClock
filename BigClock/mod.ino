static byte mode;
static gh::Timer timerFastLED(500);
static bool isAnimating = false;
static unsigned long animationStartTime;
static byte nextMode;
static int currentBrightness = 255;

void mod() {
  static uint32_t tmr;
  
  if (c.night_mode && c.night_time && flag_night) {
    TimeToArray();
    Brightness();
    timeToDots();
    FastLED.show();
    return;
  }

  // Проверка времени смены режима
  if ((millis() - tmr) >= (c.myTime[mode] * 1000)) {
    tmr = millis();
    nextMode = (mode + 1 >= c.counter) ? 0 : mode + 1;
    
    if (c.enableAnimation) {
      isAnimating = true;
      animationStartTime = millis();
    } else {
      mode = nextMode;
    }
  }

  // Обработка анимации
  if (isAnimating) {
    unsigned long elapsed = millis() - animationStartTime;
    float progress;
    
    if (elapsed < c.animationSpeed) {
      // Фаза 1: Затемнение (0-50% времени)
      progress = (float)elapsed / c.animationSpeed;
      currentBrightness = new_brg * (1.0 - progress);
      
      // Показываем старый режим с уменьшающейся яркостью
      switch (c.myMods[mode]) {
        case 1: TempHomeToArray(); break;
        case 2: TempStreetToArray(); break;
        case 3: PressToArray(); break;
        case 4: HumToArray(); break;
        case 5: DateToArray(); break;
        default: TimeToArray(); Brightness(); timeToDots(); break;
      }
    } 
    else if (elapsed < c.animationSpeed * 2) {
      // Фаза 2: Появление (50-100% времени)
      if (elapsed < c.animationSpeed * 1.1) {
        mode = nextMode; // Смена режима в начале фазы появления
      }
      
      progress = (float)(elapsed - c.animationSpeed) / c.animationSpeed;
      currentBrightness = new_brg * progress;
      
      // Показываем новый режим с увеличивающейся яркостью
      switch (c.myMods[mode]) {
        case 1: TempHomeToArray(); break;
        case 2: TempStreetToArray(); break;
        case 3: PressToArray(); break;
        case 4: HumToArray(); break;
        case 5: DateToArray(); break;
        default: TimeToArray(); Brightness(); timeToDots(); break;
      }
    } 
    else {
      // Завершение анимации
      isAnimating = false;
      currentBrightness = new_brg;
    }
    
    FastLED.setBrightness(currentBrightness);
    FastLED.show();
    return; // Выходим, чтобы избежать двойного обновления
  }

  // Обычный режим без анимации
  switch (c.myMods[mode]) {
    case 1: TempHomeToArray(); break;
    case 2: TempStreetToArray(); break;
    case 3: PressToArray(); break;
    case 4: HumToArray(); break;
    case 5: DateToArray(); break;
    default: TimeToArray(); Brightness(); timeToDots(); break;
  }
  
  if (timerFastLED) {
    FastLED.setBrightness(new_brg);
    FastLED.show();
  }
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