// Adding the required libraries
#define BLYNK_TEMPLATE_ID "TMPL6pHLgs_5H"
#define BLYNK_TEMPLATE_NAME "Enviroment Measure"
#define BLYNK_AUTH_TOKEN "oM7OySbif8DvOUktWMzxPgRQcMa0MsXx"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <BlynkSimpleEsp32.h> //You need to add it by searching "Blynk" in libraries and install it
#include <cJSON.h>
#include <WiFi.h>
// Your WiFi credentials.
// Set password to "" for open networks.
#define WIFI_SSID             "Song Quynh"
#define WIFI_PASS             "songquynh25042112"
// DHT sensor settings and configuration
#define VPIN_TEMPERATURE      V3 //Virtual pin on Blynk side
#define VPIN_HUMIDITY         V4 //Virtual pin on Blynk side
#define VPIN_LIGHT_STRENGTH   V5
#define FAN1_STATE_VPIN       V1 //Virtual pin on Blynk side
#define FAN2_STATE_VPIN       V2 //Virtual pin on Blynk side
#define LIGHT_STATE_VPIN      V0
#define WIFI_LED_PIN 2

#define FAN1_RELAY_PIN    12
#define FAN2_RELAY_PIN    13
#define LIGHT_RELAY_PIN    14

float Temperature = 0;
float humidity = 0;
float Luxury = 0;
bool fan1_state = LOW;
bool fan2_state = LOW;
bool light_state = LOW;


const char ssid[] = "SongQuynh";
const char password[] = "Songquynh25042112";
// This function creates the timer object. It's part of Blynk library
BlynkTimer timer1;
BlynkTimer timer2;
int RUN = 0;
// SETUP BLOCK
// Sending data from DHT sensor to Blynk

BLYNK_WRITE(FAN1_STATE_VPIN){
  fan1_state = param.asInt();
  if(fan1_state == 1){
    Serial.println("Bat Quat 1");
    digitalWrite(WIFI_LED_PIN, HIGH);
  }
  else{
    Serial.println("Tat quat 1");
    digitalWrite(WIFI_LED_PIN, LOW);
  }
}

BLYNK_WRITE(FAN2_STATE_VPIN){
  fan2_state = param.asInt();
  if(fan2_state == 1){
    Serial.println("Bat Quat 2");
    digitalWrite(WIFI_LED_PIN, HIGH);
  }
  else{
    Serial.println("Tat quat 2");
    digitalWrite(WIFI_LED_PIN, LOW);
  }
}

BLYNK_WRITE(LIGHT_STATE_VPIN){
  light_state = param.asInt();
  if(light_state == 1){
    Serial.println("Bat Den");
    digitalWrite(WIFI_LED_PIN, HIGH);
  }
  else{
    Serial.println("Bat Den");
    digitalWrite(WIFI_LED_PIN, LOW);
  }
}
void reandAndSendSensorsData();


void setup() 
{ 
  Serial.begin(115200); 
  pinMode(WIFI_LED_PIN,OUTPUT);
  pinMode(LIGHT_RELAY_PIN, OUTPUT);
  pinMode(FAN2_RELAY_PIN, OUTPUT);
  pinMode(FAN1_RELAY_PIN, OUTPUT);

  digitalWrite(WIFI_LED_PIN, LOW);
  digitalWrite(LIGHT_RELAY_PIN, LOW);
  digitalWrite(FAN2_RELAY_PIN, LOW);
  digitalWrite(FAN1_RELAY_PIN, LOW);
  
  /* Connect to Wifi */
  WiFi.begin(ssid, password);
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());

  digitalWrite(WIFI_LED_PIN, HIGH);
  Blynk.config(BLYNK_AUTH_TOKEN);
  timer1.setInterval(1000L, reandAndSendSensorsData);
}  



void loop(){  
    Blynk.run();
    timer1.run();
}


void reandAndSendSensorsData() {
  // Serial.println("Sending  data");
  Blynk.virtualWrite(VPIN_TEMPERATURE, Temperature);
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity);
  Blynk.virtualWrite(VPIN_LIGHT_STRENGTH,Luxury);
}
