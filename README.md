### Описание проекта

- Данный проект является продолжением проекта [Большие часы на WS2812, ESP8266, Народный мониторинг](https://community.alexgyver.ru/threads/bolshie-chasy-na-ws2812-esp8266-narodnyj-monitoring.5067/)
перенесенный на платформу GyverHub.
- Прошивку можно обновлять автоматически.

    * Благодарность за помощь в исправление ошибок: **Sergo_ST**

### Ипользуемые библиотеки

* Менеджер библиотек
    * FileData
    * GyverNTP
    * GyverDS3231
    * GyverBME280
    * GyverHTU21D
    * GyverDS18
    * GyverHTU21D
    * **FastLED 3.7.6**
    * **WebSockets 2.4.0**
    * **GSON 1.4.8**
    * DHT sensor library
 * Вручную
    * DFPlayer https://github.com/enjoyneering/DFPlayer
    * GyverHub https://github.com/GyverLibs/GyverHub
    * AHTxx https://github.com/enjoyneering/AHTxx

### Конфигурация 

* тип ленты GRB, RGB

![1](https://github.com/Serega88kos/Clock_ESP8266_WS2812_IOT/assets/57561983/47c1f3bc-1c69-47b9-8833-9de04428b040)
![2](https://github.com/Serega88kos/Clock_ESP8266_WS2812_IOT/assets/57561983/0b145928-88a9-4b65-932c-ddcf5330594d)

### Железо

* Проект собран на базе мк ESP8266 (NodeMCU или Wemos D1 mini)
* модуль температуры, давления и влажности BMP/BME280
* датчик температуры DS18B20
* датчик влажности HTU21D, для замены BME280 (если нужен)
* Фоторезистор или готовый модуль
* DFPlayer mini для голосовых оповещений времени
* модуль точного времени DS3231 (если нужен)
* БП 5В
* Радиодатчик на GPIO 13 https://community.alexgyver.ru/threads/radioudlinitel-datchika-temperatury-ds18b20.6173/

### Схемы

* Схемы https://github.com/Serega88kos/BigClock/tree/main/schemes

### Возможности

* Мониторинг показаний через сервис [Народный мониторинг](https://narodmon.ru/?invite=asm)
* OTA обновление при обнаружение новой версии
 
### Корпус и материалы

- Моя сборка выполнена на ДВП, на котором наклеены отрезки ленты, спаяны и поверх наложен лист сендвич панели, в котором вырезаны цифры.
- Поверх, для рассеиванивания света, положен лист бумаги
- Корпус выполнен из дерева

### Установка

- Если в качестве микроконтроллера вы используете Wemos D1 - в менеджере плат для компиляции все равно выбирайте "NodeMCU v1.0 (ESP-12E)", в противном случае, если выберете плату Wemos D1 (xxxx), - будет     работать нестабильно.
- Выбирайте ядро ESP версии 2.7.4.
- Папка mp3 должна быть в корне, на карте памяти.

 ### Быстрая установка
- Подключить кабель к ESP.
- Перейти на https://ota.gyver.ru/ установить драйвера, если требуется.
- Добавить Serega88kos/BigClock
- Загрузить прошивку.

### Настройка

- Если не задавали настройки WiFi, то стартует точка, адрес стандартный 192.168.4.1 пароль по умолчанию administrator, задавать не менее 8 символов, иначе название точки не отобразится, а будет ее внутреннее имя, типа ESP12345.
- Пароль на вход: 1234
- Адрес портала http://hub.gyver.ru/
- Для работы выполнить настройку: [тут](https://github.com/GyverLibs/GyverHub/wiki/2.-%D0%9D%D0%B0%D1%87%D0%B0%D0%BB%D0%BE-%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D1%8B)
- Для подключения к сервису "Народный мониторинг" MAC адрес отображается на соответствующей странице, вводите этот адрес при регистрации устройства.

### История версий

* Первый релиз 15.06.23 v1.0
Добавлена погода на библиотеке [Forecaster](https://github.com/GyverLibs/Forecaster) для теста во вкладке мониторинга
* 27.06.23 навел порядок в репозитории, буду обновлять библиотеку GyverHub...
* 28.06.23 обновлена библиотека GyverHub v1.1
* 09.07.23 исправлен режим плеера, удален прогноз погоды
* 22.08.23 исправлены точки, поправлены ошибки в коде, благодарим за помощь @Sergo_ST
* 18.02.24 (тест вариант) исправлены точки для НГ режима, уменьшен интервал мигания в 2 раза в НГ режиме, исправление в ночном режиме, исправлена ошибка вызова плеера, в текущей версии минус температуры располагается рядом с цифрой, выбор цвета часов теперь через выбор готовых цветов, показания уличной температуры реализовано через градиент, оптимизация кода, возможно что-то еще...
* 06.03.24 (тест вариант) Прошивка теперь универсальная, не нужно настраивать под каждый вариант кол-ва СД в сегменте. Переход на новую версию GyverHub. Изменение интерфейса. Появились новые цветовые режимы.
* 25.03.24 Вот и вышел этот релиз :) Было много переделок. От цветовых режимов отказались.
* 05.04.24 2.1 - 2.2 Фикс ночного режима со сменой цвета. Добавлены точки для режима дата.
* 09.04.24 2.3 Фикс запуска хаба у точки доступа, запуск автоматический, либо используйте в хабе IP 192.168.4.100
* 01.05.24 2.4 Добавлен выбор варианта платы для DFP
* 10.06.24 2.5 Изменено оформление. Показания с датчиков на странице отображаются в реальном времени.
* 03.09.24 2.6 Небольшие косметические изменения. Заменены устаревшие библиотеки. В описании требуемые библиотеки и версии указаны. Остальное последнее.
* 11.11.24 2.7 Исправлен ночной режим, исправлены точки в часах (спасибо Sergo_ST). Добавлен выбор типа ленты (GRB и RGB, остальное по надобности). Увеличено кол-во СД в сегменте до 10. Оптимизирован код. Косметические правки.
* 14.11.24 2.8 Исправлен режим освещения, кукушка, добавлен новый градиент температуры, добавлен пароль на вход (1234), изменения в интерфейсе.
* 16.12.24 3.0 Добавлено: радиодатчик, режим обмена показаниями датчика уличной температуры по UDP, сканер WiFi в режиме ТД. Небольшие правки в коде и интерфейсе.
* 22.12.24 3.1 Исправлено: баги с яркостью в ночном режиме, прием от радиодатчика. Небольшие изменения.
* 02.01.25 3.2 Исправление ошибок.
* 12.02.25 3.3 Добавлена поддержка новых датчиков (HTU21D, DHT22, AHT10/20), исправление ошибок, оптимизация, изменение интерфейса.
* 13.02.25 3.4 Добавлен режим, чтобы часы не уходили в ночной режим, если светло. Небольшие исправления.
* 12.05.25 3.5 Заменена библиотека для mp3 модуля. Добавлены новые палитры. Изменена логика в датчиках. Изменен интерфейс, пересортированы виджеты. Улучшен режим синхронизации времени. Добавлена анимация. Другие небольшие улучшения.
