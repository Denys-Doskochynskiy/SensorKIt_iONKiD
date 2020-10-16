#include "MAX30105.h"

#include "heartRate.h"
void heart_rate(){
  
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
  
 /* 
  Serial.print("IR=");
  Serial.print(irValue);
   
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);
   Serial.print("\n");*/
  lcd.setCursor(0,0);
  lcd.print("UserID:");
  lcd.setCursor(7,0);
  lcd.print(user);
  lcd.setCursor(0,1);
  lcd.print("BPM=");
  lcd.setCursor(4,1);
  lcd.print(beatAvg);
if(increment==1000){
    Firebase.setInt("users/"+stringTwo+"/kids/"+kidsId+"/SKIP/BPM",beatAvg);
    increment=0;
    }

   if (irValue < 50000){
    Serial.print(" No finger?");
  Serial.println();}
 
  }
