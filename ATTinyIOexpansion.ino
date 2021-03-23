//ATTINY1614
//reference https://github.com/SpenceKonde/megaTinyCore/blob/master/megaavr/extras/ATtiny_x14.md
//Install here: https://github.com/SpenceKonde/megaTinyCore
//tested running at 3.3V 10MHz

#include <Wire.h>

const int I2Caddress = 3;// 1.. set to I2C slave address

const int LEDpins[] = {0, 1, 2, 3};
const int BUTTONpins[] = {10, 9, 4, 5};

void setup() {
  Wire.begin(I2Caddress);        // join i2c bus
  Wire.onReceive(receiveEvent); // register event for setting LEDs
  Wire.onRequest(requestEvent); // register event for getting buttons

  // setting up pins
  for (int i = 0; i < 4; i++) {
    pinMode(LEDpins[i], OUTPUT);
    pinMode(BUTTONpins[i], INPUT_PULLUP);
  }
}

void loop() {
  //not much here - alll in the callbacks below
}

// WHEN MASTER ASKS FOR BUTTON STATUS
void requestEvent() {
  byte buttonReadByte = 0;
  for (int i = 0; i < 4; i++) {
    if (!digitalRead(BUTTONpins[i]))
      bitSet(buttonReadByte, i);
  }
  Wire.write(buttonReadByte);
}

// WHEN MASTER TELLS US TO TURN LEDS ON/OFF
void receiveEvent(int howMany) {
  byte LEDbyte = 0;
  LEDbyte = Wire.read();
// This Byte contains both pin number in first for bits, 
// then last 4 is corresponding status
// example to set first LED 00010001 then to turn off 00000001
  
  for (int i = 0; i < 4; i++) {//loop through byte and set LEDs
    if (bitRead(LEDbyte, i)) {
      digitalWrite(LEDpins[i], bitRead(LEDbyte, i + 4));
    }
  }
}
