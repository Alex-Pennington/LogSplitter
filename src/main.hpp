//#define MY_DEBUG
#define MY_RADIO_RF24
#define MY_NODE_ID 254
#define MY_TRANSPORT_UPLINK_CHECK_DISABLED
#include <MySensors.h>
#define SKETCH_NAME "LogSplitter"
#define SKETCH_VERSION "v0.1"
#define ID_S_TEMP 1
MyMessage msg_S_TEMP(ID_S_TEMP, V_TEMP);

bool led_state = false;


#include <Wire.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"

#define I2C_ADDRESS (0x67)

Adafruit_MCP9600 mcp;
float tempF = 0;