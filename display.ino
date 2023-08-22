
/*/ ============== VP адреса переменных в DWIN ================
  ***коды страниц vp:***
  10 01 | 10 = загрузка;
  10 01 | 11 = разгрузка;
  10 01 | 12 = мойка;

  *** предупреждения ***
  
  11 00 | i

  11 03 | temp1
  11 04 | temp2
  11 05 | MC
  11 06 | MK
  11 07 | K
  11 08 | C

  11 01 | адрес кнопки вкл/выкл
  11 02 | вдрес иконки вкл/выкл 
  11 10 | адрес кнопки straciatella вкл/выкл
  11 11 | адрес иконки straciatella иконка
  11 12 | адрес кнопки burrata вкл/выкл
  11 13 | адрес иконки burrata иконка
  11 14 | адрес кнопки rotation вкл/выкл
  11 15 | адрес иконки rotation иконка

  11 20 | popup для окна калибровки

  *** пароль ***
  12 00 | = password  
  12 05 | = wrong password icon
  
  *** калибровка ***
  12 06 | = предупреждение о не закрытой крышке

  13 00 | = cover sensor
  13 01 | = turn sensor
  13 02 | = ds18b20 left
  13 03 | = ds18b20 right
  13 04 | = current sensor left
  13 05 | = current sensor right
  13 06 | = solenoid sensor 1
  13 07 | = solenoid sensor 2
  13 08 | = solenoid sensor 3
  13 09 | = solenoid sensor 4
  13 0A | = solenoid sensor 5
  13 0B | = solenoid sensor 6
  13 0C | = solenoid sensor 7
  13 0D | = solenoid sensor 8
  13 0E | = solenoid sensor 9
  13 0F | = solenoid sensor 10
  13 10 | = solenoid sensor 11
  13 11 | = solenoid sensor 12
  13 12 | = solenoid sensor 13
  13 13 | = solenoid sensor 14

  **кнопки ***
  
  20 00 | 01 = калибровка
  20 01 | 01 = режим работы

  20 02 | 00 = тэны вкл/выкл

  20 03 | 01 = вкл/выкл загрузка
  20 04 | 01 = вкл/выкл разгрузка
  20 05 | 01 = вкл/выкл мойка

  !!!!!!!!!!!!!! 20 03 | 00 = нож вкл/выкл   - удалены с экрана
  !!!!!!!!!!!!!! 20 04 | 00 = круг вкл/выкл  - удалены с экрана
  !!!!!!!!!!!!!! 20 05 | 00 = пресс вкл/выкл - удалены с экрана

  20 06 | 00 = изменение температуры
  20 07 | 00 = изменение времени пайки

  20 08 | 00 = загрузка страчателлы
  20 09 | 00 = загрузка сливок
  20 0A | 00 = разгрузка страчателлы
  20 0B | 00 = разгрузка сливок
  20 0C | 00 = мойка 
  20 0D | 00 = калибровка 

  *** кнопки калибровки ***
  21 00 | 01 = начать калибровку
  
  21 01 | 01 = включить все/ 00 = выключить всё
  
  21 02 | 01 = объём
  21 03 | 01 = круг
  21 04 | 01 = прессы
  21 05 | 01 = нож
  21 06 | 01 = разгруз
  21 07 | 01 = ТЭНы

  *** адреса ручного включения цилиндров
  25 00 | 35    // голова сливок
  25 01 | 34    // голова страчателлы
  25 02 | 33    
  25 03 | 32
  25 04 | 31    // пресс-форсунка
  25 05 | 30    // цилиндр страчателлы
  25 06 | 39    // цилиндр сливок 
  25 07 | 29    // разгрузка
  25 08 | 28    // редуктор
  25 09 | 27    // левый пресс
  25 10 | 26    // правый пресс
  25 11 | 25    // нож

  *** адреса датчиков цилиндров 
  26 00 | A0  ss1
  26 01 | A1  ss2
  26 02 | A2  ss3
  26 03 | A3  ss4
  26 04 | A4  ss5
  26 05 | A5  ss6
  26 06 | A6  ss7
  26 07 | A7  ss8
  26 08 | A8  ss9
  26 09 | A9  ss10
  26 0A | A10 ss11
  26 0B | A11 ss12
  26 0C | A12 ss13
  26 0D | A13 ss14
  

  *** адреса переменных экрана ****
  30 00 - температура с экрана (float, 4 байта)
  30 04 - температура отправляемая на экран (float, 4 байта)
  30 08 - время пайки с экрана (float, 4 байта)
  30 0B - время пайки на экран (float, 4 байта)
  30 10 - обороты (byte)
  30 12 - масса (int)
  30 14 - % сливок (int)
  30 16 - температура левого тэна
  30 1A - температура правого тэна
  30 20 - циклы загрузки страчателлы
  30 22 - циклы загрузки сливок
  30 24 - циклы разгрузки страчателлы
  30 26 - циклы разгрузки сливок

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
      if (c == 0x83) {
        readPosition = 4;
        break;
      }
      if (c == 0x82) {   //ответ двина на полученное сообщение
        Serial1.read();  //0x4F
        Serial1.read();  //0x4B
        readPosition = 0;

        if (DEBUG) { Serial.println("Got 0x4F 0x4B"); }
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
        if (DEBUG) { Serial.println(" got message "); }
      }
      break;

    default:
      showError();
      if (DEBUG) { Serial.println("readInput() ERROR!!!"); }
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

    case 0x11:  //нажатие кнопки
      pressButton();
      break;

    case 0x12:  //пароль
      readPassword();
      break;

    case 0x20:  //нажали кнопку
      makeResponce();
      break;

    case 0x21:  //нажали кнопку
      calibrContor();
      break;

    case 0x25:  //включить цилиндр
      cylinders();
      break;

    case 0x30:  //изменить параметр
      changeVar();
      break;

    default:
      showError();

      if (DEBUG) {
        Serial.print("parseBuffer() address unknown: ");
        Serial.println(inputBuf[0], HEX);
      }

      break;
  }
}

// ============== изменение страницы ==================
void changePage() {  // VP10xx
  /* 
  changeSetPageBuff[] = {0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x00 };
  settings page id:
  downloading = 016 = [0x00, 0x10]
  unloading = 017 = [0x00, 0x11]
  washing = 018 = [0x00, 0x12]
  */
  switch (inputBuf[4]) {

    case 0x00:  //переход на страницу "режимы"
      changeSetPageBuff[9] = lastSetPageID;
      Serial1.write(changeSetPageBuff, 10);
      break;


    //===== переход на страницу загрузка =====
    case 0x01:
      lastSetPageID = 0x10;
      if (DEBUG) {
        showBuffer(inputBuf);
      }
      break;

    //===== переход на страницу разгрузка =====
    case 0x02:
      lastSetPageID = 0x11;
      if (DEBUG) {
        showBuffer(inputBuf);
      }
      break;

    //===== переход на страницу мойка =====
    case 0x03:
      lastSetPageID = 0x12;
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

// ============== страчателла/бурруту/оборот =================
void pressButton() {  // VP11xx 
  switch (inputBuf[1]) {  

    case 0x10:  // кнопка "страчателла"
      if (!straciatellaButtonFlag && !rotateDiskButtonFlag) {  //проверяем кнопки "буррата" и "оборот"
        burrataButtonFlag = inputBuf[4];
        if (DEBUG) {
          Serial.print("Got burrattaButton ");
          Serial.println(burrataButtonFlag);
        }
      }

  
    case 0x12:                                                 // кнопка "оборот"
      //if (!straciatellaButtonFlag && !rotateDiskButtonFlag) {  //проверяем кнопки "страчателла" и "буррата"
        rotateDiskButtonFlag = inputBuf[4];
        if (DEBUG) {
          Serial.print("Got rotateDiskButtonFlag ");
          Serial.println(rotateDiskButtonFlag);
        }
      
      //      if(rotateDiskButtonFlag && rotateDiskButtonCase != 0){
      //        rotateDiskButtonFlag = 0;
      //}
      break;

    case 0x14:  
      straciatellaButtonFlag++;
      if (DEBUG) {
        Serial.println("Got strachatellaButton ");
      }
      break;                                               // кнопка " масса"
      
      break;


    case 0x20:  // кнопка начала калибровки
      makeCalibration = 1;
      break;

    default:
      showError();
      if (DEBUG) {
        Serial.print("pressButton() address unknown: ");
        Serial.println(inputBuf[1], HEX);
      }
      break;
  }
}

// обработка введённого пользователем пароля
void readPassword() { // VP12
  // проверка на 1488
  if (inputBuf[3] == 0x31 && inputBuf[4] == 0x34 && inputBuf[5] == 0x38 && inputBuf[6] == 0x38) {
    // переход на страницу с настройками
    Serial1.write(settingsPageAddrBuf, 10);
    // переключаем кнопку в выкл
    mainButton = 0;
    mainButtonBuf[7] = mainButton;
    Serial1.write(mainButtonBuf, 8);

    //проверка состояния иконки предупреждения
    if (wrongPassBuf[7] != 0) {
      wrongPassBuf[7] = 0;
      Serial1.write(wrongPassBuf, 8);
    }
    if (DEBUG) {
      Serial.println("GOT RIGHT PASS!");
    }
  } else {
    //включение иконки с предупреждением о неправильном пароле
    wrongPassBuf[7] = 1;
    Serial1.write(wrongPassBuf, 8);
    if (DEBUG) {
      Serial.println("GOT WRONG PASS!");
    }
  }
}

// ============== адреса действий ==================
void makeResponce() {  // VP20xx
  switch (inputBuf[1]) {
    case 0x00:  // калибровка
      //calibration();
      break;

    case 0x01:  // вкл/выкл работы
      if (DEBUG) {
        Serial.print("workIsOn = ");
        Serial.println();
      }
      break;

    case 0x02:  // вкл/выкл тэна
      heaterIsOn = inputBuf[4];
      if (DEBUG) {
        Serial.print("heaterIsOn = ");
        Serial.println(heaterIsOn);
      }
      break;

    case 0x03:  // начать загрузку
      loadStart = inputBuf[4];
      if(loadStart){
        Serial1.write(loadPageAddrBuf, 10);    //перешли на страницу 
      } else {
        Serial1.write(changeSetPageBuff, 10); //вернулись на страницу "загрузка"
      }
      if (DEBUG) {
        Serial.print("start loading = ");
        Serial.println(inputBuf[4]);
      }
      break;


    case 0x04:  // начать разгрузку 
      unloadStart = inputBuf[4];
      if(loadStart){
        Serial.write(loadPageAddrBuf, 10);    //перешли на страницу 
      } else {
        Serial1.write(changeSetPageBuff, 10); //вернулись на страницу "загрузка"
      }
      if (DEBUG) {
        Serial.print("start loading = ");
        Serial.println();
      }
      break;
  /*
    case 0x03:  // вкл/выкл ножей
      knifeIsOn = inputBuf[4];
      if (DEBUG) {
        Serial.print("knifeIsOn = ");
        Serial.println(knifeIsOn);
      }
      break;

    case 0x04:  // вкл/выкл прессов
      pressIsOn = inputBuf[4];
      if (DEBUG) {
        Serial.print("pressIsOn = ");
        Serial.println(pressIsOn);
      }
      break;

    case 0x05:  // вкл/выкл мотора
      rotationIsON = inputBuf[4];
      if (DEBUG) {
        Serial.print("rotatiIsOn = ");
        Serial.println(rotationIsON);
      }
      break;

    case 0x06:  // изменение температуры
      if (inputBuf[4] > 0) {
        userTemp += 0.1;
        sendUserTemp();
      } else {
        userTemp -= 0.1;
        sendUserTemp();
      }
      if (DEBUG) {
        Serial.print("userTemp = ");
        Serial.println(userTemp);
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
      if (DEBUG) {
        Serial.print("time = ");
        Serial.println(time);
      }
      break;
  */
    default:
      showError();
      if (DEBUG) {
        Serial.print("Make responce address unknown: ");
        Serial.println(inputBuf[1], HEX);
      }
      break;
  }
}

// ============== режим калибровки ==================
void calibrContor() {  // VP21xx
  switch(inputBuf[1]){

    case 0x00:  // нажали на кнопку "начать калибровка"
        startCalibrFlag = 1;
      break;
    
    case 0x01: // нажали на кнопку "вкл/выкл всё"
      if(DEBUG){
        Serial.println("case 0x01");
      }
      if(inputBuf[3]){
        //вкл объём
        calibrButtonsBuf[5] = 0x02;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);

        //вкл круг
        calibrButtonsBuf[5] = 0x03;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);

        //вкл прессы
        calibrButtonsBuf[5] = 0x04;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);

        //вкл нож
        calibrButtonsBuf[5] = 0x05;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);

        //вкл разгруз
        calibrButtonsBuf[5] = 0x06;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);

        //вкл ТЭНы
        calibrButtonsBuf[5] = 0x07;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);

      } else {
        //выкл объём
        calibrButtonsBuf[5] = 0x02;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);

        //вкл круг
        calibrButtonsBuf[5] = 0x03;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);

        //вкл прессы
        calibrButtonsBuf[5] = 0x04;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);

        //вкл нож
        calibrButtonsBuf[5] = 0x05;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);

        //вкл разгруз
        calibrButtonsBuf[5] = 0x06;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);

        //вкл ТЭНы
        calibrButtonsBuf[5] = 0x07;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);
        
      }
      break;
    
    case 0x02: // нажали на кнопку "вкл/выкл объём"
      calibrVolume = inputBuf[3];
      if(DEBUG){
        Serial.print("calibrVolume: ");
        Serial.println(calibrVolume);
      }
      break;
    
    case 0x03: // нажали на кнопку "вкл/выкл круг"
      calibrDisk = inputBuf[3];
      if(DEBUG){
        Serial.print("calibrDisk: ");
        Serial.println(calibrDisk);
      }
      break;

    case 0x04: // нажали на кнопку "вкл/выкл прессы"
      calibrPress = inputBuf[3];
      if(DEBUG){
        Serial.print("calibrPress: ");
        Serial.println(calibrPress);
      }
      break;

    case 0x05: // нажали на кнопку "вкл/выкл нож"
      calibrKnife = inputBuf[3];
      if(DEBUG){
        Serial.print("calibrKnife: ");
        Serial.println(calibrKnife);
      }
      break;

    case 0x06:  // нажали на кнопку "вкл/выкл разгруз"
      calibrRazgruz = inputBuf[3];
      if(DEBUG){
        Serial.print("calibrRazgruz: ");
        Serial.println(calibrRazgruz);
      }
      break;

    case 0x07: // нажали на кнопку "вкл/выкл ТЭНы"
      calibrHeaters = inputBuf[3];
      if(DEBUG){
        Serial.print("calibrHeaters");
        Serial.println(calibrHeaters);
      }
      break;

    default:
      showError();
      if (DEBUG) {
        Serial.print("calibrContro() address unknown: ");
        Serial.println(inputBuf[1], HEX);
      }
      break;
  }
}

// ============== адреса ручного включения цилиндров ==================
void cylinders() {  // VP25xx

  solenoidNum = inputBuf[4];
  switch (inputBuf[1]) {
    case 0x00:
      digitalWrite(SOLENOID_SWITCH1, inputBuf[4]);  //голова сливок
      break;
    case 0x01:
      digitalWrite(SOLENOID_SWITCH2, inputBuf[4]);  //голова страчателлы
      break;
    case 0x02:
      digitalWrite(SOLENOID_SWITCH3, inputBuf[4]);  //форсунка продукта
      break;
    case 0x03:
      digitalWrite(SOLENOID_SWITCH4, inputBuf[4]);  //подставка
      break;
    case 0x04:
      digitalWrite(SOLENOID_SWITCH5, inputBuf[4]);  //пресс-форсунка
      break;
    case 0x05:
      digitalWrite(SOLENOID_SWITCH6, inputBuf[4]);  //цилиндр страчателлы
      break;
    case 0x06:
      digitalWrite(SOLENOID_SWITCH7, inputBuf[4]);  //цилиндр сливок
      break;
    case 0x07:
      digitalWrite(SOLENOID_SWITCH8, inputBuf[4]);  //разгрузка
      break;
    case 0x08:
      digitalWrite(SOLENOID_SWITCH9, inputBuf[4]);  //редуктор
      break;
    case 0x09:
      digitalWrite(SOLENOID_SWITCH10, inputBuf[4]); //левый пресс
      break;
    case 0x10:
      digitalWrite(SOLENOID_SWITCH11, inputBuf[4]); //правый пресс
      break;
    case 0x11:
      digitalWrite(SOLENOID_SWITCH12, inputBuf[4]); //нож
      break;

    default:
      showError();
      if (DEBUG) {
        Serial.print("Cylinder address unknown: ");
        Serial.println(inputBuf[1], HEX);
        //showBuffer(inputBuf);
      }
      break;
  }
  changeSSLight = 1;
}

// ============== адреса параметров машины ==================
void changeVar() {  // VP30xx
  switch (inputBuf[1]) {
    case 0x00:  // изменение температуры
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

    case 0x08:  // изменение времени пайки
      userTimeCharArray[0] = inputBuf[3];
      userTimeCharArray[1] = inputBuf[4];
      userTimeCharArray[2] = inputBuf[5];
      userTimeCharArray[3] = inputBuf[6];

      time = atof(userTimeCharArray);
      if (time > 10.0){
        time = 10.0;
      }
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

    case 0x10:  // изменение оборотов
      rotation = inputBuf[4];
      sendRotation();
      break;

    case 0x12:  // изменение общей массы бурраты
      massa = (inputBuf[3] << 8) | inputBuf[4];  // переводим два хекса с экрана в int
      if (massa > maxMassa) {                    // если значение массы на экране больше максимально допустимого значения
        massa = maxMassa;
      }
      digitalWrite(DOSE_ENABLE, LOW);
      changeMass();  // пересчет общей массы бурраты
      sendMassa();   // отправляем значение массы бурраты обратно на экран
      break;

    case 0x14:  // изменение процента сливок
      cream = (inputBuf[3] << 8) | inputBuf[4];
      digitalWrite(DOSE_ENABLE, LOW);
      changeMass();  // пересчет процента сливок
      sendCream();   // отправляем значение процентов сливок обратно на экран
      break;

    case 0x20:  // изменение загрузки страчателлы
      loadStrach = (inputBuf[3] << 8) | inputBuf[4];
      if(DEBUG){
        Serial.print("loadStrach = ");
        Serial.println(loadStrach);
      }
      break;

    case 0x22:  // изменение загрузки сливок
      loadCream = (inputBuf[3] << 8) | inputBuf[4];
      if(DEBUG){
        Serial.print("loadCream = ");
        Serial.println(loadCream);
      }
      break;
    
    case 0x24:  // изменение разгрузки страчателлы
      unloadStrach = (inputBuf[3] << 8) | inputBuf[4];
      if(DEBUG){
        Serial.print("unloadStrach = ");
        Serial.println(unloadStrach);
      }
      break;

    case 0x26:  // изменение разгрузки сливок
      unloadCream = (inputBuf[3] << 8) | inputBuf[4];
      if(DEBUG){
        Serial.print("unloadCream = ");
        Serial.println(unloadCream);
      }
      break;

    case 0x28:  // изменение циклов мойки
      unloadCream = (inputBuf[3] << 8) | inputBuf[4];
      if(DEBUG){
        Serial.print("unloadCream = ");
        Serial.println(unloadCream);
      }
      break;

    default:
      showError();
      if (DEBUG) {
        Serial.print("Change var address unknown: ");
        Serial.println(inputBuf[1], HEX);
        //showBuffer(inputBuf);
      }
      break;
  }
}

/*/ ============== калибровка машины ==================
void calibration() {
  

  byte allGood = 0x01;
  byte allBad = 0x02;

  byte calibrBuff[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X00, 0X0, 0x0 };
  byte ssBuff[] = { 0X5A, 0XA5, 0X5, 0X82, 0X26, 0X00, 0X0, 0x0 };

  ssState1 = digitalRead(SOLENOID_SENSOR1);
  ssStateBuf[5] = 0x00;
  ssStateBuf[7] = ssState1;
  Serial1.write(ssStateBuf, 8);

  ssState2 = digitalRead(SOLENOID_SENSOR2);
  ssStateBuf[5] = 0x01;
  ssStateBuf[7] = ssState2;
  Serial1.write(ssStateBuf, 8);



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
  
  checkHeaters();
  byte changeToMainPage[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x1E };
  Serial1.write(changeToMainPage, 10);
  
  calibrDone = 1;
}*/
// ============== загрузка параметров в экран ================
void setDwin() {
  sendUserTemp();  //  загрузка значения температуры
  sendTime();
  sendRotation();
  sendMassa();
  sendCream();
  sendLoadStrach();
  sendLoadCream();
}

// ============== изменить состояние кнопки на двине ========
void operatingMode(byte i) {

  mainButton = i;
}





//****************** отправка сообщений на экран *******************

// ============== отправка температуры на экран ===================
void sendUserTemp() {
  byte* f_byte = reinterpret_cast<byte*>(&userTemp);
  memcpy(userTempCharArray, f_byte, 4);

  outputUserTempBuf[6] = userTempCharArray[3];
  outputUserTempBuf[7] = userTempCharArray[2];
  outputUserTempBuf[8] = userTempCharArray[1];
  outputUserTempBuf[9] = userTempCharArray[0];
  Serial1.write(outputUserTempBuf, 10);
}

// ============== отправка левой температуры ================
void sendLeftTemp() {
  byte* f_byte = reinterpret_cast<byte*>(&tempToSendL);
  memcpy(leftTempCharArray, f_byte, 4);

  outputLeftTempBuf[6] = leftTempCharArray[3];
  outputLeftTempBuf[7] = leftTempCharArray[2];
  outputLeftTempBuf[8] = leftTempCharArray[1];
  outputLeftTempBuf[9] = leftTempCharArray[0];
  Serial1.write(outputLeftTempBuf, 10);

  if (DEBUG) {
    Serial.print("leftTemp = ");
    Serial.println(tempToSendL);
  }
}

// ============== отправка правой температуры ================
void sendRightTemp() {
  byte* f_byte = reinterpret_cast<byte*>(&tempToSendR);
  memcpy(rightTempCharArray, f_byte, 4);

  outputRightTempBuf[6] = rightTempCharArray[3];
  outputRightTempBuf[7] = rightTempCharArray[2];
  outputRightTempBuf[8] = rightTempCharArray[1];
  outputRightTempBuf[9] = rightTempCharArray[0];
  Serial1.write(outputRightTempBuf, 10);

  if (DEBUG) {
    Serial.print("rightTemp = ");
    Serial.println(tempToSendR);
  }
}

// ============== отправка времени пайки на экран ===================
void sendTime() {
  byte* f_byte = reinterpret_cast<byte*>(&time);
  memcpy(userTimeCharArray, f_byte, 4);

  outputTimeBuf[6] = userTimeCharArray[3];
  outputTimeBuf[7] = userTimeCharArray[2];
  outputTimeBuf[8] = userTimeCharArray[1];
  outputTimeBuf[9] = userTimeCharArray[0];
  Serial1.write(outputTimeBuf, 10);
}

// ============== отправка количества оборотов на экран =================
void sendRotation() {
  outputRevolBuf[6] = highByte(rotation);
  outputRevolBuf[7] = lowByte(rotation);
  Serial1.write(outputRevolBuf, 8);

  if (DEBUG) {
    Serial.print("rotation = ");
    Serial.println(rotation);
  }
}

// ============== отправка массы на экран ===================
void sendMassa() {
  outputMassaBuf[6] = highByte(massa);
  outputMassaBuf[7] = lowByte(massa);
  Serial1.write(outputMassaBuf, 8);

  if (DEBUG) {
    Serial.print("massa = ");
    Serial.println(massa);
  }
}

// ============== отправка массы на экран ===================
void sendCream() {
  outputCreamBuf[6] = highByte(cream);
  outputCreamBuf[7] = lowByte(cream);
  Serial1.write(outputCreamBuf, 8);

  if (DEBUG) {
    Serial.print("cream = ");
    Serial.println(cream);
  }
}

// ============== отправка количества циклов загрузки =======
void sendLoadStrach() {
  outputLoadBuf[6] = highByte(loadStrach);
  outputLoadBuf[7] = lowByte(loadStrach);
  Serial1.write(outputLoadBuf, 8);

  if (DEBUG) {
    Serial.print("loadStrach = ");
    Serial.println(loadStrach);
  }
}
// ============== отправка количества циклов загрузки =======
void sendLoadCream() {
  outputLoadBuf[6] = highByte(loadStrach);
  outputLoadBuf[7] = lowByte(loadStrach);
  Serial1.write(outputLoadBuf, 8);

  if (DEBUG) {
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

// ============== пересчет общей массы бурраты и процента сливок ==================
void changeMass() {
  STmassa = massa * (100 - cream);  // корректируем координату назначения датчика страчателлы (общая масса умноженная на коэффициент объема страчателлы, умноженная на процент страчателлы)
  CRmassa = massa * cream;        // корректируем координату назначения датчика сливок (общая масса умноженная на коэффициент объема сливок, умноженная на процент сливок)

  if (STInMotionFlag) {  // если в процессе движения мотора страчателлы обновилась масса или процент, корректируем текущую координату датчика страчателлы
    if (stepsForST > 0) {
      currentSTStep = currentSTStep + stepsDoneST;
    }
    if (stepsForST < 0) {
      currentSTStep = currentSTStep - stepsDoneST;
    }
  }
  if (CRInMotionFlag) {  // если в процессе движения мотора сливок обновилась масса или процент, корректируем текущую координату датчика сливок
    if (stepsForCR > 0) {
      currentCRStep = currentCRStep + stepsDoneCR;
    }
    if (stepsForCR < 0) {
      currentCRStep = currentCRStep - stepsDoneCR;
    }
  }
  stepsForST = STmassa - currentSTStep;  // корректируем кол-во полушагов до новой координаты датчика страчателлы
  stepsForCR = CRmassa - currentCRStep;  // корректируем кол-во полушагов до новой координаты датчика сливок
  stepsDoneST = 0;                       // обнуляем счетчик полушагов мотора страчателлы
  stepsDoneCR = 0;                       // обнуляем счетчик полушагов мотора сливок
  STInMotionFlag = 1;                    // включаем флаг для передвижения в новую координату датчика страчателлы
  CRInMotionFlag = 1;                    // включаем флаг для передвижения в новую координату датчика сливок
}

/*// TODO ///

2. УДАЛЕНЫ 2003/2004/2005 - инкрементал эджастмент (+/-) с экрана
3. пересмотреть makeResponce()

    case 0x01:

      Serial1.write(changeMainPageBuff, 10);
      break;

    case 0x02:
      Serial1.write(changeMainPageBuff, 10);
      break;

    case 0x03:  //обработка страницы настроек
      //changeSetPageBuff[8] = 0x01;
      changeSetPageBuff[9] = lastSetPageID;
      Serial1.write(changeSetPageBuff, 10);
      break;
*/