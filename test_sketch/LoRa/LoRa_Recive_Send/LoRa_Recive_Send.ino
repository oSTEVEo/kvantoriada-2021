#include <SPI.h>
#include <LoRa.h>

#define LED 13

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Terminal");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  String packet = ParsePacket();
  if (packet != "")
    Serial.print("[RECIVE] " + packet);
    
  if (Serial.available()) {
    String str = Serial.readString();
    LoRa.beginPacket();
    LoRa.print(str);
    LoRa.endPacket(1);
    Serial.print("[SEND] " + str);
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

void fail() {
  while(1) {
    digitalWrite(LED, !digitalRead(LED));
    delay(500);
  }
}

//LoRa.packetRssi();
