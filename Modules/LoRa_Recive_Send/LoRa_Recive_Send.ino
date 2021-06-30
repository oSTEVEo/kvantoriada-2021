#include <SPI.h>
#include <LoRa.h>

#define RED A3
#define GREEN A5

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  delay(2);
  //Serial.println("LoRa Terminal");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
}

unsigned long tmr = millis();

void loop() {
  String packet = ParsePacket();
  if (packet != "") {
    Serial.print(packet);
    tmr = millis();
    digitalWrite(RED, 0);
    digitalWrite(GREEN, 1);
  }
  else {
    if (tmr + 10000 < millis()) {
      digitalWrite(RED, 1);
      digitalWrite(GREEN, 0);
    }
  }
  if (Serial.available()) {
    sendPacket(Serial.readString());
  }
}

String ParsePacket() {
  int packetSize = LoRa.parsePacket();
  bool errorFlag = false;
  String packet;
  if (packetSize) {
    while (LoRa.available()) {
      char symbol = (char)LoRa.read();
      if ((symbol >= '0' && symbol <= '9')
          || (symbol >= 'A' && symbol <= 'Z')
          || (symbol == '.' || symbol == ' ' || symbol == '-' || symbol == ';' || symbol == ',')) { //||(symbol >= 'a' && symbol <= 'z')
        packet += symbol;
      }
      else {
        errorFlag = true;
      }
    }
    //TODO: повтор
  }
  if (errorFlag) {
    return "";
  }
  else {
    return packet;
  }
}


void sendPacket(String str) {
  LoRa.beginPacket();
  LoRa.print(str);
  LoRa.endPacket(1);
}

//LoRa.packetRssi();
