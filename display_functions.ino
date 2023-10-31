
/*/ ============== VP адреса переменных в DWIN ================
  ***коды страниц vp:***
  10 01 | 10 = загрузка;
  10 01 | 11 = разгрузка;
  10 01 | 12 = мойка;
  
  10 02 |  


  10 01 | 07 =  остановка цикла

  *** предупреждения ***
  
  11 00 | i

  11 01 | адрес кнопки авто/ручно
  11 02 | вдрес иконки вкл/выкл     ?!?!??!?!?
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

  13 14 | = cycle icon left
  13 15 | = cycle icon right

  14 00 | = иконка датчика крышки
  14 01 | = иконка датчика давления
  14 02 | = иконка левого тэна
  14 03 | = иконка правого тэна

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
  

  !!! ПРОВЕРИТЬ, ВОЗМОЖНО УДАЛИТЬ!
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
  26 00 | A0  ss1   - левый пресс от центра
  26 01 | A1  ss2   - левый пресс в центре
  26 02 | A2  ss3   - пресс-форсунка (низ)
  26 03 | A3  ss4   - правый пресс от центра
  26 04 | A4  ss5   - правый пресс в центре
  26 05 | A5  ss6   - разгрузка низ
  26 06 | A6  ss7   - нож от центра
  26 07 | A7  ss8   - нож в центре
  26 08 | A8  ss9   - подвижный датчик сливок
  26 09 | A9  ss10  - неподвижный (верхний)  датчик сливок
  26 0A | A10 ss11  - датчик редуктора верх
  26 0B | A11 ss12  - датчик редуктора низ
  26 0C | A12 ss13  - неподвижный датчик страчателлы
  26 0D | A13 ss14  - подвижный датчик страчателлы
  

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


  ** текст ошибки ***
  40 00 | = строка юникода

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
      showError("connection", 0);
      if (DEBUG) { Serial.println("readInput() ERROR!!!"); }
      break;
  }
}

// ============== парсинг адреса из сообщения ==================
void parseBuffer() {
  switch (inputBuf[0]) {

    case 0x00:
      pageBeforeEmergencyFlag = 1;

      //ПРОВЕРИТЬ!!! если кнопка СТОП зажата при включении
      if(inputBuf[4] == 0x00){
        inputBuf[4] = 0x0A;
      }
      /*  проверить что будет, если включить машину с включенной стоп кнопкой.
          если когда-нибудь двин вернёт адрес 0х00 (застаку), но поменять его на главную страницу
      if(inputBuf[4] = 0x00){
        pageBeforeEmergencyBuf[9] = 0x10         
        break;
      }
      */
      pageBeforeEmergencyBuf[9] = inputBuf[4];  //обновляем адрес последней страницы до нажатия кнопки СТОП
      
      if(DEBUG){
        Serial.print("Return page: ");
        Serial.println(inputBuf[4]);
      }
      break;

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

    case 0x27:  //функции мотора RE
      motorRE();
      break;

    case 0x30:  //изменить параметр
      changeVar();
      break;

    default:
      showError("connection", 1);

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

    //===== запускаем цикл с страницы 1 =====
    case 0x04:
      doCycle = 1; 
      numCycle = 1;
      if(DEBUG){
        Serial.print("numCycle: ");
        Serial.println(numCycle);
      }
      break;

    case 0x07:
      doCycle = 0;
      numCycle = 0;
      changeSetPageBuff[9] = lastSetPageID;
      Serial1.write(changeSetPageBuff, 10);

      // если цилиндры не в верх, то поднимаем их
      if (!digitalRead(SOLENOID_SENSOR11) && !digitalRead(SOLENOID_SENSOR13)) {      
        digitalWrite(SOLENOID_SWITCH4, HIGH); //форсунка выкл
        cylinderState[4] = 0;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH2, LOW); //голова страч
        cylinderState[2] = 0;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH1, LOW); //голова сливок
        cylinderState[1] = 0;                  //запоминаем состояние цилиндра
        straciatellaButtonCase++;
      }

      if(DEBUG){
        Serial.print("Stop cycle, ");
        Serial.print("numCycle: ");
        Serial.println(numCycle);
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

    // кнопка режима Авто/Ручной
    case 0x01:
      autoModeFlag = inputBuf[4];
      if(DEBUG){
        Serial.print("AutoModeFlag: ");
        Serial.println(autoModeFlag);
      }
      break;

    // нажали на кнопку сбросить ошибку в "Инфо"
    case 0x02:
      Serial.println("InfoError Reset");

      infoButtonBuff[7] = 0x00;                 // меняем кнопку "информация" на нейтральную
      Serial1.write(infoButtonBuff, 8);

      Serial1.write(clearErrorDisplayBuf, 10);  // очищаем экран

      defaultPosition();

      break;

    // кнопка "буррата"
    case 0x10:  
      // если автоматический режим изготовления бурраты
      if(autoModeFlag){

        // если функция изготовления бурраты уже выполняется, всё выключаем
        if(burrataButtonFlag){

          // burrataButtonFlag, предположительно должен быть равен нулю
          /*
          burrataButtonFlag = inputBuf[4];
          burrataButtonBuf[7] = burrataButtonFlag;  
          Serial1.write(burrataButtonBuf, 8); 

          // выключаем автоматический режим
          autoModeButtonBuf[7] = burrataButtonFlag;
          Serial1.write(burrataButtonBuf, 8); 

          // включаем подсветку кнопки
          digitalWrite(ILLUMINATION_OPTION1, HIGH); 

          */
          autoModeFlag = 0;
          
        } else {
          // burrataButtonFlag, предположительно, должен быть равен единице
          burrataButtonFlag = inputBuf[4];
          
          // меняем флаг для включения функции
          // и отправляем его на экран
          burrataButtonBuf[7] = burrataButtonFlag;

          //включаем таймер безопасности  
          safetyTimer = millis();
        }

      } else {

        // проверяем включены ли кнопки "шаг" и "оборот"
        // если включены, то игнорируем нажатие на кнопку "буррата"
        if (!straciatellaButtonFlag && !rotateDiskButtonFlag) {  
         
          // если функция бурраты не выполняется
          if(burrataButtonCase == 0){

            // меняем флаг для включения функции
            // и отправляем его на экран
            burrataButtonFlag = inputBuf[4];   
            burrataButtonBuf[7] = burrataButtonFlag;  
            safetyTimer = millis();
          }

      }
        
        //отжимаем нопку на экране
        /*
        if(!burrataButtonFlag){
          Serial1.write(burrataButtonBuf, 8);
          digitalWrite(ILLUMINATION_OPTION1, LOW);
          burrataButtonCase = 0;
        }
        */
        if (DEBUG) {
          Serial.print("Got burrattaButton ");
          Serial.println(burrataButtonFlag);
        }
      }
      break;

  
    case 0x12:  // кнопка "шаг"
      if (!straciatellaButtonFlag && !burrataButtonFlag) {  //проверяем кнопки "страчателла" и "буррата"

        //отжимаем нопку на экране
        if(rotateDiskButtonCase == 0){
          rotateDiskButtonFlag = inputBuf[4];
          rotateDiskButtonBuf[7] = inputBuf[4];
        }
      }
      break;

    case 0x14:  //кнопка "масса"
      if (!burrataButtonFlag && !rotateDiskButtonFlag) {  //проверяем кнопки "буррата" и "оборот" 
        straciatellaButtonFlag = inputBuf[4];
        strachiatellaButtonBuf[7] = inputBuf[4];

        //отжимаем нопку на экране
        if(!straciatellaButtonFlag){
          Serial1.write(strachiatellaButtonBuf, 8);
          digitalWrite(ILLUMINATION_OPTION3, LOW);
          straciatellaButtonCase = 0;
        }

        if (DEBUG) {
          Serial.print("Got strachatellaButtonFlag: ");
          Serial.println(straciatellaButtonFlag);
        }
      }
      break;
    

    case 0x20:  // виртуальная кнопка начала калибровки 
      makeCalibration = 1;
      break;

    default:
      showError("connection", 2);
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
        Serial.println(inputBuf[4]);
      }
      break;

    case 0x02:  // вкл/выкл тэна
      heaterIsOn = inputBuf[4];
      if (DEBUG) {
        Serial.print("heaterIsOn = ");
        Serial.println(heaterIsOn);
      }
      break;
/*
    case 0x03:  // начать загрузку (цикл 1)
      washCycle1Start = inputBuf[4];
      if(washCycle1Start){
        Serial1.write(loadPageAddrBuf, 10);    //перешли на страницу 
      } else {
        Serial1.write(changeSetPageBuff, 10); //вернулись на страницу "загрузка"
      }
      if (DEBUG) {
        Serial.print("start cycle 1 = ");
        Serial.println(inputBuf[4]);
      }
      break;

    case 0x04:  // начать разгрузку (цикл 2)
      washCycle2Start = inputBuf[4];
      if(washCycle2Start){
        Serial.write(loadPageAddrBuf, 10);    //перешли на страницу 
      } else {
        Serial1.write(changeSetPageBuff, 10); //вернулись на страницу "загрузка"
      }
      if (DEBUG) {
        Serial.print("start cycle 2 = ");
        Serial.println();
      }
      break;
  
    case 0x05:  // начать разгрузку (цикл 3)
      washCycle3Start = inputBuf[4];
      if(washCycle3Start){
        Serial.write(loadPageAddrBuf, 10);    //перешли на страницу 
      } else {
        Serial1.write(changeSetPageBuff, 10); //вернулись на страницу "загрузка"
      }
      if (DEBUG) {
        Serial.print("start cycle 2 = ");
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
      showError("connection", 3);
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
        Serial.print("calibrContor(), turn all: ");
        Serial.println(inputBuf[4]);
      }
      if(inputBuf[4]){
        //вкл объём
        calibrButtonsBuf[5] = 0x02;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);

        //вкл круг
        calibrButtonsBuf[5] = 0x03;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);

        //вкл прессы
        calibrButtonsBuf[5] = 0x04;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);

        //вкл нож
        calibrButtonsBuf[5] = 0x05;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);

        //вкл разгруз
        calibrButtonsBuf[5] = 0x06;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);        

        //вкл ТЭНы
        calibrButtonsBuf[5] = 0x07;
        calibrButtonsBuf[7] = 0x01;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);

      } else {
        //выкл объём
        calibrButtonsBuf[5] = 0x02;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);

        //вкл круг
        calibrButtonsBuf[5] = 0x03;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);

        //вкл прессы
        calibrButtonsBuf[5] = 0x04;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);

        //вкл нож
        calibrButtonsBuf[5] = 0x05;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);

        //вкл разгруз
        calibrButtonsBuf[5] = 0x06;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);

        //вкл ТЭНы
        calibrButtonsBuf[5] = 0x07;
        calibrButtonsBuf[7] = 0x00;
        Serial1.write(calibrButtonsBuf, 8);
        while(Serial1.read() != 0x4B);
      }
      break;
    
    case 0x02: // нажали на кнопку "вкл/выкл объём"
      if(digitalRead(SOLENOID_SENSOR11) == 0 && digitalRead(SOLENOID_SENSOR13) == 0){
        calibrateVolumeFlag = inputBuf[4];
      }

      if(calibrateVolumeFlag){
        digitalWrite(DOSE_ENABLE, LOW);
      } else {
        digitalWrite(DOSE_ENABLE, HIGH);
      }
      if(DEBUG){
        Serial.print("calibrVolume: ");
        Serial.println(calibrateVolumeFlag);
      }
      break;
    
    case 0x03: // нажали на кнопку "вкл/выкл круг"
      calibrDisk = inputBuf[4];
      if(DEBUG){
        Serial.print("calibrDisk: ");
        Serial.println(calibrDisk);
      }
      break;

    case 0x04: // нажали на кнопку "вкл/выкл узлы"
      calibrPosition = inputBuf[4];
      if(DEBUG){
        Serial.print("calibrPosition: ");
        Serial.println(calibrPosition);
      }
      break;

    case 0x05: // нажали на кнопку "вкл/выкл ТЭНы"
      calibrHeaters = inputBuf[4];
      if(DEBUG){
        Serial.print("calibrHeaters");
        Serial.println(calibrHeaters);
      }
      break;

    case 0x07: // нажата кнопка "начать калибовку"

      break;

    default:
      if (DEBUG) {
        Serial.print("calibrContro() address unknown: ");
        Serial.println(inputBuf[1], HEX);
      }
      break;
  }
}

// ============== адреса ручного включения цилиндров ==================
void cylinders() {  // VP25xx

  switch (inputBuf[1]) {
    case 0x01:  //голова сливок
      digitalWrite(SOLENOID_SWITCH1, inputBuf[4]);
      cylinderState[1] = inputBuf[4];                  //запоминаем состояние цилиндра
      
      //отправка состояния кнопки на экран
      sendCylinderIconState(inputBuf[1], inputBuf[4]);
      break;

    case 0x02:  //голова страчателлы
      digitalWrite(SOLENOID_SWITCH2, inputBuf[4]);
      cylinderState[2] = inputBuf[4];                  //запоминаем состояние цилиндра
      
      //отправка состояния кнопки на экран
      sendCylinderIconState(inputBuf[1], inputBuf[4]);
      break;

    case 0x03:  //подставка
      if(cylinderState[11] == 1){
        digitalWrite(SOLENOID_SWITCH3, inputBuf[4]);
        cylinderState[3] = inputBuf[4];                  //запоминаем состояние цилиндра
      
        //отправка состояния кнопки на экран
        sendCylinderIconState(inputBuf[1], inputBuf[4]);
      }
      break;

    case 0x04:  //форсунка пресс-форсунки
      digitalWrite(SOLENOID_SWITCH4, inputBuf[4]);
      cylinderState[4] = inputBuf[4];                  //запоминаем состояние цилиндра
      
      //отправка состояния кнопки на экран
      sendCylinderIconState(inputBuf[1], inputBuf[4]);
      break;

    case 0x05:  //пресс-форсунка
      digitalWrite(SOLENOID_SWITCH5, inputBuf[4]);
      cylinderState[5] = inputBuf[4];                  //запоминаем состояние цилиндра
      
      //отправка состояния кнопки на экран
      sendCylinderIconState(inputBuf[1], inputBuf[4]);
      break;

    case 0x06:  //цилиндр страчателлы
      if(cylinderState[2] == 0 || cylinderState[4] == 0){ //голова страчателлы в бункер или открытую форсунку
        digitalWrite(SOLENOID_SWITCH6, inputBuf[4]);
        cylinderState[6] = inputBuf[4];                  //запоминаем состояние цилиндра
      
        //отправка состояния кнопки на экран
        sendCylinderIconState(inputBuf[1], inputBuf[4]);
      }
      break;

    case 0x07:  //цилиндр сливок
      if(cylinderState[1] == 0 || cylinderState[4] == 0){      //цилиндр срабатвает либо в бункер, либо в открытую форсунку
        digitalWrite(SOLENOID_SWITCH7, inputBuf[4]);
        cylinderState[7] = inputBuf[4];                  //запоминаем состояние цилиндра
      
        //отправка состояния кнопки на экран
        sendCylinderIconState(inputBuf[1], inputBuf[4]);
      }
      break;

    case 0x08: //правый пресс
      if(cylinderState[10] == 0){   //проверяем нижний датчик редуктора
        digitalWrite(SOLENOID_SWITCH8, inputBuf[4]);
        cylinderState[8] = inputBuf[4];                  //запоминаем состояние цилиндра
        
        //отправка состояния кнопки на экран
        sendCylinderIconState(inputBuf[1], inputBuf[4]);
      }
      break;

    case 0x09: //левый пресс
      if(cylinderState[10] == 0){   //проверяем нижний датчик редуктора  
        digitalWrite(SOLENOID_SWITCH9, inputBuf[4]);
        cylinderState[9] = inputBuf[4];                  //запоминаем состояние цилиндра
      
        //отправка состояния кнопки на экран
        sendCylinderIconState(inputBuf[1], inputBuf[4]);
      }
      break;

    case 0x10:  //редуктор
      //!!!!!! ДОПИСАТЬ КРУЧЕНИЕ ПРИ ОПУСКАНИИ
      //проверяем датчики левого/правого прессов и ножа от центра
      if(digitalRead(SOLENOID_SENSOR1) == 0 && digitalRead(SOLENOID_SENSOR4) == 0 && digitalRead(SOLENOID_SENSOR7) == 0){
        //digitalRead(SOLENOID_SENSOR9) && digitalRead(SOLENOID_SENSOR10) && digitalRead(SOLENOID_SENSOR12)){
        digitalWrite(SOLENOID_SWITCH10, inputBuf[4]);
        cylinderState[10] = inputBuf[4];                  //запоминаем состояние цилиндра
    
        //отправка состояния кнопки на экран
        sendCylinderIconState(inputBuf[1], inputBuf[4]);
      }
      break;

    case 0x11:  //разгрузка
      if(cylinderState[3] == 0){    // подставка вверху 
        digitalWrite(SOLENOID_SWITCH11, inputBuf[4]);
        cylinderState[11] = inputBuf[4];                  //запоминаем состояние цилиндра разгрузки
     
        //отправка состояния кнопки на экран
        sendCylinderIconState(inputBuf[1], inputBuf[4]);
      }
      break;

    case 0x12: //нож
      if(cylinderState[10] == 0){   //проверяем нижний датчик редуктора
        digitalWrite(SOLENOID_SWITCH12, inputBuf[4]);
        cylinderState[12] = inputBuf[4];                  //запоминаем состояние цилиндра
      
        //отправка состояния кнопки на экран
        sendCylinderIconState(inputBuf[1], inputBuf[4]);
      }
      break;

    case 0x13:
      checkSolenoidSensorsFlag = inputBuf[4];
      break;

    default:
      showError("connection", 5);
      if (DEBUG) {
        Serial.print("Cylinder address unknown: ");
        Serial.println(inputBuf[1], HEX);
        //showBuffer(inputBuf);
      }
      break;
  }
}

// ============== ручное включение мотора RE из настроек ================
void motorRE(){   //27xx
  switch (inputBuf[1]) {
    case 0x00:  //вкл/выкл
      if(cylinderState[5] == 0 && motorREDir == 1){}
      motorREON = inputBuf[4]; 

      if(DEBUG){
        Serial.print("motorRE: ");
        Serial.println(inputBuf[4]);
      }
      break;

    case 0x02:
    motorRESpeed = (inputBuf[3] << 8) | inputBuf[4];
      if(DEBUG){
        Serial.print("speed: ");
        Serial.println(motorRESpeed);
      }
      break;

    case 0x03:
      motorREDir = inputBuf[4];
      if(DEBUG){
        Serial.print("direction: ");
        Serial.println(inputBuf[4]);
      }
      break;

    default:
      showError("connection", 6);
      if (DEBUG) {
        Serial.print("motorRE() address unknown: ");
        Serial.println(inputBuf[1], HEX);
        //showBuffer(inputBuf);
      }
      break;
  }
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
      
      if(userTemp > overHeat){
        userTemp = overHeat;
      }
      
      sendUserTemp();

      //обновили значение в энергонезависимой памяти
      EEPROM.put(EEPROMuserTemp, userTemp);

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

      //обновили значение в энергонезависимой памяти
      EEPROM.put(EEPROMtime, time);

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

      //обновили значение в энергонезависимой памяти
      EEPROM.put(EEPROMRotation, rotation);
      
      break;

    case 0x12:  // изменение общей массы бурраты
      massa = (inputBuf[3] << 8) | inputBuf[4];  // переводим два хекса с экрана в int
      if (massa > maxMassa) {                    // если значение массы на экране больше максимально допустимого значения
        massa = maxMassa;
      }
      digitalWrite(DOSE_ENABLE, LOW);
      changeMass();  // пересчет общей массы бурраты
      sendMassa();   // отправляем значение массы бурраты обратно на экран
      
      //обновили значение в энергонезависимой памяти
      EEPROM.put(EEPROMMassa, massa);
      break;

    case 0x14:  // изменение процента сливок
      cream = (inputBuf[3] << 8) | inputBuf[4];
      if (cream > maxCream){                    // если значение сливок на экране больше максимально допустимого значения
        cream = maxCream;
      }
      digitalWrite(DOSE_ENABLE, LOW);
      changeMass();  // пересчет процента сливок
      sendCream();   // отправляем значение процентов сливок обратно на экран
      
      //обновили значение в энергонезависимой памяти
      EEPROM.put(EEPROMCream, cream);
      
      break;

    case 0x20:  // изменение загрузки страчателлы
      washCycleSt = (inputBuf[3] << 8) | inputBuf[4];
      if(DEBUG){
        Serial.print("washCycleSt1 = ");
        Serial.println(washCycleSt1);
      }
      break;

    case 0x22:  // изменение загрузки сливок
      washCycleCr = (inputBuf[3] << 8) | inputBuf[4];
      if(DEBUG){
        Serial.print("washCycleCr1 = ");
        Serial.println(washCycleCr1);
      }
      break;
    

    default:
      showError("connection", 7);
      if (DEBUG) {
        Serial.print("Change var address unknown: ");
        Serial.println(inputBuf[1], HEX);
        //showBuffer(inputBuf);
      }
      break;
  }
}

// ============== изменить состояние кнопки на двине ========
void operatingMode(byte i) {

  mainButton = i;
}

// ============== загрузка параметров в экран ================
void setDwin() {
    sendUserTemp();                 //  загрузка значения температуры
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера

    sendTime();                     //  отправка времени пайки
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера

    sendRotation();                 //  отправка количества оборотов
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
    
    sendMassa();                    //  отправка объёма
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
    
    sendCream();                    //  отправка %сливок
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
    
    //  отправка значений переменных циклов
    sendWashCycleSt1();             //  отправка значения цикла 1 мойки цилиндра страчателлы
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
    sendWashCycleCr1();             //  отправка значения цикла 1 мойки цилиндра сливок
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
    sendWashCycleSt2();             //  отправка значения цикла 2 мойки цилиндра страчателлы
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
    sendWashCycleCr2();             //  отправка значения цикла 2 мойки цилиндра сливок
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
    sendWashCycleSt3();             //  отправка значения цикла 3 мойки цилиндра страчателлы
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
    sendWashCycleCr3();             //  отправка значения цикла 3 мойки цилиндра сливок
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера

    // отправка значений на экран настоек
    sendMotorRESpeed();             //  отправка значения количества оборотов
    while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера

    // отправить состояния датчиов
    checkHeaters();                 //  отправка состояния датчиков тока
    //while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
    //checkCoverSensor();             //  отправка состояния датчика крышки
    //while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
    //checkPressureSensore();
    //while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера

    Serial.println("All Sensors checked.");
    //!!!отобразить в настройках включенные цилинды!!!

    //отобразить на экране включенные цилиндры 
    if(DEBUG){
      Serial.println("Turned cylinders: ");
    }
    for(byte i = 1; i < 13; i++){
      if(cylinderState[i] > 0){
        cylinderIconBuf[5] = i;
        cylinderIconBuf[7] = cylinderState[i];
        Serial1.write(cylinderIconBuf, 8);
        while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
        
        if(DEBUG){
          Serial.print(cylinderIconBuf[5]);
          Serial.print(" ");
          Serial.println(cylinderIconBuf[7]);
        }
      }
    }

    setSolenoidSensorState();
}



//****************** отправка сообщений на экран *******************

// ============== отображает предупреждение на экране =======
void showError(char f, int caseNum) {

  if(f == 'a'){    // ошибки при включении
    switch(caseNum){
      case 0:       //нет давления
        if(DEBUG){
          Serial.print("ShowError A case ");
          Serial.println(caseNum);
        }
        sendError(0x30, 0x30);    //е00
        break;

      case 1:       //нет редуктор не доехал до верха
        if(DEBUG){
          Serial.print("ShowError A case");
          Serial.println(caseNum);
        }
        sendError(0x30, 0x31);    //е01
        break;

      case 2:       //не работает левый датчик тока
        if(DEBUG){
          Serial.print("ShowError A case");
          Serial.println(caseNum);
        }
        sendError(0x30, 0x32);    //е02
        break;

      case 3:       //не работает правый датчик тока
        if(DEBUG){
          Serial.print("ShowError A case");
          Serial.println(caseNum);
        }
        sendError(0x30, 0x33);    //е03
        break;

      case 4:       //не работают оба датчика тока
        if(DEBUG){
          Serial.print("ShowError A case");
          Serial.println(caseNum);
        }
        sendError(0x30, 0x34);    //е04
        break;

      default:
        Serial.println("showError() A case");
        break;
    }
  }
  
  if(f == 'b'){    // ошибки функции бурраты
    switch (caseNum){
      case 0: //подготовка к работе 
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x30, 0x35);    //е05
        break;

      case 1: //идём 75 шагов с ускорением
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x30, 0x36);    //е06
        break;

      case 2: //опускаем пресс-форсунку и начали накачку цилиндров
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x30, 0x37);   //е07
        break;

      case 3: //проверяем датчик пресс форсунки
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x30, 0x38);    //е08
        break;

      case 4: //дозируем
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x30, 0x39);    //е09
        break;

      case 5: //ждём, когда вернуться дозаторы
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x31, 0x30);    //е10
        break;

      case 6: //крутим чашку
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x31, 0x31);    //е11
        break;

      case 7: //сводим левый пресс
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x31, 0x32);    //е12
        break;

      case 8: //сводим правый пресс
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x31, 0x33);    //е13
        break;

      case 9: //ждём датчик ножа в центре
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x31, 0x34);    //е14
        break;

      case 10:  //ждём нож пол секунды
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x31, 0x35);    //е15
        break;

      case 11:  //ждём нож у стенки
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x31, 0x36);    //е16
        break;

      case 12:  //ждём спаивания и убираем прессы
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x31, 0x37);    //е16
        break;

      case 13:  //ждём датчики ножа и прессы чтобы разъехались
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x31, 0x33);    //е18
        break;

      case 14:  //поднимаем редуктор 
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x31, 0x39);    //е19
        break;

      case 15:  //опускаем разгрузку
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x32, 0x30);    //е20
        break;

      case 16:  //проверяем датчик разгрузки внизу
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x32, 0x31);    //е21
        break;

      case 17:  //кейс подготовки к выходу
        if(DEBUG){
          Serial.print("ShowEroor BR case");
          Serial.println(caseNum);
        }
        sendError(0x32, 0x32);    //е22
        break;

      defult:

        if(DEBUG){
          Serial.print("showError func ERROR in br");
          Serial.println(caseNum);
        }
        break;
    }
  }

  if(f == 'r'){    // ошибки функции оборот
    switch (caseNum){
      case 0:   //подготовка к работе
        if(DEBUG){
          Serial.print("ShowError Rotate case");
          Serial.println(caseNum);
        }
        sendError(0x32, 0x33);    //е23
        break;

      case 1:   //идт по шагам
        if(DEBUG){
          Serial.print("ShowEroor Rotate case");
          Serial.println(caseNum);
        }
        sendError(0x32, 0x34);    //е24
        break;

      case 2:   //идти с ускорением 
        if(DEBUG){
          Serial.print("ShowEroor Rotate case");
          Serial.println(caseNum);
        }
        sendError(0x32, 0x35);    //е25
        break;

      case 3:   //идём на максималках
        if(DEBUG){
          Serial.print("ShowEroor Rotate case");
          Serial.println(caseNum);
        }
        sendError(0x32, 0x36);    //е26
        break;

      case 4:   //замедляемся
        if(DEBUG){
          Serial.print("ShowEroor Rotate case");
          Serial.println(caseNum);
        }
        sendError(0x32, 0x37);    //е27
        break;

      case 5:   //подготовка к завершению функции
        if(DEBUG){
          Serial.print("ShowEroor Rotate case");
          Serial.println(caseNum);
        }
        sendError(0x32, 0x38);    //е28
        break;

      default:
        break;
    }
  }

  if(f == 's'){    // ошибки функции страчателла
    switch (caseNum){
      case 0:   //подготовка функции к работе
        break;
      case 1:   //опускаем цилиндры
        break;
      case 2:   //поднимаем цилиндры
        break;
      case 3:   //ждём срабатывания верхних датчиков дозаторов
        break;
      case 4:
        break;
      default:
        break;
    }
  }
  
  if(f == 'h'){ // ошибки функции проверки датчика тока
    switch (caseNum){
      case 1: //не работает левый
        
        break;

      case 2: //не работает правый
        
        break;

      case 3: //both not working

        break;

      case 4: //default case
       
       break;


    }
  }

  if(f == 'c'){    // error with buffer parsing
    switch (caseNum){
      case 0:     //readInput error

        break;

      case 1:     //parseBuffer error

        break;

      case 2:      //pressButton error

        break;

      case 3:       //makeresponse error

        break;
    
      case 4:       //calibrControl error

        break;

      case 5:       //cylinders() error

        break;

      case 6:       //motorRE error

        break;

      case 7:        //changeVar error

        break;

      default:

        break;
    }
  }

  if(f == 'connection'){
    switch(caseNum){
      case 0:
        break;

      default:
        Serial.println("showError() connectionError");
        break;
    }
  }
  //showErrorBuf[7] = 1;
  //Serial1.write(showErrorBuf, 8);
}

void sendError(byte xE, byte yE){
  infoButtonBuff[7] = 0x01;                 // меняем кнопку "информация" на аварийную
  Serial1.write(infoButtonBuff, 8);

  Serial1.write(clearErrorDisplayBuf, 10);  // очищаем экран

  showErrorMessageBuf[8] = xE;
  showErrorMessageBuf[9] = yE;
  Serial1.write(showErrorMessageBuf, 12);   // отправляем ошибку на экран
  /*
  if(DEBUG){
    Serial.print("sendErroFunc x: ");
    Serial.print(xE);
    Serial.print(", y: ");
    Serial.println(yE);
  }
  */
}


// ============== отправка температуры на экран ===================
void sendUserTemp() {
  byte* f_byte = reinterpret_cast<byte*>(&userTemp);
  memcpy(userTempCharArray, f_byte, 4);

  outputUserTempBuf[6] = userTempCharArray[3];
  outputUserTempBuf[7] = userTempCharArray[2];
  outputUserTempBuf[8] = userTempCharArray[1];
  outputUserTempBuf[9] = userTempCharArray[0];
  Serial1.write(outputUserTempBuf, 10);

  if(DEBUG){
    Serial.println(__func__);
  }
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

  if(DEBUG){
    Serial.println(__func__);
  }
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

// ============== отправка скорости мотора RE на экран ===================
void sendMotorRESpeed(){
  outputMotorRESpeed[6] = highByte(motorRESpeed);
  outputMotorRESpeed[7] = lowByte(motorRESpeed);
  Serial1.write(outputMotorRESpeed, 8);

  if (DEBUG) {
    Serial.print("motorRESpeed = ");
    Serial.println(motorRESpeed);
  }
}

// ============== отправка количества циклов страчателлы на экране 1 =======
void sendWashCycleSt1() {
  washCycleSt1Buf[6] = highByte(washCycleSt1);
  washCycleSt1Buf[7] = lowByte(washCycleSt1);
  Serial1.write(washCycleSt1Buf, 8);

  if (DEBUG) {
    Serial.print("washCycleSt1 = ");
    Serial.println(washCycleSt1);
  }
}

// ============== отправка количества циклов сливок на экране 1 ============
void sendWashCycleCr1() {
  washCycleCr1Buf[6] = highByte(washCycleCr1);
  washCycleCr1Buf[7] = lowByte(washCycleCr1);
  Serial1.write(washCycleCr1Buf, 8);

  if (DEBUG) {
    Serial.print("washCycleCr1 = ");
    Serial.println(washCycleCr1);
  }
}

// ============== отправка количества циклов страчателлы на экране 2 =======
void sendWashCycleSt2() {
  washCycleSt2Buf[6] = highByte(washCycleSt2);
  washCycleSt2Buf[7] = lowByte(washCycleSt2);
  Serial1.write(washCycleSt2Buf, 8);

  if (DEBUG) {
    Serial.print("washCycleSt2 = ");
    Serial.println(washCycleSt2);
  }
}

// ============== отправка количества циклов сливок на экране 2 ============
void sendWashCycleCr2() {
  washCycleCr2Buf[6] = highByte(washCycleCr2);
  washCycleCr2Buf[7] = lowByte(washCycleCr2);
  Serial1.write(washCycleCr2Buf, 8);

  if (DEBUG) {
    Serial.print("washCycleCr2 = ");
    Serial.println(washCycleCr2);
  }
}

// ============== отправка количества циклов страчателлы на экране 3 =======
void sendWashCycleSt3() {
  washCycleSt3Buf[6] = highByte(washCycleSt3);
  washCycleSt3Buf[7] = lowByte(washCycleSt3);
  Serial1.write(washCycleSt3Buf, 8);

  if (DEBUG) {
    Serial.print("washCycleSt3 = ");
    Serial.println(washCycleSt3);
  }
}

// ============== отправка количества циклов сливок на экране 3 ============
void sendWashCycleCr3() {
  washCycleCr3Buf[6] = highByte(washCycleCr3);
  washCycleCr3Buf[7] = lowByte(washCycleCr3);
  Serial1.write(washCycleCr3Buf, 8);

  if (DEBUG) {
    Serial.print("washCycleCr3 = ");
    Serial.println(washCycleCr3);
  }
}

// ============== отправка количества сделанных циклов на экран ============
void sendTodayCycle(int cycle){
  todayCyclesBuff[6] = highByte(cycle);
  todayCyclesBuff[7] = lowByte(cycle);
  Serial1.write(todayCyclesBuff, 8);

  if (DEBUG) {

    Serial.print("todayCycle: ");
    Serial.println(cycle);
  }

}

// ============== отправка времени цикла на экран =================
void sendCycleTimer(float timer){
  
  byte* f_byte = reinterpret_cast<byte*>(&timer);
  memcpy(timerCharArray, f_byte, 4);

  timerBuf[6] = timerCharArray[3];
  timerBuf[7] = timerCharArray[2];
  timerBuf[8] = timerCharArray[1];
  timerBuf[9] = timerCharArray[0];
  Serial1.write(timerBuf, 10);

  if(DEBUG){
    Serial.print("Timer: ");
    Serial.println(timer);
  }
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
  //оборот = 400 полушагов, плодадь цилиндра страчателлы = 2732,585. шаг резьбы 2мм, 
  //5000(полушагов = 50 на экране)/400 (полушагов)= 12,5 оборотов * 2 = 25мм.
  //2732.585 * 25 / 1000 = 68.314 мл (в 5000 полушагах)
  //50 / 68.314 = 0.732 - коэффециент 
  STmassa = int(massa * (100 - cream) * 0.732);  // корректируем координату назначения датчика страчателлы (общая масса умноженная на коэффициент объема страчателлы, умноженная на процент страчателлы)
  
  //площадь цилиндра сливок = 660.185 мм2
  //16.5 мл в 5000
  //коффециент сливок 3
  CRmassa = massa * cream * 3 ;        // корректируем координату назначения датчика сливок (общая масса умноженная на коэффициент объема сливок, умноженная на процент сливок)

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

// ============== отправялем состояние кнопки на экран ==========
void sendCylinderIconState(byte addr, byte state){
  cylinderIconBuf[5] = addr;
  cylinderIconBuf[7] = state;
  Serial1.write(cylinderIconBuf, 8);
}


//проверка датчиков цилиндров
void setSolenoidSensorState(){
  ssState1  = !digitalRead(SOLENOID_SENSOR1);
  ssState2  = !digitalRead(SOLENOID_SENSOR2);
  ssState3  = !digitalRead(SOLENOID_SENSOR3);
  ssState4  = !digitalRead(SOLENOID_SENSOR4);
  ssState5  = !digitalRead(SOLENOID_SENSOR5);
  ssState6  = !digitalRead(SOLENOID_SENSOR6);
  ssState7  = !digitalRead(SOLENOID_SENSOR7);
  ssState8  = !digitalRead(SOLENOID_SENSOR8);
  ssState9  = !digitalRead(SOLENOID_SENSOR9);
  ssState10 = !digitalRead(SOLENOID_SENSOR10);  
  ssState11 = !digitalRead(SOLENOID_SENSOR11);
  ssState12 = !digitalRead(SOLENOID_SENSOR12);
  ssState13 = !digitalRead(SOLENOID_SENSOR13);
  ssState14 = !digitalRead(SOLENOID_SENSOR14); 

  ssStateBuf[5] = 0x00;
  ssStateBuf[7] = ssState1;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x01;
  ssStateBuf[7] = ssState2;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x02;
  ssStateBuf[7] = ssState3;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x03;
  ssStateBuf[7] = ssState4;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x04;
  ssStateBuf[7] = ssState5;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x05;
  ssStateBuf[7] = ssState6;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x06;
  ssStateBuf[7] = ssState7;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x07;
  ssStateBuf[7] = ssState8;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x08;
  ssStateBuf[7] = ssState9;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x09;
  ssStateBuf[7] = ssState10;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x0A;
  ssStateBuf[7] = ssState11;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x0B;
  ssStateBuf[7] = ssState12;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x0C;
  ssStateBuf[7] = ssState13;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);

  ssStateBuf[5] = 0x0D;
  ssStateBuf[7] = ssState14;
  Serial1.write(ssStateBuf, 8);
  while(Serial1.read() != 0x4B);
}
//отображение состояния датчика цилиндра на экране
void checkSolenoidSensors() {
  
  //датчик левого пресса от центру
  if (ssState1 != digitalRead(SOLENOID_SENSOR1)) {
    //ssState1 = digitalRead(SOLENOID_SENSOR1);
    ssStateBuf[5] = 0x00;
    ssStateBuf[7] = ssState1;
    Serial1.write(ssStateBuf, 8);
    ssState1 = !ssState1;
  }
  //датчик левого пресса в центра
  if (ssState2 != digitalRead(SOLENOID_SENSOR2)) {
    //ssState2 = digitalRead(SOLENOID_SENSOR2);
    ssStateBuf[5] = 0x01;
    ssStateBuf[7] = ssState2;
    Serial1.write(ssStateBuf, 8);
    ssState2 = !ssState2;
  }
  //датчик пресс-форсунка низ
  if (ssState3 != digitalRead(SOLENOID_SENSOR3)) {
    //ssState3 = digitalRead(SOLENOID_SENSOR3);
    ssStateBuf[5] = 0x02;
    ssStateBuf[7] = ssState3;
    Serial1.write(ssStateBuf, 8);
    ssState3 = !ssState3;
  }
  //датчик правого пресса от центра
  if (ssState4 != digitalRead(SOLENOID_SENSOR4)) {
    //ssState4 = digitalRead(SOLENOID_SENSOR4);
    ssStateBuf[5] = 0x03;
    ssStateBuf[7] = ssState4;
    Serial1.write(ssStateBuf, 8);
    ssState4 = !ssState4;
  }
  //датчик правого пресса в центре
   if (ssState5 != digitalRead(SOLENOID_SENSOR5)) {
    //ssState5 = digitalRead(SOLENOID_SENSOR5);
    ssStateBuf[5] = 0x04;
    ssStateBuf[7] = ssState5;
    Serial1.write(ssStateBuf, 8);
    ssState5 = !ssState5;
  }
  //датчик рзгрузки низ
   if (ssState6 != digitalRead(SOLENOID_SENSOR6)) {
    //ssState6 = digitalRead(SOLENOID_SENSOR6);
    ssStateBuf[5] = 0x05;
    ssStateBuf[7] = ssState6;
    Serial1.write(ssStateBuf, 8);
    ssState6 = !ssState6;
  }
  //датчик ножа от центра
   if (ssState7 != digitalRead(SOLENOID_SENSOR7)) {
    //ssState7 = digitalRead(SOLENOID_SENSOR7);
    ssStateBuf[5] = 0x06;
    ssStateBuf[7] = ssState7;
    Serial1.write(ssStateBuf, 8);
    ssState7 = !ssState7;
  }
  //датчик ножа центр
   if (ssState8 != digitalRead(SOLENOID_SENSOR8)) {
    //ssState8 = digitalRead(SOLENOID_SENSOR8);
    ssStateBuf[5] = 0x07;
    ssStateBuf[7] = ssState8;
    Serial1.write(ssStateBuf, 8);
    ssState8 = !ssState8;
  }
  //датчик сливок подвижный
   if (ssState9 != digitalRead(SOLENOID_SENSOR9)) {
    //ssState9 = digitalRead(SOLENOID_SENSOR9);
    ssStateBuf[5] = 0x08;
    ssStateBuf[7] = ssState9;
    Serial1.write(ssStateBuf, 8);
    ssState9 = !ssState9;
  }
  //датчик редуктора верх
   if (ssState10 != digitalRead(SOLENOID_SENSOR10)) {
    //ssState10 = digitalRead(SOLENOID_SENSOR10);
    ssStateBuf[5] = 0x09;
    ssStateBuf[7] = ssState10;
    Serial1.write(ssStateBuf, 8);
    ssState10 = !ssState10;
  }
  //датчик сливок неподвижный верхний
   if (ssState11 != digitalRead(SOLENOID_SENSOR11)) {
    //ssState11 = digitalRead(SOLENOID_SENSOR11);
    ssStateBuf[5] = 0x0A;
    ssStateBuf[7] = ssState11;
    Serial1.write(ssStateBuf, 8);
    ssState11 = !ssState11;
  }
  //датчик редуктора низ
   if (ssState12 != digitalRead(SOLENOID_SENSOR12)) {
    //ssState12 = digitalRead(SOLENOID_SENSOR12);
    ssStateBuf[5] = 0x0B;
    ssStateBuf[7] = ssState12;
    Serial1.write(ssStateBuf, 8);
    ssState12 = !ssState12;
  }
  //датчик страчателлы неподвижный
   if (ssState13 != digitalRead(SOLENOID_SENSOR13)) {
    //ssState13 = digitalRead(SOLENOID_SENSOR13);
    ssStateBuf[5] = 0x0C;
    ssStateBuf[7] = ssState13;
    Serial1.write(ssStateBuf, 8);
    ssState13 = !ssState13;
  }
  ///датчик страчателлы подвижный
   if (ssState14 != digitalRead(SOLENOID_SENSOR14)) {
    //ssState14 = digitalRead(SOLENOID_SENSOR14);
    ssStateBuf[5] = 0x0D;
    ssStateBuf[7] = ssState14;
    Serial1.write(ssStateBuf, 8);
    ssState14 = !ssState14;
  }
}

/*// TODO ///

1. сверить все адреса с адресами экрана
2. пересмотреть makeResponce()
3. дописать выходы из циклов при нажатии на 1110/1112/1114
4. дописать проверку по времени срабатывания while в setDwin() - на случай, если повиснет сообщение/оборвётся провод
5. исправить название в calibrContor
6. удалить из калибровки прессы/нож/разгрузку - сделать одну - стартовое положение цилиндров
7. написать кнопку "включение/выключение пароля"
8. поменять цвет кнопок 
*/