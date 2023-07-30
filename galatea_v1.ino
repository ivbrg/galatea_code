// Atmega 2560

byte DEBUG = 1;

// SOLENOIDS
#define SOLENOID_SWITCH1 35     //пресс-форсунка
#define SOLENOID_SWITCH2 34     //цилиндр страчателлы
#define SOLENOID_SWITCH3 33     //цилиндр сливок
#define SOLENOID_SWITCH4 32     //редуктор
#define SOLENOID_SWITCH5 31
#define SOLENOID_SWITCH6 30
#define SOLENOID_SWITCH7 39
#define SOLENOID_SWITCH8 29
#define SOLENOID_SWITCH9 28
#define SOLENOID_SWITCH10 27
#define SOLENOID_SWITCH11 26
#define SOLENOID_SWITCH12 25

// SENSORS
#define SOLENOID_SENSOR1 A0   //датчик пресс-форсунки верх
#define SOLENOID_SENSOR2 A1   //датчик пресс-форсунки низ
#define SOLENOID_SENSOR3 A2   //датчик страчателлы верх
#define SOLENOID_SENSOR4 A3   //датчик страчателлы низ
#define SOLENOID_SENSOR5 A4   //датчик сливок верх
#define SOLENOID_SENSOR6 A5   //датчик сливок низ
#define SOLENOID_SENSOR7 A6   //датчик редуктора верх
#define SOLENOID_SENSOR8 A7   //датчик редуктор низ
#define SOLENOID_SENSOR9 A8   
#define SOLENOID_SENSOR10 A9
#define SOLENOID_SENSOR11 A10
#define SOLENOID_SENSOR12 A11
#define SOLENOID_SENSOR13 A12
#define SOLENOID_SENSOR14 A13
#define COVER_SENSOR 43
#define TURN_SENSOR 42
#define PRESSURE_METER 21
#define DS18B20_L 9
#define DS18B20_R 8
#define CURRENT_SENSOR_L A15
#define CURRENT_SENSOR_R A14

// DWIN POWER
#define DWIN_POWER 44

// BUTTONS
#define PEDAL 36
#define STOP 2
#define OPTIONAL_BUTTON1 41
#define OPTIONAL_BUTTON2 40
#define OPTIONAL_BUTTON3 37

// RESET
#define RESET 10

// STEP MOTORS
#define REDUCER_ENABLE 11
#define DOSE_ENABLE 49
#define REDUCER_DIR 13
#define STRACCIATELLA_DIR 47
#define CREAM_DIR 38
#define REDUCER_STEP 12
#define STRACCIATELLA_STEP 48
#define CREAM_STEP 20

// HEATERS
#define DIMMER_L 45
#define DIMMER_R 46
#define ZERO 3

// ILLUMINATION
#define ILLUMINATION_OPTION1 23
#define ILLUMINATION_OPTION2 24
#define ILLUMINATION_OPTION3 22

// библиотеки
#include <GyverTimers.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ACS712.h>

ACS712 ACS712_L(CURRENT_SENSOR_L, 5.0, 1023, 185);
ACS712 ACS712_R(CURRENT_SENSOR_R, 5.0, 1023, 185);

OneWire oneWireLeft(DS18B20_L);
DallasTemperature sensorLeft(&oneWireLeft);

OneWire oneWireRight(DS18B20_L);
DallasTemperature sensorRight(&oneWireRight);

// переменные датчиков тока (mA)
float leftHeaterCurrent;
float rightHeaterCurrent;

// переменные для подсветки датчиков цилиндров на экране
bool ssState1 = 0;
byte prevState1 = 0;
bool ssState2 = 0;
byte prevState2 = 0;

// переменные таймеров
uint32_t tempTimer, pidTimer, stepTimer, pushTimer, pressTimer, testTimer;

// переменные флагов
byte coverSensorState = 0;            // флаг состояние датчика защитного экрана
byte changeSSLight = 0;               // цилиндр выстрелил, нужно проверить сенсоры
byte solenoidNum = 0;                 // номер цилиндра для проверки сенсоров
byte defaultFlag = 0;                 // флаг выставления стартовой позиции
byte defaultPositionsFlag = 0;        // флаг запуска выставления стартовой позиции
byte burrataFlag = 0;                 // флаг основного цикла изготовления бурраты
byte sensorFlag[] = { 0, 0, 0 };      // флаги дозации
byte pedalFlag = 0;                   // флаг нажатия педали

byte stepFlag = 0;                    // тестовый степ флаг  
byte stepFlagRE = 0;                  // степ флаг диска
float checkStepFlag = 0;
float stepsCW = 0;   // колиество шагов редуктора сделанных по часовой стрелке
float stepsCCW = 0;  // колиество шагов редуктора сделанных против часовой стрелке
int stepsCWSpeed = 1000;
int stepsCCWSpeed = 1000;
float CCWSpeedTest = 0;

// переменные шаговика цилиндра страчателлы
byte stepFlagST = 0;
int stepsDoneST = 0;
int stepsForST = 0;
int currentSTStep = 0;
byte STInMotionFlag = 0;
byte updateST = 0;  // ???

// переменные шаговика цилиндра сливок
byte stepFlagCR = 0;
int stepsDoneCR = 0;
int stepsForCR = 0;
int currentCRStep = 0;
byte CRInMotionFlag = 0;
byte updateCR = 0;  // ???

// переменные ПИД
float Int, tempLeft, tempRight, error, oldError, integral, diff, p = 700, ti = 8, td = 200;
float setpoint = 50;
int dimmer, out, test;

// переменные калибровки
byte calibrDone = 0;       // флаг калибровки
byte makeCalibration = 0;  // флаг начала калибровки
byte calibrationCase = 0;  // флаг места калибровки

// переменные для ДВИН
byte mainButton = 0;    // состояние главной кнопки
byte digitalPedal = 1;  // состояние

byte straciatellaButtonFlag = 0;  // кнопка "страчателлы"для
byte straciatellaButtonCase = 0;  // переменная функции "страчателла"

byte burrataButtonFlag = 0;  // кнопка "буррата"
byte burrataButtonCase = 0;  // переменная функции "буррата"

byte rotateDiskButtonFlag = 0;  // кнопка "оборот"
byte rotateDiskButtonCase = 0;  // переменная функции "оборот"

byte heaterIsOn = 0;    // флаг включенных тэнов
byte knifeIsOn = 0;     // флаг включенного ножа
byte pressIsOn = 0;     // флаг включенных прессов
byte rotationIsON = 0;  // флаг включенного основного шаговика
byte testFlag = 0;      // тестовый флаг

byte loadStrach = 10;       // количество циклов загрузки страчателлы
byte loadCream = 10;        // количество циклов загрузки сливок
byte razgruzkaStrach = 10;  // количество циклов разргузки страчателлы
byte razgruzkaSlivki = 10;  // количество циклов разгрузки сливок
byte moikaCycli = 10;       // количество циклов мойки
byte err = 0;
float tempToSend;                    // вспомогательная переменная для проверки температуры
float overHeat = 50.0;               // температура отключения ТЭНов
float userTemp = 30.0;               // температура пайки
char userTempCharArray[] = "0000";   // массив для перевода температуры из ascii в float
char leftTempCharArray[] = "0000";   // массив для перевода левого ТЭНа из float в ascii
char rightTempCharArray[] = "0000";  // массив для перевода правого ТЭНа из float в ascii
float time;                          // время пайки
char userTimeCharArray[] = "0000";   // массив для перевода времени пайки из ascii в float
int massa = 50;                      // страчателла в мл
int STmassa;
int CRmassa;
int maxMassa = 300;  // максимальное значение массы (без пропорции)
int cream = 10;      // сливки в %
int rotation = 2;    // количество оборотов бурраты


//переменные калибровки
byte startCalibrFlag = 0;
byte calibrVolume = 0;
byte calibrDisk = 0;
byte calibrPress = 0;
byte calibrKnife = 0;
byte calibrRazgruz = 0;
byte calibrHeaters = 0;


int revol = 1200;
uint32_t stepTimerRE = 0;
uint32_t stepTimerST = 0;
uint32_t stepTimerCR = 0;

uint32_t testCylinderTimer = 0;
byte lastPageID = 0x00;
byte lastSetPageID = 0x9A;  // навигация страниц двин


// буффер чтения из DWIN
const byte BUFFER_SIZE = 12;
byte inputBuf[BUFFER_SIZE];
byte readPosition = 0;
byte inputBufPosition = 0;
byte dwinMessageLength = 0;
bool inputMessageComplete = false;

// буфферы передачи в DWIN
//буфферы передачи параметров
byte outputUserTempBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X04, 0X0, 0x0, 0x00, 0x00 };   // буффер отправки температуры на экран
byte outputTimeBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X0B, 0X0, 0x0, 0x00, 0x00 };       // буффер отправки времени прессовки на экран
byte outputRevolBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X10, 0X0, 0x0 };                  // буффер отправки количества оборот на экран
byte outputMassaBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X12, 0X0, 0x0 };                  // буффер отправки объёма на экран
byte outputCreamBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X14, 0X0, 0x0 };                  // буффер отправки %сливок на экран
byte outputLeftTempBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X16, 0X0, 0x0, 0x00, 0x00 };   // буффер отправки температуры левого ТЭНА на экран
byte outputRightTempBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X1A, 0X0, 0x0, 0x00, 0x00 };  // буффер отправки температуры правого ТЭНА на экран
byte cylindersBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X25, 0X00, 0X0, 0x0 };                    // буффер отправки изменённого состояния цилиндра
byte wrongPassBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X12, 0X05, 0X0, 0x0 };                    // буффер отправки сообщения о вводе неправильного пароля
byte outputLoadBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X20, 0X08, 0X0, 0x0 };                   // буффер отправки параметров загрузки страчателлы
byte showErrorBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X00, 0X0, 0x0 };                    // буффер отправки ошибки
byte ssStateBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X26, 0X00, 0X0, 0x0 };                      // буффер отправки состояния датчика цилиндра

//буфферы кнопок
byte mainButtonBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X02, 0X0, 0x0 };           // буффер отправки значения кнопки вкл/выкл
byte strachiatellaButtonBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X11, 0X0, 0x0 };  // буффер отправки значения кнопки страчателлы
byte burrataButtonBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X13, 0X0, 0x0 };        // буффер отправки значения кнопки страчателлы
byte rotateDiskButtonBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X15, 0X0, 0x0 };     // буффер отправки значения кнопки страчателлы

byte popUpCalibrPageBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X20, 0X11, 0x20 };  // буффер отправки вызова попап меню с калибровкой
byte calibrBuff[] = { 0X5A, 0XA5, 0X05, 0X82, 0X13, 0X00, 0X00, 0x00 };         // буффер отправки состояния кнопок при калибровке
byte coverSensorErrorBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X12, 0X06, 0X0, 0x0 };   // буффер отправки сообщения об открытой крышке

byte changeMainPageBuff[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x01 };   // буффер перехода на другую страницу
byte changeSetPageBuff[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x01, 0x00 };    // буффер перехода на другую страницу
byte settingsPageAddrBuf[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x3F };  // !!!!!!!! либо 0x3f либо 0x09
byte settingsPageErrorAddrBuf[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x62 };

//буфферы кнопок калибровок
byte calibrButtonsBuf[] = { 0X5A, 0XA5, 0X05, 0X82, 0X21, 0X00, 0X00, 0x00 };         // буффер отправки состояния кнопок при калибровке


byte switchBuff[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X00, 0X0, 0x0 };




void setup() {
  pinMode(SOLENOID_SWITCH1, OUTPUT);
  pinMode(SOLENOID_SWITCH2, OUTPUT);
  pinMode(SOLENOID_SWITCH3, OUTPUT);
  pinMode(SOLENOID_SWITCH4, OUTPUT);
  pinMode(SOLENOID_SWITCH5, OUTPUT);
  pinMode(SOLENOID_SWITCH6, OUTPUT);
  pinMode(SOLENOID_SWITCH7, OUTPUT);
  pinMode(SOLENOID_SWITCH8, OUTPUT);
  pinMode(SOLENOID_SWITCH9, OUTPUT);
  pinMode(SOLENOID_SWITCH10, OUTPUT);
  pinMode(SOLENOID_SWITCH11, OUTPUT);
  pinMode(SOLENOID_SWITCH12, OUTPUT);
  pinMode(SOLENOID_SENSOR1, INPUT);
  pinMode(SOLENOID_SENSOR2, INPUT);
  pinMode(SOLENOID_SENSOR3, INPUT);
  pinMode(SOLENOID_SENSOR4, INPUT);
  pinMode(SOLENOID_SENSOR5, INPUT);
  pinMode(SOLENOID_SENSOR6, INPUT);
  pinMode(SOLENOID_SENSOR7, INPUT);
  pinMode(SOLENOID_SENSOR8, INPUT);
  pinMode(SOLENOID_SENSOR9, INPUT);
  pinMode(SOLENOID_SENSOR10, INPUT);
  pinMode(SOLENOID_SENSOR11, INPUT);
  pinMode(SOLENOID_SENSOR12, INPUT);
  pinMode(SOLENOID_SENSOR13, INPUT);
  pinMode(SOLENOID_SENSOR14, INPUT);
  pinMode(COVER_SENSOR, INPUT);
  pinMode(TURN_SENSOR, INPUT);
  pinMode(PRESSURE_METER, INPUT);
  pinMode(DWIN_POWER, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(REDUCER_ENABLE, OUTPUT);
  pinMode(DOSE_ENABLE, OUTPUT);
  pinMode(REDUCER_DIR, OUTPUT);
  pinMode(STRACCIATELLA_DIR, OUTPUT);
  pinMode(CREAM_DIR, OUTPUT);
  pinMode(REDUCER_STEP, OUTPUT);
  pinMode(STRACCIATELLA_STEP, OUTPUT);
  pinMode(CREAM_STEP, OUTPUT);
  pinMode(DIMMER_L, OUTPUT);
  pinMode(DIMMER_R, OUTPUT);
  pinMode(ZERO, INPUT);
  pinMode(PEDAL, INPUT);
  pinMode(STOP, INPUT);
  pinMode(OPTIONAL_BUTTON1, INPUT);
  pinMode(OPTIONAL_BUTTON2, INPUT);
  pinMode(OPTIONAL_BUTTON3, INPUT);
  pinMode(ILLUMINATION_OPTION1, OUTPUT);
  pinMode(ILLUMINATION_OPTION2, OUTPUT);
  pinMode(ILLUMINATION_OPTION3, OUTPUT);

  // моторы дозации в режим ожидания (включен enable)
  digitalWrite(DOSE_ENABLE, HIGH);

  // прерывания детектора нуля
  attachInterrupt(1, isr, RISING);
  Timer2.enableISR();

  // RXTX
  Serial.begin(115200);

  // RS232
  Serial1.begin(115200);

  sensorLeft.begin();
  sensorLeft.setWaitForConversion(false);

  digitalWrite(DWIN_POWER, HIGH);

  // переход на страницу домой
  byte changeFirstPage[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x1E };  //01 - страница калибровки/1Е - основная
  delay(2000);
  Serial1.write(changeFirstPage, 10);

  /*while(calibrDone != 1){
    //ждём нажатие пользователем кнопки "старт"
    readDwin();
  }
  */
  //загрузка параметров
  setDwin();
  /*
  // выставление стартовой позиции
   while (defaultPositionsFlag == 0) {
     defaultPositions();
   }
  */
}

/*void defaultPositions() {
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
*/

void isr() {
  static int lastDim;
  digitalWrite(DIMMER_L, LOW);
  digitalWrite(DIMMER_R, LOW);
  if (lastDim != dimmer) {
    Timer2.setPeriod(lastDim = dimmer);
  } else {
    Timer2.restart();
  }
}

ISR(TIMER2_A) {
  if (heaterIsOn && tempLeft < overHeat && tempRight < overHeat) {
    digitalWrite(DIMMER_L, HIGH);
    digitalWrite(DIMMER_R, HIGH);
    Timer2.stop();
  }
}

void pid() {
  oldError = error;
  error = userTemp - tempLeft;
  Int = Int + (oldError + error) / 2;
  integral = constrain(Int * (1 / ti), 0, 9300);
  if (integral == 9300) {
    Int = integral;
  }
  diff = (error - oldError) * td;
  out = constrain(p * error + integral + diff, 500, 9300);
}

void getTemp() {
  sensorLeft.requestTemperatures();
  sensorRight.requestTemperatures();

  tempLeft = sensorLeft.getTempCByIndex(0);
  tempRight = sensorRight.getTempCByIndex(0);
}

byte pedalStatus() {
  if (digitalRead(PEDAL) == LOW || digitalPedal == LOW) {
    return 0;
  }
  return 1;
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

// проверка датчиков тока
void checkHeaters() {
  byte state = 0;

  digitalWrite(DIMMER_L, HIGH);
  leftHeaterCurrent = ACS712_L.mA_AC(50, 1);
  digitalWrite(DIMMER_L, LOW);

  digitalWrite(DIMMER_R, HIGH);
  rightHeaterCurrent = ACS712_R.mA_AC(50, 1);
  digitalWrite(DIMMER_R, LOW);

  Serial.print("Left heater: ");
  Serial.println(leftHeaterCurrent);
  Serial.print("Right heater: ");
  Serial.println(rightHeaterCurrent);


  //проверяем тэны
  if (leftHeaterCurrent < 200.0) {
    state += 1;
  }
  if (rightHeaterCurrent < 200.0) {
    state += 2;
  }

  Serial.print("state: ");
  Serial.println(state);

  //обработка проверки работы тэнов

  switch (state) {
    case 0:  //ТЭНы ок

      break;
    case 1:  //не работает левый
      Serial.println("left not working");
      showError();
      break;
    case 2:  //не работает правый
      Serial.println("right not working");
      showError();
      break;
    case 3:  //не работают оба
      Serial.println("both not working");
      showError();
      break;
    default:
      showError();
      Serial.println("heaters state program error");
      break;
  }

  state = 0;
}
// проверка датчика закрытой крышки
void checkCoverSensor() {
}
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
          stepsCCW = checkStepFlag;       //замеряем количество шагов до срабатывания датчика

          if (DEBUG) {
            Serial.print("CCW steps: ");
            Serial.print(checkStepFlag);
          }

          checkStepFlag = 0;
          calibrationCase++;

        } else {  // датчик оборота не сработал
          makeRE_CCWR();
          checkStepFlag++;
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
          stepsCW = checkStepFlag;  //замеряем количество шагов до срабатывания датчика

          if (DEBUG) {
            Serial.print("CW steps: ");
            Serial.print(checkStepFlag);
          }

          calibrBuff[5] = 1;             //адрес иконки датчика TS
          calibrBuff[7] = 1;             //индикатор CS зелёный
          Serial1.write(calibrBuff, 8);  //отправка значния индикатора

          checkStepFlag = 0;
          calibrationCase++;
        } else {
          makeRE_CWR();
          checkStepFlag++;
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

void displaySolenoidSensorLight(byte solenoid) {
  /* switch (solenoid){
    case(0x00): // датчики левого пресса
        if(ssState1 != digitalRead(SOLENOID_SENSOR1)){
          ssState1 = !ssState1;
          ssStateBuf[5] = 0x00;
          ssStateBuf[7] = ssState1;
          Serial1.write(ssStateBuf, 8);
        };
        if(ssState2 != digitalRead(SOLENOID_SENSOR2)){
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

  if (ssState1 != digitalRead(SOLENOID_SENSOR1)) {
    ssStateBuf[5] = 0x00;
    ssStateBuf[7] = ssState1;
    Serial1.write(ssStateBuf, 8);
    ssState1 = !ssState1;
    //changeSSLight = 0;
  }

  if (ssState2 != digitalRead(SOLENOID_SENSOR2)) {
    ssStateBuf[5] = 0x01;
    ssStateBuf[7] = ssState2;
    Serial1.write(ssStateBuf, 8);
    ssState2 = !ssState2;
    //changeSSLight = 0;
  }
}

void straciatellaButton() {  // нажата кнопка включения страчателлы

  if (straciatellaButtonFlag == 1) {  //значение кнопки 1

    switch (straciatellaButtonCase) {
      case 0:  // отправляем информацию на экран
        //mainButton = 0;                       // отключаем основную кнопку
        // меняем состояние кнопки на вкл;
        strachiatellaButtonBuf[7] = 1;
        Serial1.write(strachiatellaButtonBuf, 8);
        straciatellaButtonCase++;  // переходим в следующий кейс
        digitalWrite(DOSE_ENABLE, LOW);
        break;

      case 1:  // перемещаем подвижные датчики страчателлы и сливок до их штоков цилиндров в верхнем положении
        if (digitalRead(SOLENOID_SENSOR6) == HIGH) {
          if (micros() - stepTimerST >= 300) {
            stepTimerST = micros();
            makeST_CWR();  // вращаем мотор страчателлы по часовой
          }
        }
        if (digitalRead(SOLENOID_SENSOR4) == HIGH) {
          if (micros() - stepTimerCR >= 300) {
            stepTimerCR = micros();
            makeCR_CWR();  // вращаем мотор сливок по часовой
          }
        }
        if (digitalRead(SOLENOID_SENSOR4) == LOW && digitalRead(SOLENOID_SENSOR6) == LOW) {
          straciatellaButtonCase++;  // переходим в следующий кейс
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
        straciatellaButtonCase++;  // переходим в следующий кейс
        break;

      case 3:  // отправвляем информацию на экран

        straciatellaButtonFlag = 0;  // изменение состояния кнопки "страчателла" на выкл
        strachiatellaButtonBuf[7] = straciatellaButtonFlag;
        Serial1.write(strachiatellaButtonBuf, 8);
        straciatellaButtonCase = 0;  // обнуляем кейс
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
    }
  }
  if (straciatellaButtonFlag == 0) {  //значение кнопки 0
    straciatellaButtonFlag = 0;       //изменение состояния кнопки "страчателла" на выкл
    straciatellaButtonCase = 0;

    strachiatellaButtonBuf[7] = 0;
    Serial1.write(strachiatellaButtonBuf, 8);
  }
}

void burrataButton() {
  switch (burrataButtonCase) {
    case 0:
      //mainButton = 0;             // отключаем основную кнопку
      //straciatellaButtonFlag = 0;      // не реагируем на нажатие кнопки "буррата"
      //rotateDiskButtonFlag = 0;   // не реагируем на нажатие кнопки "оборот"

      // меняем состояние кнопки на вкл;
      burrataButtonBuf[7] = 1;
      Serial1.write(burrataButtonBuf, 8);

      burrataButtonCase++;
      break;

    case 1:
      delay(500);
      //изменение состояния кнопки "страчателла" на выкл
      burrataButtonBuf[7] = 1;
      Serial1.write(burrataButtonBuf, 8);

      //обнуляем состояние по выполнению всей функции
      burrataButtonFlag = 0;
      burrataButtonCase = 0;
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

void rotateDiskButton() {

  switch (rotateDiskButtonCase) {
    case 0:
      //mainButton = 0;  // отключаем основную кнопку

      // меняем состояние кнопки на вкл;
      rotateDiskButtonBuf[7] = 1;
      Serial1.write(rotateDiskButtonBuf, 8);

      break;

    case 1: //едем, пока не сработан датчик оборота
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
      /*      if(DEBUG){
        Serial.print("rotateDiskButton case: ");
        Serial.println(rotateDiskButtonCase);
        Serial.print("stepFlagRE ");
        Serial.println(stepFlagRE);
      }
      */
      break;

    case 2: //идём 500 шагов против часовой 
      if (checkStepFlag < 500) {
        if (micros() - stepTimerRE >= stepsCCWSpeed) {
          stepTimerRE = micros();
          makeRE_CCWR();
          checkStepFlag++;
        }
      } else {
        rotateDiskButtonCase++;
        checkStepFlag = 0;
      }
      break;

    case 3: //замедляемся перед оснатовкой
      if (checkStepFlag < 200) {
        if (micros() - stepTimerRE >= stepsCCWSpeed) {
          stepTimerRE = micros();
          makeRE_CCWR();
          checkStepFlag++;

          CCWSpeedTest = stepsCCWSpeed * 1.005;
          stepsCCWSpeed = (int)CCWSpeedTest;
        }
      } else {
        rotateDiskButtonCase++;
        Serial.println(stepsCCWSpeed);

        checkStepFlag = 0;
      }
      break;

    case 4: //едем на минимуме, пока не сработает датчик оборота
      if (digitalRead(TURN_SENSOR)) {
        if (micros() - stepTimerRE >= stepsCCWSpeed) {
          stepsCCWSpeed += 7;
          stepTimerRE = micros();
          makeRE_CCWR();
          stepsCCW++;
        }
      } else {
        rotateDiskButtonCase++;
        Serial.println(stepsCCW);
      }
      break;
    
    case 5:  //опускам пресс-форсунку и поднимаем редуктор
      digitalWrite(SOLENOID_SWITCH1, HIGH);   //поднимаем редуктор
      digitalWrite(SOLENOID_SWITCH1, HIGH);   //опускем чашку
      
      if(digitalRead(SOLENOID_SENSOR1 == 1) && digitalRead(SOLENOID_SENSOR7 == 1)){ //если редуктор, голова опущена поднят - идём дальше
        rotateDiskButtonCase++;
      };
      break;

    case 6:   //опускаем цилиндры страчателлы и сливок
      digitalWrite(SOLENOID_SWITCH2, LOW); //опускаем цилиндр страчателлы
      digitalWrite(SOLENOID_SWITCH3, LOW); //опускаем цилиндр сливок
      rotateDiskButtonCase++;
      break;
      
    case 7:  //дозируем
      if (digitalRead(SOLENOID_SENSOR4) == LOW && sensorFlag[1] == 0) {
        digitalWrite(SOLENOID_SWITCH2, HIGH);
        sensorFlag[1] = 1;
      }
      if (digitalRead(SOLENOID_SENSOR6) == LOW && sensorFlag[2] == 0) {
        digitalWrite(SOLENOID_SWITCH3, HIGH);
        sensorFlag[2] = 1;
      }
      if (sensorFlag[1] == 1 && sensorFlag[2] == 1) {
        sensorFlag[1] = 0;
        sensorFlag[2] = 0;
        rotateDiskButtonCase++;
      }
      break;

    case 8:   //опускаем чашку
      digitalWrite(SOLENOID_SWITCH4, LOW);
      stepTimerRE = micros();
      rotateDiskButtonCase++;
      break;

    case 9:   //крутим чашку
      if (digitalRead(SOLENOID_SENSOR7)) {
        if (micros() - stepTimerRE >= 400) {
          stepTimerRE = micros();
          makeRE_CWR();
          checkStepFlag++;
        }
      } else {
        rotateDiskButtonCase++;
        checkStepFlag = 0;
      }
      break;

    case 10:
      //delay(500);
      //изменение состояния кнопки "страчателла" на выкл
      //rotationDone = 0;
      rotateDiskButtonFlag = 0;
      rotateDiskButtonBuf[7] = rotateDiskButtonFlag;
      Serial1.write(rotateDiskButtonBuf, 8);

      stepsCCWSpeed = 1250;
      stepsCCW = 0;

      //обнуляем состояние по выполнению всей функции
      rotateDiskButtonCase = 0;
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

void calibrate(){
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

  if(calibrVolume){

  }

  if(calibrDisk){
    rotateDiskButton();
  }

  if(calibrPress){

  }

  if(calibrKnife){

  }

  if(calibrRazgruz){

  }

  if(calibrHeaters){

  }
}

void readDwin() {
  // читаем serial1 пока не получим полное сообщение
  if (Serial1.available() && inputMessageComplete == false) {
    readInput();
  }

  // если полное сообщение пришло, то парсим его
  if (inputMessageComplete) {
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
    if (tempToSend != tempLeft) {
      tempToSend = tempLeft;
      sendLeftTemp();
    }

    /*if(DEBUG){
      Serial.print("tempLeft = ");
      Serial.println(tempLeft);
    }
    */
  }

  // работа с ПИД
  if (millis() - pidTimer >= 40) {
    pidTimer = millis();
    pid();
    dimmer = map(out, 500, 9300, 9300, 500);
  }

  /*if (makeCalibration) {
    calibration();
  }*/

  // включена главная кнопка
  if (mainButton) {
    // нажали педаль
    if (pedalStatus() == LOW && pedalFlag == 0) {
      pedalFlag = 1;
    }
  }

  // включена кнопка "страчателла"
  if (straciatellaButtonFlag) {
    straciatellaButton();
  }

  // включена кнопка "буррата"
  if (burrataButtonFlag) {
    burrataButton();
  }

  // включена кнопка "оборот"
  if (rotateDiskButtonFlag) {
    rotateDiskButton();
  }

  // перемещаем датчик страчателлы в нужную координату
  if (STInMotionFlag == 1 && straciatellaButtonFlag == 0) {
    calcMassaST();
  }

  // перемещаем датчик сливок в нужную координату
  if (CRInMotionFlag == 1 && straciatellaButtonFlag == 0) {
    calcMassaCR();
  }

  // проверка наличия сообщения на экране
  readDwin();

  // ============= тест шаговика ==================
  if (rotationIsON && micros() - stepTimerRE >= 150) {
    stepTimerRE = micros();
    makeStep();
  }

  if (changeSSLight) {
    displaySolenoidSensorLight(solenoidNum);
  }

  // ============= калибровка =====================
  if(startCalibrFlag){
    calibrate();
  }
}


/*TODO:
  2. float checkStepFlag = 0; - изменить название на checkStepREFlag - проверить
  3.  проверить нужны ли:
      float stepsCW = 0;       // колиество шагов редуктора сделанных по часовой стрелке
      float stepsCCW = 0;      // колиество шагов редуктора сделанных против часовой стрелке
      int stepsCWSpeed = 1000;  
      int stepsCCWSpeed = 1000;
      float CCWSpeedTest = 0;
*/