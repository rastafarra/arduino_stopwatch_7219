#include <Arduino.h>
#include <SPI.h>

#include "GyverMAX7219.h"

#define AM_W 32  // 4 матрицы (32 точки)
#define AM_H 16  // 2 матрицы (16 точек)

MAX7219< 4, 1, 5 > mtrx;

const byte buttonPin = 8;
const byte resetPin = 9;
int readedButtonValue = 42;

byte sec = 1;
byte mSec = 1;
byte minute = 1;
byte state = 1;

char bMin[2];
char bSec[2];
char bmSec[2];

String sMin;
String sSec;
String sMSec;

int BTTN = 8;
bool STOP = false;

// void ButtonRising() {
//   BTTN = false;
//   Serial.println("Button is HIGH, stop timer");
// }

void ButtonFalling() {
  BTTN = 1;
  // Serial.println("Button is LOW, start timer");
}

void ResetRising() {
  STOP = false;
  BTTN = 1;
  state = 0;
  minute = 0;
  sec = 0;
  mSec = 0;
}

void ResetFalling() {
  STOP = false;
  BTTN = 42;
  state = 0;
  minute = 0;
  sec = 0;
  mSec = 0;

  updateDisplay();
}



void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(resetPin, INPUT);

  sec = 0;
  mSec = 0;
  minute = 0;
  state = 1;
  BTTN = 42;

  mtrx.begin();
  mtrx.update();
  mtrx.setBright(7);

  updateDisplay();
}



void loop() {
  // mtrx.clear();

  // mtrx.setCursor(2, 0);
  // mtrx.print("2347");
  // mtrx.update();

  if (0 != millis() % 100)
    return;

  readedButtonValue = digitalRead(buttonPin);

  if (1 == readedButtonValue and 42 == BTTN)
    BTTN = 1;

  if (0 == readedButtonValue and 1 == BTTN)
    BTTN = 0;

  if (1 == digitalRead(resetPin) and 0 == readedButtonValue)
    ResetFalling();

  if (1 == BTTN) {
    mSec += 1;

    if (mSec > 9) {
      mSec = 0;
      sec += 1;
    }

    if (sec > 59) {
      sec = 0;
      minute += 1;
    }

    if (minute > 99) {
      minute = 0;
    };

    updateDisplay();
  }
}

void updateDisplay() {
  sprintf(bMin, "%0.2d", minute);
  sMin = String(bMin);
  // sMin = "88";

  sprintf(bSec, "%0.2d", sec);
  sSec = String(bSec);

  sprintf(bmSec, "%d", mSec);
  sMSec = String(bmSec);


  mtrx.clear();

  mtrx.setCursor(2, 0);
  mtrx.print(sMin);

  mtrx.dot(12, 2);
  mtrx.dot(12, 5);

  mtrx.setCursor(14, 0);
  mtrx.print(sSec);

  mtrx.dot(24, 2);
  mtrx.dot(24, 5);

  mtrx.setCursor(26, 0);
  mtrx.print(sMSec);

  mtrx.update();
}
