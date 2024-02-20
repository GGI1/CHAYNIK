

#include <OneWire.h>
#include <DallasTemperature.h>
#include <IRremote.h>

const int A7_PIN = A7;      // Пин фоторезистора
const int ECHO_PIN = 2;     // Пин echo датчика HC-SR04
const int TRIG_PIN = 3;     // Пин trig датчика HC-SR04
const int IR_PIN = 4;       // Пин инфракрасного диода
const int TEMP_PIN = 5;     // Пин терморезистора DS18B20

unsigned long lastLightValue = 0;
unsigned long lastDistance = 0;
float lastTempC = 0.0;

OneWire oneWire(TEMP_PIN);  // Создание экземпляра OneWire для взаимодействия с датчиком температуры
DallasTemperature sensors(&oneWire);  // Создание экземпляра DallasTemperature
IRrecv irrecv(IR_PIN);
decode_results results;


struct sens {
  int LV;
  unsigned long CD;
  float tC;
  
};

sens D;




void setup() {
  Serial.begin(9600);
  pinMode(A7_PIN, INPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);  // Переводим пин ИК-датчика в режим ввода
  pinMode(TEMP_PIN, INPUT);

  sensors.begin();  // Инициализация библиотеки для работы с датчиками температуры
  irrecv.enableIRIn(); // Включение приемника ИК-сигналов
}

void sens_upd(){
  
  float tempC = sensors.getTempCByIndex(0);  // Получение температуры от датчика DS18B20

  int lightValue = analogRead(A7_PIN);  // Чтение значения фоторезистора

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long currentDistance = pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;  // Измерение расстояния




  
  D.LV = lightValue;
  D.CD = currentDistance;
  D.tC = tempC;
  
}

void loop() {
  
  
      sens_upd();


  if (irrecv.decode(&results)) {
    // Если получен сигнал от ИК-пульта
    Serial.print("IR Code: 0x");
    Serial.println(results.value, HEX);

    // Дополнительная обработка кода, если необходимо

    irrecv.resume(); // Подготовка к приему следующего сигнала
  }





}
