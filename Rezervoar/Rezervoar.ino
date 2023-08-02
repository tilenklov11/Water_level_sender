#include <SPI.h>
#include <LoRa.h>
#include "ds3231.h"
//#include <WiFi.h>
//#include <SD.h>

#define PROBE_POWER 14 // 3.3V to probe
#define PROBE_PIN_1 4 // 25% probe
#define PROBE_PIN_2 2 // 50% probe
#define PROBE_PIN_3 15 // 75% probe
#define PROBE_PIN_4 13 // 100% probe
#define BAT_V 35 // Battery voltage 100k/100k divider
#define LORA_SENDER 1

void setup()
{  
    pinMode(PROBE_POWER, OUTPUT); 
    pinMode(PROBE_PIN_1, INPUT);
    pinMode(PROBE_PIN_2, INPUT);
    pinMode(PROBE_PIN_3, INPUT);
    pinMode(PROBE_PIN_4, INPUT);
    pinMode(BAT_V, INPUT);
    //Serial.begin(9600);
    while (!Serial);

 

    SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
    LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
    if (!LoRa.begin(BAND)) {
       // Serial.println("Starting LoRa failed!");
        while (1);
    }
    if (!LORA_SENDER) {
        
    }
    LoRa.setSpreadingFactor(12);
}



void loop()
{
    digitalWrite(PROBE_POWER, HIGH); 
    delay(1000);
    float BAT_VOL = (float)(analogRead(BAT_V)) * 3600 / 4095 * 2 / 1000 - 0.11;
    int probeState1 = digitalRead(PROBE_PIN_1);
    int probeState2 = digitalRead(PROBE_PIN_2);
    int probeState3 = digitalRead(PROBE_PIN_3);
    int probeState4 = digitalRead(PROBE_PIN_4);
    int waterLevel1 = probeState1 == HIGH ? 100 : 0;
    int waterLevel2 = probeState2 == HIGH ? 100 : 0;
    int waterLevel3 = probeState3 == HIGH ? 100 : 0;
    int waterLevel4 = probeState4 == HIGH ? 100 : 0;
    //Serial.println(waterLevel1);
    digitalWrite(PROBE_POWER, LOW); 
    delay(200);

#if LORA_SENDER
    if (1 == 1) {
        
        LoRa.beginPacket();
        LoRa.print((waterLevel1/100));
        LoRa.print((waterLevel2/100));
        LoRa.print((waterLevel3/100));
        LoRa.print((waterLevel4/100));
        LoRa.print(",");
        LoRa.print(BAT_VOL);
        LoRa.print("V");
        LoRa.endPacket();
    } 
    
#endif

 ESP.deepSleep(3600e6);

}
