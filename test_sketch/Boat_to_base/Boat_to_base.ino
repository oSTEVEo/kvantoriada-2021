#include <SoftwareSerial.h>

#define SERIAL_TYPE SoftwareSerial

SERIAL_TYPE mySerial(2, 3); //RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}

struct data {
  float lat;
  float lng;
  int sost;
  int term;
};

struct data curentData;

void sendData() {
  mySerial.print(curentData.lat);
  mySerial.print('-');
  mySerial.print(curentData.lng);
  mySerial.print(' ');
  mySerial.print(curentData.sost);
  mySerial.print(' ');
  mySerial.print(curentData.term);
  mySerial.print(';');
}

void loop() {
  char c;
  if (mySerial.available() > 0) {
    c = (mySerial.read());

    //обработка сигналов с датчиков робота (GPS, компас и т.д.)
    curentData.lat = 100000 + millis() % 1000;
    curentData.lng = 200000 + millis() % 1000;
    curentData.sost = curentData.sost + 0;
    curentData.term = 400 + millis() % 10;
    switch (c) {
      case 'S':
        //send curent sost
        sendData();
        break;
      case 'A':
        //go gome
        curentData.sost = 2;
        break;
      case 'E':
        //alarm stop
        curentData.sost = 3;
        break;
      case 'R':
        //right motor go
        break;
      case 'L':
        //left motor go
        break;
      case 'Z':
        //chill
        break;
    }
  }

}
