#include <DHT11.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"
#include <ArduinoJson.h>

#define WIFI_SSID "XiaoQ"
#define WIFI_PASS "welcomehome"

#define FIREBASE_HOST "doantn-3844d-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "myUxxyLOnr6VOvxDTQIAfP9AkoJS16UZNYvichZ6"

FirebaseData fData;
String path = "/";
FirebaseJson json;

LiquidCrystal_I2C lcd(0x27,16,2);

DHT11 dht11(2); // GPIO 2

int my_temperature = 30;
int my_humidity = 60;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  pinMode(12,OUTPUT);
  digitalWrite(12, LOW);
  lcd.init();     // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
}

void loop() {
  Firebase.getInt(fData, path + "/t");
  my_temperature = fData.intData();
  Firebase.getInt(fData, path + "/h");
  my_humidity = fData.intData();
  int temperature = 0;
  int humidity = 0;
  // Attempt to read the temperature and humidity values from the DHT11 sensor.
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  Firebase.setInt(fData,path + "/ct",temperature);
  Firebase.setInt(fData,path + "/ch",humidity);
  // Read
  lcd.setCursor(1,0);
  lcd.print("T:");
  lcd.setCursor(3,0);
  lcd.print(temperature);
  lcd.setCursor(1,1);
  lcd.print("H:");
  lcd.setCursor(3,1);
  lcd.print(humidity);
  // Your setting
  lcd.setCursor(10,0);
  lcd.print("MT:");
  lcd.setCursor(13,0);
  lcd.print(my_temperature);
  lcd.setCursor(10,1);
  lcd.print("MH:");
  lcd.setCursor(13,1);
  lcd.print(my_humidity);
  //
  if(my_temperature < temperature || my_humidity < humidity)
  {
    digitalWrite(12, HIGH);

  }
  else
  {
    digitalWrite(12, LOW);
  }
}
