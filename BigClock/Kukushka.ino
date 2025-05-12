//[I]░░░▒▒▒▓▓▓ ♫ Кукушка by Romshteks ♫ ▓▓▓▒▒▒░░░[/I]\\

//    [I]░░░▒▒▒▓▓▓ ♪ V2 Без delay ♪ ▓▓▓▒▒▒░░░[/I]    \\

//      [I]░░░▒▒▒▓▓▓ ♂ 3 Голоса ♀ ▓▓▓▒▒▒░░░[/I]      \\


void kuku_tick() {
  if (dfp.status_kuku) {
    if (hour >= dfp.start_kuku && hour <= dfp.stop_kuku) {
      if (minute == 0 && flag_kuku == 1) {
        DFPlayer();
        flag_kuku = 0;
      }
      if (minute == 59 && flag_kuku == 0) {
        flag_kuku = 1;
      }
      mp3.volume(dfp.grom_mp3);
      Vremy();
      Utro();
      Vecher();
    }
  }
}

//////////// Функция DFPlayer
/*
[*]автор Maxx1200
[*]Каждый час будет говорить время!
*/
/***************************************************************************************************/
/*
   исходный код: https://github.com/enjoyneering/DFPlayer

   Возможности DFPlayer Mini:
    - 3,2В..5,0В, обычно 4,2В
    - microSD-карта до 32 ГБ (FAT16, FAT32)
    - USB-диск до 32 ГБ (FAT16, FAT32)
    - поддерживает частоту дискретизации mp3 8кГц, 11,025кГц, 12кГц, 16кГц, 22,05кГц, 24кГц, 32кГц, 44,1кГц, 48кГц
    - поддерживает до 100 папок, каждая папка может быть назначена на 001..255 песен
    - встроенный моноусилитель мощностью 3 Вт, NS8002 AB-класса с функцией ожидания
    - UART для связи, 9600 бит/с (четность: нет, биты данных: 8, стоповые биты: 1, управление потоком: нет)

    ПРИМЕЧАНИЕ:
    - добавьте резистор 1K последовательно с выводом DFPlayer TX

   Фреймворки и библиотеки:
   ESP8266 Core      -  https://github.com/esp8266/Arduino
   EspSoftwareSerial -  https://github.com/plerup/espsoftwareserial
*/
/***************************************************************************************************/

void DFPlayer_setup() {
  Serial.println("Инициализация DFPlayedfp... (может занять 3~5 секунд)");  //для отладки
  mp3Serial.begin(9600);
  mp3.begin(mp3Serial, /*isACK = */ true, /*doReset = */ true);  
  mp3.stop();                                                    //если плеер работал во время перезагрузки ESP8266
  mp3.reset();                                                   //сбросить все настройки по умолчанию
  mp3.volume(dfp.grom_mp3);                                      //0..30, модуль сохраняет громкость при сбое питания
  mp3Serial.enableRx(false);  //отключить прерывания на RX-контакте, меньше накладных расходов, чем mp3Serial.listen()
  mp3.play(27);
  Serial.println(" ♫ Приветствие");  // Музыкальное приветствие
}

void DFPlayer() {              // Функция срабатывающая раз в час
  if (dfp.status_kuku) {       // Если кукушка включена(проверка второй раз для корректной работы портала)
    if (dfp.kuku_mp3_check) {  // Если включено воспроизведение кукушки
      Serial.println(" ♫ Ку-Ку");
      mp3.play(26);  // проигрываем файл c кукушкой
    }
    if (dfp.vrem_mp3_check) Vremy_flag = 1;
  }
}

void Vremy() {
  static uint32_t VremyTime;
  if (Vremy_flag == 1) {
    VremyTime = millis();
    Vremy_play_flag = 1;
    Vremy_flag = 0;
  }
  if (Vremy_play_flag == 1 && (millis() - VremyTime) >= 3000) {  // Пауза чтобы выговорилась кукушка

    // mp3.playMP3Folder((dfp.golos * 100) + (hour) ? hour : 24);
    // mp3.playMP3Folder(hour ? (hour + dfp.golos * 100) : (24 + dfp.golos * 100));
    mp3.play((dfp.golos * 30) + hour + 2);
    Serial.println((String) " ♫ " + hour + " часов");

    if (hour == dfp.start_kuku) Utro_flag = 1;
    if (hour == dfp.stop_kuku) Vecher_flag = 1;
    Vremy_play_flag = 0;
  }
}

void Utro() {
  static uint32_t utroTime;
  if (Utro_flag == 1) {
    utroTime = millis();
    Utro_play_flag = 1;
    Utro_flag = 0;
  }
  if (Utro_play_flag == 1 && (millis() - utroTime) >= 3000) {  // Пауза чтобы выговорилось время
    mp3.play((dfp.golos * 30) + 28);
    Serial.println(" ♫ Доброе утро");  //для отладки
    Utro_play_flag = 0;
  }
}
void Vecher() {
  static uint32_t VecherTime;
  if (Vecher_flag == 1) {
    VecherTime = millis();
    Vecher_play_flag = 1;
    Vecher_flag = 0;
  }
  if (Vecher_play_flag == 1 && (millis() - VecherTime) >= 3000) {  // Пауза чтобы выговорилось время
    mp3.play((dfp.golos * 30) + 29);
    Serial.println(" ♫ Доброй ночи");  //для отладки
    Vecher_play_flag = 0;
  }
}
