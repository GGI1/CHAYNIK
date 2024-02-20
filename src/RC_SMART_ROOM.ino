// быстрый старт

#include <Arduino.h>

// логин-пароль WiFi
#define AP_SSID "TP-Link_64FF"
#define AP_PASS "89142325"


//пины для внешних модулей
#define b_dh_out D3//button door handle outside
#define b_dh_in D2 //button door handle inside
#define r_dl D4    //rele door lock
#define r_AL D1    //rele ALARM

// подключаем библиотеку и настраиваем устройство
#include <GyverHub.h>
GyverHub hub("MyDevices", "ДУ-1", "");
// иконки
// https://fontawesome.com/v5/cheatsheet/free/solid
// https://fontawesome.com/v5/search?o=r&m=free&s=solid

// переменные для интерфейса
GHbutton b2;
bool sw_alert, sw_acces,sw_1,sw_2,sw_3,sw_4,sw_5,b_OUT,b_IN;
uint8_t tab = 0, spin_am = 2;
float sld_f;
float temp = 22.5; // Пример значения температуры
float light = 500; // Пример значения уровня света

String inp_str;
char inp_cstr[11];  // строка на 10 символов (+1 нулевой)
int16_t inp_int;

uint32_t btnTimer = 0, timer;

//переменные доступа
bool acces_out = true;
bool acces_in = true;




// это наш билдер. Он будет вызываться библиотекой
// для сборки интерфейса, чтения значений и проч.
void build(){
    // сделаем интерфейс в виде стильных виджетов
    // BeginWidgets() начинает новую горизонтальную строку виджетов
    hub.BeginWidgets();

    // ширина следующих виджетов задаётся в %
    hub.WidgetSize(100);
    
    
    
    hub.Button(&b2, F("Открыть замок"), GH_VIOLET);

    hub.Spinner(&spin_am, GH_UINT8, F("Amount"), 0, 20, 1);
    
    hub.WidgetSize(50);

    //hub.Label_(F("lbl"), F("Something Что-то"));

   
    hub.Title(F(" "));
    
    hub.WidgetSize(100);
    
    if(hub.Tabs(&tab, "Главное Меню,Настройки")) hub.refresh();

    if (tab == 0 || hub.buildRead()){

 /*           // 5 переключателей
  hub.WidgetSize(25);
  hub.Switch(&sw_1,F("Switch 1"));            //переключатель 1
  hub.Switch(&sw_2,F("Switch 2"), GH_RED);    //переключатель 2
  hub.Space();hub.Space();                    ////пустое пространство
  hub.Switch(&sw_3,F("Switch 3"), GH_GREEN);  //переключатель
  hub.Switch(&sw_4,F("Switch 4"), GH_YELLOW);   //переключатель
 
  hub.Space();hub.Space();hub.Space();    //пустое пространство
*/
  // 2 модуля Gauge
  hub.WidgetSize(50);
  hub.Gauge(temp, F("°C"), F("Temperature"), -15, 40, 0.1, GH_RED); //шкала температуры
  hub.Gauge(light, F("lux"), F("Light"), 0, 10000, 0.1, GH_BLUE);   //шкала силы света

  // 4 модуля Label_
  hub.WidgetSize(50);
  hub.Label_(F("Label 1"), F("Ручка снаружи:"));//надпись 1
  hub.LED(b_OUT, F("Icon"), F(""));
  
  hub.Label_(F("Label 3"), F("Ручка изнутри:"));//надпись 2
  hub.LED(b_IN, F("Icon"), F(""));

    }
    

    if (tab == 1 || hub.buildRead()){
      //hub.sendGet("lbl", F("Кто-то зашёл в настройки!⚙️"));
      
      hub.Switch_(F("sw_alert"),&sw_alert,F("СИРЕНА"), GH_RED);

      hub.Switch_(F("sw_acces"),&sw_acces,F("Доступ с ручки"));

      
    }


 


 
    //hub.sendGet("lbl", F("Главное меню"));
    
    hub.EndWidgets();
}

void setup() {
  Serial.begin(115200);  

#ifdef GH_ESP_BUILD
    WiFi.mode(WIFI_STA);
    WiFi.begin(AP_SSID, AP_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println(WiFi.localIP());
#endif

    // можно подключить обработчик веб-консоли
    // зайди в info/cli в приложении
    // или нажми ~ при открытом устройстве
    /*hub.onCLI([](String& s) {
        Serial.println(s);
        hub.print(s);       // отправить обратно
        // вдогонку отправим строку из Input inp_str
        hub.print(inp_str, GH_BLUE);
    });
*/
    hub.onBuild(build);     // подключаем билдер
    Serial.println("подключаем билдер");
    hub.begin();            // запускаем систему
    Serial.println("запускаем систему");

    //pinMode(r_dl, OUTPUT);  //определим пин, как вывод
 // Serial.println("определим пин, как вывод");
    pinMode(b_dh_out, INPUT_PULLUP);
    pinMode(b_dh_in, INPUT_PULLUP);
    
    pinMode(r_dl, OUTPUT);
    pinMode(r_AL, OUTPUT);
   
  
  }
  

void loop() {

  hub.tick();  // обязательно тикаем тут
  
  
  b_OUT =!digitalRead(b_dh_out);
  b_IN =!digitalRead(b_dh_in);
  
  
  if (b_OUT && acces_out) {
      
      digitalWrite(r_dl, HIGH);   //открываем замок
      //delay(500);
      timer = millis();
  }
  if (b_IN && acces_in) {
    
      digitalWrite(r_dl, HIGH);
      //delay(500);
      timer = millis();
      }
  

  if (b2) {
        //Serial.println("press");
        
        hub.sendNotice("ЗАМОК ОТКРЫТ!", GH_RED);
        digitalWrite(r_dl, HIGH);   //
        //delay(1000);
        timer = millis() + 500;
  }
  else{
        if (millis() - timer > 500){
        timer = millis();  
        digitalWrite(r_dl, LOW);
        }
      }
}
    
