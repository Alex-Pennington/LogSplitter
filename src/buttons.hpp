bool backlight_state = false;

#define A_BUTTON_PIN PIN2
#define B_BUTTON_PIN PIN3 
#define C_BUTTON_PIN PIN4 
#define D_BUTTON_PIN PIN5
#define E_BUTTON_PIN PIN6 
#define F_BUTTON_PIN PIN7 
#define X_BUTTON_PIN 8 

bool button_A_state = false;
//bool button_B_state = false;
bool button_C_state = false;
//bool button_D_state = false;
bool button_E_state = false;
bool button_F_state = false;
bool button_X_state = false;

#include <Bounce2.h>
// INSTANTIATE A Button OBJECT FROM THE Bounce2 NAMESPACE
Bounce2::Button button_A = Bounce2::Button();
Bounce2::Button button_B = Bounce2::Button();
Bounce2::Button button_C = Bounce2::Button();
Bounce2::Button button_D = Bounce2::Button();
Bounce2::Button button_E = Bounce2::Button();
Bounce2::Button button_F = Bounce2::Button();
Bounce2::Button button_X = Bounce2::Button();

void buttons() {
  button_A.update();  
  button_B.update();
  button_C.update();
  button_D.update();
  button_E.update();
  button_F.update();
  button_X.update();
  // <Button>.pressed() RETURNS true IF THE STATE CHANGED
  // AND THE CURRENT STATE MATCHES <Button>.setPressedState(<HIGH or LOW>);
  // WHICH IS LOW IN THIS EXAMPLE AS SET WITH button.setPressedState(LOW); IN setup()
  if ( button_A.changed() ) {
     if (button_A.isPressed()) {
      Serial.print("p");
     } 
  }
  if ( button_B.changed() ) {
    if (button_B.isPressed()) {
      Serial.print("1");
    } else {
      //Serial.print("s");
    }
  }
if ( button_C.changed() ) {
     if (button_C.isPressed()) {
      Serial.print("s");
    }
  }
  if ( button_D.changed() ) {
        if (button_D.isPressed()) {
      Serial.print("0");
    } else {
      //Serial.println("s");
    }
  }
  if ( button_E.changed() ) {
     if (button_E.isPressed()) {
      Serial.println("on");
     } else {
      Serial.println("off");
    }
  }if ( button_F.changed() ) {
     if (button_F.isPressed()) {
      backlight_state = !backlight_state;
    }
  }
  if ( button_X.changed() ) {
     if (button_X.isPressed()) {
      Serial.println("X");
     } else {
      Serial.println("x");
    }
  }
}