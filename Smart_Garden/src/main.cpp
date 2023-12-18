#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <DHT_U.h>
#include <math.h>
#include "Wire.h"
#include "BH1750.h"

#define DHTPIN 10
#define DHTTYPE    DHT11
#define LIGTH_SENSOR A0

#define LIGHTCTRL_PIN 11
#define PUMPCTRL_PIN 13
#define FANCTRL_PIN 12

#define HUMIDITY_LEVEL 70
#define TEMPERATURE_LEVEL 27
#define LIGHT_CONST   2000

DHT_Unified dht(DHTPIN, DHTTYPE);
BH1750  light_sensor;
uint32_t delayMS;

float humidity = 0;;
float temperature = 0;
double lux = 0;

uint8_t fan_state = 0;
uint8_t pump_state = 0;
uint8_t light_state = 0;

unsigned long current_time = 0;
void sensor_init();
void measure_task_process();
void control_task_process();
void send_data_task_process();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  sensor_init();
  Wire.begin();
  light_sensor.begin();
  pinMode(LIGHTCTRL_PIN, OUTPUT);
  pinMode(PUMPCTRL_PIN, OUTPUT);
  pinMode(FANCTRL_PIN, OUTPUT);
  pinMode(LIGTH_SENSOR, INPUT);
  current_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  measure_task_process();
  control_task_process();
  if(millis() - current_time > 2000){
    send_data_task_process();
    current_time = millis();
  }
  delay(100);
}

void sensor_init(){
  dht.begin();
  // Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  // Serial.println(F("------------------------------------"));
  // Serial.println(F("Temperature Sensor"));
  // Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  // Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  // Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  // Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  // Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  // Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  // Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  // Serial.println(F("Humidity Sensor"));
  // Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  // Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  // Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  // Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  // Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  // Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  // Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void measure_task_process(){
  /********************Light Measure*************************/
  lux = light_sensor.readLightLevel();
  // Serial.print("LUX: ");
  // Serial.println(lux);

  /********************Humidity and Temperature Measure*************************/
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    // Serial.println(F("Error reading temperature!"));
    return;
  }
  else {
    // Serial.print(F("Temperature: "));
    // Serial.print(event.temperature);
    temperature = event.temperature;
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    // Serial.println(F("Error reading humidity!"));
    return;
  }
  else {
    // Serial.print(F("Humidity: "));
    // Serial.print(event.relative_humidity);
    // Serial.println(F("%"));
    humidity = event.relative_humidity;
  }
}

void control_task_process(){
  if(round(humidity) > HUMIDITY_LEVEL){
    digitalWrite(PUMPCTRL_PIN, HIGH);
    pump_state = 0;
  }
  else{
    digitalWrite(PUMPCTRL_PIN, LOW);
    pump_state = 1;
  }
  if(round(temperature) < TEMPERATURE_LEVEL){
    digitalWrite(FANCTRL_PIN, HIGH);
    fan_state = 0;
  }
  else{
    digitalWrite(FANCTRL_PIN, LOW);
    fan_state = 1;
  }
  if(round(lux) > LIGHT_CONST){
    digitalWrite(LIGHTCTRL_PIN, HIGH);
    light_state = 0;
  }
  else{
    digitalWrite(LIGHTCTRL_PIN, LOW);
    light_state = 1;
  }
    
}
void send_data_task_process(){
  StaticJsonDocument<100> doc;
  doc["Temp"] = temperature;
  doc["Humi"] = humidity;
  doc["Lux"] = lux;
  doc["Fan"] = fan_state;
  doc["Pump"] = pump_state;
  doc["Light"] = light_state;
  serializeJsonPretty(doc, Serial);
  Serial.print('*');
}