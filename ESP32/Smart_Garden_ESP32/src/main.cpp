// Adding the required libraries
#define BLYNK_TEMPLATE_ID "TMPL6w8I2-IrO"
#define BLYNK_TEMPLATE_NAME "Smart Garden"
#define BLYNK_AUTH_TOKEN "sSmADQkIJw5N_5tSvw6F8iaKx2bNV2FN"
#include <Arduino.h>
#include <BlynkSimpleEsp32.h> //You need to add it by searching "Blynk" in libraries and install it



// Your WiFi credentials.
// Set password to "" for open networks.
#define WIFI_SSID             "Song Quynh"
#define WIFI_PASS             "songquynh25042112"
#define BLYNK_PRINT Serial  // Comment this out to disable serial monitor prints

// DHT sensor settings and configuration
#define DHT_BLYNK_VPIN_TEMPERATURE V4 //Virtual pin on Blynk side
#define DHT_BLYNK_VPIN_HUMIDITY V3 //Virtual pin on Blynk side
#define DHT_BLYNK_VPIN_LIGHT    V5


// This function creates the timer object. It's part of Blynk library
BlynkTimer timer;
int RUN = 0;
// SETUP BLOCK
// Sending data from DHT sensor to Blynk
void sendDhtData() {
  Serial.println("Sending DHT data");
  if(Serial2.)
  Blynk.virtualWrite(DHT_BLYNK_VPIN_TEMPERATURE, 25);
  Blynk.virtualWrite(DHT_BLYNK_VPIN_HUMIDITY, 60);
  
}

void reandAndSendSensorsData() {
  sendDhtData();
}

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  // Set up timer to run every 5 sec
  timer.setInterval(5000L, reandAndSendSensorsData);
}

void loop() {
  Blynk.run();
  timer.run();
}
