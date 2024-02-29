#include <Arduino.h>

unsigned long timeout60_var = 0;
unsigned long timeout1_var = 0;
unsigned long timeout2_var = 0;
bool firstrun = true;


bool timeout60() {
    if (firstrun) {
        timeout60_var = millis();
        timeout1_var = millis();
        timeout2_var = millis();
        firstrun = false;
        return true;
    }
    if (millis() > (timeout60_var + 10000)) {
        timeout60_var = millis();
        return true;
    } else {
        return false;
    }
}

bool timeout1() {
    if (millis() > (timeout1_var + 1000)) {
        timeout1_var = millis();
        return true;
    } else {
        return false;
    }
}

bool timeout2() {
    if (millis() > (timeout2_var + 5000)) {
        timeout2_var = millis();
        return true;
    } else {
        return false;
    }
}