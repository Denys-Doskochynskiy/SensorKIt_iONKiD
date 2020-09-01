#include "ADS1X15.h"
void noise_sensor(){
  ADS.setGain(0);

  int16_t val_0 = ADS.readADC(0);  
  if(val_0>1030||val_0<965){
    Serial.println("Чого ти так репиш))?");
    Serial.println(val_0);
    }
  }
