#include <Arduino.h>
#include "main.hpp"
#include "timeout.hpp"
#include "buttons.hpp"

//void presentation() {
//  present(ID_S_TEMP, S_TEMP, "HF");
//}

void setup() {
  Serial.begin(9600);
  Serial.print("test");

  // set up I2C
  Wire.begin(thisAddress); // join i2c bus
  //~ Wire.onReceive(receiveEvent); // register function to be called when a message arrives


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
  delay(1000);
  //  Serial.begin(115200);
  //  while (!Serial) {
  //    delay(10);
  //  }

  
}

void loop() {

// this bit checks if a message has been received
  if (newRxData == true) {
    showNewData();
    newRxData = false;
  }
  // this function updates the data in txData
  updateDataToSend();
  // this function sends the data if one is ready to be sent
  transmitData();
  requestData();


  if (timeout60()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    float tempF = Steinhart();
    //send(msg_S_TEMP.set((int)tempF));

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