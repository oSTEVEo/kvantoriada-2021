#include <SoftwareSerial.h>

SoftwareSerial nextion(2, 3); //RX, TX
#define DEBUG 0

#include "heder.h"
#include "function.h"

void setup() {
  Serial.begin(9600);
  nextion.begin(9600);
  if (DEBUG) {
    Serial.println("AAA");
  }
}

long time = 0;

void loop() {
  myLisn();
  if (bufer[3] == 1) {
    findComand();
    bufer[3] = 0;
  }

  if (time + 3000 < millis()) {
    time = millis();
    getInfo();
  }
}


/*
  void writeToOther() {
  textToNextion("t3", "GpsData");
  textToNextion("t5", actionStatus);
  textToNextion("t6", dataBase);
  }*/

void listening() {
  while (Serial.available() > 0) {
    nextion.print((char) Serial.read());
    delay(1);
  }
}
