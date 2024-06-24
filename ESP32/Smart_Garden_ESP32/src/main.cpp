// Adding the required libraries
#define BLYNK_TEMPLATE_ID "TMPL6pHLgs_5H"
#define BLYNK_TEMPLATE_NAME "Enviroment Measure"
#define BLYNK_AUTH_TOKEN "oM7OySbif8DvOUktWMzxPgRQcMa0MsXx"
#include <Arduino.h>
#include <BlynkSimpleEsp32.h> //You need to add it by searching "Blynk" in libraries and install it
#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <BH1750.h>

// Your WiFi credentials.
// Set password to "" for open networks.
#define WIFI_SSID             "SongQuynh"
#define WIFI_PASS             "Songquynh25042112"
// DHT sensor settings and configuration
#define VPIN_TEMPERATURE      V3 //Virtual pin on Blynk side
#define VPIN_HUMIDITY         V4 //Virtual pin on Blynk side
#define VPIN_LIGHT_STRENGTH   V5
#define FAN1_STATE_VPIN       V1 //Virtual pin on Blynk side
#define FAN2_STATE_VPIN       V2 //Virtual pin on Blynk side
#define LIGHT_STATE_VPIN      V0
#define WIFI_LED_PIN 2

#define DHT_PIN  2
#define DHTTYPE DHT11   
#define SCL_PIN 22
#define SDA_PIN 21
#define FAN1_RELAY_PIN    25
#define FAN2_RELAY_PIN    26
#define LIGHT_RELAY_PIN   27

float Temperature = 0;
float humidity = 0;
float Luxury = 0;
bool fan1_state = LOW;
bool fan2_state = LOW;
bool light_state = LOW;


// This function creates the timer object. It's part of Blynk library
BlynkTimer timer1;
DHT dht(DHT_PIN, DHTTYPE);
BH1750 lightMeter;
int RUN = 0;
// SETUP BLOCK
// Sending data from DHT sensor to Blynk

BLYNK_WRITE(FAN1_STATE_VPIN){
  fan1_state = param.asInt();
  if(fan1_state == 1){
    Serial.println("Bat Quat 1");
    digitalWrite(FAN1_RELAY_PIN, HIGH);
  }
  else{
    Serial.println("Tat quat 1");
    digitalWrite(FAN1_RELAY_PIN, LOW);
  }
}

BLYNK_WRITE(FAN2_STATE_VPIN){
  fan2_state = param.asInt();
  if(fan2_state == 1){
    Serial.println("Bat Quat 2");
    digitalWrite(FAN2_RELAY_PIN, HIGH);
  }
  else{
    Serial.println("Tat quat 2");
    digitalWrite(FAN2_RELAY_PIN, LOW);
  }
}

BLYNK_WRITE(LIGHT_STATE_VPIN){
  light_state = param.asInt();
  if(light_state == 1){
    Serial.println("Bat Den");
    digitalWrite(LIGHT_RELAY_PIN, HIGH);
  }
  else{
    Serial.println("Tat Den");
    digitalWrite(LIGHT_RELAY_PIN, LOW);
  }
}
void reandAndSendSensorsData();


void setup() 
{ 
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200); 
  dht.begin();
  lightMeter.begin();
  pinMode(WIFI_LED_PIN,OUTPUT);
  pinMode(LIGHT_RELAY_PIN, OUTPUT);
  pinMode(FAN2_RELAY_PIN, OUTPUT);
  pinMode(FAN1_RELAY_PIN, OUTPUT);

  digitalWrite(WIFI_LED_PIN, LOW);
  digitalWrite(LIGHT_RELAY_PIN, LOW);
  digitalWrite(FAN2_RELAY_PIN, LOW);
  digitalWrite(FAN1_RELAY_PIN, LOW);
  
  /* Connect to Wifi */
  WiFi.begin(WIFI_SSID, WIFI_PASS);
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
  Luxury = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(Luxury);
  Serial.println(" lx");
  Temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(Temperature);
  Serial.println(" oC");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Blynk.virtualWrite(VPIN_TEMPERATURE, Temperature);
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity);
  Blynk.virtualWrite(VPIN_LIGHT_STRENGTH,Luxury);
}
