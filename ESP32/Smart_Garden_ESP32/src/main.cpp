// Adding the required libraries
#define BLYNK_TEMPLATE_ID "TMPL6w8I2-IrO"
#define BLYNK_TEMPLATE_NAME "Smart Garden"
#define BLYNK_AUTH_TOKEN "sSmADQkIJw5N_5tSvw6F8iaKx2bNV2FN"
#include <Arduino.h>
#include <BlynkSimpleEsp32.h> //You need to add it by searching "Blynk" in libraries and install it
#include <cJSON.h>


// Your WiFi credentials.
// Set password to "" for open networks.
#define WIFI_SSID             "Song Quynh"
#define WIFI_PASS             "songquynh25042112"
#define BLYNK_PRINT Serial  // Comment this out to disable serial monitor prints

// DHT sensor settings and configuration
#define DHT_BLYNK_VPIN_TEMPERATURE  V4 //Virtual pin on Blynk side
#define DHT_BLYNK_VPIN_HUMIDITY     V3 //Virtual pin on Blynk side
#define DHT_BLYNK_VPIN_LIGHT        V5
#define FAN_STATE_VPIN V1 //Virtual pin on Blynk side
#define PUMP_STATE_VPIN  V2 //Virtual pin on Blynk side
#define LIGHT_STATE_VPIN V0

float Temperature = 0;
float humidity = 0;
float Luxury = 0;
uint8_t fan_state;
uint8_t pump_state;
uint8_t light_state;
char data_buffer[100] = {0};

// This function creates the timer object. It's part of Blynk library
BlynkTimer timer;
int RUN = 0;
// SETUP BLOCK
// Sending data from DHT sensor to Blynk

void get_Data();

void reandAndSendSensorsData() {
  Serial.println("Sending DHT data");
  if(Serial2.available()){
    Serial.println(Serial2.readBytesUntil('*', data_buffer, 25));
    get_Data();
  }
  
  Blynk.virtualWrite(DHT_BLYNK_VPIN_TEMPERATURE, 25);
  Blynk.virtualWrite(DHT_BLYNK_VPIN_HUMIDITY, 60);
  Blynk.virtualWrite(DHT_BLYNK_VPIN_LIGHT,1200);
  Blynk.virtualWrite(FAN_STATE_VPIN, fan_state);
  Blynk.virtualWrite(PUMP_STATE_VPIN, pump_state);
  Blynk.virtualWrite(LIGHT_STATE_VPIN, light_state);
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  // Set up timer to run every 5 sec
  timer.setInterval(3000L, reandAndSendSensorsData);
}

void loop() {
  Blynk.run();
  timer.run();
}

void get_Data(){
    cJSON* root = cJSON_Parse(data_buffer);
    if(root == NULL){
        Serial.println("Parser ERROR");
        return;
    }
    cJSON* Temp = cJSON_GetObjectItemCaseSensitive(root, "Temp");
    cJSON* Humi = cJSON_GetObjectItemCaseSensitive(root, "Humi");
    cJSON* Lux = cJSON_GetObjectItemCaseSensitive(root, "Lux");
    cJSON* Fan = cJSON_GetObjectItemCaseSensitive(root, "Fan");
    cJSON* Pump = cJSON_GetObjectItemCaseSensitive(root, "Pump");
    cJSON* Light = cJSON_GetObjectItemCaseSensitive(root, "Light");

    Temperature = Temp->valuedouble;
    humidity = Humi->valuedouble;
    Luxury = Lux->valuedouble;
    fan_state = Fan->valueint;
    pump_state = Pump->valueint;
    light_state = Light->valueint;

    Serial.print("Temp: ");
    Serial.println(Temperature);
    Serial.print("Humi: ");
    Serial.println(humidity);
    Serial.print("Lux: ");
    Serial.println(Luxury);
    Serial.print("Fan: ");
    Serial.println(fan_state); 
    Serial.print("Pump: ");
    Serial.println(pump_state);
    Serial.print("Light: ");
    Serial.println(light_state);   
}