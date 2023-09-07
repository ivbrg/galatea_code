
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
          stepsCCW = checkStepFlagRE;       //замеряем количество шагов до срабатывания датчика

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



//отображение состояния датчика цилиндра на экране
void displaySolenoidSensorLight(byte solenoid) {
  /* switch (solenoid){
    case(0x00): // датчики левого пресса
        if(ssState1 != digitalRead(SOLENOID_SENSOR10)){
          ssState1 = !ssState1;
          ssStateBuf[5] = 0x00;
          ssStateBuf[7] = ssState1;
          Serial1.write(ssStateBuf, 8);
        };
        if(ssState2 != digitalRead(SOLENOID_SENSOR11)){
          ssState2 = !ssState2;
          ssStateBuf[5] = 0x00;
          ssStateBuf[7] = ssState1;
          Serial1.write(ssStateBuf, 8);
        }
      break;

    default:
      showError();
      break;
  }
  */

  /*
  //датчик левого пресса к центру
  if (ssState1 != digitalRead(SOLENOID_SENSOR8)) {
    ssStateBuf[5] = 0x00;
    ssStateBuf[7] = ssState1;
    Serial1.write(ssStateBuf, 8);
    ssState1 = !ssState1;
    changeSSLight = 0;
  }
  //датчик левого пресса от центра
  if (ssState2 != digitalRead(SOLENOID_SENSOR9)) {
    ssStateBuf[5] = 0x01;
    ssStateBuf[7] = ssState2;
    Serial1.write(ssStateBuf, 8);
    ssState2 = !ssState2;
    //changeSSLight = 0;
  }
  //датчик правого пресса к центру
  if (ssState3 != digitalRead(SOLENOID_SENSOR11)) {
    ssStateBuf[5] = 0x02;
    ssStateBuf[7] = ssState3;
    Serial1.write(ssStateBuf, 8);
    ssState3 = !ssState3;
    //changeSSLight = 0;
  }
  //датчик правого пресса от центра
    if (ssState4 != digitalRead(SOLENOID_SENSOR10)) {
    ssStateBuf[5] = 0x03;
    ssStateBuf[7] = ssState4;
    Serial1.write(ssStateBuf, 8);
    ssState4 = !ssState4;
    //changeSSLight = 0;
  }
  //датчик ножа к центру
   if (ssState5 != digitalRead(SOLENOID_SENSOR13)) {
    ssStateBuf[5] = 0x04;
    ssStateBuf[7] = ssState5;
    Serial1.write(ssStateBuf, 8);
    ssState5 = !ssState5;
    //changeSSLight = 0;
  }
  //датчик ножа от центра
   if (ssState6 != digitalRead(SOLENOID_SENSOR12)) {
    ssStateBuf[5] = 0x05;
    ssStateBuf[7] = ssState5;
    Serial1.write(ssStateBuf, 8);
    ssState5 = !ssState5;
    //changeSSLight = 0;
  }
  //датчик редкутора вверх
   if (ssState7 != digitalRead(SOLENOID_SENSOR10)) {
    ssStateBuf[5] = 0x06;
    ssStateBuf[7] = ssState7;
    Serial1.write(ssStateBuf, 8);
    ssState7= !ssState7;
    //changeSSLight = 0;
  }
  //датчик редкутора низ
   if (ssState8 != digitalRead(SOLENOID_SENSOR2)) {
    ssStateBuf[5] = 0x07;
    ssStateBuf[7] = ssState8;
    Serial1.write(ssStateBuf, 8);
    ssState8 = !ssState8;
    //changeSSLight = 0;
  }
  //датчик пресс форсунки низ
   if (ssState9 != digitalRead(SOLENOID_SENSOR7)) {
    ssStateBuf[5] = 0x08;
    ssStateBuf[7] = ssState9;
    Serial1.write(ssStateBuf, 8);
    ssState9 = !ssState9;
    //changeSSLight = 0;
  }
  //датчик страчателлы верх
   if (ssState10 != digitalRead(SOLENOID_SENSOR3)) {
    ssStateBuf[5] = 0x0A;
    ssStateBuf[7] = ssState10;
    Serial1.write(ssStateBuf, 8);
    ssState10 = !ssState10;
    //changeSSLight = 0;
  }
  //датчик страчателлы подвижный
   if (ssState11 != digitalRead(SOLENOID_SENSOR4)) {
    ssStateBuf[5] = 0x0B;
    ssStateBuf[7] = ssState11;
    Serial1.write(ssStateBuf, 8);
    ssState11 = !ssState11;
    //changeSSLight = 0;
  }
  //датчик сливок верх
   if (ssState12 != digitalRead(SOLENOID_SENSOR5)) {
    ssStateBuf[5] = 0x0C;
    ssStateBuf[7] = ssState12;
    Serial1.write(ssStateBuf, 8);
    ssState12 = !ssState12;
    //changeSSLight = 0;
  }
  //датчик сливок подвижный
   if (ssState13 != digitalRead(SOLENOID_SENSOR6)) {
    ssStateBuf[5] = 0x0D;
    ssStateBuf[7] = ssState13;
    Serial1.write(ssStateBuf, 8);
    ssState13 = !ssState13;
    //changeSSLight = 0;
  }
  ///датчик разгрузки
   if (ssState14 != digitalRead(SOLENOID_SENSOR14)) {
    ssStateBuf[5] = 0x07;
    ssStateBuf[7] = ssState14;
    Serial1.write(ssStateBuf, 8);
    ssState14 = !ssState14;
    changeSSLight = 0;
  }
  */
}

// нажата кнопка буррата
void burrataButton() {
  switch (burrataButtonCase) {
    case 0:
      //burrataButtonFlag = 1;
      burrataButtonBuf[7] = 1;
      Serial1.write(burrataButtonBuf, 8);   //изменение состояния кнопки "буррата" на вкл

      digitalWrite(ILLUMINATION_OPTION1, HIGH);  //включаем подсветку кнопки

      burrataButtonIncrementalBuf[7] = 1;
      Serial1.write(burrataButtonIncrementalBuf, 8);   //изменение состояния кнопки "буррата" на вкл

      burrataButtonCase++;
      stepTimerRE = micros();
    break;

    case 1:   //крутим редуктор, пока не сработан датчик оборота
      if (!digitalRead(TURN_SENSOR)) {
        if (micros() - stepTimerRE >= 1500) {
          stepTimerRE = micros();
          makeRE_CCWR();
        }
        //burrataButtonCase = 1;
      } else {
        //burrataButtonCase = 2;
        burrataButtonCase++;
      }
    break;

    case 2:   //идём 500 шагов против часовой
      if (checkStepFlagRE < 500) {
        if (micros() - stepTimerRE >= stepsCCWSpeed) {
          stepTimerRE = micros();
          makeRE_CCWR();
          checkStepFlagRE++;
        }
      } else {
        burrataButtonCase++;
        checkStepFlagRE = 0;
      }

      break;

    case 3:   //замедляемся перед оснатовкой
      if (checkStepFlagRE < 200) {
        if (micros() - stepTimerRE >= stepsCCWSpeed) {
          stepTimerRE = micros();
          makeRE_CCWR();
          checkStepFlagRE++;

          CCWSpeedTest = stepsCCWSpeed * 1.005;
          stepsCCWSpeed = (int)CCWSpeedTest;
        }
      } else {
        burrataButtonCase++;
        //Serial.println(stepsCCWSpeed);

        checkStepFlagRE = 0;
      }

      break;

    case 4:   //едем на минимуме, пока не сработает датчик оборота
      if (digitalRead(TURN_SENSOR)) {
        if (micros() - stepTimerRE >= stepsCCWSpeed) {
          stepsCCWSpeed += 7;
          stepTimerRE = micros();
          makeRE_CCWR();
          stepsCCW++;
        }
      } else {
        burrataButtonCase++;
      }

      break;

    case 5:   //опускам пресс-форсунку и поднимаем редуктор

      if (digitalRead(SOLENOID_SENSOR12) == 0 && digitalRead(SOLENOID_SENSOR7) == 1) {  //если пресс-форсунка опущенаесли и редуктор поднят - идём дальше
        digitalWrite(SOLENOID_SWITCH5, HIGH);  //опускем пресс-форсункe
        cylinderState[5] = 1;                  //запоминаем состояние цилиндра
        burrataButtonCase++;
      };
      break;

    case 6:   //проверяем датчик пресс-форсунки низ
        if(digitalRead(SOLENOID_SENSOR7)){      //проверяем датчик пресс-форсунки низ
          digitalWrite(SOLENOID_SWITCH6, LOW);   //опускаем цилиндр страчателлы
          cylinderState[6] = 0;                  //запоминаем состояние цилиндра
          digitalWrite(SOLENOID_SWITCH7, LOW);   //опускаем цилиндр сливок        
          cylinderState[7] = 0;                  //запоминаем состояние цилиндра
          burrataButtonCase++;
        }
      break;


    case 7:   //дозируем
      if (digitalRead(SOLENOID_SENSOR4) == LOW && sensorFlag[1] == 0) { //страчателла
        digitalWrite(SOLENOID_SWITCH3, HIGH); //форсунка вкл
        cylinderState[3] = 1;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH2, HIGH); //голова страч
        cylinderState[2] = 1;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH6, HIGH); //страч
        cylinderState[6] = 1;                  //запоминаем состояние цилиндра
        sensorFlag[1] = 1;
      }
      if (digitalRead(SOLENOID_SENSOR6) == LOW && sensorFlag[2] == 0) {
        digitalWrite(SOLENOID_SWITCH3, HIGH); //форсунка вкл
        cylinderState[3] = 1;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH1, HIGH); //голова сливок
        cylinderState[1] = 1;                  //запоминаем состояние цилиндра
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

    case 8:   //ждём когда вернуться цилиндры страчателлы и сливок обратно
      if (!digitalRead(SOLENOID_SENSOR3) && !digitalRead(SOLENOID_SENSOR5)) {
          digitalWrite(SOLENOID_SWITCH3, LOW); //форсунка выкл
          cylinderState[3] = 0;                  //запоминаем состояние цилиндра
          digitalWrite(SOLENOID_SWITCH2, LOW); //голова страч
          cylinderState[2] = 0;                  //запоминаем состояние цилиндра
          digitalWrite(SOLENOID_SWITCH1, LOW); //голова сливок
          cylinderState[1] = 0;                  //запоминаем состояние цилиндра
        burrataButtonCase++;
        if (DEBUG) {
          Serial.print("burrataButtonCase case: ");
          Serial.println(burrataButtonCase);
        }
      }
      break;


    case 9:   //опускаем (редуктор)
      digitalWrite(SOLENOID_SWITCH10, LOW);  //опускаем редуктор
      cylinderState[10] = 0;                  //запоминаем состояние редуктора

      burrataButtonCase++;

      stepTimerRE = micros();
      break;

    case 10:  //крутим редуктор по часовой
      if (checkStepFlagRE < 4000) {  // идём 1500 шагов
        if (micros() - stepTimerRE >= 800) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlagRE++;
        }
      }

      if (checkStepFlagRE == 4000 && !digitalRead(SOLENOID_SENSOR11)) {
        burrataButtonCase++;
        Serial.println(checkStepFlagRE);
        checkStepFlagRE = 0;
      }
      break;

    case 11:  //сводим левый/правый пресс
      digitalWrite(SOLENOID_SWITCH9, HIGH);   //левый пресс!!!
      cylinderState[9] = 1;                  //запоминаем состояние цилиндра
      digitalWrite(SOLENOID_SWITCH8, HIGH); //правый пресс!!!
      cylinderState[8] = 1;                  //запоминаем состояние цилиндра
      //delay(1000);
      burrataButtonCase++;
      break;

    case 12:  // отслеживаем срабатывание левого датчика правого пресса и правого датчика левого пресса
      if (!digitalRead(SOLENOID_SENSOR8) && !digitalRead(SOLENOID_SENSOR11)) {
        pressTimer = millis();
        burrataButtonCase++;
      }

      break;

    case 13:  // ждём спаивания
      if(millis() - pressTimer >= 2500){
        burrataButtonCase++;
      }
      break;
    
    case 14:  // выстреливаем нож
      digitalWrite(SOLENOID_SWITCH12, HIGH);
      cylinderState[12] = 1;                  //запоминаем состояние цилиндра
      pressTimer = millis();
      burrataButtonCase++;
      break;

    case 15: // ждём нож пол секунды 
      if(millis() - pressTimer >= 1500){
        burrataButtonCase++;
      }
      break;

    case 16:  // ждём пока нож доедет до датчика в цнетре и убираем нож обратно
      if(!digitalRead(SOLENOID_SENSOR13) ){     // датчик ножа в центре
        digitalWrite(SOLENOID_SWITCH12, LOW);   //отводим нож
        cylinderState[12] = 0;                  //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH9, LOW);   //пресс левый отводим
        cylinderState[9] = 0;                   //запоминаем состояние цилиндра
        digitalWrite(SOLENOID_SWITCH8, LOW);   //пресс правый отводим
        cylinderState[8] = 0;                  //запоминаем состояние цилиндра

        burrataButtonCase++;
      }
      break;

    case 17:  // ждём срабатываение датчиков ножа и прессов от центра
      if(!digitalRead(SOLENOID_SENSOR12) && !digitalRead(SOLENOID_SENSOR9) && !digitalRead(SOLENOID_SENSOR10)){
        burrataButtonCase++;
      }
      break;

    case 18:  //поднимаем редуктор,
      digitalWrite(SOLENOID_SWITCH10, HIGH);  // поднимаем редуктор
      cylinderState[10] = 1;                  //запоминаем состояние цилиндра
      digitalWrite(SOLENOID_SWITCH5, LOW);   // поднимаем пресс-форсунку
      cylinderState[5] = 0;                  //запоминаем состояние цилиндра
      digitalWrite(SOLENOID_SWITCH4, LOW);   // опускаем чашечку
      cylinderState[4] = 0;                  //запоминаем состояние цилиндра

      if (!digitalRead(SOLENOID_SENSOR10)) {
        burrataButtonCase++;   
      }
      break;

    case 19:
      digitalWrite(SOLENOID_SWITCH11, HIGH);  // опускаем 
      cylinderState[11] = 1;                  //запоминаем состояние цилиндра

      pressTimer = millis();
      burrataButtonCase++;
      break;

    case 20:  
      if(millis() - pressTimer >= 3000){
        burrataButtonCase++;
      }            
      break;

    case 21:  // поднимаем разгрузку
      digitalWrite(SOLENOID_SWITCH11, LOW);
      cylinderState[11] = 0;                  //запоминаем состояние цилиндра
      if(!digitalRead(SOLENOID_SENSOR14)){
        burrataButtonCase++;
      }
      break;

    case 22:  // поднимаем чашечку
      digitalWrite(SOLENOID_SWITCH4, HIGH);  
      cylinderState[4] = 0;                  //запоминаем состояние цилиндра 
      burrataButtonCase++;
      break;


    case 23:
      burrataButtonCase = 0;
      burrataButtonFlag = 0;    //изменение состояния кнопки "шаг" на выкл
      burrataButtonBuf[7] = 0;  //отправляем значение на экран  
      Serial1.write(burrataButtonBuf, 8); 
      digitalWrite(ILLUMINATION_OPTION1, LOW);  //выключаем кнопку

      stepsCCWSpeed = 1250;
      stepsCCW = 0;

      break;

    default:
      burrataButtonFlag = 0;
      burrataButtonCase = 0;
      showError();
      if (DEBUG) {
        Serial.println("burrataButton() error!");
        Serial.println(__func__);
      }
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
     
      digitalWrite(ILLUMINATION_OPTION2, HIGH);  //включаем подсветку кнопки

      rotateDiskButtonIncrementalBuff[7] = 1;
      Serial1.write(rotateDiskButtonIncrementalBuff, 8);

      rotateDiskButtonCase++;
      stepTimerRE = micros();
      break;

    case 1:   //крутим редуктор, пока не сработан датчик оборота
      if (!digitalRead(TURN_SENSOR)) {
        if (micros() - stepTimerRE >= 1500) {
          stepTimerRE = micros();
          makeRE_CCWR();
        }
        //rotateDiskButtonCase = 1;
      } else {
        //rotateDiskButtonCase = 2;
        rotateDiskButtonCase++;
      }
      break;

    case 2:   //идём 500 шагов против часовой
      if (checkStepFlagRE < 500) {
        if (micros() - stepTimerRE >= stepsCCWSpeed) {
          stepTimerRE = micros();
          makeRE_CCWR();
          checkStepFlagRE++;
        }
      } else {
        rotateDiskButtonCase++;
        checkStepFlagRE = 0;
      }

      break;

    case 3:   //замедляемся перед оснатовкой
      if (checkStepFlagRE < 200) {
        if (micros() - stepTimerRE >= stepsCCWSpeed) {
          stepTimerRE = micros();
          makeRE_CCWR();
          checkStepFlagRE++;

          CCWSpeedTest = stepsCCWSpeed * 1.005;
          stepsCCWSpeed = (int)CCWSpeedTest;
        }
      } else {
        rotateDiskButtonCase++;
        //Serial.println(stepsCCWSpeed);

        checkStepFlagRE = 0;
      }

      break;

    case 4:   //едем на минимуме, пока не сработает датчик оборота
      if (digitalRead(TURN_SENSOR)) {
        if (micros() - stepTimerRE >= stepsCCWSpeed) {
          stepsCCWSpeed += 7;
          stepTimerRE = micros();
          makeRE_CCWR();
          stepsCCW++;
        }
      } else {
        rotateDiskButtonCase++;
      }

      break;


    case 5:
      rotateDiskButtonCase = 0;
      rotateDiskButtonFlag = 0;
      rotateDiskButtonBuf[7] = 0;
      Serial1.write(rotateDiskButtonBuf, 8);    //изменение состояния кнопки "шаг" на выкл
      digitalWrite(ILLUMINATION_OPTION2, LOW);  //отправляем значение на экран

      stepsCCWSpeed = 1250;
      stepsCCW = 0;

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
}