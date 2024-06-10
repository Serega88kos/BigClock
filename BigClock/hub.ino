void build(gh::Builder& b) {
  {
    GH::Row r(b);
    b.Tabs(&tab).text(F("Главная;Часы;Другие;Мониторинг;Плеер")).noLabel().click();
    if (b.changed()) b.refresh();
  }
  switch (tab) {
    case 0:
      {
        bool flag_w = 0;
        b.Title(F("Настройки WiFi")).fontSize(20);
        {
          gh::Row r(b);
          b.Input(w.ssid).label(F("SSID")).attach(&flag_w);
          b.Pass(w.pass).label(F("PASS")).attach(&flag_w);
        }
        {
          gh::Row r(b);
          b.Input_("host", w.host).label(F("Сервер NTP")).size(4).attach(&flag_w);
          b.Input_("gmt", &w.gmt).label(F("GMT зона")).size(1).attach(&flag_w);
        }
        if (flag_w) {
          _wifi.update();
        }
      }
      break;

    case 1:
      {
        bool flag_c = 0;
        bool flag_sync = 0;
        bool flag_rst = 0;
        {
          gh::Row r(b);
          if (!c.rtc_check) {
            b.Label_("n1", ntp.timeString()).noLabel().fontSize(20).size(3);
            b.Label_("n2", ntp.dateString()).noLabel().fontSize(20).size(3);
          }
          if (c.rtc_check) {
            b.Label_("n1", rtc.getTimeString()).noLabel().fontSize(20).size(3);
            b.Label_("n2", rtc.getDateString()).noLabel().fontSize(20).size(3);
          }
          b.Button_("btnn").icon("f0e2").noLabel().size(1).fontSize(20).attach(&flag_sync);
          if (flag_sync) {
            rtcCheck();
          }
        }
        b.Title(F("Настройки ленты")).fontSize(20);
        {
          gh::Row r(b);
          b.Spinner_("lis", &c.LEDS_IN_SEGMENT).label(F("СД в сегменте")).size(2).fontSize(15).range(1, 6, 1).attach(&flag_c);
          b.Spinner_("dn", &c.DOTS_NUM).label(F("СД точек")).size(2).fontSize(15).range(2, 8, 2).attach(&flag_c);
          b.Spinner_("dt", &c.DOT_TEMP).label(F("СД десятки температуры")).size(2).fontSize(15).range(0, 1, 1).attach(&flag_c);
          //b.Button_("rst").icon("f0e2").label("Применить").size(1).fontSize(15).attach(&flag_rst);
        }
        if (flag_rst) {
          ESP.restart();
        }
        b.Title(F("Настройки часов")).fontSize(20);
        {
          gh::Row r(b);
          b.SwitchIcon(&c.rtc_check).label(F("Есть RTC?")).fontSize(15).size(1).attach(&flag_c);
          b.SwitchIcon(&c.htu21d).label(F("Есть htu21d?")).fontSize(15).size(1).attach(&flag_c);
          b.Select(&c.modeColor).text(F("Одноцветный;Новый год;Градиент темп")).label(F("Режимы цветов")).size(2).attach(&flag_c);
        }
        {
          gh::Row r(b);
          b.Select(&c.change_color).text(F("выключена;раз в минуту;каждые 10 минут;каждый час")).label(F("Смена цвета?")).attach(&flag_c);
          b.Select(&c.Ledcolor).text(F("Amethyst;Aqua;Blue;Chartreuse;DarkGreen;DarkMagenta;DarkOrange;DeepPink;Fuchsia;Gold;GreenYellow;LightCoral;Tomato;Salmon;Red;Orchid")).label(F("Цвет часов")).attach(&flag_c);
        }
        b.Title(F("Символы")).fontSize(20);
        {
          gh::Row r(b);
          b.SwitchIcon(&c.prs).label(F("Символ давления")).fontSize(15).attach(&flag_c);
          b.SwitchIcon(&c.hmd).label(F("Символ влажности")).fontSize(15).attach(&flag_c);
          b.SwitchIcon(&c.symbol).label(F("Первый ноль в часах")).fontSize(15).attach(&flag_c);
        }
        {
          gh::Row r(b);
          b.SwitchIcon(&c.mode_sec).label(F("Режим секунд")).fontSize(15).attach(&flag_c);
          b.SwitchIcon(&c.dotDate).label(F("Точка даты")).fontSize(15).attach(&flag_c);
          b.SwitchIcon(&c.dotInv).label(F("Сменить точку")).fontSize(15).attach(&flag_c);
        }
        if (flag_c) {
          _clock.update();
          ledColor = ColorTable[c.Ledcolor];
        }
        b.Title(F("Режимы часов")).fontSize(20);
        bool flag_modes = 0;
        {
          GH::Row r(b);
          b.Spinner_("spin", &c.counter).label(F("Кол-во режимов")).size(2).range(1, 8, 1).size(1).attach(&flag_modes);
          b.Space().size(2);
          if (flag_modes)
            b.refresh();
        }
        b.Title("");
        for (int i = 0; i < c.counter; i++) {
          {
            gh::Row r(b);
            b.Select_(String("myMods_") + i, &c.myMods[i]).text(F("Часы;Темп. дома;Темп. улица;Давление;Влажность;Дата")).label(String(F("Режим ")) + (i + 1)).size(2).attach(&flag_modes);
            b.Spinner_(String("myTime_") + i, &c.myTime[i]).label(String(F("Время ")) + (i + 1)).range(2, 30, 1).size(1).attach(&flag_modes);
          }
        }
        if (flag_modes) {
          c.myMods[c.counter] = convertMods();
          c.myTime[c.counter] = convertTime();
          _clock.update();
        }
      }
      break;

    case 2:
      {
        bool flag_o = 0;
        b.Title(F("Настройка датчика")).fontSize(20);
        {
          gh::Row r(b);
          b.SwitchIcon(&o.auto_brg).label(F("Автояркость")).fontSize(15).attach(&flag_o);
          b.SwitchIcon(&o.type_brg).label(F("Цифровой датчик?")).fontSize(15).attach(&flag_o);
          b.SwitchIcon(&o.min_max).label(F("Инвертировать")).fontSize(15).attach(&flag_o);
          b.Label_("new_bright", String(new_brg)).label(F("Текущая")).fontSize(20);
        }
        {
          gh::Row r(b);
          b.Spinner(&o.min_brg).label(F("Минимальная")).range(0, o.max_brg - 1, 1).fontSize(15).attach(&flag_o);
          b.Spinner(&o.max_brg).label(F("Максимальная")).range(o.min_brg + 1, 255, 1).fontSize(15).attach(&flag_o);
          b.Spinner(&o.brg).label(F("Задержка, в сек.")).range(0, 30, 1).fontSize(15).attach(&flag_o);
        }
        b.Title(F("Ночной режим")).fontSize(20);
        {
          gh::Row r(b);
          b.SwitchIcon(&o.night_mode).label(F("Включить")).fontSize(15).attach(&flag_o);
          b.Spinner(&o.night_brg).label(F("Яркость")).range(0, 20, 1).fontSize(15).attach(&flag_o);
          b.SwitchIcon(&o.night_time).label(F("Только часы")).fontSize(15).attach(&flag_o);
        }
        {
          gh::Row r(b);
          b.Select(&o.start_night).text(F("0;1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19;20;21;22;23")).label(F("Включить в")).fontSize(20).attach(&flag_o);
          b.Select(&o.stop_night).text(F("0;1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19;20;21;22;23")).label(F("Выключить в")).fontSize(20).attach(&flag_o);
          b.Select(&o.NightColor).text(F("Amethyst;Aqua;Blue;Chartreuse;DarkGreen;DarkMagenta;DarkOrange;DeepPink;Fuchsia;Gold;GreenYellow;LightCoral;Tomato;Salmon;Red;Orchid")).label(F("Цвет часов")).attach(&flag_o);
        }
        b.Title(F("Корректировка показаний")).fontSize(20);
        {
          gh::Row r(b);
          b.Label_("FtempH", String(FtempH)).label(F("Комнатная")).fontSize(20);
          b.Spinner(&o.cor_tempH).noLabel().range(-9, 9, 0.1, 1).fontSize(15).attach(&flag_o);
          b.Label_("pres", String(pres)).label(F("Давление")).fontSize(20);
          b.Spinner(&o.cor_pres).noLabel().range(-9, 9, 1).fontSize(15).attach(&flag_o);
        }
        {
          gh::Row r(b);
          b.Label_("FtempS", String(FtempS)).label(F("Уличная")).fontSize(20);
          b.Spinner(&o.cor_tempS).noLabel().range(-9, 9, 0.1, 1).fontSize(15).attach(&flag_o);
          b.Label_("hum", String(hum)).label(F("Влажность")).fontSize(20);
          b.Spinner(&o.cor_hum).noLabel().range(-9, 9, 1).fontSize(15).attach(&flag_o);
        }
        if (flag_o) {
          _other.update();
        }
      }
      break;

    case 3:
      {
        bool flag_m = 0;
        {
          gh::Row r(b);
          b.SwitchIcon(&nm.Enable).label(F("Включить")).fontSize(15).size(1).attach(&flag_m);
          b.HTML(F("<a href='https://narodmon.ru/?invite=asm'>narodmon.ru</a>")).noLabel().fontSize(15).size(2);        
          b.Input(&nm.delay).label(F("Интервал, в сек.")).size(1).attach(&flag_m);
        }

        {
          gh::Row r(b);
          b.SwitchIcon(&nm.tempH).label(F("Комнатная")).fontSize(15).attach(&flag_m);
          b.SwitchIcon(&nm.tempS).label(F("Уличная")).fontSize(15).attach(&flag_m);
          b.SwitchIcon(&nm.pres).label(F("Давление")).fontSize(15).attach(&flag_m);
          b.SwitchIcon(&nm.hum).label(F("Влажность")).fontSize(15).attach(&flag_m);
        }
        if (flag_m) {
          _narod.update();
        }
      }
      break;

    case 4:
      {

        b.Title(F("Настройки DFP")).fontSize(20);
        bool flag_dfp = 0;
        bool flag_test_dfp = 0;
        bool flag_dfp_vol = 0;
        {
          gh::Row r(b);
          b.Select(&dfp.board).text(F("DFPLAYER_MINI;DFPLAYER_FN_X10P;DFPLAYER_HW_247A;DFPLAYER_NO_CHECKSUM")).label(F("Тип платы")).size(2).attach(&flag_dfp);
          b.SwitchIcon(&dfp.status_kuku).label(F("Вкл. модуль")).fontSize(15).size(1).attach(&flag_dfp);
          if (dfp.status_kuku) {
            b.Button_("btnTest").label(F("Тест")).fontSize(20).icon("f04b").size(1).attach(&flag_test_dfp);
          } else {
            b.Button_("btnTest").label(F("Тест [выкл]")).fontSize(20).icon("f04b").size(1).disabled();
          }
        }
        {
          gh::Row r(b);

          b.Select(&dfp.start_kuku).text(F("0;1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19;20;21;22;23")).label(F("Вкл с")).fontSize(20).attach(&flag_dfp);
          b.Select(&dfp.stop_kuku).text(F("0;1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19;20;21;22;23")).label(F("Вкл до")).fontSize(20).attach(&flag_dfp);
          b.Spinner(&dfp.grom_mp3).label(F("Громкость")).fontSize(15).range(0, 30, 1).attach(&flag_dfp_vol);
        }
        if (flag_test_dfp) {
          mp3.playMP3Folder((dfp.golos * 100) + hour + 1);
        }
        b.Title(F("Режимы")).fontSize(20);
        {
          gh::Row r(b);
          b.SwitchIcon(&dfp.kuku_mp3_check).label(F("Кукушка")).fontSize(15).attach(&flag_dfp);
          b.SwitchIcon(&dfp.vrem_mp3_check).label(F("Время")).fontSize(15).attach(&flag_dfp);
          b.Select(&dfp.golos).text(F("Алиса;Женский;Мужской")).label(F("Голос")).attach(&flag_dfp);
        }
        if (flag_dfp) {
          _dfp.update();
        }
        if (flag_dfp_vol) {
          mp3.setVolume(dfp.grom_mp3);
        }
      }
      break;
  }
}

int convertMods() {
  int s;
  for (int i = 0; i < c.counter; i++) {
    s = c.myMods[i];
  }
  return s;
}

int convertTime() {
  int s;
  for (int i = 0; i < c.counter; i++) {
    s = c.myTime[i];
  }
  return s;
}
