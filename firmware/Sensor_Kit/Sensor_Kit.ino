//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "ADS1X15.h"
#include <Wire.h> 
#include "MAX30105.h"

#include "heartRate.h"
#include <LiquidCrystal_I2C.h>
MAX30105 particleSensor;
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

// Set these to run example.
#define FIREBASE_HOST "ionkid-abd2f.firebaseio.com"
#define FIREBASE_AUTH "12Bs4sG60U96pN0VqAYxIRzKmo55omcTIUPejNZF"
#define WIFI_SSID "TP-Link"
#define WIFI_PASSWORD "26042002"

ADS1115 ADS(0x48);
void setup() { 
  Serial.begin(9600);
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("MAX30105 not found");
    while (1);
  }
 Serial.println("Place your index finger on the sensor with steady pressure.");
  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);
lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  
 
ADS.begin();
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    lcd.print(".");
    delay(500);
    
      
  }lcd.clear();
 
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  if(WiFi.isConnected()){
      lcd.setCursor(6,1);
    lcd.print("STATUS:200");
    }else{
     
    lcd.setCursor(6,1);
    lcd.print("STATUS:404");
    }
    
      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
      }
  
  

int increment=0;
int n = 0;
int isChecked;
String isActivated;
String user;
void loop() {
  
  if( isChecked==0){
   user= Firebase.getString("Arduino/ActivateCode/User");
isActivated= Firebase.getString("Arduino/ActivateCode/Activate");
 isChecked=1;
    }
   long irValue = particleSensor.getIR();
 if( isActivated=="1"){
  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
    
  }
  
 // 
  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);
  lcd.setCursor(0,0);
  lcd.print("UserID:");
  lcd.setCursor(7,0);
  lcd.print(user);
  lcd.setCursor(0,1);
  lcd.print("BPM=");
  lcd.setCursor(4,1);
  lcd.print(beatAvg);
if(increment==1000){
    Firebase.setInt("users/"+user+"/BPM",beatAvg);
    increment=0;
    }

   if (irValue < 50000)
    Serial.print(" No finger?");
  Serial.println();
 
  }else{
   
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("STATUS:404");
 }
increment++;
}
 /* 
  // set bool value
  Firebase.setBool("Arduino/messageArduino/truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("Arduino/logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);*/
 // set value
/*  
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // update value
  Firebase.setFloat("Arduino/messageArduino/numbers", 43.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // get value 
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remove value
  Firebase.remove("Arduino/messageArduino/number");
  delay(1000);
*/
