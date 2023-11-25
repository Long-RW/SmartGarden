/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6w8I2-IrO"
#define BLYNK_TEMPLATE_NAME "Smart Garden"
#define BLYNK_AUTH_TOKEN "sSmADQkIJw5N_5tSvw6F8iaKx2bNV2FN"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "cJSON.h"
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Song Quynh";
char pass[] = "songquynh25042112";

char recv_buff[64] = {0};
uint8_t rx_index = 0;

bool newData = false;
void get_Data();
void Data_process_handle();

BlynkTimer timer;
float Temperature = 27.0;
float humidity = 85.0;
float Luxury = 1500;
uint8_t fan_state;
uint8_t pump_state;
uint8_t light_state;
// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int light_swtich = param.asInt();
  // Update state
  Blynk.virtualWrite(V0, light_swtich);
}
BLYNK_WRITE(V1)
{
  // Set incoming value from pin V0 to a variable
  int fan_switch = param.asInt();
  // Update state
  Blynk.virtualWrite(V1, fan_switch);
}
BLYNK_WRITE(V2)
{
  // Set incoming value from pin V0 to a variable
  int pump_swtich = param.asInt();
  // Update state
  Blynk.virtualWrite(V2, pump_swtich);
}
// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 2000);
  
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
  // Data_process_handle();
  Blynk.virtualWrite(V3, humidity);
  Blynk.virtualWrite(V4, Temperature);
  Blynk.virtualWrite(V5, Luxury);
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

void Data_process_handle(){
    if(Serial.available()){
    char rx_data = Serial.read();
    recv_buff[rx_index] = rx_data;
    rx_index++;
    if(rx_data == '*'){
        recv_buff[rx_index-1] = 0;
        rx_index = 0;
        newData = true;
    }
   }
   if(newData == true){
        get_Data();
        memset(recv_buff, 0, 64);
        newData = false;
        Blynk.virtualWrite(V3, humidity);
        Blynk.virtualWrite(V4, Temperature);
        Blynk.virtualWrite(V5, Luxury);
   }

}
void get_Data(){
    cJSON* root = cJSON_Parse(recv_buff);
    if(root == NULL){
        // Serial.println("Parser ERROR");
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
    cJSON_Delete(root);
}