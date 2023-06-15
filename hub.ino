void build() {
  hub.BeginWidgets();
  hub.WidgetSize(100);
  if (hub.Tabs(F("tabs"), &tab, F("Главная,Часы,Другие,Мониторинг,Плеер"), GH_NO_LABEL)) {
    hub.refresh();
  }
  switch (tab) {
    case 0:
      {
        bool flag_wifi = 0;
        hub.Title(F("Настройки WiFi"));
        hub.WidgetSize(100);
        flag_wifi |= hub.Input(F("ssid"), wifi.ssid, GH_CSTR, F("SSID"), 32);
        flag_wifi |= hub.Pass(F("pass"), wifi.pass, GH_CSTR, F("PASS"), 32);

        hub.WidgetSize(70);
        flag_wifi |= hub.Input(F("inp_ntp"), &wifi.host, GH_CSTR, F("Сервер NTP"), 32);
        hub.WidgetSize(30);
        flag_wifi |= hub.Input(F("inp_gmt"), &wifi.gmt, GH_UINT16, F("GMT зона"), 3);
        //flag_wifi |= hub.Spinner(F("LEDS_IN_SEGMENT"), &wifi.LEDS_IN_SEGMENT, GH_INT16, F("Точек в сегменте"), 1, 5, 1);
        if (flag_wifi) {
          wifi_.update();
        }
      }
      break;

    case 1:
      {
        bool flag_clock = 0;
        bool flag_sync = 0;
        bool flag_color1 = 0;
        hub.WidgetSize(45);
        if (!clck.rtc_check) {
          hub.Label(F("n1"), String(ntp.timeString()), GH_NO_LABEL, GH_DEFAULT, 20);
          hub.Label(F("n2"), String(ntp.dateString()), GH_NO_LABEL, GH_DEFAULT, 20);
        }
        if (clck.rtc_check) {
          hub.Label(F("n1"), String(rtc.getTimeString()), GH_NO_LABEL, GH_DEFAULT, 20);
          hub.Label(F("n2"), String(rtc.getDateString()), GH_NO_LABEL, GH_DEFAULT, 20);
        }
        hub.WidgetSize(10);
        flag_sync |= hub.ButtonIcon(F("sync"), 0, F(""), GH_DEFAULT, 20);
        if (flag_sync) {
          rtcCheck();
        }
        hub.Title(F("Настройки часов"));

        hub.WidgetSize(33);
        flag_clock |= hub.Switch(F("rtc_check"), &clck.rtc_check, F("Есть RTC?"));
        flag_clock |= hub.Switch(F("htu21d"), &clck.htu21d, F("Есть htu21d?"));
        flag_clock |= hub.Switch(F("new_god"), &clck.new_god, F("Новый год?"));
        hub.WidgetSize(50);
        flag_clock |= hub.Select(F("change_color"), &clck.change_color, F("выключена,раз в минуту,каждые 10 минут,каждый час,каждые 10 часов"), F("Смена цвета?"));
        flag_color1 |= hub.Color(F("ledColor"), &color1, F("Выбор цвета"));
        hub.Title(F("Символы"));
        hub.WidgetSize(50);
        flag_clock |= hub.Switch(F("prs"), &clck.prs, F("Символ давления"));
        flag_clock |= hub.Switch(F("hmd"), &clck.hmd, F("Символ влажности"));
        flag_clock |= hub.Switch(F("symbol"), &clck.symbol, F("Первый ноль в часах"));
        flag_clock |= hub.Switch(F("m1"), &clck.type_sec, F("Режим секунд"));
        if (flag_clock) {
          clock_.update();
        }
        if (flag_color1) {
          //ledColor = CRGB(color1);
          ledColor.setRGB(color1.r, color1.g, color1.b);
          color1_.update();
        }
        hub.Title(F("Режимы часов"));
        bool flag_modes = 0;
        hub.WidgetSize(50);
        if (flag_modes |= hub.Spinner(F("countTime"), &modes.counter, GH_UINT8, F("Кол-во режимов"), 1, 8, 1)) {
          hub.refresh();
        }
        hub.Title("");
        for (int i = 0; i < modes.counter; i++) {
          hub.WidgetSize(50);
          flag_modes |= hub.Select (String("myMods_") + i, &modes.myMods[i], F("Часы,Темп. дома,Темп. улица,Давление,Влажность,Дата"), String("Режим ") + (i + 1));
          flag_modes |= hub.Spinner(String("myTime_") + i, &modes.myTime[i], GH_UINT8, String("Время ") + (i + 1), 2, 30, 1);
        }
        if (flag_modes) {
          modes.myMods[modes.counter] = convertMods();
          modes.myTime[modes.counter] = convertTime();
          modes_.update();
          Serial.println(convertMods());
          Serial.println(convertTime());
        }
      }
      break;

    case 2:
      {
        bool flag_other = 0;
        hub.WidgetSize(33);
        hub.Title(F("Настройка датчика"));
        flag_other |= hub.Switch(F("auto_bright"), &other.auto_bright, F("Автояркость"));
        flag_other |= hub.Switch(F("type_brg"), &other.type_brg, F("Цифровой датчик?"));
        flag_other |= hub.Switch(F("min_max"), &other.min_max, F("Инвертировать"));
        hub.WidgetSize(25);
        hub.Label(F("new_bright"), String(new_bright), F("Текущая"), GH_DEFAULT, 20);
        flag_other |= hub.Spinner(F("min_bright"), &other.min_bright, GH_INT16, F("Минимальная"), 0, 255, 1);
        flag_other |= hub.Spinner(F("max_bright"), &other.max_bright, GH_INT16, F("Максимальная"), 9, 255, 1);
        flag_other |= hub.Spinner(F("brg"), &other.brg, GH_INT16, F("Задержка, в сек."), 0, 30, 1);
        hub.WidgetSize(50);
        hub.Title(F("Ночной режим"));
        flag_other |= hub.Switch(F("night_mode"), &other.night_mode, F("Включить"));
        flag_other |= hub.Spinner(F("night_bright"), &other.night_bright, GH_INT16, F("Яркость"), 0, 20, 1);
        flag_other |= hub.Select(F("start_night"), &other.start_night, F("0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23"), F("Выкл с"));
        flag_other |= hub.Select(F("stop_night"), &other.stop_night, F("0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23"), F("Выкл до"));
        hub.Title(F("Корректировка показаний"));
        hub.WidgetSize(25);
        hub.Label(F("FtempH"), String(FtempH), F("Комнатная"), GH_DEFAULT, 20);
        flag_other |= hub.Spinner(F("cor_tempH"), &other.cor_tempH, GH_FLOAT, GH_NO_LABEL, -9, 9, 0.1);
        hub.Label(F("FtempS"), String(FtempS), F("Уличная"), GH_DEFAULT, 20);
        flag_other |= hub.Spinner(F("cor_tempS"), &other.cor_tempS, GH_FLOAT, GH_NO_LABEL, -9, 9, 0.1);
        hub.WidgetSize(25);
        hub.Label(F("Fpres"), String(pres), F("Давление"), GH_DEFAULT, 20);
        flag_other |= hub.Spinner(F("cor_pres"), &other.cor_pres, GH_INT32, GH_NO_LABEL, -9, 9, 1);
        hub.Label(F("hum"), String(hum), F("Влажность"), GH_DEFAULT, 20);
        flag_other |= hub.Spinner(F("cor_hum"), &other.cor_hum, GH_INT16, GH_NO_LABEL, -9, 9, 1);
        if (flag_other) {
          other_.update();
        }
      }
      break;

    case 3:
      {
        hub.EndWidgets();
        hub.HTML(F("link"), String("<a href=\"https://narodmon.ru/?invite=asm\">Народный мониторинг</a>&nbsp;&nbsp;" + WiFi.macAddress()), F("ссылка"));
        hub.BeginWidgets();
        hub.WidgetSize(50);
        bool flag_monitor = 0;
        flag_monitor |= hub.Switch(F("Monitoring"), &narod.Monitoring, F("Включить"));
        flag_monitor |= hub.Input(F("delay_narod"), &narod.delay_narod, GH_UINT16, F("Интервал, в сек."), 20);
        hub.WidgetSize(25);
        flag_monitor |= hub.Switch(F("nm_tempH"), &narod.nm_tempH, F("Комнатная"));
        flag_monitor |= hub.Switch(F("nm_tempS"), &narod.nm_tempS, F("Уличная"));
        flag_monitor |= hub.Switch(F("nm_pres"), &narod.nm_pres, F("Давление"));
        flag_monitor |= hub.Switch(F("nm_hum"), &narod.nm_hum, F("Влажность"));
        hub.Label(F("pogoda"), String(pogoda), F("Погода 0-10"), GH_DEFAULT, 20);
        if (flag_monitor) {
          narod_.update();
        }
      }
      break;

    case 4:
      {
        hub.Title(F("Настройки DFP"));
        bool flag_dfp = 0;
        bool flag_test_dfp = 0;
        hub.WidgetSize(50);
        flag_dfp |= hub.Switch(F("status_kuku"), &dfp.status_kuku, F("Включить модуль"));
        flag_test_dfp |= hub.ButtonIcon(F("sync"), 0, F(""), GH_DEFAULT, 20);
        
        flag_dfp |= hub.Select(F("start_kuku"), &dfp.start_kuku, F("0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23"), F("Вкл с"));
        flag_dfp |= hub.Select(F("stop_kuku"), &dfp.stop_kuku, F("0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23"), F("Вкл до"));

        if (flag_test_dfp) {
          mp3.playMP3Folder(25);
        }
        hub.Title(F("Режимы"));
        hub.WidgetSize(50);
        flag_dfp |= hub.Switch(F("kuku_mp3_check"), &dfp.kuku_mp3_check, F("Кукушка"));
        flag_dfp |= hub.Switch(F("vrem_mp3_check"), &dfp.vrem_mp3_check, F("Время"));
        flag_dfp |= hub.Select(F("golos"), &dfp.golos, F("Алиса,Женский,Мужской"), F("Голос"));
        flag_dfp |= hub.Spinner(F("grom_mp3"), &dfp.grom_mp3, GH_INT16, F("Громкость"), 0, 30, 1);
        if (flag_dfp) {
          dfp_.update();
        }
      }
      break;
  }
  hub.EndWidgets();
}

int convertMods() {
  int s;
  for (int i = 0; i < modes.counter; i++) {
    s = modes.myMods[i];
    Serial.println(s);
  }
  return s;
}

int convertTime() {
  int s;
  for (int i = 0; i < modes.counter; i++) {
    s = modes.myTime[i];
  }
  return s;
}
