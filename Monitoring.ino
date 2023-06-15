//////////// функция для народного мониторинга
void narodMonitor() {  
  WiFi.setAutoReconnect(true);

  String buf = "#" + WiFi.macAddress();
  // добавление и удаление датчиков по шаблону
  if (narod.nm_tempH) {
    buf = buf + "\n#T1#" + FtempH;
  }
  if (narod.nm_tempS) {
    buf = buf + "\n#T2#" + FtempS;
  }
  if (narod.nm_pres) {
    buf = buf + "\n#T3#" + Fpres;
  }
  if (narod.nm_hum) {
    buf = buf + "\n#T4#" + hum;
  }
  buf = buf + "\n##";
  WiFiClient client;
  client.connect("narodmon.ru", 8283);
  client.print(buf);
  //Вывод в монитор порта
  Serial.println((String) + "Показания отправлены! " + FtempH + " | " + FtempS + " | " + Fpres + " | " + hum);
}
