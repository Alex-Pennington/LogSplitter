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
        //https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html
        const float A = 1.499168475e-3;
        const float B = 2.766247366e-4;
        const float C = 0.2413822162e-7;

        float logR = log(thermistorResistance);
        float Kelvin = 1.0 / (A + B * logR + C * logR * logR * logR);
        float tempF = (Kelvin - 273.15) * (9.0/5.0) + 32.0;
        /* Debug Values
        Serial.print("ADCvalue ");
        Serial.println(ADCvalue);
        Serial.print("Va3 ");
        Serial.println(Va3);
        Serial.print("thermistorResistance ");
        Serial.println(thermistorResistance);
        Serial.print("A ");
        Serial.println((double)A,10);
        Serial.print("B ");
        Serial.println((double)B,10);
        Serial.print("C ");
        Serial.println((double)C,10);
        Serial.print("logR ");
        Serial.println(logR);
        Serial.print("Kelvin ");
        Serial.println(((double)Kelvin),10);
        Serial.print("tempF ");
        Serial.println((double)tempF,10);
        Serial.println("------------------------");
        */
        return tempF;
}