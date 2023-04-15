//#define MY_DEBUG
#define MY_RADIO_RF24
#define MY_NODE_ID 254
#define MY_TRANSPORT_UPLINK_CHECK_DISABLED
#include <MySensors.h>
#define SKETCH_NAME "LogSplitter"
#define SKETCH_VERSION "v0.1"
#define ID_S_TEMP 1
MyMessage msg_S_TEMP(ID_S_TEMP, V_TEMP);

float steinhart = 0;

#include <Wire.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

float Steinhart() {
        int ADCvalue = 0;
        const float seriesResistor = 996.0;
        const float Vcc = 4.9;
        #define NTCPin A3
        pinMode(NTCPin,INPUT);
        for (int n = 0; n < 10; n++){
            wait(10); //this increased resolution signifigantly
            ADCvalue += analogRead(NTCPin);
        }
        ADCvalue /= 10;
        float Va3 = (1.0 - ((float)ADCvalue / 1023.0)) * Vcc;
        float thermistorResistance = (seriesResistor * Va3) / (Vcc - Va3);
        float tempF = (-0.259 * thermistorResistance) + 228.554;
        // Serial.print("ADCvalue ");
        // Serial.println(ADCvalue);
        // Serial.print("Va3 ");
        // Serial.println(Va3);
        // Serial.print("thermistorResistance ");
        // Serial.println(thermistorResistance);
        // Serial.print("tempF ");
        // Serial.println(tempF);
        return tempF;
}