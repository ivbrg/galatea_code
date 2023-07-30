
/*/ ============== VP адреса переменных в DWIN ================
  ***коды страниц vp:***
  10 01 | 01 = режим;
  10 01 | 01 = загрузка;
  10 01 | 02 = разгрузка;
  10 01 | 03 = мойка;
  10 01 | 04 = калибровка;  

  *** предупреждения ***
  
  11 00 | i

  11 01 | Serial
  11 02 | Serial1
  11 03 | temp1
  11 04 | temp2
  11 05 | MC
  11 06 | MK
  11 07 | K
  11 08 | C

  11 10 | 01 = straciatella
  11 11 | 02 = burrata
  11 12 | 03 = rotation

  *** пароль ***
  12 00 | = password  
  12 05 | = wrong password icon

  **кнопки ***
  
  20 00 | 01 = калибровка
  20 01 | 01 = режим работы

  20 02 | 00 = тэны вкл/выкл
  20 03 | 00 = нож вкл/выкл
  20 04 | 00 = круг вкл/выкл
  20 05 | 00 = пресс вкл/выкл

  20 06 | 00 = изменение температуры
  20 07 | 00 = изменение времени пайки

  20 08 | 00 = загрузка страчателлы
  20 09 | 00 = загрузка сливок
  20 0A | 00 = разгрузка страчателлы
  20 0B | 00 = разгрузка сливок
  20 0C | 00 = мойка 
  20 0D | 00 = калибровка 


  *** адреса ручного включения цилиндров
  25 00 | А0
  25 01 | А1
  25 02 | А2
  25 03 | А3
  25 04 | А4
  25 05 | А5
  25 06 | А6
  25 07 | А7
  25 08 | А8
  25 09 | А9
  25 10 | А10
  25 11 | А11
  

  *** адреса переменных экрана ****
  30 00 - температура с экрана (float, 4 байта)
  30 04 - температура отправляемая на экран (float, 4 байта)
  30 08 - время пайки с экрана (float, 4 байта)
  30 0B - время пайки на экран (float, 4 байта)
  30 10 - обороты (byte)
  30 12 - масса (float, 4 байта)
  30 14 - % сливок 
  30 16 - температура левого тэна
  30 1A - температура правого тэна
  30 20 - скорость

  50 00 - текст ошибок

*/

// ============== чтение Serial ==================
void readInput() {
  byte c;
  c = Serial1.read();

  switch (readPosition) {
    case 0:  //читаем нулевой байт начала сообщения
      if (c == 0x5A) { readPosition = 1; }
      if (DEBUG) { printByte(c); }
      break;

    case 1:  //читаем перый байт начала сообщения
      if (c == 0xA5) { readPosition = 2; }
      if (DEBUG) { printByte(c); }
      break;

    case 2:  //читаем длинну сообщения
      dwinMessageLength = c;
      readPosition = 3;
      if (DEBUG) { printByte(c); }
      break;

    case 3:  //читаем байт
      if (DEBUG) { printByte(c); }
      if (c == 0x83) { readPosition = 4; break;}
      if (c == 0x82) {                      //ответ двина на полученное сообщение 0x4F 0x4B
        Serial1.read();  //0x4F
        Serial1.read();  //0x4B
        Serial.println("Got 0x4F 0x4B");
        readPosition = 0;
      }
      break;

    case 4:  //читаем сообщение
      if (DEBUG) { printByte(c); }
      
      inputBuf[inputBufPosition] = c;
      inputBufPosition += 1;

      if (inputBufPosition == (dwinMessageLength - 1)) {
        inputMessageComplete = 1;
        readPosition = 0;
        inputBufPosition = 0;
        Serial.println(" got message ");
      }
      break;

    default:
      Serial.println("readInput ERROR!!!");
      showError();
      break;
  }
}

// ============== парсинг адреса из сообщения ==================
void parseBuffer() {
  switch (inputBuf[0]) {

    case 0x4f:  //ответ двина об успехе
      break;

    case 0x10:  //адрес страницы
      changePage();
      break;

    case 0x12:  //пароль
      readPassword();
      break;

    case 0x20:  //нажали кнопку
      makeResponce();
      break;

    case 0x25:  //включить цилиндр
      cylinders();
      break;

    case 0x30:  //изменить параметр
      changeVar();
      break;

    default:
      Serial.print("Parse address unknown: ");
      Serial.println(inputBuf[0], HEX);
      showError();
      //showBuffer(inputBuf);
      break;
  }
}

// ============== изменение страницы ==================
void changePage() {  // VP10xx
  /*
  changePageBuff[] = {0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x00 };
  settings page id:
  downloading = 0410
  unloading = 0420
  washing = 0430
  calibration = 0440
  */
  switch (inputBuf[4]) {

    case 0x00:  //обработка страницы настройки
      changePageBuff[8] = 0x01;
      changePageBuff[9] = lastSetPageID;
      Serial1.write(changePageBuff, 10);
      break;

    //===== переход на страницу загрузка =====
    case 0x01:
      lastSetPageID = 0x9A;
      if (DEBUG) {
        //showBuffer(inputBuf);
      }
      break;

    //===== переход на страницу разгрузка =====
    case 0x02:
      lastSetPageID = 0xA4;
      if (DEBUG) {
        //showBuffer(inputBuf);
      }
      break;

    //===== переход на страницу мойка =====
    case 0x03:
      lastSetPageID = 0xAE;
      if (DEBUG) {
        //showBuffer(inputBuf);
      }
      break;

    //===== переход на страницу калибровка =====
    case 0x04:
      lastSetPageID = 0xB8;
      if (DEBUG) {
        //showBuffer(inputBuf);
      }
      break;

    default:
      if (DEBUG) {
        Serial.println("Change page adress error!");
        showBuffer(inputBuf);
      }
      break;
  }
}
// ============== адреса действий ==================
void makeResponce() {  // VP20xx
  switch (inputBuf[1]) {

    case 0x00:  // калибровка
      calibration();
      break;

    case 0x01:  // вкл/выкл работы
      operatingMode(inputBuf[4]);
      break;

    case 0x02:  // вкл/выкл тэна
      heaterIsOn = inputBuf[4];
      if(DEBUG){
        Serial.print("heaterIsOn = ");
        Serial.println(heaterIsOn);
      }
      break;

    case 0x03:  // вкл/выкл ножей
      knifeIsOn = inputBuf[4];
      break;

    case 0x04:  // вкл/выкл прессов
      pressIsOn = inputBuf[4];
      break;
      
    case 0x05:  // вкл/выкл мотора
      rotationIsON = inputBuf[4];
      Serial.print("rotatiIsOn = ");
      Serial.println(rotationIsON);
      break;

    case 0x06:  // изменение температуры
      if (inputBuf[4] > 0) {
        userTemp += 0.1;
        sendUserTemp();
      } else {
        userTemp -= 0.1;
        sendUserTemp();
      }
      break;

    case 0x07:  // изменение время пайки
      if (inputBuf[4] > 0) {
        time += 0.1;
        sendTime();
      } else {
        time -= 0.1;
        sendTime();
      }
      break;
    default:
      Serial.print("Make responce address unknown: ");
      Serial.println(inputBuf[1], HEX);
      showError();
      //showBuffer(inputBuf);
      break;
  }
}
// ============== адреса ручного включения цилиндров машины ==================
void cylinders(){   // VP25xx
  switch (inputBuf[1]){
    case 0x00:
      digitalWrite(SOLENOID1, inputBuf[4]);
      Serial.println(inputBuf[4]);
      break;
    case 0x01:
      digitalWrite(SOLENOID2, inputBuf[4]);      
      Serial.println(inputBuf[4]);
      break;
    case 0x02:
      digitalWrite(SOLENOID3, inputBuf[4]);
      Serial.println(inputBuf[4]);
      break;
    case 0x03:
      digitalWrite(SOLENOID4, inputBuf[4]);
      break;
    case 0x04:
      digitalWrite(SOLENOID5, inputBuf[4]);      
      break;
    case 0x05:
      digitalWrite(SOLENOID6, inputBuf[4]);      
      break;
    case 0x06:
      digitalWrite(SOLENOID7, inputBuf[4]);
      break;
    case 0x07:
      digitalWrite(SOLENOID8, inputBuf[4]);
      break;
    case 0x08:
      digitalWrite(SOLENOID9, inputBuf[4]);
      break;
    case 0x09:
      digitalWrite(SOLENOID10, inputBuf[4]);
      break;
    case 0x10:
      digitalWrite(SOLENOID11, inputBuf[4]);
      break;
    case 0x11:
      digitalWrite(SOLENOID12, inputBuf[4]);
      break;
    
    default:
      Serial.print("Cylinder address unknown: ");
      Serial.println(inputBuf[1], HEX);
      //showBuffer(inputBuf);
      break;
  }
}
// ============== адреса параметров машины ==================
void changeVar() {  // VP30xx
  switch (inputBuf[1]) {
    case 0x00:  //изменение температуры
      userTempCharArray[0] = inputBuf[3];
      userTempCharArray[1] = inputBuf[4];
      userTempCharArray[2] = inputBuf[5];
      userTempCharArray[3] = inputBuf[6];

      userTemp = atof(userTempCharArray);
 
      sendUserTemp();

      if (DEBUG) {   
        Serial.print("temp atof = ");
        Serial.println(userTemp);

        Serial.print(outputUserTempBuf[6], HEX);
        Serial.print(outputUserTempBuf[7], HEX);
        Serial.print(outputUserTempBuf[8], HEX);
        Serial.print(outputUserTempBuf[9], HEX);

        Serial.println();
      
      }
      break;

    case 0x08:  //изменение времени пайки
      userTimeCharArray[0] = inputBuf[3];
      userTimeCharArray[1] = inputBuf[4];
      userTimeCharArray[2] = inputBuf[5];
      userTimeCharArray[3] = inputBuf[6];

      time = atof(userTimeCharArray);
 
      sendTime();

      if (DEBUG) {   
        Serial.print("time atof = ");
        Serial.println(time);

        Serial.print(outputTimeBuf[6], HEX);
        Serial.print(outputTimeBuf[7], HEX);
        Serial.print(outputTimeBuf[8], HEX);
        Serial.print(outputTimeBuf[9], HEX);
        Serial.println();
      }
      break;

    case 0x10:  //изменение оборотов
      rotation = inputBuf[4];
      sendRotation();
      break;
    case 0x12:  //изменение массы
      massa = (inputBuf[3] << 8) | inputBuf[4];
      sendMassa();
      break;
    case 0x14:  //изменение сливок
      cream = (inputBuf[3] << 8) | inputBuf[4];
      sendCream();
      break;
    default:
      Serial.print("Change var address unknown: ");
      Serial.println(inputBuf[1], HEX);
      showError();
      //showBuffer(inputBuf);
      break;
  }
}

// ============== калибровка машины ==================
void calibration() {
  byte allGood = 0x01;
  byte allBad = 0x02;

  byte calibrBuff[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X00, 0X0, 0x0 };
  calibrBuff[5] = 0x01;
  calibrBuff[7] = allGood;
  Serial1.write(calibrBuff, 8);
  delay(100);

  calibrBuff[5] = 0x02;
  calibrBuff[7] = allGood;
  Serial1.write(calibrBuff, 8);
  delay(100);

  calibrBuff[5] = 0x03;
  calibrBuff[7] = allGood;
  Serial1.write(calibrBuff, 8);
  delay(100);

  calibrBuff[5] = 0x04;
  calibrBuff[7] = allGood;
  Serial1.write(calibrBuff, 8);
  delay(100);

  calibrBuff[5] = 0x05;
  calibrBuff[7] = allBad;
  Serial1.write(calibrBuff, 8);
  delay(100);

  calibrBuff[5] = 0x06;
  calibrBuff[7] = allGood;
  Serial1.write(calibrBuff, 8);
  delay(100);

  calibrBuff[5] = 0x07;
  calibrBuff[7] = 0x01;
  Serial1.write(calibrBuff, 8);
  delay(100);

  calibrBuff[5] = 0x08;
  calibrBuff[7] = 0x01;
  Serial1.write(calibrBuff, 8);
  delay(100);

  /*
    1. чтение датчика закрытой крышки
    2. чтение датчика круга (проверить на месте ли круг)
    3. проверка дитчиков цилиндров
    4. проверка объема
  */

  byte changeToMainPage[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x1E };
  Serial1.write(changeToMainPage, 10);

  calibrDone = 1;
}
// ============== загрузка параметров в экран ================
void setDwin() {
  sendUserTemp();   //  загрузка значения температуры
  sendTime(); 
  sendRotation();
  sendMassa();
  sendCream();
  sendLoadStrach();
  sendLoadCream();
}

// ============== изменить состояние кнопки на двине ========
void operatingMode(byte i) {

  digitalPedal = i;

  // change diaplay button state
  //byte switchBuff[] = {0X5A, 0XA5, 0X5, 0X82, 0X20, 0X00, 0X0, 0x0};
  switchBuff[4] = 0x20;  // display button switch address

  /*
  if(i == 1){
    switchBuff[7] = i;    
  } else{
    switchBuff[7] = i;
  }

  for (byte n = 2; n < 6; n++){
      switchBuff[5] = n;
      Serial1.write(switchBuff, 8);
  }
  */
}

// обработка введённого пользователем пароля
void readPassword() {
  // проверка на 1488
  if (inputBuf[3] == 0x31 && inputBuf[4] == 0x34 && inputBuf[5] == 0x38 && inputBuf[6] == 0x38) {
    //переход на страницу с настройками
    Serial1.write(settingsPageAddrBuf, 10);
    
    //проверка состояния иконки предупреждения
    if(wrongPassBuf[7] != 0){
      wrongPassBuf[7] = 0;
      Serial1.write(wrongPassBuf, 8);
    }

    if(DEBUG){
      Serial.println("GOT RIGHT PASS!");
    }
  } else {
    //включение иконки с предупреждением о неправильном пароле
    wrongPassBuf[7] = 1;
    Serial1.write(wrongPassBuf, 8);
    if(DEBUG){
      Serial.println("GOT WRONG PASS!");
    }
  }
}



//****************** отправка сообщений на экран *******************

// ============== отправка температуры на экран ===================
void sendUserTemp(){
  byte* f_byte = reinterpret_cast<byte*>(&userTemp);
  memcpy(userTempCharArray, f_byte, 4);
    
  outputUserTempBuf[6] = userTempCharArray[3];
  outputUserTempBuf[7] = userTempCharArray[2];
  outputUserTempBuf[8] = userTempCharArray[1];
  outputUserTempBuf[9] = userTempCharArray[0];
  Serial1.write(outputUserTempBuf, 10);
}
// ============== отправка времени пайки на экран ===================
void sendTime(){
  byte* f_byte = reinterpret_cast<byte*>(&time);
  memcpy(userTimeCharArray, f_byte, 4);
    
  outputTimeBuf[6] = userTimeCharArray[3];
  outputTimeBuf[7] = userTimeCharArray[2];
  outputTimeBuf[8] = userTimeCharArray[1];
  outputTimeBuf[9] = userTimeCharArray[0];
  Serial1.write(outputTimeBuf, 10);
}
// ============== отправка вреколичества оборотов на экран =================
void sendRotation(){
  outputRevolBuf[6] = highByte(rotation);
  outputRevolBuf[7] = lowByte(rotation);
  Serial1.write(outputRevolBuf, 8);

  if (DEBUG){
    Serial.print("rotation = ");
    Serial.println(rotation);
  }
}
// ============== отправка массы на экран ===================
void sendMassa(){
  outputMassaBuf[6] = highByte(massa);
  outputMassaBuf[7] = lowByte(massa);
  Serial1.write(outputMassaBuf, 8);

  if (DEBUG){
    Serial.print("massa = ");
    Serial.println(massa);
  }
}
// ============== отправка массы на экран ===================
void sendCream(){
  outputCreamBuf[6] = highByte(cream);
  outputCreamBuf[7] = lowByte(cream);
  Serial1.write(outputCreamBuf, 8);

  if (DEBUG){
    Serial.print("cream = ");
    Serial.println(cream);
  }
}
// ============== отправка левой температуры ================
void sendLeftTemp(){
  byte* f_byte = reinterpret_cast<byte*>(&tempToSend);
  memcpy(leftTempCharArray, f_byte, 4);
    
  outputLeftTempBuf[6] = leftTempCharArray[3];
  outputLeftTempBuf[7] = leftTempCharArray[2];
  outputLeftTempBuf[8] = leftTempCharArray[1];
  outputLeftTempBuf[9] = leftTempCharArray[0];
  Serial1.write(outputLeftTempBuf, 10);
}
// ============== отправка количества циклов загрузки =======
void sendLoadStrach(){
  outputLoadBuf[6] = highByte(loadStrach);
  outputLoadBuf[7] = lowByte(loadStrach);
  Serial1.write(outputLoadBuf, 8);

  if (DEBUG){
    Serial.print("loadStrach = ");
    Serial.println(loadStrach);
  }
}
// ============== отправка количества циклов загрузки =======
void sendLoadCream(){
  outputLoadBuf[6] = highByte(loadStrach);
  outputLoadBuf[7] = lowByte(loadStrach);
  Serial1.write(outputLoadBuf, 8);

  if (DEBUG){
    Serial.print("loadStrach = ");
    Serial.println(loadStrach);
  }
}
// ============== отображает предупреждение на экране =======
void showError() {
  showErrorBuf[7] = 1;
  Serial1.write(showErrorBuf, 8);
}


// ======================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ =========================
// ============== отображение содержимого буффера в Serial ==================
void showBuffer(byte buf[BUFFER_SIZE]) {
  for (byte i = 0; i < BUFFER_SIZE; i++) {
    Serial.print(buf[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
}
// ============== шлёт байт в хексе в Serial ==================
void printByte(byte c) {
  Serial.print(c, HEX);
  Serial.print(" ");
}

/*// TODO ///

1. попробовать написать makeResponse через bitwise


*/