// Atmega 2560

byte DEBUG = 1;

// SOLENOIDS
#define SOLENOID_SWITCH1 35   // голова сливок
#define SOLENOID_SWITCH2 34   // голова страчателлы
#define SOLENOID_SWITCH3 33   // подставка
#define SOLENOID_SWITCH4 32   // форсунка пресс-форсунки
#define SOLENOID_SWITCH5 31   // пресс-форсунка
#define SOLENOID_SWITCH6 30   // цилиндр страчателлы
#define SOLENOID_SWITCH7 39   // цилиндр сливок
#define SOLENOID_SWITCH8 29   // правый пресс
#define SOLENOID_SWITCH9 28   // левый пресс
#define SOLENOID_SWITCH10 27  // редуктор
#define SOLENOID_SWITCH11 26  // разгрузка
#define SOLENOID_SWITCH12 25  // нож

// SENSORS
#define SOLENOID_SENSOR1 A0   //левый пресс от центра 
#define SOLENOID_SENSOR2 A1   //левый пресс в ценитре 
#define SOLENOID_SENSOR3 A2   //пресс-форсунка низ
#define SOLENOID_SENSOR4 A3   //правый пресс от центра 
#define SOLENOID_SENSOR5 A4   //правый пресс в центре 
#define SOLENOID_SENSOR6 A5   //разгрузка низ 
#define SOLENOID_SENSOR7 A6   //нож от центра 
#define SOLENOID_SENSOR8 A7   //нож центра 
#define SOLENOID_SENSOR9 A8   //подвижный датчик сливок
#define SOLENOID_SENSOR10 A9  //датчик редуктора верх 
#define SOLENOID_SENSOR11 A10 //верхний сливки
#define SOLENOID_SENSOR12 A11 //датчик редуктора низ
#define SOLENOID_SENSOR13 A12 //верхний датчик страчателлы
#define SOLENOID_SENSOR14 A13 //подвижный датчик страчателла 

#define COVER_SENSOR 43       //датчик защитной крышки
#define TURN_SENSOR 42        //датчик круга
#define PRESSURE_METER 21     //датчик давления
#define DS18B20_L 9           //датчик температуры левый
#define DS18B20_R 8           //датчик температуры правый
#define CURRENT_SENSOR_L A15  //датчик тока левый
#define CURRENT_SENSOR_R A14  //датчик тока правый

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
#include <EEPROM.h>

ACS712 ACS712_L(CURRENT_SENSOR_L, 5.0, 1023, 185);  //инициализация левого датчика тока, дискретность 185мА
ACS712 ACS712_R(CURRENT_SENSOR_R, 5.0, 1023, 185);  //инициализация правого датчика тока, дискретность 185мА

OneWire oneWireLeft(DS18B20_L);
DallasTemperature sensorLeft(&oneWireLeft);

OneWire oneWireRight(DS18B20_R);
DallasTemperature sensorRight(&oneWireRight);

// ***** адреса переменных в EEPROM ***** //
#define EEPROMUserTempAddr     =  0;    //float
#define EEPROMTempAddr         =  4;    //float
#define EEPROMMassaAddr        =  8;    //int  
#define EEPROMCreamAddr        =  10;   //int
#define EEPROMRotationAddr     =  12;   //int
#define EEPROMWashCycleSt1     =  14;   //int
#define EEPROMWashCycleCr1     =  16;   //int
#define EEPROMWashCycleSt2     =  18;   //int
#define EEPROMWashCycleCr1     =  20;   //int
#define EEPROMWashCycleSt3     =  22;   //int
#define EEPROMWashCycleCr1     =  24;   //int

// переменные датчиков тока (mA)
float leftHeaterCurrent;
float rightHeaterCurrent;

// переменные кнопок
bool OptionalButton1State = 0;
byte OptionalButton2State = 0;
uint32_t OptionalButton1Timer;

// переменные для подсветки датчиков SS на экране
bool ssState1 = 0;
bool ssState2 = 0;
bool ssState3 = 0;
bool ssState4 = 0;
bool ssState5 = 0;
bool ssState6 = 0;
bool ssState7 = 0;
bool ssState8 = 0;
bool ssState9 = 0;
bool ssState10 = 0;
bool ssState11 = 0;
bool ssState12 = 0;
bool ssState13 = 0;
bool ssState14 = 0;

//буффер состояния соленоидов/цилиндров
byte cylinderState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //0, SSwitch1, SSwitch2, SSwitch3, ...

// переменные таймеров
uint32_t tempTimer, pidTimer, stepTimer, pushTimer, pressTimer, testTimer, cycleIconTimer, knifeTimer1, knifeTimer2, cycleTimer, previousCycleTimer;

// переменные флагов
byte coverSensorState = 0;        // флаг состояние датчика защитного экрана
//ПРОВЕРИТЬ необходимость!
byte checkSolenoidSensorsFlag = 0;    // флаг включения цикла проверки датчиков соленоидов
byte changeSSLight = 0;           // цилиндр выстрелил, нужно проверить сенсоры
byte solenoidNum = 0;             // номер цилиндра для проверки сенсоров
byte defaultFlag = 0;             // флаг выставления стартовой позиции
byte defaultPositionsFlag = 0;    // флаг запуска выставления стартовой позиции
byte burrataFlag = 0;             // флаг основного цикла изготовления бурраты
byte sensorFlag[] = { 0, 0, 0 };  // флаги дозации
byte pedalFlag = 0;               // флаг нажатия педали

//переменные шаговиков
byte stepFlag = 0;                // тестовый степ флаг

int revol = 1200;
uint32_t stepTimerRE = 0;
uint32_t stepTimerST = 0;
uint32_t stepTimerCR = 0;

// переменные шаговика RE - круга
byte stepFlagRE = 0;  // степ флаг диска
float checkStepFlagRE = 0;
int stepsCWSpeed = 4000;   // скорость шаговика по часовой
int stepsCCWSpeed = 150;  // скорость шаговика против часовой
int stepsRE = 0;
float RESpeedTest = 0;
    //настройки на странице "настройки"
byte motorREON = 0;
int motorRESpeed = 1000;
byte motorREDir = 0;

// переменные шаговика ST - цилиндра страчателлы
byte stepFlagST = 0;
int stepsDoneST = 0;
int stepsForST = 0;
int currentSTStep = 0;
byte STInMotionFlag = 0;

// переменные шаговика CR - цилиндра сливок
byte stepFlagCR = 0;
int stepsDoneCR = 0;
int stepsForCR = 0;
int currentCRStep = 0;
byte CRInMotionFlag = 0;

// переменные ПИД
float IntL, tempLeft, errorL, oldErrorL, integralL, diffL, p = 700, ti = 8, td = 200;
float IntR, tempRight, errorR, oldErrorR, integralR, diffR;

float setpoint = 50;
int dimmer, dimmerL, dimmerR, outL, outR, test;

// переменные калибровки
byte calibrDone = 0;       // флаг калибровки
byte makeCalibration = 0;  // флаг начала калибровки
byte calibrationCase = 0;  // флаг места калибровки

// переменные для ДВИН
byte mainButton = 0;    // состояние главной кнопки
byte digitalPedal = 1;  // состояние

byte stateCS = 1;       //состояние датчика крышки
byte statePS = 0;       //состояние датчика давления
byte stateLH = 0;       //состояние датчика левого ТЭНа
byte stateRH = 0;       //состояние датчика правого ТЭНа

byte straciatellaButtonFlag = 0;  // флаг кнопки "масса"
byte straciatellaButtonCase = 0;  // кейс функции "масса"

byte burrataButtonFlag = 0;       // флаг кнопки "буррата"
byte burrataButtonCase = 0;       // кейс функции "буррата"

byte rotateDiskButtonFlag = 0;    // флаг кнопки "шаг"
byte rotateDiskButtonCase = 0;    // кейс функции "шаг"

byte heaterIsOn = 0;              // флаг включенных тэнов
byte knifeIsOn = 0;               // флаг включенного ножа
byte pressIsOn = 0;               // флаг включенных прессов
byte rotationIsON = 0;            // флаг включенного основного шаговика
byte testFlag = 0;                // тестовый флаг

// ***** переменные для вспомогательного меню мойки *****
byte doCycle = 0;                 // флаг начала цикла для основного лупа
byte numCycle = 0;                // номер цикла

byte washCycleSt1 = 10;           // циклов загрузки страчателлы
byte washCycleCr1 = 10;           // циклов загрузки сливок
//?!?!byte washCycle1Start = 0;         // флаг загрузки
byte cycle1Case = 0;              // кейс для функции cycle1

byte washCycleSt2 = 10;           // циклов разргузки страчателлы
byte washCycleCr2 = 10;           // циклов разгрузки сливок
//?!?!byte washCycle2Start = 0;         // флаг разгрузки
byte cycle2Case = 0;              // кейс для функции cycle2

byte washCycleSt3 = 10;           // количество циклов мойки
byte washCycleCr3 = 10;           // количество циклов мойки
//?!?!byte washCycle3Start = 0;         
byte cycle3Case = 0;              // кейс для функции cycle3


byte err = 0;
float tempToSend;  // вспомогательная переменная для проверки температуры
float tempToSendL;
float tempToSendR;
float overHeat = 50.0;               // температура отключения ТЭНов

float userTemp = 30.0;               // температура пайки

char userTempCharArray[] = "0000";   // массив для перевода температуры из ascii в float
char leftTempCharArray[] = "0000";   // массив для перевода левого ТЭНа из float в ascii
char rightTempCharArray[] = "0000";  // массив для перевода правого ТЭНа из float в ascii
float time = 2.5;                          // время пайки
char userTimeCharArray[] = "0000";   // массив для перевода времени пайки из ascii в float

int massa = 95;  // страчателла в мл
int STmassa;
int maxMassa = 225;  // максимальное значение массы (без пропорции)

int cream = 10;  // сливки в %
int CRmassa;
int maxCream = 40;  // максимальное значение сливок

int rotation = 2;  // количество оборотов бурраты


//переменные калибровки
byte startCalibrFlag = 0;
byte calibrateVolumeFlag = 0;
byte calibrateVolumeCase = 0;
byte calibrDisk = 0;
byte calibrPosition = 0;
byte calibrHeaters = 0;


uint32_t testCylinderTimer = 0;

// навигации страниц для двин
byte lastPageID = 0x00;
byte lastSetPageID = 0x10;  // навигация страниц двин


// буффер чтения из DWIN
const byte BUFFER_SIZE = 12;
byte inputBuf[BUFFER_SIZE];
byte readPosition = 0;
byte inputBufPosition = 0;
byte dwinMessageLength = 0;
bool inputMessageComplete = false;

// буфферы передачи в DWIN
//буфферы передачи температуры
byte outputUserTempBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X04, 0X0, 0x0, 0x00, 0x00 };   // буффер отправки температуры на экран
//буфферы передачи времени пайки
byte outputTimeBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X0C, 0X0, 0x0, 0x00, 0x00 };       // буффер отправки времени прессовки на экран
//буфферы передачи оборотов
byte outputRevolBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X10, 0X0, 0x0 };                  // буффер отправки количества оборот на экран
//буфферы передачи объёма продукта
byte outputMassaBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X12, 0X0, 0x0 };                  // буффер отправки объёма на экран
//буфферы передачи процентов сливок
byte outputCreamBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X14, 0X0, 0x0 };                  // буффер отправки %сливок на экран
//буфферы передачи температуры левого тэна
byte outputLeftTempBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X16, 0X0, 0x0, 0x00, 0x00 };   // буффер отправки температуры левого ТЭНА на экран
//буфферы передачи температуры правого тэна
byte outputRightTempBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X1A, 0X0, 0x0, 0x00, 0x00 };  // буффер отправки температуры правого ТЭНА на экран
//буффер отправки скорости мотора RE на экран настроек
byte outputMotorRESpeed[] = { 0X5A, 0XA5, 0X5, 0X82, 0X27, 0X02, 0X0, 0x0};               // буффер отправки скорости мотора RE
//буфферы передачи кнопок цилиндров
byte cylindersBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X25, 0X00, 0X0, 0x0 };                    // буффер отправки изменённого состояния цилиндра
//буфферы передачи значения иконок цилиндров
byte cylinderIconBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X24, 0X00, 0X0, 0x0 };                 // буффер отправки иконки состояния цилиндра
//буфферы включения иконки с неправильным паролем
byte wrongPassBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X12, 0X05, 0X0, 0x0 };                    // буффер отправки сообщения о вводе неправильного пароля

//!!!!!?!?!?!? найти что это
byte outputLoadBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X20, 0X08, 0X0, 0x0 };                   // буффер отправки параметров загрузки страчателлы
//буфферы передачи количества циклов страчателлы на 1 экране
byte washCycleSt1Buf [] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X20, 0X0, 0x0 };
//буфферы передачи количества циклов сливок на 1 экране
byte washCycleCr1Buf [] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X22, 0X0, 0x0 };
//буфферы передачи количества циклов страчателлы на 2 экране
byte washCycleSt2Buf [] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X24, 0X0, 0x0 };
//буфферы передачи количества циклов сливок на 2 экране
byte washCycleCr2Buf [] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X26, 0X0, 0x0 };
//буфферы передачи количества циклов страчателлы на 3 экране
byte washCycleSt3Buf [] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X28, 0X0, 0x0 };
//буфферы передачи количества циклов сливок на 3 экране
byte washCycleCr3Buf [] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X2A, 0X0, 0x0 };

//буффер анимированной иконки циклов
byte  cycleIconLeftBuff [] = { 0X5A, 0XA5, 0X5, 0X82, 0X13, 0X14, 0X0, 0x0 };
byte  cycleIconRightBuff [] = { 0X5A, 0XA5, 0X5, 0X82, 0X13, 0X15, 0X0, 0x0 };

//буффер вывода сообщения о ошибке в пароле
byte showErrorBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X00, 0X0, 0x0 };                    // буффер отправки ошибки
//буффер иконок датчиков цилиндров 
byte ssStateBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X26, 0X00, 0X0, 0x0 };                      // буффер отправки состояния датчика цилиндра

//буфферы значения иконок датчиков крышки/давления/тока
byte stateCSBuff[] = { 0X5A, 0XA5, 0X5, 0X82, 0X14, 0X00, 0X0, 0x0 };                      // буффер состояния датчика защитной крышки CS
byte statePSBuff[] = { 0X5A, 0XA5, 0X5, 0X82, 0X14, 0X01, 0X0, 0x0 };                      // буффер состояния датчика датчика давления PS
byte stateLHBuff[] = { 0X5A, 0XA5, 0X5, 0X82, 0X14, 0X02, 0X0, 0x0 };                      // буффер состояния датчика левого ТЭНА LH
byte stateRHBuff[] = { 0X5A, 0XA5, 0X5, 0X82, 0X14, 0X03, 0X0, 0x0 };                      // буффер состояния датчика правого ТЭНА RH

//буфферы кнопок
byte mainButtonBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X02, 0X0, 0x0 };           // буффер отправки значения кнопки вкл/выкл
byte burrataButtonBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X11, 0X0, 0x0 };        // буффер отправки значения кнопки страчателлы
byte rotateDiskButtonBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X13, 0X0, 0x0 };     // буффер отправки значения кнопки страчателлы
byte strachiatellaButtonBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X15, 0X0, 0x0 };  // буффер отправки значения кнопки страчателлы

//буфферы для отжатия кнопки в один клик
byte burrataButtonIncrementalBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X10, 0X0, 0x0 };        // буффер отправки значения кнопки буррата
byte rotateDiskButtonIncrementalBuff[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X12, 0X0, 0x0 };     // буффер отправки значения кнопки страчателлы
byte strachiatellaButtonIncrementalBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X14, 0X0, 0x0 };  // буффер отправки значения кнопки страчателлы


byte popUpCalibrPageBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X20, 0X11, 0x20 };  // буффер отправки вызова попап меню с калибровкой
byte calibrBuff[] = { 0X5A, 0XA5, 0X05, 0X82, 0X13, 0X00, 0X00, 0x00 };         // буффер отправки состояния кнопок при калибровке

//ПРОВЕРИТЬ БУФФЕР! ТАКОГО АДРЕСА НЕТ НА ДВИНЕ!!
byte coverSensorErrorBuf[] = { 0X5A, 0XA5, 0X5, 0X82, 0X12, 0X06, 0X0, 0x0 };   // буффер отправки сообщения об открытой крышке

byte changeMainPageBuff[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x01 };   // буффер перехода на другую страницу
byte changeSetPageBuff[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x00 };    // буффер перехода на страницу калибровки
byte settingsPageAddrBuf[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x2B };  // 
byte settingsPageErrorAddrBuf[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x62 };

byte loadPageAddrBuf[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x01, 0x9C };
byte unloadPageAddrBuf[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x01, 0x3F };

//буфферы кнопок калибровок
byte calibrButtonsBuf[] = { 0X5A, 0XA5, 0X05, 0X82, 0X21, 0X00, 0X00, 0x00 };  // буффер отправки состояния кнопок при калибровке

byte switchBuff[] = { 0X5A, 0XA5, 0X5, 0X82, 0X11, 0X00, 0X0, 0x0 };

//буффер количества сделанных циклов
byte todayCyclesBuff[] = { 0X5A, 0XA5, 0X5, 0X82, 0X30, 0X30, 0X0, 0x0 };                      // буффер состояния датчика правого ТЭНА RH
int todayCycles = 0;
byte todayCycleFlag = 0;
//буффер отправки длинны времени цикла на экран
byte timerBuf[] = { 0X5A, 0XA5, 0X7, 0X82, 0X30, 0X32, 0X00, 0x00, 0X00, 0x00 };
char timerCharArray[] = "0000";      // массив для перевода времени цикла из asdcii в float
float cycleTime = 0;
float sec = 0.0;
float mSec = 0.0;


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
  // подать питание на экран
  digitalWrite(DWIN_POWER, HIGH);


  // прерывания детектора нуля
  attachInterrupt(1, isr, RISING);
  Timer2.enableISR();

  // RXTX
  Serial.begin(115200);

  // RS232
  Serial1.begin(115200);

  // 
  sensorLeft.begin();
  sensorLeft.setWaitForConversion(false);
  sensorRight.begin();
  sensorRight.setWaitForConversion(false);




  //ПЕРЕМЕСТИТЬ ВЫСТАВЛЕНИЯ В КАЛИБРОВКУ!
  digitalWrite(SOLENOID_SWITCH6, HIGH);  //страч
  cylinderState[6] = 1;                  //запоминаем состояние цилиндра
  digitalWrite(SOLENOID_SWITCH7, HIGH);  //сливки
  cylinderState[7] = 1;                  //запоминаем состояние цилиндра
  digitalWrite(SOLENOID_SWITCH4, HIGH);  //подставка
  cylinderState[4] = 1;                  //запоминаем состояние цилиндра
  
  //проверяем крышку, датчик давления, ТЭНы
  //checkSensors();

  //поднимаем редуктор вверх с кручением
  stepTimerRE = millis();
  while (digitalRead(SOLENOID_SENSOR10) == 1) {  //редуктор поднят - идём дальше
    if (micros() - stepTimerRE >= 500) {
      stepTimerRE = micros();
      makeRE_CCWR();
    }
    digitalWrite(SOLENOID_SWITCH10, HIGH);  //поднимаем редуктор
    cylinderState[10] = 1; 
  }


  delay(2000);


  //загрузка параметров
  setDwin();

  // переход на страницу домой
  byte changeFirstPage[] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0x01, 0x00, 0x0A };  //01 - страница калибровки/1Е - основная
  Serial1.write(changeFirstPage, 10);

  // включаем подсветку кнопок
  digitalWrite(ILLUMINATION_OPTION1, HIGH);
  digitalWrite(ILLUMINATION_OPTION2, HIGH);

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
  if (lastDim != dimmerL) {
    Timer2.setPeriod(lastDim = dimmerL);
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


// проверка датчиков тока
void checkHeaters() {
  byte heaterState = 0;

  digitalWrite(DIMMER_L, HIGH);
  leftHeaterCurrent = ACS712_L.mA_AC(50, 1);
  digitalWrite(DIMMER_L, LOW);

  digitalWrite(DIMMER_R, HIGH);
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

  Serial.print("state: ");
  Serial.println(heaterState);

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

      Serial.println("left not working");
      showError();
      break;

    case 2:  //не работает правый
      stateRH = 1;
      stateRHBuff[7] = stateRH;
      Serial1.write(stateRHBuff, 8);
      
      Serial.println("right not working");
      showError();
      break;

    case 3:  //не работают оба
      stateLH = 1;
      stateLHBuff[7] = stateLH;
      Serial1.write(stateLHBuff, 8);
      
      stateRH = 1;
      stateRHBuff[7] = stateRH;
      Serial1.write(stateRHBuff, 8);

      Serial.println("both not working");
      showError();
      break;

    default:
      showError();
      Serial.println("checkHeaters() error!");
      break;
  }

  heaterState = 0;
}

// проверка датчика CS
void checkCoverSensor() {
  stateCS = digitalRead(COVER_SENSOR);
  stateCSBuff[7] = stateCS;
  Serial1.write(stateCSBuff, 8);
/*
    if(stateCS){ //защитная крышка открыта
      coverSensorError = 1;
    } else {
      coverSensorError = 0;
    }
*/
    if(DEBUG){
      Serial.print("stateCS: ");
      Serial.println(stateCS);
    }
}

// проверка датчика PS
void checkPressureSensore(){
  statePS = digitalRead(PRESSURE_METER);
  statePSBuff[7] = statePS;
  Serial1.write(statePSBuff, 8);
}



// чтение сообщения с двина
void readDwin() {
  // читаем serial1 пока не получим полное сообщение
  if (Serial1.available() && inputMessageComplete == false) {
    readInput();
  }

  // если полное сообщение пришло, то парсим его
  if (inputMessageComplete) {
    parseBuffer();
    inputMessageComplete = false;
  }
}

void loop() {
/*
  // работа с температурой
  if (millis() - tempTimer >= 800) {
    tempTimer = millis();
    getTemp();
    if (tempToSendL != tempLeft) {
      tempToSendL = tempLeft;
      sendLeftTemp();
    }
    if (tempToSendR != tempRight) {
      tempToSendR = tempRight;
      sendRightTemp();
    }
  }

  // работа с ПИД
  if (millis() - pidTimer >= 40) {
    pidTimer = millis();
    pid();
    dimmerL = map(outL, 500, 9300, 9300, 500);
    dimmerR = map(outR, 500, 9300, 9300, 500);
  }
*/

  if(digitalRead(STOP)){
    // включена кнопка "буррата"
    /*if(OptionalButton1State != digitalRead(OPTIONAL_BUTTON1)){
      OptionalButton1State  = digitalRead(OPTIONAL_BUTTON1);
    }
    */
    if(!digitalRead(OPTIONAL_BUTTON1)){
      burrataButtonFlag = !digitalRead(OPTIONAL_BUTTON1);    //меняем флаг кнопки
      burrataButtonBuf[7] = !digitalRead(OPTIONAL_BUTTON1);  //меняем буффер кнопки "буррата"
    }
    if (burrataButtonFlag) {  // || digitalRead(OPTIONAL_BUTTON1)){
      //запускаем функцию изготовления бурраты
      burrataButton();
      blinkButton1(500);                         //функция моргания кнопки
    }

    // включена кнопка "шаг"
    /*if(OptionalButton2State != digitalRead(OPTIONAL_BUTTON2)){
      OptionalButton2State  = digitalRead(OPTIONAL_BUTTON2);
      Serial.println(digitalRead(OPTIONAL_BUTTON2));
    }*/
    if(!digitalRead(OPTIONAL_BUTTON2)){
      rotateDiskButtonFlag = 1;
      rotateDiskButtonBuf[7] = 1;
    }
    if (rotateDiskButtonFlag) { // || digitalRead(OPTIONAL_BUTTON2)) {
      //запускаем функцию оборота
      rotateDiskButton();
    } 

    // включена кнопка "масса"
    if (straciatellaButtonFlag ){//|| digitalRead(OPTIONAL_BUTTON3)) {
      //straciatellaButton();
    }

    //калибровка объёма дозаторов
    if (calibrateVolumeFlag){
      calibrateVolume();
    }

    // перемещаем датчик страчателлы в нужную координату
    if (STInMotionFlag == 1 && calibrateVolumeFlag == 0) {
      calcMassaST();
    }

    // перемещаем датчик сливок в нужную координату
    if (CRInMotionFlag == 1 && calibrateVolumeFlag == 0) {
      calcMassaCR();
    }

    //проверка датчика CS
    if(stateCS != digitalRead(COVER_SENSOR)){
      checkCoverSensor();
    }

    // проверка датчика PS
    if(statePS != digitalRead(PRESSURE_METER)){
      checkPressureSensore();
    };


    //!!!! ДОПИСАТЬ:
    // проверяем вспомогательный цикл
    if(doCycle){
      switch(numCycle){
        case 0:
          if(DEBUG){
            Serial.println("doCycle zero case error");
          }
        break;

        case 1:
          cycle1();
          break;

        case 2:
          cycle2();
          break;

        case 3:
          cycle3();
          break;

        default:
          if(DEBUG){
            Serial.print("doCycle in loop error!!!");
          }
          break;
      }
    }

    //управление мотором из настроек
    if(motorREON){
      if(motorREDir){
        if(cylinderState[5] == 0){    //если пресс-форсунка поднята
          if (micros() - stepTimerRE >= motorRESpeed) {
            stepTimerRE = micros();
            makeRE_CWR();  // вращаем мотора диска
        }

        }
      } else {
        if (micros() - stepTimerRE >= motorRESpeed) {
          stepTimerRE = micros();
          makeRE_CCWR();  // вращаем мотора диска
        }
      }
    }

    //включаем редуктор вверх-вниз из настроек
    if(calibrPosition){
      if(digitalRead(SOLENOID_SENSOR10) && !digitalRead(SOLENOID_SENSOR12)){
        digitalWrite(SOLENOID_SWITCH10, HIGH);
        if(todayCycleFlag == 0){
          todayCycleFlag = 1;
          todayCycles++;
          todayCyclesBuff[6] = highByte(todayCycles);
          todayCyclesBuff[7] = lowByte(todayCycles);
          Serial1.write(todayCyclesBuff, 8);
        }
      }
      if(digitalRead(SOLENOID_SENSOR12) && !digitalRead(SOLENOID_SENSOR10)){
        digitalWrite(SOLENOID_SWITCH10, LOW);
        todayCycleFlag = 0;
      }
    }

    //проверяем датчики цилиндров
    if(checkSolenoidSensorsFlag){
      checkSolenoidSensors();
    }
    // проверка наличия сообщения с экрана и обработка сообщения
    readDwin();

  }

}


  /*TODO:
  1   переместить стартовую позицию из сетапа
  2.  написать функцию проверки положения элементов перед выполнением любой из функций
  3   проверить coverSensorErrorBuf[]
  4.  пересчитать коэффециенты для массы 
  5.  дописать проверку датчика давления
  6.  дописать проверка ТЭНов
  7.  проверить SOLENOID_SWITCH3 и SOLENOID_SWITCH4
  8.  проверить необходимость solenoidNum
  9.  энергонезависимая память
  10. loadStrach/unloadStrach/washCycles - переименовать и переделать
  11. проверить pressIsOn итд
  12. добавить в настройки страницу с "вкл/выкл" пароля/перезагрузка
  13. проверить нужен ли cylinderBuf[], тк появился cylinderIconBuf
  14. чекать поршень в дозаторах вверху перед включениеп калибиовки
  15. если калибровку олъёма не сделали и меняем значение массы/сливок, то менять значение через калибровку
  16. лимиты по сливкам с экрана - 40, по общему объёму 225мл - дописать в экран - чтобы любое перекидывал на максимум
  17. добавить кнопку автомат/ручнойввввв: задействовать педаль для включения/отключения
  18. при стоп кнопке - всё останавливать и переводить в положение сетапа
  19. отключать кнопки и включение цикла, до состояния датчиков по уполчанию (например дозатор страчателлы вверху)
  20. дописать моргания кнопок, если дозатор страчателлы, например, не доехал через 5-10 секунд и отобразить ошибкой в "инфо"

  */

