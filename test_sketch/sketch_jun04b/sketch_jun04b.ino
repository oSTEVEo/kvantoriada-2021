//gps
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define RXPin 3
#define TXPin 4
#define GPSBaud 9600

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

//compass
#include <Wire.h>
#include <HMC5883L.h>
#include <avr/eeprom.h>

HMC5883L compass;
float MagnitCorrect = -0.06785;
long tmr = millis();

#include "serialPort.h"
#define RED A1
#define GREEN A0
#define BUTTON A3   

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  while (!compass.begin())
  {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }

  compass.setRange(HMC5883L_RANGE_1_3GA);
  compass.setMeasurementMode(HMC5883L_CONTINOUS);                   // Set measurement mode
  compass.setDataRate(HMC5883L_DATARATE_30HZ);                      // Set data rate
  compass.setSamples(HMC5883L_SAMPLES_8);                           // Set number of samples averaged

  compass.setOffset(eeprom_read_byte(5), eeprom_read_byte(10));     //calibration  (-651, 570)

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  //indication
  if (gps.location.isValid()) {
    digitalWrite(RED, 0);
    digitalWrite(GREEN, 1);
  }
  else {
    digitalWrite(RED, 1);
    digitalWrite(GREEN, 0);
  }

  //gps
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);

  //compass
  printFloat(calculateCompass(), 1, 5, 6);
  Serial.println();

  //callibrate Menu
  if (digitalRead(BUTTON) && millis() - tmr > 100) {
    calibrateCompass();
    tmr = millis();
  }

  smartDelay(1000);
}

float calculateCompass() {
  Vector norm = compass.readNormalize();
  float heading = atan2(norm.YAxis, norm.XAxis);  // Calculate heading

  if (heading < 0)
    heading += 2 * PI;
  if (heading > 2 * PI)
    heading -= 2 * PI;

  return heading * 180 / M_PI;;
}

void calibrateCompass() { //калибровка компаса//
  digitalWrite(GREEN, 0);
  for(int i = 0; i < 50; i++) {
    digitalWrite(RED, 1);
    delay(20);
    digitalWrite(RED, 0);
    delay(20);
  }
  
  Vector mag = compass.readRaw();

  int minX = mag.XAxis;
  int maxX = mag.XAxis;
  int minY = mag.YAxis;
  int maxY = mag.YAxis;
  int offX = 0;
  int offY = 0;

  bool flag = 0;
  long timer = millis();

  while (!digitalRead(BUTTON)) {
    mag = compass.readRaw();

    // Determine Min / Max values
    if (mag.XAxis < minX) minX = mag.XAxis;
    if (mag.XAxis > maxX) maxX = mag.XAxis;
    if (mag.YAxis < minY) minY = mag.YAxis;
    if (mag.YAxis > maxY) maxY = mag.YAxis;

    // Calculate offsets
    offX = (maxX + minX) / 2;
    offY = (maxY + minY) / 2;

    if (millis() - timer > 450) {
      if (flag) {
        digitalWrite(RED, 0);
        digitalWrite(GREEN, 1);
      }
      else {
        digitalWrite(RED, 1);
        digitalWrite(GREEN, 0);
      }
      timer = millis();
      flag = !flag;
    }
  }
  eeprom_write_byte(5, offX);
  eeprom_write_byte(10, offY);
  compass.setOffset(eeprom_read_byte(5), eeprom_read_byte(10));
}
