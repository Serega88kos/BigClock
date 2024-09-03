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
      mp3.setVolume(dfp.grom_mp3);
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
DFPLAYER_MODULE_TYPE board;
void DFPlayer_setup() {
  switch (dfp.board) {
    case 1:
      board = DFPLAYER_FN_X10P;
      break;
    case 2:
      board = DFPLAYER_HW_247A;
      break;
    case 3:
      board = DFPLAYER_NO_CHECKSUM;
      break;
    default:
      board = DFPLAYER_MINI;
      break;
  }

  Serial.println("Инициализация DFPlayedfp... (может занять 3~5 секунд)");                                    //для отладки
  mp3Serial.begin(MP3_SERIAL_SPEED, SWSERIAL_8N1, MP3_RX_PIN, MP3_TX_PIN, false, MP3_SERIAL_BUFFER_SIZE, 0);  //false=сигнал не инвертирован, 0=размер буфера ISR/RX (общий с последовательным буфером TX)
  mp3.begin(mp3Serial, MP3_SERIAL_TIMEOUT, board, false);                                                     //"DFPLAYER_HW_247A" см. ПРИМЕЧАНИЕ, false = нет обратной связи от модуля после команды
  mp3.stop();                                                                                                 //если плеер работал во время перезагрузки ESP8266
  mp3.reset();                                                                                                //сбросить все настройки по умолчанию
  mp3.setSource(2);                                                                                           //1=USB-диск, 2=TF-карта, 3=Aux, 4=спящий режим, 5=NOR Flash
  mp3.setEQ(0);                                                                                               //0=Выкл., 1=Поп, 2=Рок, 3=Джаз, 4=Классика, 5=Бас
  mp3.setVolume(dfp.grom_mp3);                                                                                //0..30, модуль сохраняет громкость при сбое питания

  Serial.print(" ♫ Громкость: ");   //для отладки, Получить громкость
  Serial.println(mp3.getVolume());  //0..30

  mp3Serial.enableRx(false);  //отключить прерывания на RX-контакте, меньше накладных расходов, чем mp3Serial.listen()

  mp3.playMP3Folder(26);
  Serial.println(" ♫ Приветствие");  // Музыкальное приветствие
}

void DFPlayer() {              // Функция срабатывающая раз в час
  if (dfp.status_kuku) {       // Если кукушка включена(проверка второй раз для корректной работы портала)
    if (dfp.kuku_mp3_check) {  // Если включено воспроизведение кукушки
      Serial.println(" ♫ Ку-Ку");
      mp3.playMP3Folder(25);  // проигрываем файл c кукушкой
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
    mp3.playMP3Folder((dfp.golos * 100) + hour + 1);
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
    mp3.playMP3Folder((dfp.golos * 100) + 27);
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
    mp3.playMP3Folder((dfp.golos * 100) + 28);
    Serial.println(" ♫ Доброй ночи");  //для отладки
    Vecher_play_flag = 0;
  }
}
