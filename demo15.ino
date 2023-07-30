// развязка Atmega 2560
#define SOLENOID1 A0    // форсунка страчателлы (цилиндр открыт)
#define SOLENOID2 A1    // форсунка сливок (цилиндр открыт)
#define SOLENOID3 A2    // дозатор сливок (цилиндр открыт)
#define SOLENOID4 A3    // голова сливок (цилиндр открыт)
#define SOLENOID5 A4    // верхний пресс (цилиндр открыт)
#define SOLENOID6 A5    // нож (цилиндр закрыт)
#define SOLENOID7 A6    // правый пресс (цилиндр закрыт)
#define SOLENOID8 A7    // чашка (цилиндр закрыт)
#define SOLENOID9 A8    // левый пресс (цилиндр закрыт)
#define SOLENOID10 A9   // сброс (цилиндр открыт)
#define SOLENOID11 A10  // дозатор страчателлы (цилиндр открыт)
#define SOLENOID12 A11  // голова страчателлы (цилиндр открыт)
#define SENSOR1 31      // подвижный датчик страчателлы
#define SENSOR2 30      // подвижный датчик сливок
#define SENSOR3 39      // нижний датчик верхнего пресса
#define SENSOR4 29      // правый датчик ножа
#define SENSOR5 28      // правый датчик правого пресса
#define SENSOR6 27      // верхний датчик чашки
#define SENSOR7 26      // верхний датчик дозатора страчателлы
#define SENSOR8 25      // верхний датчик дозатора сливок
#define SENSOR9 24      // левый датчик правого пресса
#define SENSOR10 A13    // правый датчик левого пресса
#define SENSOR11 A12    // левый датчик левого пресса
#define SENSOR12 33
#define SENSOR13 32
#define CHECK1 A15  // датчик тока 1
#define CHECK2 A14  // датчик тока 2
#define DWIN_POWER 36
#define PEDAL 34
#define STOP 2
#define RESET 35
#define MOTOR_EN 43
#define MOTOR1_1 37
#define MOTOR1_2 40
#define MOTOR2_1 41
#define MOTOR2_2 38
#define ENABLE 5
#define STEP 17 //оригинальный степ16
#define DIR 16  //оригинальный дир17ee
//#define DIR 17
//#define STEP 16

#define DIMMER1 20
#define DIMMER2 21
#define ZERO 3
#define DS18B20 9

// библиотеки
#include <GyverTimers.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//#include <Stepper.h>
#include <AccelStepper.h>

AccelStepper reductor(1, STEP, DIR);
//#include "ACS712.h"

OneWire oneWire(DS18B20);
DallasTemperature sensor(&oneWire);

// переменные таймеров
uint32_t tempTimer, pidTimer, stepTimer, pushTimer, pressTimer;

// переменные флагов
 byte defaultFlag = 0;
 byte defaultPositionsFlag = 0;
 byte burrataFlag = 0;
 byte sensorFlag[] = { 0, 0, 0 };
 byte pedalFlag = 0;
 byte stepFlag = 0;
 int checkStepFlag = 0;

// переменные ПИД
float Int, temp, error, oldError, integral, diff, p = 700, ti = 8, td = 200;
float tempToSend;
float setpoint = 80;
int dimmer, out;

// температура отключения при перегреве датчика




// ============== DWIN ================
 byte DEBUG = 1; 

// переменные DWIN
  byte digitalPedal = 1; //нажатие педали с экранаж

  byte heaterIsOn = 0;
  byte knifeIsOn = 0;
  byte pressIsOn = 0;
  byte rotationIsON = 0;

  byte err = 0;

// значения изменяемых переменных с предварительной загрузкой
  byte loadStrach = 10;
  byte loadCream = 10;
  byte razgruzkaStrach = 10;
  byte razgruzkaSlivki = 10;
  byte moikaCycli = 10;
  byte setupValues[] = {
    loadStrach,
    loadCream,
    razgruzkaStrach,
    razgruzkaSlivki,
    moikaCycli
  };

// адреса изменяемых переменных с предварительной загрузкой
  byte zagruzkaStrachAddr = 0x08;
  byte zagruzkaSlivkiAddr = 0x09;  
  byte razgruzkaStrachAddr = 0x0A;
  byte razgruzkaSlivkiAddr = 0x0B;
  byte moikaCycliAddr = 0x0C;
  byte setAddresses[] = {
    zagruzkaStrachAddr,
    zagruzkaSlivkiAddr,
    razgruzkaStrachAddr,
    razgruzkaSlivkiAddr,
    moikaCycliAddr
  };

  float overHeat = 50.0;

  float userTemp = 30.0; //температура пайки
  char userTempCharArray[] = "0000";
  float time;  //время пайки
  char userTimeCharArray[] = "0000";
  int massa = 100;
  int cream = 10; 
  int rotation = 2;

  char leftTempCharArray[] = "0000";
  char rightTempCharArray[] = "0000";

  int revol = 1200;
  
  uint32_t stepTimer2 = 0;
  uint32_t testCylinderTimer = 0;
  byte testFlag = 0;


// переменные навигации DWIN
 byte lastSetPageID = 0x9A;
 //byte changePageBuff[] = {0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x00 };

// буффер чтения из DWIN
 const byte BUFFER_SIZE = 12;
 byte inputBuf[BUFFER_SIZE];
 byte readPosition = 0;
 byte inputBufPosition = 0;
 byte dwinMessageLength = 0;
 bool inputMessageComplete = false;

// буффер передачи в DWIN
  byte outputUserTempBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X04, 0X0, 0x0, 0x00, 0x00};
  byte outputTimeBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X0B, 0X0, 0x0, 0x00, 0x00};
  byte outputRevolBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X10, 0X0, 0x0};
  byte outputMassaBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X12, 0X0, 0x0};
  byte outputCreamBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X14, 0X0, 0x0};
  byte outputLeftTempBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X16, 0X0, 0x0, 0x00, 0x00};
  byte outputRightTempBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X1A, 0X0, 0x0, 0x00, 0x00};
  byte cylindersBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X25, 0X00, 0X0, 0x0};
  byte wrongPassBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X12, 0X05, 0X0, 0x0};
  byte outputLoadBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X20, 0X08, 0X0, 0x0};
  byte showErrorBuf[] = {0X5A, 0XA5, 0X5, 0X82, 0X11, 0X00, 0X0, 0x0};

  byte changePageBuff[] = {0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x00 };
  byte settingsPageAddrBuf[] = {0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x3F }; // !!!!!!!! либо 0x3f либо 0x09
  byte settingsPageErrorAddrBuf[] = {0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x62 };

  byte switchBuff[] = {0X5A, 0XA5, 0X5, 0X82, 0X11, 0X00, 0X0, 0x0};

// ============== CALIBRATION ================ 
 byte calibrDone = 0;


void setup() {
  pinMode(SOLENOID1, OUTPUT);
  pinMode(SOLENOID2, OUTPUT);
  pinMode(SOLENOID3, OUTPUT);
  pinMode(SOLENOID4, OUTPUT);
  pinMode(SOLENOID5, OUTPUT);
  pinMode(SOLENOID6, OUTPUT);
  pinMode(SOLENOID7, OUTPUT);
  pinMode(SOLENOID8, OUTPUT);
  pinMode(SOLENOID9, OUTPUT);
  pinMode(SOLENOID10, OUTPUT);
  pinMode(SOLENOID11, OUTPUT);
  pinMode(SOLENOID12, OUTPUT);
  pinMode(DWIN_POWER, OUTPUT);
  pinMode(MOTOR_EN, OUTPUT);
  pinMode(MOTOR1_1, OUTPUT);
  pinMode(MOTOR1_2, OUTPUT);
  pinMode(MOTOR2_1, OUTPUT);
  pinMode(MOTOR2_2, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIMMER1, OUTPUT);
  pinMode(DIMMER2, OUTPUT);
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  pinMode(SENSOR5, INPUT);
  pinMode(SENSOR6, INPUT);
  pinMode(SENSOR7, INPUT);
  pinMode(SENSOR8, INPUT);
  pinMode(SENSOR9, INPUT);
  pinMode(SENSOR10, INPUT);
  pinMode(SENSOR11, INPUT);
  pinMode(SENSOR12, INPUT);
  pinMode(SENSOR13, INPUT);
  pinMode(CHECK1, INPUT);
  pinMode(CHECK2, INPUT);
  pinMode(PEDAL, INPUT);
  pinMode(STOP, INPUT);
  pinMode(ZERO, INPUT);

  attachInterrupt(1, isr, RISING);
  Timer2.enableISR();



  // RXTX
  Serial.begin(115200);
  // RS232
  Serial1.begin(115200);
  
  sensor.begin();
  sensor.setWaitForConversion(false);
  
  digitalWrite(DWIN_POWER, HIGH);

  byte changeFirstPage[] = {0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x01 };
  delay(2000);
  Serial1.write(changeFirstPage, 10); 
  
  //калибровка узлов
  while(calibrDone != 1){
    //ждём нажатие пользователем кнопки "старт"
    readDwin();
  }
  digitalWrite(DIMMER1, HIGH);
  digitalWrite(DIMMER2, HIGH);
  //загрузка параметров
  setDwin();
}

void defaultPositions() {
  switch (defaultFlag) {
    case 0:  // выстреливание правого пресса
      digitalWrite(SOLENOID7, HIGH);
      defaultFlag++;
      break;
    case 1:  // ожидание левого датчика правого пресса
      if (digitalRead(SENSOR9) == LOW) {
        defaultFlag++;
      }
      break;
    case 2:  // выстреливание ножа
      digitalWrite(SOLENOID6, HIGH);
      defaultFlag++;
      break;
    default:
      defaultFlag = 0;
      defaultPositionsFlag = 1;
  }
}


void isr() {
  static int lastDim;
  digitalWrite(DIMMER1, LOW);
  digitalWrite(DIMMER2, LOW);
  if (lastDim != dimmer) {
    Timer2.setPeriod(lastDim = dimmer);
  } else {
    Timer2.restart();
  }
}

ISR(TIMER2_A) {
  if(heaterIsOn && temp < overHeat){
    digitalWrite(DIMMER1, HIGH);
    digitalWrite(DIMMER2, HIGH);
    Timer2.stop();
  }
}

void pid() {
  oldError = error;
  
  error = userTemp - temp;
  Int = Int + (oldError + error) / 2;
  integral = constrain(Int * (1 / ti), 0, 9300);
  if (integral == 9300) {
    Int = integral;
  }
  diff = (error - oldError) * td;
  out = constrain(p * error + integral + diff, 500, 9300);
}

void getTemp() {
  sensor.requestTemperatures();
  temp = sensor.getTempCByIndex(0);
}

void makeStep() {
  if (stepFlag == 0) {
    digitalWrite(STEP, HIGH);
    digitalWrite(DIR, LOW);
    stepFlag = 1;
  } else {
    digitalWrite(STEP, LOW);
    digitalWrite(DIR, LOW);
    stepFlag = 0;
  }
}

byte pedalStatus(){
  if (digitalRead(PEDAL) == LOW || digitalPedal == LOW){
    return 0;
  }
  return 1;
}

void makeBurrata() {

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
        if (checkStepFlag < 1200) {
          makeStep();
          checkStepFlag++;
        } else {
          checkStepFlag = 0;
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
      if (millis() - pressTimer >= time) {
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

void readDwin(){
  // читаем serial1 пока не получим полное сообщение
  if (Serial1.available() && inputMessageComplete == false){
    readInput();
  }

  // если полное сообщение пришло, то парсим его
  if (inputMessageComplete){
    parseBuffer();
    inputMessageComplete = false;
    //memcpy(inputBuf, 0, BUFFER_SIZE);
  }
}
  

void loop() {
  // работа с температурой
  if (millis() - tempTimer >= 800) {
    tempTimer = millis();
    getTemp();
    if (tempToSend != temp){
      tempToSend = temp;
      sendLeftTemp();
    }
    
    if(DEBUG){
      //Serial.print("temp = ");
      //Serial.println(temp);
    }
  }

  // работа с ПИД
  if (millis() - pidTimer >= 40) { 
    pidTimer = millis();
    pid();
    dimmer = map(out, 500, 9300, 9300, 500);
  }

  if (pedalStatus() == LOW && pedalFlag == 0) {
    pedalFlag = 1;
  }

  //  if (pedalFlag == 1) {
  //    makeBurrata();
  //  }


  // ============== reading DWIN ================
  readDwin();
  
  // ============= тест шаговика ==================
  if (rotationIsON && micros() - stepTimer2 >= 150){
    stepTimer2 = micros();
    makeStep();
  }

  // ============== тест цилиндров ================
  if (knifeIsOn && millis() - testCylinderTimer > 1000){
    testCylinderTimer = millis();
    switch (testFlag){
      case 0:
        digitalWrite(SOLENOID1, !digitalRead(SOLENOID1));
        testFlag += 1;
        break;
      case 1:
        digitalWrite(SOLENOID1, !digitalRead(SOLENOID1));
        digitalWrite(SOLENOID2, !digitalRead(SOLENOID2));
        testFlag += 1;
        break;
      case 2:
        digitalWrite(SOLENOID2, !digitalRead(SOLENOID2));
        digitalWrite(SOLENOID3, !digitalRead(SOLENOID3));
        testFlag += 1;
        break;
      case 3:
        digitalWrite(SOLENOID3, !digitalRead(SOLENOID3));
        digitalWrite(SOLENOID4, !digitalRead(SOLENOID4));
        testFlag += 1;
        break;
      case 4:
        digitalWrite(SOLENOID4, !digitalRead(SOLENOID4));
        digitalWrite(SOLENOID5, !digitalRead(SOLENOID5));
        testFlag += 1;
        break;
      case 5:
        digitalWrite(SOLENOID5, !digitalRead(SOLENOID5));
        digitalWrite(SOLENOID6, !digitalRead(SOLENOID6));
        testFlag += 1;
        break;
      case 6:
        digitalWrite(SOLENOID6, !digitalRead(SOLENOID6));
        digitalWrite(SOLENOID7, !digitalRead(SOLENOID7));
        testFlag += 1;
        break;
      case 7:
        digitalWrite(SOLENOID7, !digitalRead(SOLENOID7));
        digitalWrite(SOLENOID8, !digitalRead(SOLENOID8));
        testFlag += 1;
        break;
      case 8:
        digitalWrite(SOLENOID8, !digitalRead(SOLENOID8));
        digitalWrite(SOLENOID9, !digitalRead(SOLENOID9));
        testFlag += 1;
        break;
      case 9:
        digitalWrite(SOLENOID9, !digitalRead(SOLENOID9));
        digitalWrite(SOLENOID10, !digitalRead(SOLENOID10));
        testFlag += 1;
        break;
      case 10:
        digitalWrite(SOLENOID10, !digitalRead(SOLENOID10));
        digitalWrite(SOLENOID11, !digitalRead(SOLENOID11));
        testFlag += 1;
        break;
      case 11:
        digitalWrite(SOLENOID11, !digitalRead(SOLENOID11));
        digitalWrite(SOLENOID12, !digitalRead(SOLENOID12));
        testFlag += 1;
        break;
      case 12:
        digitalWrite(SOLENOID12, !digitalRead(SOLENOID12));
        testFlag = 0;
        break;
      default:
        showError();
        break;
    }
  } 
}


