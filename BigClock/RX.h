/*
  Arduino IDE 1.8.13 версия библиотеки RX 4.3.1 релиз от 07.12.24

  Автор Radon-lab.
*/
#define SLOW_MODE 1  //режим приема данных(0 - быстрый | 1 - медленный)

#if SLOW_MODE
#define BASE_BIT_TIME 1500
#else
#define BASE_BIT_TIME 375
#endif

#define MAX_BIT_TIME (BASE_BIT_TIME * 5)

#define HIGH_BIT_TIME (BASE_BIT_TIME / 2)
#define LOW_BIT_TIME (BASE_BIT_TIME + HIGH_BIT_TIME)
#define STOP_BIT_TIME (BASE_BIT_TIME + LOW_BIT_TIME)
#define PITCH_TIME (STOP_BIT_TIME)
#define START_BIT_TIME (BASE_BIT_TIME + STOP_BIT_TIME)

#define RECEIVE_DS_ADDR_LENGTH 8
#define RECEIVE_DS_TEMP_LENGTH 9

enum {
  RECEIVE_IDLE,
  RECEIVE_START,
  RECEIVE_CRC_ERROR,
  RECEIVE_ADDR_ERROR,
  RECEIVE_LENGTH_ERROR,
  RECEIVE_STOP_BIT_ERROR,
  RECEIVE_ADDR_OK,
  RECEIVE_DATA_OK
};

uint16_t receiveTime;     //счетчик импульса приёма
uint8_t receiveStatus;    //регистр статуса приёмника
uint8_t receiveBytes;     //регистр принятых байт
uint8_t receiveBits;      //регистр принятых бит
uint8_t receiveAddr;      //адрес передатчика
uint8_t receiveData[10];  //буфер приёмника

uint8_t receivePin;  //пин приемника


uint8_t checkCRC(uint8_t* buff, uint8_t size) {                            //сверка контрольной суммы
  uint8_t crc = 0;                                                         //буфер проверки crc
  uint8_t data = 0;                                                        //буфер нового байта
  for (uint8_t c = 0; c < size; c++) {                                     //проверяем все байты
    data = buff[c];                                                        //скопировали новый байт из буфера
    for (uint8_t i = 0; i < 8; i++) {                                      //считаем для всех бит
      crc = ((crc ^ data) & 0x01) ? (crc >> 0x01) ^ 0x8C : (crc >> 0x01);  //рассчитываем значение
      data >>= 0x01;                                                       //сдвигаем буфер
    }
  }
  return crc;  //возвращаем остаток контрольной суммы
}

#ifdef __AVR__
void receiveCheck(void) {
#else
void ICACHE_RAM_ATTR receiveCheck(void) {
#endif
  static uint32_t _us;

  if (digitalRead(receivePin)) _us = micros();  //если высокий уровень
  else {                                        //иначе низкий уровень
    uint32_t temp = micros() - _us;             //запомнили время
    if (temp < MAX_BIT_TIME) receiveTime = (uint16_t)temp;
    else receiveTime = MAX_BIT_TIME;
    _us += temp;

    if (receiveTime >= HIGH_BIT_TIME) {  //если обнаружили спад и длинна импульса больше минимальной
      switch (receiveStatus) {
        case RECEIVE_IDLE:
        case RECEIVE_CRC_ERROR:
        case RECEIVE_ADDR_ERROR:
        case RECEIVE_LENGTH_ERROR:
        case RECEIVE_STOP_BIT_ERROR:
          if (receiveTime >= HIGH_BIT_TIME) {     //если низкий уровень и длинна импульса больше минимальной
            receiveBits <<= 0x01;                 //сместили биты маски приема
            if (receiveTime >= START_BIT_TIME) {  //если получили старт бит
              if (receiveBits == 0xFE) {          //если получили все биты раскачки
                receiveBits = 0;                  //сбросили буфер раскачки
                receiveBytes = 0;
                receiveData[0] = 0;  //очищаем байт буфера приёма
                receiveStatus = RECEIVE_START;
              } else receiveBits = 0;                                   //сбросили буфер раскачки
            } else if (receiveTime >= PITCH_TIME) receiveBits |= 0x01;  //установли бит маски приема
          }
          break;
        case RECEIVE_START:
        case RECEIVE_ADDR_OK:
          receiveBits++;                                                                  //добавили бит
          receiveData[receiveBytes] >>= 0x01;                                             //сместили байт
          if (receiveTime < LOW_BIT_TIME) receiveData[receiveBytes] |= 0x80;              //утанавливаем единицу в буфер
          else if (receiveTime >= STOP_BIT_TIME) {                                        //иначе если был стоп бит
            if (receiveBytes) {                                                           //если принят хотя-бы один байт
              if (!checkCRC(receiveData, receiveBytes)) receiveStatus = RECEIVE_DATA_OK;  //если контрольная сумма совпала
              else receiveStatus = RECEIVE_CRC_ERROR;
            } else receiveStatus = RECEIVE_STOP_BIT_ERROR;
            receiveBits = 0;  //сбросили буфер раскачки
          }
          if (receiveBits >= 8) {
            receiveBytes++;
            receiveBits = 0;
            if (receiveStatus != RECEIVE_ADDR_OK) {  //если адрес не прочитан
              receiveStatus = RECEIVE_ADDR_OK;       //установили флаг прочитанного адреса
              receiveAddr = receiveData[0];          //запомнили новый адрес передатчика
              receiveBytes = 0;
            }
            receiveData[receiveBytes] = 0;  //очищаем байт буфера приёма
            if (receiveBytes >= 10) {
              receiveBits = 0;  //сбросили буфер бит
              receiveStatus = RECEIVE_LENGTH_ERROR;
            }
          }
          break;
      }
    }
  }
}

void initPinRX(uint8_t _pin) {
  pinMode(_pin, INPUT);  //инициализация приемника данных
  attachInterrupt(digitalPinToInterrupt(_pin), receiveCheck, CHANGE);

  receivePin = _pin;
  receiveStatus = RECEIVE_IDLE;
  receiveBits = 0;
  receiveBytes = 0;
}
void waitAddrRX(void) {
  receiveAddr = 0;
}
uint8_t getAddrRX(void) {
  return receiveAddr;
}
uint8_t getStatusRX(void) {
  return receiveStatus;
}

uint8_t availableDataRX(void) {
  if (receiveStatus == RECEIVE_DATA_OK) return receiveBytes;
  return 0;
}
uint8_t getDataRX(void) {
  if ((receiveStatus == RECEIVE_DATA_OK) && receiveBytes) {
    uint8_t temp = receiveData[0];
    if (!--receiveBytes) receiveStatus = RECEIVE_IDLE;
    for (uint8_t i = 0; i < receiveBytes; i++) receiveData[i] = receiveData[i + 1];
    return temp;
  }
  return 0;
}

boolean availableTempRX(void) {
  uint8_t temp = availableDataRX();
  if (temp == RECEIVE_DS_TEMP_LENGTH) return 1;
  else if (temp) {
    receiveBytes = 0;
    receiveStatus = RECEIVE_IDLE;
  }
  return 0;
}
float getTempRX(void) {
  if (availableTempRX()) {
    float temp = (int16_t)(((uint16_t)receiveData[1] << 8) | receiveData[0]) / 16.0;
    receiveBytes = 0;
    receiveStatus = RECEIVE_IDLE;
    return temp;
  }
  return 0;
}