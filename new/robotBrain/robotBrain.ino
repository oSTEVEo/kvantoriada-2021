#include <SoftwareSerial.h>
#include <DHT.h>
DHT dht(10, DHT11);

SoftwareSerial ss(2, 3);

#include "lib.h"
#include "moving.h"

void setup() {
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(CONV, OUTPUT);
  pinMode(CEP1, OUTPUT);
  pinMode(CEP2, OUTPUT);
  dht.begin();
  ss.begin(9600);

  pinMode (Trig, OUTPUT);
  pinMode (Echo, INPUT);

  pinMode(AG0, INPUT); // 1 - 470 Ом, другой 1000 Ом (Примерно 4.8В на выходе)

}

void loop() {
  readGps();
  if (curentData.sost == GoHome) {
    analogWrite(CONV, LOW);
    GoToBase();
  } else if (curentData.sost == AutoMode) {
    analogWrite(CONV, 200);
    GoFor();
  }

  if (gpCouter == 1) {
    hp.lat = curentData.lat;
    hp.lng = curentData.lng;
  }
  curentData.carg = (int)analogRead(AG0);

  char c;
  if (Serial.available() > 0) {
    c = (Serial.read());

    switch (c) {
      case 'S': {
          curentData.term = dht.readTemperature();

          unsigned int impulseTime = 0;
          unsigned int distance_sm = 0;
          digitalWrite (Trig, HIGH);
          delayMicroseconds (10);
          digitalWrite (Trig, LOW);
          impulseTime = pulseIn (Echo, HIGH);
          distance_sm = impulseTime / 58;
          curentData.bak = (int(distance_sm / 29.5f * 100));

          sendData();

          break;
        }
      case 'A':
        if (curentData.sost == AutoMode) {
          curentData.sost = GoHome;
        } else {
          curentData.sost = AutoMode;
          startAngle = curentData.angle;
        }
        break;
      case '
      ':
        //alarm stop
        digitalWrite(D1, LOW);
        analogWrite(M1, LOW);
        digitalWrite(D2, LOW);
        analogWrite(M2, LOW);
        digitalWrite(CONV, LOW);
        digitalWrite(CEP1, LOW);
        analogWrite(CEP2, LOW);

        curentData.sost = AlramStop;
        break;
      case 'R':
        digitalWrite(D1, LOW);
        analogWrite(M1, maxSpeed);
        break;
      case 'D':
        digitalWrite(D1, HIGH);
        analogWrite(M1, 255 - maxSpeed);
        break;

      case 'U':
        digitalWrite(D2, LOW);
        analogWrite(M2, maxSpeed);
        break;
      case 'L':
        digitalWrite(D2, HIGH);
        analogWrite(M2, 255 - maxSpeed);
        break;

      case 'Z':
        digitalWrite(D1, LOW);
        analogWrite(M1, LOW);
        break;
      case 'J':
        digitalWrite(D2, LOW);
        analogWrite(M2, LOW);
        break;
    }
  }
}
