// Adding the required libraries
#define BLYNK_TEMPLATE_ID "TMPL6l1VIqxI-"
#define BLYNK_TEMPLATE_NAME "Polution SuperVisor"
#define BLYNK_AUTH_TOKEN "3oO_JQK-ukYaBohArCXEHcRfXj4irA2F"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <BlynkSimpleEsp32.h> //You need to add it by searching "Blynk" in libraries and install it
#include <BH1750.h>
#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>
#include <MQ135.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define SCL_PIN 22
#define SDA_PIN 21
#define DHTPIN 2 
#define DHTTYPE DHT11  
#define WIFI_SSID "Your Wifi Name"
#define WIFI_PASS "Your Wifi Password"

#define GAS_INPUT_PIN 32
#define GAS_THRESH_HOLD 100

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte degree_symbol[8] = {0b00111,0b00101,0b00111,0b00000,0b00000,0b00000,0b00000,0b00000};
              
BlynkTimer timer1;
BlynkTimer timer2;
DHT dht(DHTPIN, DHTTYPE);

/*Function Macro*/
void read_and_sendData();
void Init_LCD();
void LCD_process();

void setup()
{   
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  dht.begin();
  lcd.begin(16,2);
  Init_LCD();
  /*Connect to Wifi*/
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());
  Blynk.config(BLYNK_AUTH_TOKEN);
  timer1.setInterval(1000L, read_and_sendData);
}

void loop()   
{
    Blynk.run();
    timer1.run();
    LCD_process();
}

void read_and_sendData()
{ 
    float humi = dht.readHumidity();
    float temp = dht.readTemperature();
    if (isnan(humi) || isnan(temp)){
        Serial.println("Failed to read from DHT sensor!");
        return;
    }
    int analogSensor = analogRead(GAS_INPUT_PIN);
    Blynk.virtualWrite(V2, analogSensor);
    Serial.print("Gas Value: ");
    Serial.println(analogSensor);
    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, humi);
   
    Serial.print("Temperature : ");
    Serial.print(temp);
    Serial.print("Humidity : ");
    Serial.println(humi);
    
 
}

void Init_LCD(){
  lcd.setCursor(3,0);
  lcd.print("Air Quality");
  lcd.setCursor(3,1);
  lcd.print("Monitoring");
  delay(2000);
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Team 15");
  delay(2000);
  lcd.clear();
}
void LCD_process(){
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  int gasValue = analogRead(GAS_INPUT_PIN);
  if(temp<=40)
  {
    lcd.setCursor(0,0);
    lcd.print("Temp "); 
    lcd.print(temp);
    lcd.setCursor(11,0);
    lcd.write(1);
    lcd.createChar(1, degree_symbol);
    lcd.setCursor(12,0);
    lcd.print("C");
    lcd.setCursor(0,1);
    lcd.print("Normal Temp");
    Serial.println("Normal Temp");
    delay(4000);
    lcd.clear();
  }
  else if(temp>40)
  {
    lcd.setCursor(0,0);
    lcd.print("Temp "); 
    lcd.print(temp);
    lcd.setCursor(11,0);
    lcd.write(1);
    lcd.createChar(1, degree_symbol);
    lcd.setCursor(12,0);
    lcd.print("C");
    lcd.setCursor(0,1);
    lcd.print("High Temp");
    Serial.println("High Temp");
    delay(4000);
    lcd.clear();
  }

  if(humi>85){
    lcd.setCursor(0, 0);
    lcd.print("Humidity ");
    lcd.print(humi);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("High Humidity");
    Serial.println("High Humidity");
    delay(4000);
    lcd.clear();
  }
  else if(humi<40)
  {
    lcd.setCursor(0, 0);
    lcd.print("Humidity ");
    lcd.print(humi);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("Low Humidity");
    Serial.println("Low Humidity");
    delay(4000);
    lcd.clear();
  }
  else 
  {
    lcd.setCursor(0, 0);
    lcd.print("Humidity ");
    lcd.print(humi);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("Normal Humidity");
    Serial.println("Normal Humidity");
    delay(4000);
    lcd.clear();
  }

  Serial.println("Gas Value");
  Serial.println(gasValue);
  if(gasValue<1200)
  {
      lcd.setCursor(0,0);
      lcd.print("Gas Value: ");
      lcd.print(gasValue);
      lcd.setCursor(0, 1);
      lcd.print("Fresh Air");
      Serial.println("Fresh Air");
      delay(4000);
      lcd.clear();
  }
  else if(gasValue>1200)
  {
      lcd.setCursor(0,0);
      lcd.print("Gas Value: ");
      lcd.print(gasValue);
      lcd.setCursor(0, 1);
      lcd.print("Bad Air");
      Serial.println("Bad Air");
      delay(4000);
      lcd.clear();
  }
}