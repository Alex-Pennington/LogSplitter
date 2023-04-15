#include <Arduino.h>
#include "main.hpp"
#include "timeout.hpp"
#include "buttons.hpp"

void presentation() {
  present(ID_S_TEMP, S_TEMP, "HF");
}

void setup() {
  Serial.begin(9600);
  // BUTTON SETUP 
  
  // SELECT ONE OF THE FOLLOWING :
  // 1) IF YOUR BUTTON HAS AN INTERNAL PULL-UP
  // button.attach( BUTTON_PIN ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  // 2) IF YOUR BUTTON USES AN EXTERNAL PULL-UP
  button_A.attach( A_BUTTON_PIN, INPUT ); // USE EXTERNAL PULL-UP
  button_B.attach( B_BUTTON_PIN, INPUT ); // USE EXTERNAL PULL-UP
  button_C.attach( C_BUTTON_PIN, INPUT ); // USE EXTERNAL PULL-UP
  button_D.attach( D_BUTTON_PIN, INPUT ); // USE EXTERNAL PULL-UP
  button_E.attach( E_BUTTON_PIN, INPUT ); // USE EXTERNAL PULL-UP
  button_F.attach( F_BUTTON_PIN, INPUT ); // USE EXTERNAL PULL-UP
  button_X.attach( X_BUTTON_PIN, INPUT ); // USE EXTERNAL PULL-UP

  // DEBOUNCE INTERVAL IN MILLISECONDS
  button_A.interval(5); 
  button_B.interval(5); 
  button_C.interval(5); 
  button_D.interval(5); 
  button_E.interval(5); 
  button_F.interval(5); 
  button_X.interval(5); 

  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button_A.setPressedState(LOW);
  button_B.setPressedState(LOW);
  button_C.setPressedState(LOW);
  button_D.setPressedState(LOW);
  button_E.setPressedState(LOW);
  button_F.setPressedState(LOW);
  button_X.setPressedState(LOW);

  /*
  Output Goal Max 4rpm
  4 * 3.3 (Sprokets) = 13.2
  13.2 * 100 (gearbox) = 1320 rpm
  1320 rpm / 60 (sec/min) = 22 rps
  22 rps * 200 (steps/rev) = 4400 steps/sec
  */
  
  lcd.init();                      // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Starting...");
  delay(5000);
  //  Serial.begin(115200);
  //  while (!Serial) {
  //    delay(10);
  //  }

  /* Initialise the driver with I2C_ADDRESS and the default I2C bus. */
  if (! mcp.begin(I2C_ADDRESS)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("mcp not found");
    while (1);
  }

  // Serial.println("Found MCP9600!");

  mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
  //Serial.print("ADC resolution set to ");
  /*
  switch (mcp.getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:   Serial.print("18"); break;
    case MCP9600_ADCRESOLUTION_16:   Serial.print("16"); break;
    case MCP9600_ADCRESOLUTION_14:   Serial.print("14"); break;
    case MCP9600_ADCRESOLUTION_12:   Serial.print("12"); break;
  }
  */
  //Serial.println(" bits");

  mcp.setThermocoupleType(MCP9600_TYPE_K);
  //Serial.print("Thermocouple type set to ");
  /*
  switch (mcp.getThermocoupleType()) {
    case MCP9600_TYPE_K:  Serial.print("K"); break;
    case MCP9600_TYPE_J:  Serial.print("J"); break;
    case MCP9600_TYPE_T:  Serial.print("T"); break;
    case MCP9600_TYPE_N:  Serial.print("N"); break;
    case MCP9600_TYPE_S:  Serial.print("S"); break;
    case MCP9600_TYPE_E:  Serial.print("E"); break;
    case MCP9600_TYPE_B:  Serial.print("B"); break;
    case MCP9600_TYPE_R:  Serial.print("R"); break;
  }
  */
  //Serial.println(" type");

  mcp.setFilterCoefficient(3);
  //Serial.print("Filter coefficient value set to: ");
  //Serial.println(mcp.getFilterCoefficient());

  mcp.setAlertTemperature(1, 30);
  //Serial.print("Alert #1 temperature set to ");
  //Serial.println(mcp.getAlertTemperature(1));
  //mcp.configureAlert(1, true, true);  // alert 1 enabled, rising temp

  mcp.enable(true);

  //Serial.println(F("------------------------------"));
}

void loop() {
  if (timeout60()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    tempF = (mcp.readThermocouple() * 1.8) + 32.0;
    send(msg_S_TEMP.set((int)tempF));

    lcd.print(tempF);
    lcd.print(" F");
    // go to line #1
    lcd.setCursor(0, 1);
  }

  if (timeout1()) {
   if (backlight_state == true) {
    lcd.setBacklight(100);
   } else {
    lcd.setBacklight(0);
   }
  }

  buttons();

  if (Serial.available()) {     //wait for data available
    String teststr = Serial.readString();  //read until timeout
    teststr.trim();
    lcd.setCursor(0, 1);
    lcd.print(teststr);
  }
}