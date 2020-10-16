 #include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>       
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "ADS1X15.h"
#include <Wire.h> 
#include "MAX30105.h"

#include "heartRate.h"
#include <LiquidCrystal_I2C.h>
MAX30105 particleSensor;
#include <EEPROM.h>
#define FIREBASE_HOST "ionkid-abd2f.firebaseio.com"
#define FIREBASE_AUTH "12Bs4sG60U96pN0VqAYxIRzKmo55omcTIUPejNZF"
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
const byte RATE_SIZE = 7; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
long irValue;
float beatsPerMinute;
int beatAvg;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


ADS1115 ADS(0x48);

 
//Function Decalration

 
//Establishing Local server at port 80 whenever required
 ESP8266WebServer server(80);
 String kidsId;
String stringTwo;
const char* ssid = "text";
const char* passphrase = "text";
String st;
String content;
 bool testWifi(void);
void launchWeb(void);
void setupAP(void);
void noise_sensor(void);
void setup() { 
  Serial.begin(9600);
   Serial.println();
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
 
  //---------------------------------------- Read EEPROM for SSID and pass
Serial.println("Reading EEPROM ssid");
 
  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);

  
  Serial.println("Reading EEPROM pass");
  String epass = "";
  for (int i = 32; i < 64; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);

Serial.println("Reading EEPROM userID");
  String eUserID = "";
  for (int i = 64; i < 96; ++i)
  {
    eUserID += char(EEPROM.read(i));
  }
  Serial.print("USER_ID: ");
 String testString =eUserID.c_str();
  stringTwo = testString;
  // then perform the replacements:
  stringTwo.replace(".", ",");
  Serial.println(stringTwo);
Serial.println("Reading EEPROM deviceID");
  String eKidsID = "";
  for (int i = 96; i <128; ++i)
  {
    eKidsID += char(EEPROM.read(i));
  }
   String kids=eKidsID.c_str();
   kidsId=kids;
  Serial.print("KIDS_ID: ");
  Serial.println(eKidsID);
 
  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {
     if(WiFi.isConnected()){
    Serial.println("Succesfully Connected!!!");
     Serial.println(WiFi.localIP());
      Serial.println("STATUS:200");
   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    /* if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("MAX30105 not found");
    while (1);
  }*/
   Firebase.setString("users/"+stringTwo+"/KidInfo/"+kidsId+"/SKIP/status","200 wifi is connected");
   delay(2000);
 
  }
 Serial.println("Place your index finger on the sensor with steady pressure.");
// particleSensor.setup(); //Configure sensor with default settings
 //particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
 //particleSensor.setPulseAmplitudeGreen(0);
lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  
 
ADS.begin();
   lcd.clear();
 
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
    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    launchWeb();
    setupAP();// Setup HotSpot
  }
 
  Serial.println();
  Serial.println("Waiting.");
  
  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }
  
}
  
  

int increment=0;
int n = 0;
int isChecked;
String isActivated;
String user;
void loop() {
  /*
  if( isChecked==0){
   user= Firebase.getString("Arduino/ActivateCode/User");
isActivated= Firebase.getString("Arduino/ActivateCode/Activate");
 isChecked=1;
    }
  
 if( isActivated=="1"){

 } else{
   
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("STATUS:404");
 }*/
  /* noise_sensor();
  irValue = particleSensor.getIR();
  heart_rate();*/
 Firebase.setInt("users/"+stringTwo+"/KidInfo/"+kidsId+"/SKIP/LogTest",increment);
 delay(1000);
increment++;
}


 
