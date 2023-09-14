
// алгоритм калибровки
/*void calibration() {
  switch (calibrationCase) {

      // проверяем состояние датчика защитного экрана
    case 0:
      if (!digitalRead(COVER_SENSOR)) {  //если датчик сработан

        calibrBuff[7] = 1;                      //индикатор CS зелёный
        Serial1.write(calibrBuff, 8);           //отправка значния индикатора
        coverSensorErrorBuf[7] = 0;             //иконка с ошибкой CS выкл
        Serial1.write(coverSensorErrorBuf, 8);  //отображение иконки с ошибкой

        coverSensorState = 0;
        calibrationCase++;  //переход к следующему этапу калибровки

      } else if (coverSensorState == 0) {

        calibrBuff[7] = 2;                      //индикатор CS красный
        Serial1.write(calibrBuff, 8);           //отправка значния индикатора
        coverSensorErrorBuf[7] = 1;             //иконка с ошибкой CS вкл
        Serial1.write(coverSensorErrorBuf, 8);  //отображение иконки с ошибкой

        coverSensorState = 1;
      }
      break;

    // крутить мотор против часовой до срабатывания датчика
    case 1:
      if (micros() - stepTimerRE >= 75) {
        stepTimerRE = micros();
        if (!digitalRead(TURN_SENSOR)) {  // датчик оборота сработал
          stepsRE = checkStepFlagRE;       //замеряем количество шагов до срабатывания датчика

          if (DEBUG) {
            Serial.print("CCW steps: ");
            Serial.print(checkStepFlagRE);
          }

          checkStepFlagRE = 0;
          calibrationCase++;

        } else {  // датчик оборота не сработал
          makeRE_CCWR();
          checkStepFlagRE++;
        }
      }
      if (DEBUG) {
        //Serial.println("calibration() case:1");
      }
      break;

    // пауза между оборотами
    case 2:
      delay(500);
      calibrationCase++;
      break;

    // крутить мотор по часовой до срабатывания датчика
    case 3:
      if (micros() - stepTimerRE >= 150) {
        stepTimerRE = micros();
        if (!digitalRead(TURN_SENSOR)) {

          if (DEBUG) {
            Serial.print("CW steps: ");
            Serial.print(checkStepFlagRE);
          }

          calibrBuff[5] = 1;             //адрес иконки датчика TS
          calibrBuff[7] = 1;             //индикатор CS зелёный
          Serial1.write(calibrBuff, 8);  //отправка значния индикатора

          checkStepFlagRE = 0;
          calibrationCase++;
        } else {
          makeRE_CWR();
          checkStepFlagRE++;
        }
      }
      break;

    //проверка левого датчика температуры
    case 4:
      getTemp();
      if (tempLeft > -1) {
        calibrBuff[5] = 2;             //адрес иконки датчика температуры
        calibrBuff[7] = 1;             //индикатор DSL зелёный
        Serial1.write(calibrBuff, 8);  //отправка значния индикатора
        calibrationCase++;
      } else {
        calibrBuff[5] = 2;             //адрес иконки датчика температуры
        calibrBuff[7] = 2;             //индикатор DSL красный
        Serial1.write(calibrBuff, 8);  //отправка значния индикатора
        calibrationCase++;
      }
      break;

    case 5:
      calibrationCase = 0;
      makeCalibration = 0;
      calibrDone = 1;
      delay(1000);

      changeMainPageBuff[9] = 0x1E;
      Serial1.write(changeMainPageBuff, 10);
      break;

    default:
      showError();
      if (DEBUG) {
        Serial.println("calibration() ERROR!");
      }
  }
}

//основной цикл изготовления бурраты
/* void makeBurrata() {
   switch (burrataFlag) {
     case 0:  // прессуем верхним прессом, отводим нож
       //     digitalWrite(SOLENOID5, HIGH);
       digitalWrite(SOLENOID6, LOW);
       burrataFlag++;
       break;
     case 1:  // отслеживаем завершение отвода ножа по датчику
       if (digitalRead(SENSOR4) == LOW) {
         burrataFlag++;
       }
       break;
     case 2:  // отводим правый пресс
       digitalWrite(SOLENOID7, LOW);
       burrataFlag++;
       break;
     case 3:  // отслеживаем завершение отвода правого пресса по датчику
       if (digitalRead(SENSOR5) == LOW) {
         burrataFlag++;
       }
       break;
     case 4:  // поднимаем чашку, начинаем загрузку страчателлы и сливок
       digitalWrite(SOLENOID8, HIGH);
       digitalWrite(SOLENOID11, HIGH);
       digitalWrite(SOLENOID3, HIGH);
       burrataFlag++;
       break;
     case 5:  // дозируем
       if (digitalRead(SENSOR1) == LOW && sensorFlag[1] == 0) {
         digitalWrite(SOLENOID12, HIGH);
         digitalWrite(SOLENOID1, HIGH);
         digitalWrite(SOLENOID2, HIGH);
         digitalWrite(SOLENOID11, LOW);
         sensorFlag[1] = 1;
       }
       if (digitalRead(SENSOR2) == LOW && sensorFlag[2] == 0) {
         digitalWrite(SOLENOID4, HIGH);
         digitalWrite(SOLENOID2, HIGH);
         digitalWrite(SOLENOID3, LOW);
         sensorFlag[2] = 1;
       }
       if (sensorFlag[1] == 1 && sensorFlag[2] == 1) {
         sensorFlag[1] = 0;
         sensorFlag[2] = 0;
         burrataFlag++;
       }
       break;
     case 6:  // закрываем форсунки по срабатыванию обоих верхних датчиков дозаторов
       if (digitalRead(SENSOR7) == LOW && digitalRead(SENSOR8) == LOW) {
         digitalWrite(SOLENOID1, LOW);
         digitalWrite(SOLENOID2, LOW);
         burrataFlag++;
       }
       break;
     case 7:  // опускаем чашку
       digitalWrite(SOLENOID8, LOW);
       burrataFlag++;
       stepTimer = millis();
       break;
     case 8:  // крутим по шагам
       if (millis() - stepTimer >= 1) {
         stepTimer = millis();
         if (checkStepFlagRE < 1200) {
           makeStep();
           checkStepFlagRE++;
         } else {
           checkStepFlagRE = 0;
           burrataFlag++;
         }
       }
       break;
     case 9:  // выстреливаем прессы
       digitalWrite(SOLENOID7, HIGH);
       digitalWrite(SOLENOID9, HIGH);
       burrataFlag++;
       break;
     case 10:  // отслеживаем срабатывание левого датчика правого пресса и правого датчика левого пресса
       if (digitalRead(SENSOR9) == LOW && digitalRead(SENSOR10) == LOW) {
         burrataFlag++;
         pressTimer = millis();
       }
       break;
     case 11:  // ждем спаивание прессами
       if (millis() - pressTimer >= 1800) {
         burrataFlag++;
       }
       break;
     case 12:  // выстреливаем нож
       digitalWrite(SOLENOID6, HIGH);
       burrataFlag++;
       pressTimer = millis();
       break;
     case 13:  // ждем срабатывание ножа
       if (millis() - pressTimer >= 200) {
         burrataFlag++;
       }
       break;
     case 14:  // отводим нож
       digitalWrite(SOLENOID6, LOW);
       burrataFlag++;
       break;
     case 15:  // отслеживаем завершение отвода ножа по датчику
       if (digitalRead(SENSOR4) == LOW) {
         burrataFlag++;
       }
       break;
     case 16:  // отводим прессы
       digitalWrite(SOLENOID7, LOW);
       digitalWrite(SOLENOID9, LOW);
       burrataFlag++;
       break;
     case 17:  // отслеживаем отвод прессов по левому датчику левого пресса и правому датчику правого пресса
       if (digitalRead(SENSOR5) == LOW && digitalRead(SENSOR11) == LOW) {
         burrataFlag++;
       }
       break;
     case 18:  // поднимаем чашку, открываем сброс
       digitalWrite(SOLENOID8, HIGH);
       digitalWrite(SOLENOID10, HIGH);
       burrataFlag++;
       break;
     case 19:  // отслеживаем верхний датчик чашки
       if (digitalRead(SENSOR6) == LOW) {
         burrataFlag++;
       }
       break;
     case 20:  // опускаем чашку
       digitalWrite(SOLENOID8, LOW);
       burrataFlag++;
       pushTimer = millis();
       break;
     case 21:  // ждем опускание чашки
       if (millis() - pushTimer >= 400) {
         burrataFlag++;
       }
       break;
     case 22:  // выстреливаем правый пресс, поднимаем верхний пресс
       digitalWrite(SOLENOID7, HIGH);
       digitalWrite(SOLENOID5, LOW);
       burrataFlag++;
       break;
     case 23:  // отслеживаем выстреливание правого пресса по левому датчику
       if (digitalRead(SENSOR9) == LOW) {
         burrataFlag++;
       }
       break;
     case 24:  // выстреливаем нож, закрываем сброс, открываем бункеры
       digitalWrite(SOLENOID6, HIGH);
       digitalWrite(SOLENOID10, LOW);
       digitalWrite(SOLENOID4, LOW);
       digitalWrite(SOLENOID12, LOW);
       burrataFlag++;
     default:
       burrataFlag = 0;
       pedalFlag = 0;
   }
 }
*/

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
  }
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
      stepTimerRE = micros();
      checkStepFlagRE = 0;

      //меняем кейс функции rotateDiskButton() для перехода к кручению
      rotateDiskButtonCase = 1;
      //начинаем засекать цикл изготовления бурраты
      
    break;
/*
    // case 1:   //крутим редуктор, пока не сработан датчик оборота
    //   digitalWrite(SOLENOID_SWITCH11, HIGH);  //подняли разгрузку
    //   if (digitalRead(TURN_SENSOR)) {
    //     if (micros() - stepTimerRE >= 5000) {
    //       stepTimerRE = micros();
    //       makeRE_CWR();
    //     }

    //     //burrataButtonCase = 1;
    //   } else {
    //     //burrataButtonCase = 2;
    //     stepsCWSpeed = 5000;
    //     burrataButtonCase++;
    //   }
    // break;
*/
    case 1:   //идём 75 шагов против часовой с ускорением
      digitalWrite(SOLENOID_SWITCH11, HIGH);  //подняли разгрузку
      
      //пропускаем первый кейс в функции оборотов
      
      /*
      rotateDiskButton();
      if(rotateDiskButtonCase == 0){
        burrataButtonCase++;
      }
      */
      burrataButtonCase++;
      break;

      /* заменили старые кейсы на функцию
      if (checkStepFlagRE < 250) {
        if (micros() - stepTimerRE >= stepsCWSpeed) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;
          stepsCWSpeed -= 9;       //ускоряем каждый полушаг
        }
      } else {
        burrataButtonCase++;
        checkStepFlagRE = 0;
      }

      break;

    case 2:   //идём 225 шагов на максимальной скорости
      if (checkStepFlagRE < 410) {
        if (micros() - stepTimerRE >= stepsCWSpeed) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;
        }
      } else {
        burrataButtonCase++;
        checkStepFlagRE = 0;
      }
      break;

    case 3:   //замедляемся перед оснатовкой
      if (checkStepFlagRE < 70) {
        if (micros() - stepTimerRE >= stepsCWSpeed) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;

          RESpeedTest = stepsCWSpeed + 14;
          stepsCWSpeed = (int)RESpeedTest;
          
        }
      } else {
        burrataButtonCase++;
        //Serial.println(stepsCCWSpeed);
        checkStepFlagRE = 0;
      }

      break;

    case 4:   //едем на минимуме, пока не сработает датчик оборота
      if (digitalRead(TURN_SENSOR)) {
        if (micros() - stepTimerRE >= stepsCWSpeed) {
          stepsCWSpeed += 7;
          stepTimerRE = micros();
          makeRE_CWR();
        }
      } else {
        burrataButtonCase++;
      }
      break;

    case 5:     //идём нужное количество шагов после срабатывания датчика = 0!
      if (checkStepFlagRE < 0) {
        if (micros() - stepTimerRE >= stepsCWSpeed) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;
        }
      } else {
        burrataButtonCase++;
      }
      break;

ИЗМЕНИЛИ НУМЕРАЦИЮ КЕЙСОВ
*/

    case 2:   //опускаем пресс-форсунку и начали накачку цилиндров
        cycleTimer = millis();
        digitalWrite(SOLENOID_SWITCH3, HIGH);   //поднимаем подставку
        cylinderState[3] = 1;                   //запоминаем состояние цилиндра подставки  
        digitalWrite(SOLENOID_SWITCH5, HIGH);  //опускем пресс-форсункe
        cylinderState[5] = 1;                   //запоминаем состояние цилиндра пресс форсунки
        burrataButtonCase++;
      break;

    case 3:   //проверяем датчик пресс-форсунки низ
        
        if(digitalRead(SOLENOID_SENSOR3) == 0){      //проверяем датчик пресс-форсунки низ
          digitalWrite(SOLENOID_SWITCH6, LOW); //страч
          cylinderState[6] = 1;                  //запоминаем состояние цилиндра
          digitalWrite(SOLENOID_SWITCH7, LOW); //сливки
          cylinderState[7] = 1;                  //запоминаем состояние цилиндра
        
          burrataButtonCase++;
        }
      break;


    case 4:   //дозируем
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
        burrataButtonCase++;
      }
      break;

    case 5:   //ждём когда вернуться цилиндры страчателлы и сливок обратно
      if (!digitalRead(SOLENOID_SENSOR11) && !digitalRead(SOLENOID_SENSOR13)) {     //ждём срабатывания верхних датчиков страчателлы и сливок 
        digitalWrite(SOLENOID_SWITCH4, HIGH); //форсунка выкл
        cylinderState[4] = 0;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH2, LOW); //голова страч
        cylinderState[2] = 0;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH1, LOW); //голова сливок
        cylinderState[1] = 0;                  //запоминаем состояние цилиндра
        burrataButtonCase++;
      }
      break;

    case 6:  //крутим чашку 
      if(digitalRead(SOLENOID_SENSOR12) == 0 && checkStepFlagRE > (rotation * 800 * 6)){ //полушаги + передатка
          cylinderState[10] = 0;                  //запоминаем состояние редуктора
          burrataButtonCase++;
          checkStepFlagRE = 0;

        } else {
          if (micros() - stepTimerRE >= stepsCCWSpeed) {
            stepTimerRE = micros();
            makeRE_CCWR();
            checkStepFlagRE++;
          }
          digitalWrite(SOLENOID_SWITCH10, LOW);  //опускаем редуктор
        }
      break;

    case 7:  //сводим левый/правый пресс
      digitalWrite(SOLENOID_SWITCH9, HIGH);   //левый пресс!!!
      cylinderState[9] = 1;                  //запоминаем состояние цилиндра
      digitalWrite(SOLENOID_SWITCH8, HIGH); //правый пресс!!!
      cylinderState[8] = 1;                  //запоминаем состояние цилиндра
      //delay(1000);
      burrataButtonCase++;
      break;

    case 8:  // отслеживаем срабатывание датчиков прессов в центре и стреляем ножом
      if (!digitalRead(SOLENOID_SENSOR5) && !digitalRead(SOLENOID_SENSOR2)) {
        knifeTimer2 = millis();
        pressTimer = millis();
        burrataButtonCase++;
      }
      break;

    case 9:    //ждём срабатывания датчика ножа в центре
      if(millis() - knifeTimer2 > 1500){
        digitalWrite(SOLENOID_SWITCH12, HIGH);   //стреляем ножом
        cylinderState[12] = 1;                  //запоминаем состояние цилиндра
        
        if(digitalRead(SOLENOID_SENSOR8) == 0){
          knifeTimer1 = millis();
          burrataButtonCase++;
        }
      }
      
      break;

    case 10: // ждём нож пол секунды 
      if(millis() - knifeTimer1 >= 1000){
        digitalWrite(SOLENOID_SWITCH12, LOW);   //убираем нож 
        cylinderState[12] = 0;                  //запоминаем состояние цилиндра
        burrataButtonCase++;
      }
      break;

    case 11:  //ждём датчик ножа от центра
      if(digitalRead(SOLENOID_SENSOR7) == 0){
        burrataButtonCase++;
      }
      break;

    case 12:  // ждём спаивания и убираем прессы
      if(millis() - pressTimer >= int(time*1000)){
        digitalWrite(SOLENOID_SWITCH9, LOW);   //пресс левый отводим
        cylinderState[9] = 0;                   //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH8, LOW);   //пресс правый отводим
        cylinderState[8] = 0;                  //запоминаем состояние цилиндра
        burrataButtonCase++;
      }
      break;


    case 13:  // ждём срабатываение датчиков ножа и прессов от центра
      if(!digitalRead(SOLENOID_SENSOR1) && !digitalRead(SOLENOID_SENSOR4)){
       
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
      
        burrataButtonCase++;
      } else {
        if (micros() - stepTimerRE >= 500) {
          stepTimerRE = micros();
          makeRE_CCWR();
          checkStepFlagRE++;
        }
        digitalWrite(SOLENOID_SWITCH10, HIGH);  //поднимаем редуктор
        digitalWrite(SOLENOID_SWITCH5, LOW);  //подниамем пресс-форсункe
        digitalWrite(SOLENOID_SWITCH3, LOW);   //опускаем подставку 
      }
      break;

    case 15:  //опускаем разгрузку
      digitalWrite(SOLENOID_SWITCH11, LOW);
      cylinderState[11] = 0; 
      burrataButtonCase++;
      break;  

    case 16:  //проверяем датчик разгрузки в нижнем положении
      if(digitalRead(SOLENOID_SENSOR6) == 0){
        burrataButtonCase++;
      }
      break;

    case 17:
      burrataButtonCase = 0;
      burrataButtonFlag = 0;    //изменение состояния кнопки "шаг" на выкл
      
      //отправка времени цикла на экран
      sec = cycleTimer / 1000;
      mSec = (cycleTimer % 1000)*0.001;
      cycleTime = sec + mSec;      
      sendCycleTimer(cycleTime);
      if(DEBUG){
        Serial.print("cycle time in burrata: ");
        Serial.print(cycleTime);
      }

      //отправка количества циклов
      todayCycles++;
      sendTodayCycle(todayCycles);

      burrataButtonBuf[7] = 0;  //отправляем значение на экран  
      Serial1.write(burrataButtonBuf, 8); 
      digitalWrite(ILLUMINATION_OPTION1, HIGH);  //выключаем кнопку

      break;

    default:
      showError();
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
    case 0:
      //rotateDiskButtonFlag = 1;
      rotateDiskButtonBuf[7] = 1;
      Serial1.write(rotateDiskButtonBuf, 8);    //включаем кнопку "шаг" на экране
     
      digitalWrite(ILLUMINATION_OPTION2, LOW);  //включаем подсветку кнопки

/*
      rotateDiskButtonIncrementalBuff[7] = 1;
      Serial1.write(rotateDiskButtonIncrementalBuff, 8);
*/
      ///checkStepFlagRE = 0;

      if(DEBUG){
        Serial.print("rotation case: ");
        Serial.println(rotateDiskButtonCase);
      }
      rotateDiskButtonCase++;
      stepTimerRE = micros();

      //начинаем засекать цикл изготовления бурраты
      cycleTimer = millis();
      break;

    case 1:
      if (checkStepFlagRE < 203) {
        if (micros() - stepTimerRE >= 1500) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;
        }
      } else {
        checkStepFlagRE = 0;

        if(DEBUG){
          Serial.print("rotation case: ");
          Serial.println(rotateDiskButtonCase);
        }
        rotateDiskButtonCase++;
      }
    break;

    case 2:   //идём 125 шагов против часовой с ускорением
      if (checkStepFlagRE < 100) {
        if (micros() - stepTimerRE >= stepsCWSpeed) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;
          stepsCWSpeed -= 32;       //ускоряем каждый полушаг
          /* 
          if(DEBUG){
            Serial.println(checkStepFlagRE);
            Serial.println(stepsCWSpeed);
          }
          */
        }
      } else {
        checkStepFlagRE = 0;
        
        if(DEBUG){
          Serial.print("rotation case: ");
          Serial.println(rotateDiskButtonCase);
        }
        rotateDiskButtonCase++;
      }
      break;

    case 3:   //идём 250 шагов на максимальной скорости
      if (checkStepFlagRE < 600) {
        if (micros() - stepTimerRE >= stepsCWSpeed) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;
        }
      } else {
        checkStepFlagRE = 0;

        if(DEBUG){
          Serial.print("rotation case: ");
          Serial.println(rotateDiskButtonCase);
        }
        rotateDiskButtonCase++;

      }

      break;

    case 4:   //замедляем
      if (checkStepFlagRE < 100) {
        if (micros() - stepTimerRE >= stepsCWSpeed) {
          stepTimerRE = micros();
          makeRE_CWR();
          stepsCWSpeed += 34;
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
      }

    break;

/*  кейс по датчику
    case 4:
      if (digitalRead(TURN_SENSOR)) {
        if (micros() - stepTimerRE >= stepsCWSpeed) {
          stepsCWSpeed += 7;
          stepTimerRE = micros();
          makeRE_CWR();
          stepsRE++;
        }
      } else {
          cycleTimer = (millis() - cycleTimer);
      break;

*/
    case 5:
      if(DEBUG){
        Serial.print("rotation case: ");
        Serial.println(rotateDiskButtonCase);
      }

      //рассчёт времени цикла "при нажатой копки шаг"
      if(rotateDiskButtonFlag){
        sec = cycleTimer / 1000;
        mSec = (cycleTimer % 1000)*0.001;
        cycleTime = sec + mSec;
        Serial.print(cycleTime);
        sendCycleTimer(cycleTime);
      }
        
      rotateDiskButtonCase = 0;
      rotateDiskButtonFlag = 0;
      stepsCWSpeed = 4000;
      checkStepFlagRE = 0;

      rotateDiskButtonBuf[7] = 0;               //изменение состояния кнопки "шаг" на выкл
      Serial1.write(rotateDiskButtonBuf, 8);    //отправка состояния кнопки  на экран
      digitalWrite(ILLUMINATION_OPTION2, HIGH);  //отправляем значение на экран

      //обнуляем состояние по выполнению всей функции
      break;

    default:
      rotateDiskButtonFlag = 0;
      rotateDiskButtonCase = 0;
      showError();
      if (DEBUG) {
        Serial.println("rotateDiskButton() error!");
        Serial.println(__func__);
      }
      break;
  }
}

// нажата кнопка включения масса
void straciatellaButton() {
  /*
  case 0:  // отправляем информацию на экран
      //straciatellaButtonFlag = 1;
      strachiatellaButtonBuf[7] = 1;
      Serial1.write(strachiatellaButtonBuf, 8); //включаем кнопку "масса" на экране

      digitalWrite(ILLUMINATION_OPTION3, HIGH);  //включаем подсветку кнопки
      
      strachiatellaButtonIncrementalBuf[7] = 1;
      Serial1.write(strachiatellaButtonIncrementalBuf, 8); //включаем кнопку "масса" на экране

      straciatellaButtonCase++;  // переходим в следующий кейс
      digitalWrite(DOSE_ENABLE, LOW);
      break;

    default:
        showError();
        if (DEBUG) {
          straciatellaButtonFlag = 0;
          straciatellaButtonCase = 0;
          Serial.print("straciatellaButton() error!  case:");
          Serial.println(straciatellaButtonCase);
        }
      break;

  */
}




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

      calibrButtonsBuf[5] = 0x02;
      calibrButtonsBuf[7] = calibrateVolumeFlag;
      Serial1.write(calibrButtonsBuf, 8);      
      break;
      
    default:
        showError();
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

// алгоритм калибровки
void calibrate() {
  if (!digitalRead(COVER_SENSOR)) {  //если датчик крышки сработан

    calibrBuff[7] = 1;                      //индикатор CS зелёный
    Serial1.write(calibrBuff, 8);           //отправка значния индикатора
    coverSensorErrorBuf[7] = 0;             //иконка с ошибкой CS выкл
    Serial1.write(coverSensorErrorBuf, 8);  //отображение иконки с ошибкой

    coverSensorState = 0;
    //calibrationCase++;  //переход к следующему этапу калибровки

  } else if (coverSensorState == 0) {

    calibrBuff[7] = 2;                      //индикатор CS красный
    Serial1.write(calibrBuff, 8);           //отправка значния индикатора
    coverSensorErrorBuf[7] = 1;             //иконка с ошибкой CS вкл
    Serial1.write(coverSensorErrorBuf, 8);  //отображение иконки с ошибкой

    coverSensorState = 1;
  }

  /*if (calibrVolume) {
  }*/

  if (calibrDisk) {
    rotateDiskButton();
  }

  if (calibrPosition) {
  }

  if (calibrHeaters) {
  }
}



// проверка датчиков при включении
void checkSensors(){
  // проверка датчика крышки
  stateCS = digitalRead(COVER_SENSOR);
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