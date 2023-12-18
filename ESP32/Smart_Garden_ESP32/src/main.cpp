// Adding the required libraries
#define BLYNK_TEMPLATE_ID "TMPL6w8I2-IrO"
#define BLYNK_TEMPLATE_NAME "Smart Garden"
#define BLYNK_AUTH_TOKEN "viEMNjwnfBro9hxChvPMpWAZ84yaCZQ1"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <BlynkSimpleEsp32.h> //You need to add it by searching "Blynk" in libraries and install it
#include <cJSON.h>
#include <WiFi.h>
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
#define WIFI_LED_PIN 2
float Temperature = 0;
float humidity = 0;
float Luxury = 0;
uint8_t fan_state = 1 ;
uint8_t pump_state = 1;
uint8_t light_state = 1;
char data_buffer[100] = {0};
uint8_t data_index = 0;

const char ssid[] = "Song Quynh";
const char password[] = "songquynh25042112";
// This function creates the timer object. It's part of Blynk library
BlynkTimer timer1;
BlynkTimer timer2;
int RUN = 0;
// SETUP BLOCK
// Sending data from DHT sensor to Blynk

void get_Data();
void read_Data();
void reandAndSendSensorsData();
// void Task1code(void* parameter);
// void Task2code(void* parameter);

HardwareSerial SerialPort(2); // use UART2
// TaskHandle_t Task1; 
// TaskHandle_t Task2;  

void setup() 
{ 
  Serial.begin(115200); 
  SerialPort.begin(9600, SERIAL_8N1, 16, 17); 
  pinMode(WIFI_LED_PIN,OUTPUT);
  digitalWrite(WIFI_LED_PIN, LOW);



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
  timer1.setInterval(3000L, reandAndSendSensorsData);
  timer2.setInterval(10L, read_Data);
  // xTaskCreatePinnedToCore(Task1code,"Task1",10000,NULL,1,&Task1,0);  delay(500);   
  // xTaskCreatePinnedToCore(Task2code,"Task2",10000,NULL,1,&Task2,1);  delay(500); 
}  



void loop(){  
    Blynk.run();
    timer1.run();
    timer2.run();
}

void read_Data(){
    if(SerialPort.available()){
      char rx_data = SerialPort.read();
      if(rx_data != '*'){
        data_buffer[data_index] = rx_data;
        data_index++;
      }
      else{
        data_buffer[data_index] = 0;
        data_index = 0;
        get_Data();
      }
  }
}
void reandAndSendSensorsData() {
  Serial.println("Sending  data");
  Blynk.virtualWrite(DHT_BLYNK_VPIN_TEMPERATURE, Temperature);
  Blynk.virtualWrite(DHT_BLYNK_VPIN_HUMIDITY, humidity);
  Blynk.virtualWrite(DHT_BLYNK_VPIN_LIGHT,Luxury);
  Blynk.virtualWrite(FAN_STATE_VPIN, fan_state);
  Blynk.virtualWrite(PUMP_STATE_VPIN, pump_state);
  Blynk.virtualWrite(LIGHT_STATE_VPIN, light_state);
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