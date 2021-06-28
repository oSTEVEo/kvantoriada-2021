#include <SPI.h>
#include <LoRa.h>

#define RED 6
#define GREEN 5

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  while (!Serial);
  Serial.println("LoRa Terminal");

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
    String str = Serial.readString();
    LoRa.beginPacket();
    LoRa.print(str);
    LoRa.endPacket(1);
  }
}

String ParsePacket() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String packet;
    while (LoRa.available()) {
      packet += (char)LoRa.read();
    }
    return packet;
  }
  return "";
}

//LoRa.packetRssi();
