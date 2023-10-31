//********** Основные функции **********

//действия по прерыванию нуля
void isr() {
  static int lastDimL;
  static int lastDimR;
  
  digitalWrite(DIMMER_L, LOW);
  digitalWrite(DIMMER_R, LOW);
  
  // установили таймер левого тэна
  if (lastDimL != dimmerL) {
    Timer2.setPeriod(lastDimL = dimmerL);
  } else {
    Timer2.restart();
  }

  // установили таймер правого тэна
  if (lastDimR != dimmerR) {
    Timer3.setPeriod(lastDimR = dimmerR);
  } else {
    Timer3.restart();
  }

}

// таймер для пид
ISR(TIMER2_A) {
  if (heaterIsOn && tempLeft < overHeat ) {
    digitalWrite(DIMMER_L, HIGH);
    //digitalWrite(DIMMER_R, HIGH);
    
    Timer2.stop();
  }
}

// таймер для пид
ISR(TIMER3_A) {
  if (heaterIsOn &&  tempRight < overHeat) {
    //digitalWrite(DIMMER_L, HIGH);
    digitalWrite(DIMMER_R, HIGH);
    Timer3.stop();
  }
}

// функция ПИД
void pid() {
  //посчитать скорость выполнения расчёта
  
  oldErrorL = errorL;
  errorL = userTemp - tempLeft;
  IntL = IntL + (oldErrorL + errorL) / 2;
  integralL = constrain(IntL * (1 / ti), 0, 9300);
  if (integralL == 9300) {
    IntL = integralL;
  }
  diffL = (errorL - oldErrorL) * td;
  outL = constrain(p * errorL + integralL + diffL, 500, 9300);

  oldErrorR = errorR;
  errorR = userTemp - tempRight;
  IntR = IntR + (oldErrorR + errorR) / 2;
  integralR = constrain(IntR * (1 / ti), 0, 9300);
  if (integralR == 9300) {
    IntR = integralR;
  }
  diffR = (errorR - oldErrorR) * td;
  outR = constrain(p * errorR + integralR + diffR, 500, 9300);

}

// запрос температуры
void getTemperature() {   //microDS18b20
  tempLeft = sensorLeft.getTempCByIndex(0);
  tempRight = sensorRight.getTempCByIndex(0); 

  sensorLeft.requestTemperatures();
  sensorRight.requestTemperatures();
}

void pidVaulesMonitor() {
  //m
  Serial.print(">tempL:");
  Serial.println(tempLeft);
  Serial.print(">tempR:");
  Serial.println(tempRight);
  Serial.print(">userTemp:");
  Serial.println(userTemp);
  //g
//  Serial.print(">EL:");
//  Serial.println(errorL);
//  Serial.print(">P*err:");
//  Serial.println(p * errorL);
//  Serial.print(">I:");
//  Serial.println(integralL);
//  Serial.print(">D:");
//  Serial.println(diffL);
//  Serial.print(">Out:");
//  Serial.println(outL);
//  Serial.print(">DimmerL:");
//  Serial.println(dimmerL);

  //g
 // Serial.print(">ER:");
 // Serial.println(errorR);
 // Serial.print(">P*errR:");
 // Serial.println(p * errorR);
 // Serial.print(">i:");
 // Serial.println(integralR);
 // Serial.print(">d:");
 // Serial.println(diffR);
 // Serial.print(">out:");
 // Serial.println(outR);
 // Serial.print(">DimmerR:");
 // Serial.println(dimmerR);
}

  // !!!! дописать вызов ошибки, если датчик не работает


byte pedalStatus() {
  if (digitalRead(PEDAL) == LOW || digitalPedal == LOW) {
    return 0;
  }
  return 1;
}


// проверка датчиков тока
// попробовать переписать свитч на иф кейс, чтобы проверить не проблема ли в брейке
void checkHeaters() { 
  byte heaterState = 0;

  digitalWrite(DIMMER_L, HIGH);
  delay(10);
  leftHeaterCurrent = ACS712_L.mA_AC(50, 1);
  digitalWrite(DIMMER_L, LOW);

  digitalWrite(DIMMER_R, HIGH);
  delay(10);
  rightHeaterCurrent = ACS712_R.mA_AC(50, 1);
  digitalWrite(DIMMER_R, LOW);

  //Serial.print("Left heater: ");
  //Serial.println(leftHeaterCurrent);
  //Serial.print("Right heater: ");
  //Serial.println(rightHeaterCurrent);

  //проверяем тэны
  if (leftHeaterCurrent < 200.0) {
    heaterState += 1;
  }
  if (rightHeaterCurrent < 200.0) {
    heaterState += 2;
  }

  if(DEBUG){
    Serial.print("Heaters state: ");
    Serial.println(heaterState);
  }

  //обработка проверки работы тэнов

  switch (heaterState) {
    case 0:  //ТЭНы ок
      stateLH = 0;
      stateRH = 0;
      break;

    case 1:  //не работает левый
      stateLH = 1;
      stateLHBuff[7] = stateLH;
      Serial1.write(stateLHBuff, 8);
      //while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера

      if(DEBUG){
        Serial.println("left heater not working");
      }
      showError("a", 2);
      break;

    case 2:  //не работает правый
      stateRH = 1;
      stateRHBuff[7] = stateRH;
      Serial1.write(stateRHBuff, 8);
      //while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера
      if(DEBUG){
        Serial.println("right not cheackHeaters");
      }
      showError("a", 3);
      break;

    case 3:  //не работают оба
      stateLH = 1;
      stateLHBuff[7] = stateLH;
      Serial1.write(stateLHBuff, 8);
      //while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера

      
      stateRH = 1;
      stateRHBuff[7] = stateRH;
      Serial1.write(stateRHBuff, 8);
      //while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера

      if(DEBUG){
        Serial.println("both not working");  
      }
      showError("a", 4);
      //while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера

      break;

    default:
      Serial.println("checkHeaters() error!");
      break;
  }

  heaterState = 0;
}

// проверка датчика CS !!! отключили, тк нет крышки
void checkCoverSensor() {
  /*
  //stateCS = digitalRead(COVER_SENSOR);
  stateCS = 1;                              //заглушка для датчика крышки
  stateCSBuff[7] = stateCS;
  Serial1.write(stateCSBuff, 8);

    if(stateCS){ //защитная крышка открыта
      coverSensorError = 1;
    } else {
      coverSensorError = 0;
    }

    if(DEBUG){
      Serial.print("stateCS: ");
      Serial.println(stateCS);
    }
*/
}


// проверка датчика PS
void checkPressureSensore(){
  statePS = digitalRead(PRESSURE_METER);
  statePSBuff[7] = statePS;
  Serial1.write(statePSBuff, 8);
  //while(Serial1.read() != 0x4B);  //  проверка сообщения от двина и обнуление буффера

  if(statePS){
    showError('a', 0);
  }
}



// ********** ОСНОВНЫЕ АЛГОРИТМЫ РАБОТЫ **********

// выставление машины в дефолт
void defaultPosition(){
  if(DEBUG){
    Serial.println("defaultPosition()");
  }

  //ПЕРЕМЕСТИТЬ ВЫСТАВЛЕНИЯ В КАЛИБРОВКУ!
  //TODO дописать по каждому цилиндру
  if(!digitalRead(SOLENOID_SENSOR13)){      //если датчик дозатора страчателлы не наверху
    digitalWrite(SOLENOID_SWITCH6, HIGH);  //страч
    cylinderState[6] = 1;                  //запоминаем состояние цилиндра
  }
  if(!digitalRead(SOLENOID_SENSOR11)){      //если датчик дозатора сливок не наверху
    digitalWrite(SOLENOID_SWITCH7, HIGH);  //сливки
    cylinderState[7] = 1;                  //запоминаем состояние цилиндра
  }
  
  digitalWrite(SOLENOID_SWITCH4, HIGH);  //подставка
  cylinderState[4] = 1;                  //запоминаем состояние цилиндра
  
  if(digitalRead(SOLENOID_SENSOR7)){    //если датчик ножа не с краю
    digitalWrite(SOLENOID_SWITCH12, LOW);   //убираем нож
    cylinderState[12] = 0;                  //запоминаем состояние цилиндра
  } 

  //если датчики прессов от цетра не сработаны
  if(digitalRead(SOLENOID_SENSOR1) && digitalRead(SOLENOID_SENSOR4)){
    digitalWrite(SOLENOID_SWITCH9, LOW);   //пресс левый отводим
    cylinderState[9] = 0;                  //запоминаем состояние цилиндра
    digitalWrite(SOLENOID_SWITCH8, LOW);   //пресс правый отводим
    cylinderState[8] = 0;                  //запоминаем состояние цилиндра   
  }

  digitalWrite(SOLENOID_SWITCH2, LOW);    //голова страч
  cylinderState[2] = 0;                   //запоминаем состояние цилиндра
  digitalWrite(SOLENOID_SWITCH1, LOW);    //голова сливок
  cylinderState[1] = 0;                   //запоминаем состояние цилиндра

  digitalWrite(SOLENOID_SWITCH5, LOW);    //подниамем пресс-форсункe
  cylinderState[5] = 0;                   //запоминаем состояние цилиндра
  digitalWrite(SOLENOID_SWITCH11, LOW);  //поднимаем разгрузку
  cylinderState[11] = 0;                  //поднимаем разгрузку
  digitalWrite(SOLENOID_SWITCH3, LOW);    //опускаем подставку 
  cylinderState[3] = 0;                   //запоминаем состояние цилиндра

  //проверяем крышку, датчик давления, ТЭНы
  //checkSensors();

  // проверяем давление и поднимаем редуктор
  if(!digitalRead(PRESSURE_METER)){
    //поднимаем редуктор вверх с кручением
    stepTimerRE = millis();
    safetyTimer = millis();
    while (digitalRead(SOLENOID_SENSOR10) == 1) {  //редуктор поднят - идём дальше
      if (micros() - stepTimerRE >= 500) {
        stepTimerRE = micros();
        makeRE_CCWR();
      }
      digitalWrite(SOLENOID_SWITCH10, HIGH);  //поднимаем редуктор
      cylinderState[10] = 1; 
      if(millis() - safetyTimer > 5000){
        Serial.println("defaultPosition base reductor");
        Serial.print("safetyTimer ");
        Serial.println(safetyTimer);
        showError('a', 1);
        //ДОПИСАТЬ ПРЕДУПРЕЖЕНИЕ
        break;
      }
    }
  } else {
    showError('a', 0);
  }

  //включаем подсветку кнопок
  digitalWrite(ILLUMINATION_OPTION1, HIGH);
  digitalWrite(ILLUMINATION_OPTION2, HIGH);
  digitalWrite(ILLUMINATION_OPTION3, HIGH);


}

// нажата кнопка буррата
void burrataButton() {
  switch (burrataButtonCase) {
    case 0:
      //burrataButtonFlag = 1;
      burrataButtonBuf[7] = 1;
      Serial1.write(burrataButtonBuf, 8);   //изменение состояния кнопки "буррата" на вкл

      digitalWrite(ILLUMINATION_OPTION1, LOW);  //включаем подсветку кнопки
      OptionalButton1State = 0;
      OptionalButton1Timer = millis();

      burrataButtonIncrementalBuf[7] = 1;
      Serial1.write(burrataButtonIncrementalBuf, 8);   //изменение состояния кнопки "буррата" на вкл

      burrataButtonCase++;
      stepTimerRE = millis();
      checkStepFlagRE = 0;

      //меняем кейс функции rotateDiskButton() для перехода к кручению
      rotateDiskButtonCase = 1;
      //начинаем засекать цикл изготовления бурраты
      safetyTimer = millis();
    break;

    case 1:   //идём 75 шагов против часовой с ускорением
      digitalWrite(SOLENOID_SWITCH11, HIGH);  //подняли разгрузку
      
      //сделали оборот
      rotateDiskButton();
      if(rotateDiskButtonCase == 0){
        burrataButtonCase++;
        safetyTimer = millis();
      }
      
      //burrataButtonCase++;
      break;

    case 2:   //опускаем пресс-форсунку и начали накачку цилиндров
        cycleTimer = millis();
        digitalWrite(SOLENOID_SWITCH3, HIGH);   //поднимаем подставку
        cylinderState[3] = 1;                   //запоминаем состояние цилиндра подставки  
        digitalWrite(SOLENOID_SWITCH5, HIGH);  //опускем пресс-форсункe
        cylinderState[5] = 1;                   //запоминаем состояние цилиндра пресс форсунки
        burrataButtonCase++;
        safetyTimer = millis();
      break;

    case 3:   //проверяем датчик пресс-форсунки низ
        
        if(digitalRead(SOLENOID_SENSOR3) == 0){      //проверяем датчик пресс-форсунки низ
          digitalWrite(SOLENOID_SWITCH6, LOW); //страч
          cylinderState[6] = 0;                  //запоминаем состояние цилиндра
          digitalWrite(SOLENOID_SWITCH7, LOW); //сливки
          cylinderState[7] = 0;                  //запоминаем состояние цилиндра
        
          burrataButtonCase++;
          safetyTimer = millis();
        }
      break;


    case 4:   //дозируем
      if (digitalRead(SOLENOID_SENSOR14) == LOW && sensorFlag[1] == 0) { //страчателла
        digitalWrite(SOLENOID_SWITCH2, HIGH); //голова страч
        cylinderState[2] = 1;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH4, LOW); //форсунка вкл
        cylinderState[4] = 0;                  //запоминаем состояние цилиндра
         digitalWrite(SOLENOID_SWITCH6, HIGH); //страч
        cylinderState[6] = 1;                  //запоминаем состояние цилиндра
        
        sensorFlag[1] = 1;
      }
      if (digitalRead(SOLENOID_SENSOR9) == LOW && sensorFlag[2] == 0) {
        digitalWrite(SOLENOID_SWITCH1, HIGH); //голова сливок
        cylinderState[1] = 1;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH4, LOW); //форсунка вкл
        cylinderState[4] = 0;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH7, HIGH); //сливки
        cylinderState[7] = 1;                  //запоминаем состояние цилиндра
       
        sensorFlag[2] = 1;
      }
      if (sensorFlag[1] == 1 && sensorFlag[2] == 1) {
        sensorFlag[1] = 0;
        sensorFlag[2] = 0;
        safetyTimer = millis();
        burrataButtonCase++;
      }
      break;

    case 5:   //ждём когда вернуться цилиндры страчателлы и сливок обратно
      if (!digitalRead(SOLENOID_SENSOR11) && !digitalRead(SOLENOID_SENSOR13)) {     //ждём срабатывания верхних датчиков страчателлы и сливок 
        digitalWrite(SOLENOID_SWITCH4, HIGH); //форсунка выкл
        cylinderState[4] = 1;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH2, LOW); //голова страч
        cylinderState[2] = 0;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH1, LOW); //голова сливок
        cylinderState[1] = 0;                  //запоминаем состояние цилиндра
        safetyTimer = millis();
        burrataButtonCase++;
      }
      break;

    case 6:  //крутим чашку 
      if(digitalRead(SOLENOID_SENSOR12) == 0 && checkStepFlagRE > (rotation * 800 * 6)){ //полушаги + передатка
          cylinderState[10] = 0;                  //запоминаем состояние редуктора
          safetyTimer = millis();
          burrataButtonCase++;
          checkStepFlagRE = 0;

        } else {
          if (micros() - stepTimerRE >= RECCWSpeed) {
            stepTimerRE = micros();
            makeRE_CCWR();
            checkStepFlagRE++;
          }
          digitalWrite(SOLENOID_SWITCH10, LOW);  //опускаем редуктор
          cylinderState[10] = 0;                  //запоминаем состояние цилиндра
        }
      break;

    case 7:  //сводим левый/правый пресс
      digitalWrite(SOLENOID_SWITCH9, HIGH);   //левый пресс!!!
      cylinderState[9] = 1;                  //запоминаем состояние цилиндра
      digitalWrite(SOLENOID_SWITCH8, HIGH); //правый пресс!!!
      cylinderState[8] = 1;                  //запоминаем состояние цилиндра
      //delay(1000);
      safetyTimer = millis();
      burrataButtonCase++;
      break;

    case 8:  // отслеживаем срабатывание датчиков прессов в центре и стреляем ножом
      if (!digitalRead(SOLENOID_SENSOR5) && !digitalRead(SOLENOID_SENSOR2)) {
        knifeTimer2 = millis();
        pressTimer = millis();
        safetyTimer = millis();
        burrataButtonCase++;
      }
      break;

    case 9:    //ждём срабатывания датчика ножа в центре
      if(millis() - knifeTimer2 > 1500){
        digitalWrite(SOLENOID_SWITCH12, HIGH);   //стреляем ножом
        cylinderState[12] = 1;                  //запоминаем состояние цилиндра
        
        if(digitalRead(SOLENOID_SENSOR8) == 0){   //сработан датчик ножа в центре 
          knifeTimer1 = millis();
          safetyTimer = millis();                 //таймер безопасности
          burrataButtonCase++;
        }
      }
      
      break;

    case 10: // ждём нож пол секунды 
      if(millis() - knifeTimer1 >= 1000){
        digitalWrite(SOLENOID_SWITCH12, LOW);   //убираем нож 
        cylinderState[12] = 0;                  //запоминаем состояние цилиндра
        safetyTimer = millis();                 //таймер безопасности
        burrataButtonCase++;
      }
      break;

    case 11:  //ждём датчик ножа от центра
      if(digitalRead(SOLENOID_SENSOR7) == 0){
        safetyTimer = millis();                 //таймер безопасности
        burrataButtonCase++;
      }
      break;

    case 12:  // ждём спаивания и убираем прессы
      if(millis() - pressTimer >= int(time*1000)){
        digitalWrite(SOLENOID_SWITCH9, LOW);   //пресс левый отводим
        cylinderState[9] = 0;                   //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH8, LOW);   //пресс правый отводим
        cylinderState[8] = 0;                  //запоминаем состояние цилиндра
        safetyTimer = millis();
        burrataButtonCase++;
      }
      break;


    case 13:  // ждём срабатываение датчиков ножа и прессов от центра
      if(!digitalRead(SOLENOID_SENSOR1) && !digitalRead(SOLENOID_SENSOR4)){
        safetyTimer = millis();
        burrataButtonCase++;
      }
      break;

    case 14:  //поднимаем редуктор с кручением для выгрузки и опускаем подтсавку, поднимаем пресс-форсунку
      if (digitalRead(SOLENOID_SENSOR10) == 0) {  //редуктор поднят - идём дальше
        cylinderState[3] = 0;                   //запоминаем состояние цилиндра подставки  
        cylinderState[5] = 0;                   //запоминаем состояние цилиндра пресс форсунки
        cylinderState[10] = 1; 
        checkStepFlagRE = 0;
        cycleTimer = (millis() - cycleTimer);
      
        safetyTimer = millis();
        burrataButtonCase++;
      } else {
        if (micros() - stepTimerRE >= 500) {
          stepTimerRE = micros();
          makeRE_CCWR();
          checkStepFlagRE++;
        }
        digitalWrite(SOLENOID_SWITCH10, HIGH); //поднимаем редуктор
        cylinderState[10] = 1;                 //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH5, LOW);   //подниамем пресс-форсункe
        cylinderState[5] = 0;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH3, LOW);   //опускаем подставку 
        cylinderState[3] = 0;                  //запоминаем состояние цилиндра
      }
      break;

    case 15:  //опускаем разгрузку
      digitalWrite(SOLENOID_SWITCH11, LOW);
      cylinderState[11] = 0; 
      safetyTimer = millis();
      burrataButtonCase++;
      break;  

    case 16:  //проверяем датчик разгрузки в нижнем положении
      if(digitalRead(SOLENOID_SENSOR6) == 0){
        safetyTimer = millis();
        burrataButtonCase++;

        if(autoModeFlag){
          autoModeDelayTimer = millis();
        }
      }
      break;

    case 17:

      // если включен автоматический режим
      // то начинаем цикл с начала 
      if(autoModeFlag){
        if(millis() - autoModeDelayTimer > autoModeDelay){
          //изменение состояния кнопки "шаг" на выкл
          burrataButtonFlag = 1;
          burrataButtonCase = 0;
        }  
      } else {

        // отправляем значение кнопки на экран 
        burrataButtonBuf[7] = 0x00;                     
        Serial1.write(burrataButtonBuf, 8); 

        // выключаем автоматический режим
        autoModeButtonBuf[7] = 0x00;
        Serial1.write(autoModeButtonBuf, 8); 

        //выключаем кнопку
        digitalWrite(ILLUMINATION_OPTION1, HIGH);
        burrataButtonCase++;
      }
      break;

    case 18:

       //изменение состояния кнопки "шаг" на выкл
      burrataButtonFlag = 0;
      
      burrataButtonCase = 0;

      //расчёт времени цикла и отправка его на экран
      sec = cycleTimer / 1000;
      mSec = (cycleTimer % 1000)*0.001;
      cycleTime = sec + mSec;      
      sendCycleTimer(cycleTime);

      if(DEBUG){
        Serial.print("cycle time in burrata: ");
        Serial.print(cycleTime);
      }

      //отправка количества циклов
      totalCycle++;

      todayCycles++;
      sendTodayCycle(todayCycles);

      break;

    default:
      if (DEBUG) {
        Serial.print("burrataButton() error! case:");
        Serial.println(burrataButtonCase);
      }
      burrataButtonFlag = 0;
      burrataButtonCase = 0;
      break;
  }
}

// нажата кнопка оборот
void rotateDiskButton() {

  switch (rotateDiskButtonCase) {

    case 0:                                     // подготовка 
      rotateDiskButtonBuf[7] = 1;
      Serial1.write(rotateDiskButtonBuf, 8);    //включаем кнопку "шаг" на экране
      digitalWrite(ILLUMINATION_OPTION2, LOW);  //включаем подсветку кнопки
      
      rotateDiskButtonCase++;

      stepTimerRE = micros();     // засекаем время для шага
      cycleTimer = millis();      // засекаем время оборота
      safetyTimer = millis();     // засекаем время для принудительного выхода из функции
      break;

    case 1:
      if (checkStepFlagRE < 100) {              //идём 100 шагов
        if (micros() - stepTimerRE >= RECWSpeed) {   //со скоростью 1500
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;                    //посчитали шаг
          RECWSpeed -= 6;
          if(!digitalRead(TURN_SENSOR)){  //если датчик сработал
            wrongDiskPosition = 1;        //то диск расположен не правильно
            if(DEBUG){
              Serial.println("wrongDiskPosition = 1");
            }
          }
        }
      } else {

        checkStepFlagRE = 0;        // обнуляем шаги
        rotateDiskButtonCase++;
        safetyTimer = millis();     // обновляем таймер безопасности
        if(DEBUG){
          Serial.print("rotation case: ");
          Serial.print(rotateDiskButtonCase);
          Serial.print(" RESpeed: ");
          Serial.println(RECWSpeed);
        }
      }
    break;

    case 2:   //делаем 100 полушагов против часовой с ускорением
      if (checkStepFlagRE < 400) {  
        if (micros() - stepTimerRE >= RECWSpeed) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;
          //RECWSpeed -= 4;       //ускоряем каждый полушаг
          /* 
          if(DEBUG){
            Serial.println(checkStepFlagRE);
            Serial.println(RECWSpeed);
          }
          */
        }
      } else {
        checkStepFlagRE = 0;
        
        if(DEBUG){
          Serial.print("rotation case: ");
          Serial.print(rotateDiskButtonCase);
          Serial.print(" RESpeed: ");
          Serial.println(RECWSpeed);
        }
        rotateDiskButtonCase++;
        safetyTimer = millis();
      }
      break;

   /*
   
   */ case 3:   //делаем 300 полушагов на максимальной скорости
      if (checkStepFlagRE < 0) {
        if (micros() - stepTimerRE >= RECWSpeed) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;
        }
      } else {
        checkStepFlagRE = 0;

        if(DEBUG){
          Serial.print("rotation case: ");
          Serial.print(rotateDiskButtonCase);
          Serial.print(" RESpeed: ");
          Serial.println(RECWSpeed);
        }
        rotateDiskButtonCase++;
        safetyTimer = millis();
      }

      break;
/*
    case 4:   //замедляем
      if (checkStepFlagRE < 100) {
        if (micros() - stepTimerRE >= RECWSpeed) {
          stepTimerRE = micros();
          makeRE_CWR();
          RECWSpeed += 34;
          checkStepFlagRE++;
        }
      } else {
        if(rotateDiskButtonFlag){
          cycleTimer = (millis() - cycleTimer);
        }
        checkStepFlagRE = 0;

        if(DEBUG){
          Serial.print("rotation case: ");
          Serial.println(rotateDiskButtonCase);
        }
        rotateDiskButtonCase++;
        safetyTimer = millis();
      }

    break;
*/
    case 4:   // замедляем до срабатывания датчика
      if (digitalRead(TURN_SENSOR)) {
        if (micros() - stepTimerRE >= RECWSpeed) {
          RECWSpeed += 8;
          stepTimerRE = micros();
          makeRE_CWR();
        }
      } else {
        if(DEBUG){
          Serial.print("rotation case: ");
          Serial.print(rotateDiskButtonCase);
          Serial.print(" RESpeed: ");
          Serial.println(RECWSpeed);
        }
          cycleTimer = (millis() - cycleTimer);
          safetyTimer = millis();
          rotateDiskButtonCase++;
      }
      break;
    
    case 5:
      if (checkStepFlagRE < 62) {
        if (micros() - stepTimerRE >= RECWSpeed) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;
          RECWSpeed += 4;       //ускоряем каждый полушаг
        }
      } else {
        if(DEBUG){
          Serial.print("rotation case: ");
          Serial.print(rotateDiskButtonCase);
          Serial.print(" RESpeed: ");
          Serial.println(RECWSpeed);
        }

        checkStepFlagRE = 0;
        rotateDiskButtonCase++;
        cycleTimer = (millis() - cycleTimer);
        safetyTimer = millis();
      }
      break;
/*
    case 6:
      if (digitalRead(TURN_SENSOR)) {
        if (micros() - stepTimerRE >= RECWSpeed) {
          RECWSpeed += 10;
          stepTimerRE = micros();
          makeRE_CWR();
        }
      } else {
        if(DEBUG){
          Serial.print("rotation case: ");
          Serial.println(rotateDiskButtonCase);
        }
          cycleTimer = (millis() - cycleTimer);
          safetyTimer = millis();
          rotateDiskButtonCase++;
      }
      break;

*/

    case 6:
      if(DEBUG){
          Serial.print("rotation case: ");
          Serial.print(rotateDiskButtonCase);
          Serial.print(" RESpeed: ");
          Serial.println(RECWSpeed);
        }

      //рассчёт времени цикла "при нажатой копки шаг"
      if(rotateDiskButtonFlag){
        sec = cycleTimer / 1000;
        mSec = (cycleTimer % 1000)*0.001;
        cycleTime = sec + mSec;
        Serial.print(cycleTime);
        sendCycleTimer(cycleTime);
      }

    

      RECWSpeed = 1500;
      checkStepFlagRE = 0;        
      rotateDiskButtonCase = 0;
      rotateDiskButtonFlag = 0;

      rotateDiskButtonBuf[7] = 0;               //изменение состояния кнопки "шаг" на выкл
      Serial1.write(rotateDiskButtonBuf, 8);    //отправка состояния кнопки  на экран
      digitalWrite(ILLUMINATION_OPTION2, HIGH);  //включаем подсветку кнопки

      //обнуляем состояние по выполнению всей функции
      break;

    default:
      rotateDiskButtonFlag = 0;
      rotateDiskButtonCase = 0;
      if (DEBUG) {
        Serial.println("rotateDiskButton() default error!");
        //Serial.println(__func__);
      }
      break;
  }
}

// нажата кнопка включения масса
void straciatellaButton() {
  switch(straciatellaButtonCase){
    case 0:  // отправляем информацию на экран
      //straciatellaButtonFlag = 1;

      //включаем кнопку "масса" на экране
      strachiatellaButtonBuf[7] = 1;
      Serial1.write(strachiatellaButtonBuf, 8); 

      //включаем подсветку кнопки
      digitalWrite(ILLUMINATION_OPTION3, LOW);  
      
      //включаем кнопку "масса" на экране
      strachiatellaButtonIncrementalBuf[7] = 1;
      Serial1.write(strachiatellaButtonIncrementalBuf, 8); 

      straciatellaButtonCase++;  // переходим в следующий кейс
      digitalWrite(DOSE_ENABLE, LOW);
      break;

    case 1:   // опускаем цилиндры
      digitalWrite(SOLENOID_SWITCH6, LOW); //страч
      cylinderState[6] = 1;                  //запоминаем состояние цилиндра
      digitalWrite(SOLENOID_SWITCH7, LOW); //сливки
      cylinderState[7] = 1;                  //запоминаем состояние цилиндра
      straciatellaButtonCase++;
      break;

    case 2:   // ждём срабатывание подвижных датчикв и поднимаем цилиндры
      if (digitalRead(SOLENOID_SENSOR14) == LOW && sensorFlag[1] == 0) { //страчателла
        digitalWrite(SOLENOID_SWITCH2, HIGH); //голова страч
        cylinderState[2] = 1;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH4, LOW); //форсунка вкл
        cylinderState[4] = 1;                  //запоминаем состояние цилиндра
         digitalWrite(SOLENOID_SWITCH6, HIGH); //страч
        cylinderState[6] = 1;                  //запоминаем состояние цилиндра
        
        sensorFlag[1] = 1;
      }
      if (digitalRead(SOLENOID_SENSOR9) == LOW && sensorFlag[2] == 0) {
        digitalWrite(SOLENOID_SWITCH1, HIGH); //голова сливок
        cylinderState[1] = 1;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH4, LOW); //форсунка вкл
        cylinderState[4] = 1;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH7, HIGH); //сливки
        cylinderState[7] = 1;                  //запоминаем состояние цилиндра
       
        sensorFlag[2] = 1;
      }
      if (sensorFlag[1] == 1 && sensorFlag[2] == 1) {
        sensorFlag[1] = 0;
        sensorFlag[2] = 0;
        straciatellaButtonCase++;
      }
      break;

    case 3:   //ждём срабатывания верхних датчиков страчателлы и сливок
      if (!digitalRead(SOLENOID_SENSOR11) && !digitalRead(SOLENOID_SENSOR13)) {      
        digitalWrite(SOLENOID_SWITCH4, HIGH); //форсунка выкл
        cylinderState[4] = 0;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH2, LOW); //голова страч
        cylinderState[2] = 0;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH1, LOW); //голова сливок
        cylinderState[1] = 0;                  //запоминаем состояние цилиндра
        straciatellaButtonCase++;
      }
      break;

//!!! ПРОВЕРИТЬ ЧТО ЗНАЧИТ ЭТОТ ИФ (ДУБЛИРУЕТ ПРЕДЫДУЩИЙ КЕЙС)
    case 4:
      if (!digitalRead(SOLENOID_SENSOR11) && !digitalRead(SOLENOID_SENSOR13)) {
        straciatellaButtonCase = 0;
        straciatellaButtonFlag = 0;
        strachiatellaButtonBuf[7] = 0;
        Serial1.write(strachiatellaButtonBuf, 8);
        digitalWrite(ILLUMINATION_OPTION3, HIGH);
      }
      break;

    default:
        if (DEBUG) {
          straciatellaButtonFlag = 0;
          straciatellaButtonCase = 0;
          Serial.print("straciatellaButton() error!  case:");
          Serial.println(straciatellaButtonCase);
        }
      break;

  }
}


// ============== запуск цикла 1 ============
void cycle(){
  // запуск прокачки цилиндров страчателлы и сливок 
  switch(cycleCase){
    case 0:
      /*
      Serial1.write(cycleIconLeftBuff, 8);
      Serial1.write(cycleIconRightBuff, 8);
      Serial.println("cycleCase0");
      
      */
      cycleCase++;
      break;

    case 1:
      // опускаем цилиндры
      digitalWrite(SOLENOID_SWITCH6, LOW); //страч
      cylinderState[6] = 1;                  //запоминаем состояние цилиндра
      digitalWrite(SOLENOID_SWITCH7, LOW); //сливки
      cylinderState[7] = 1;                  //запоминаем состояние цилиндра
      
      cycleIconTimer = millis();
      cycleCase++;
    /*
      if(millis() - cycleIconTimer > 1000){
        cycleIconLeftBuff[7] = 1;
        cycleIconRightBuff[7] = 1;
        Serial1.write(cycleIconLeftBuff, 8);
        Serial1.write(cycleIconRightBuff, 8);
        cycle1Case++;
        cycleIconTimer = millis();
            
        Serial.println("cycle1Case1");
      }

    */
      break;

    case 2:
      // ждём 5 секунд, чтобы цилиндры опустились
      if(millis() - cycleIconTimer >= 5000){

        // считаем количество циклов опускания цилиндра страчателлы
        if(countWashCycleSt < washCycleSt && cycleFlag[0] == 0){
          digitalWrite(SOLENOID_SWITCH2, HIGH); //голова страч
          cylinderState[2] = 1;                  //запоминаем состояние цилиндра
          digitalWrite(SOLENOID_SWITCH4, LOW); //форсунка вкл
          cylinderState[4] = 1;                  //запоминаем состояние цилиндра
          digitalWrite(SOLENOID_SWITCH6, HIGH); //страч
          cylinderState[6] = 1;                  //запоминаем состояние цилиндра

          cycleFlag[0] = 1;
        }

        // считаем количество циклов опускания цилиндра сливок
        if(countWashCycleCR < washCycleCr && cycleFlag[1] == 0){
          digitalWrite(SOLENOID_SWITCH1, HIGH); //голова сливок
          cylinderState[1] = 1;                  //запоминаем состояние цилиндра
          digitalWrite(SOLENOID_SWITCH4, LOW); //форсунка вкл
          cylinderState[4] = 1;                  //запоминаем состояние цилиндра
          digitalWrite(SOLENOID_SWITCH7, HIGH); //сливки
          cylinderState[7] = 1;                  //запоминаем состояние цилиндра

          cycleFlag[1] == 1;
        }  

        //
        if(cycleFlag[0] == 1 && cycleFlag[1] == 1){
          if(countWashCycleSt == washCycleSt && countWashCycleCR == washCycleCr){
            cycleCase++;
          } else {
            cycleFlag[0] = 0;
            cycleFlag[1] = 0;
            cycleCase = 1;
          }
        }
      }
      /*
      if(millis() - cycleIconTimer > 1000){
        cycleIconLeftBuff[7] = 2;
        cycleIconRightBuff[7] = 2;
        Serial1.write(cycleIconLeftBuff, 8);
        Serial1.write(cycleIconRightBuff, 8);
        cycle1Case++;
        cycleIconTimer = millis();
        
        Serial.println("cycle1Case2");
      }
      */
      break;

    case 3:
      cycleCase = 0;
      doCycle = 0;
      /*
      if(millis() - cycleIconTimer > 1000){
        cycleIconLeftBuff[7] = 3;
        cycleIconRightBuff[7] = 3;
        Serial1.write(cycleIconLeftBuff, 8);
        Serial1.write(cycleIconRightBuff, 8);
        cycle1Case++;
        Serial.println("cycle1Case2");

        cycleIconTimer = millis();
      }
      */
      break;

    default:
      cycleCase = 0;
      doCycle = 0;
      if(DEBUG){
        Serial.println("cycle1() ERROR!");
      }
      break;
  }
}



//******* функции управделния шаговиками **********

// сделать шаг шаговиком
void makeStep() {
  if (stepFlag == 0) {
    digitalWrite(REDUCER_STEP, HIGH);
    digitalWrite(REDUCER_DIR, LOW);
    stepFlag = 1;
  } else {
    digitalWrite(REDUCER_STEP, LOW);
    digitalWrite(REDUCER_DIR, LOW);
    stepFlag = 0;
  }
}

// сделать оборот диска по часовой
void makeRE_CWR() {
  if (stepFlagRE == 0) {
    digitalWrite(REDUCER_STEP, LOW);
    digitalWrite(REDUCER_DIR, LOW);
    stepFlagRE = 1;
  } else {
    digitalWrite(REDUCER_STEP, HIGH);
    digitalWrite(REDUCER_DIR, LOW);
    stepFlagRE = 0;
  }
}

// сделать оборот диска против часовой
void makeRE_CCWR() {
  if (stepFlagRE == 0) {
    digitalWrite(REDUCER_STEP, LOW);
    digitalWrite(REDUCER_DIR, HIGH);
    stepFlagRE = 1;
  } else {
    digitalWrite(REDUCER_STEP, HIGH);
    digitalWrite(REDUCER_DIR, HIGH);
    stepFlagRE = 0;
  }
}

// сделать оборот страчателлы по часовой
void makeST_CWR() {
  if (stepFlagST == 0) {
    digitalWrite(STRACCIATELLA_STEP, LOW);
    digitalWrite(STRACCIATELLA_DIR, LOW);
    stepFlagST = 1;
  } else {
    digitalWrite(STRACCIATELLA_STEP, HIGH);
    digitalWrite(STRACCIATELLA_DIR, LOW);
    stepFlagST = 0;
  }
}

// сделать оборот страчателлы против часовой
void makeST_CCWR() {
  if (stepFlagST == 0) {
    digitalWrite(STRACCIATELLA_STEP, LOW);
    digitalWrite(STRACCIATELLA_DIR, HIGH);
    stepFlagST = 1;
  } else {
    digitalWrite(STRACCIATELLA_STEP, HIGH);
    digitalWrite(STRACCIATELLA_DIR, HIGH);
    stepFlagST = 0;
  }
}

// сделать оборот сливок по часовой
void makeCR_CWR() {
  //Serial.println("Make Step");
  if (stepFlagCR == 0) {
    digitalWrite(CREAM_STEP, LOW);
    digitalWrite(CREAM_DIR, LOW);
    stepFlagCR = 1;
  } else {
    digitalWrite(CREAM_STEP, HIGH);
    digitalWrite(CREAM_DIR, LOW);
    stepFlagCR = 0;
  }
}

// сделать оборот сливок против часовой
void makeCR_CCWR() {
  if (stepFlagCR == 0) {
    digitalWrite(CREAM_STEP, LOW);
    digitalWrite(CREAM_DIR, HIGH);
    stepFlagCR = 1;
  } else {
    digitalWrite(CREAM_STEP, HIGH);
    digitalWrite(CREAM_DIR, HIGH);
    stepFlagCR = 0;
  }
}


//********** Функции расчёта объема масс **********
// перемещение датчика страчателлы в координату
void calcMassaST() {
  if (stepsDoneST < abs(stepsForST)) {
    if (micros() - stepTimerST >= 500) {
      stepTimerST = micros();
      if (stepsForST > 0) {
        makeST_CCWR();
      }
      if (stepsForST < 0) {
        makeST_CWR();
      }
      stepsDoneST++;
    }
  } else {
    if (DEBUG) {
      Serial.print("calcMassaST steps: ");
      Serial.println(stepsDoneST);
    }
    STInMotionFlag = 0;
    currentSTStep = STmassa;
    stepsDoneST = 0;
    if (STInMotionFlag == 0 && CRInMotionFlag == 0) {
      digitalWrite(DOSE_ENABLE, HIGH);
    }

    //включается один раз при запуске машины
    calibrationButtonLights += 1;
    if(calibrationButtonLights == 2){
      digitalWrite(ILLUMINATION_OPTION1, HIGH);
      digitalWrite(ILLUMINATION_OPTION2, HIGH);
      digitalWrite(ILLUMINATION_OPTION3, HIGH);
      calibrationButtonLights = 0;
      calibrDone = 1;
      //изменить состояние кнопки инфо
    }
  }
}

// перемещение датчика сливок в координату
void calcMassaCR() {
  if (stepsDoneCR < abs(stepsForCR)) {
    if (micros() - stepTimerCR >= 500) {
      stepTimerCR = micros();
      if (stepsForCR > 0) {
        makeCR_CCWR();
      }
      if (stepsForCR < 0) {
        makeCR_CWR();
      }
      stepsDoneCR++;
    }
  } else {
    if (DEBUG) {
      Serial.print("calcMassaCR steps: ");
      Serial.println(stepsDoneCR);
    }
    CRInMotionFlag = 0;
    currentCRStep = CRmassa;
    stepsDoneCR = 0;
    if (STInMotionFlag == 0 && CRInMotionFlag == 0) {
      digitalWrite(DOSE_ENABLE, HIGH);
    }

    //включается один раз при запуске машины
    calibrationButtonLights += 1;
    if(calibrationButtonLights == 2){
      digitalWrite(ILLUMINATION_OPTION1, HIGH);
      digitalWrite(ILLUMINATION_OPTION2, HIGH);
      digitalWrite(ILLUMINATION_OPTION3, HIGH);
      calibrationButtonLights = 0;
      calibrDone = 1;
      //добавить: изменить состояние кнопки инфо
    }
  }
}


//********** Вспомогательные функции **********
/*void pushCylinder(byte cylinder, ){
  digitalWrite()
}
*/

void calibrateVolume(){
  switch (calibrateVolumeCase) {
    
    case 0:
      calibrateVolumeCase++;  // переходим в следующий кейс
      digitalWrite(DOSE_ENABLE, LOW);
      break;

    case 1:  // перемещаем подвижные датчики страчателлы и сливок до штоков цилиндров в верхнем положении
      if (digitalRead(SOLENOID_SENSOR14) == HIGH) {
        if (micros() - stepTimerST >= 300) {
          stepTimerST = micros();
          makeST_CWR();  // вращаем мотор страчателлы по часовой
        }
      }
      if (digitalRead(SOLENOID_SENSOR9) == HIGH) {
        if (micros() - stepTimerCR >= 300) {
          stepTimerCR = micros();
          makeCR_CWR();  // вращаем мотор сливок по часовой
          //Serial.println("making CR STEP");
        }
      }
      if (digitalRead(SOLENOID_SENSOR14) == LOW && digitalRead(SOLENOID_SENSOR9) == LOW) {
        calibrateVolumeCase++;  // переходим в следующий кейс
        currentSTStep = 0;         // обнуляем текущуу координату датчика страчателлы
        currentCRStep = 0;         // обнуляем текущуу координату датчика сливок
        stepsForST = STmassa;      // корректируем кол-во полушагов для перемещения датчика страчателлы в нужную координату
        stepsForCR = CRmassa;      // корректируем кол-во полушагов для перемещения датчика сливок в нужную координату
        changeMass();
      }
      break;

    case 2:  // включаем флаги перемещения датчика страчателлы и датчика сливок в нужные координаты
      STInMotionFlag = 1;
      CRInMotionFlag = 1;
      calibrateVolumeCase++;  // переходим в следующий кейс
      break;

    case 3:  // отправвляем информацию на экран
      calibrateVolumeFlag = 0;  // изменение состояния кнопки "страчателла" на выкл
      calibrateVolumeCase = 0;  // обнуляем кейс
      calibrateVolumeAtStart = 0; //не вызываем функцию калибровки из главного лупа

      break;
      
    default:
        if (DEBUG) {
          Serial.print("calibrateVolume() error!  case:");
          Serial.println(calibrateVolumeCase);
          calibrateVolumeCase = 0;
          calibrateVolumeFlag = 0;
        }
      break;
  }
}

// загрузка продукта
void loading() {
}

// разгрузка продукта
void unloading() {
}

// проверка датчиков при включении
void checkSensors(){
  // проверка датчика крышки
  //stateCS = digitalRead(COVER_SENSOR);
  stateCSBuff[7] = stateCS;
  Serial1.write(stateCSBuff, 8);

  // проверка датчика давления
  statePS = digitalRead(PRESSURE_METER);
  statePSBuff[7] = statePS;
  Serial1.write(statePSBuff, 8);

  // проверка датчика ТЭНов
  checkHeaters();
  //checkSolenoidSensorState();
}

//мигаем 
void blinkButton1(int x){
  if(millis() - OptionalButton1Timer > x){
    OptionalButton1Timer = millis();
    if(OptionalButton1State){
      digitalWrite(ILLUMINATION_OPTION1, HIGH);
      OptionalButton1State = 0;
    } else {
      digitalWrite(ILLUMINATION_OPTION1, LOW);
      OptionalButton1State = 1;
    }
  }
}

void blinkButton2(int x){
  if(millis() - OptionalButton2Timer > x){
    OptionalButton2Timer = millis();
    if(OptionalButton2State){
      digitalWrite(ILLUMINATION_OPTION2, HIGH);
      OptionalButton2State = 0;
    } else {
      digitalWrite(ILLUMINATION_OPTION2, LOW);
      OptionalButton2State = 1;
    }
  }
}

void blinkButton(int time, int button){
  if(millis() - OptionalButtonTimer > time){
    OptionalButtonTimer = millis();
    if(OptionalButtonState){
      digitalWrite(button, HIGH);
      OptionalButtonState = 0;
    } else {
      digitalWrite(button, LOW);
      OptionalButtonState = 1;
    }
  }
}