//#define MY_DEBUG
//#define MY_RADIO_RF24
//#define MY_NODE_ID 254
//#define MY_TRANSPORT_UPLINK_CHECK_DISABLED
//#include <MySensors.h>
//#define SKETCH_NAME "LogSplitter"
//#define SKETCH_VERSION "v0.1"
//#define ID_S_TEMP 1
//MyMessage msg_S_TEMP(ID_S_TEMP, V_TEMP);

float steinhart = 0;

// data to be sent and received
struct I2cTxStruct {
    char textA[16];         // 16 bytes
    int valA;               //  2
    unsigned long valB;     //  4
    byte padding[10];       // 10
                            //------
                            // 32
};

struct I2cRxStruct {
    char textB[16];         // 16 bytes
    int valC;               //  2
    unsigned long valD;     //  4
    byte padding[10];       // 10
                            //------
                            // 32
};

I2cTxStruct txData = {"xxx", 236, 0};
I2cRxStruct rxData;

bool newTxData = false;
bool newRxData = false;
bool rqData = false;


        // I2C control stuff
#include <Wire.h>

const byte thisAddress = 8; // these need to be swapped for the other Arduino
const byte otherAddress = 9;


        // timing variables
unsigned long prevUpdateTime = 0;
unsigned long updateInterval = 500;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void updateDataToSend() {

    if (millis() - prevUpdateTime >= updateInterval) {
        prevUpdateTime = millis();
        if (newTxData == false) { // ensure previous message has been sent

            char sText[] = "xxx";
            strcpy(txData.textA, sText);
            txData.valA += 10;
            if (txData.valA > 300) {
                txData.valA = 0;
            }
            txData.valB = millis();
            newTxData = true;
        }
    }
}

void transmitData() {

    if (newTxData == true) {
        Wire.beginTransmission(otherAddress);
        Wire.write((byte*) &txData, sizeof(txData));
        Wire.endTransmission();    // this is what actually sends the data

            // for demo show the data that as been sent
        Serial.print("Sent ");
        Serial.print(txData.textA);
        Serial.print(' ');
        Serial.print(txData.valA);
        Serial.print(' ');
        Serial.println(txData.valB);

        newTxData = false;
        rqData = true;
    }
}

void requestData() {
    if (rqData == true) { // just one request following every Tx
        byte stop = true;
        byte numBytes = 32;
        Wire.requestFrom(otherAddress, numBytes, stop);
            // the request is immediately followed by the read for the response
        Wire.readBytes( (byte*) &rxData, numBytes);
        newRxData = true;
        rqData = false;
    }
}

void showNewData() {

    Serial.print("This just in    ");
    Serial.print(rxData.textB);
    Serial.print(' ');
    Serial.print(rxData.valC);
    Serial.print(' ');
    Serial.println(rxData.valD);
}

float Steinhart() {
        int ADCvalue = 0;
        const float seriesResistor = 996.0;
        const float Vcc = 4.9;
        #define NTCPin A3
        pinMode(NTCPin,INPUT);
        for (int n = 0; n < 10; n++){
            delayMicroseconds(10); //this increased resolution signifigantly
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