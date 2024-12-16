//////////// функция для народного мониторинга
void narodMonitor() {
  WiFi.setAutoReconnect(true);
  String buf = "#" + WiFi.macAddress();
  // добавление и удаление датчиков по шаблону
  if (nm.tempH) buf = buf + "\n#T1#" + FtempH;
  if (nm.tempS) buf = buf + "\n#T2#" + FtempS;
  if (nm.pres) buf = buf + "\n#T3#" + Fpres;
  if (nm.hum) buf = buf + "\n#T4#" + hum;
  buf = buf + "\n##";
  WiFiClient client;
  client.connect("narodmon.ru", 8283);
  client.print(buf);
  //Вывод в монитор порта
  Serial.println((String) + "Показания отправлены! " + FtempH + " | " + FtempS + " | " + Fpres + " | " + hum);
}

void sendUDP() {
  Udp.beginPacket("255.255.255.255", 8888);
  Udp.print(FtempS);
  Udp.endPacket();
  //Serial.println((String) + "sendUDP " + FtempS);
}

void readUDP() {
  Udp.beginPacket("255.255.255.255", 8888);
  if (Udp.parsePacket()) {
    String str = "none";
    str = Udp.readString();
    FtempS = str.toFloat();
    //Serial.println((String) + "readUDP " + str + "toFloat " + FtempS);
  }
}
