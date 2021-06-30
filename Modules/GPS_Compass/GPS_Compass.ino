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

HMC5883L compass;

#include "lib.h"
#define RED A1
#define GREEN A0
#define BUTTON A3

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);

  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  // Initialize Initialize HMC5883L
  while (!compass.begin())
  {
    delay(500);
  }

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);
  pinMode(BUTTON, INPUT_PULLUP);
  delay(10);
  if (!digitalRead(BUTTON) ) {
    CalibrateCompass();
  }
  compass.setOffset((int)eeprom_read_word((const uint16_t*)5), (int)eeprom_read_word((const uint16_t*)10));
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

  smartDelay(1000);
}

float calculateCompass() {
  Vector norm = compass.readNormalize();

  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }

  // Convert to degrees
  return (float) (heading * 180 / M_PI);
}


void CalibrateCompass() {
  int minX = 0;
  int maxX = 0;
  int minY = 0;
  int maxY = 0;
  int offX = 0;
  int offY = 0;

  Vector mag = compass.readRaw();

  minX = mag.XAxis;
  maxX = mag.XAxis;
  minY = mag.YAxis;
  maxY = mag.YAxis;

  for (int i = 0; i < 400; i++) {
    Vector mag = compass.readRaw();

    if (mag.XAxis < minX) minX = mag.XAxis;
    if (mag.XAxis > maxX) maxX = mag.XAxis;
    if (mag.YAxis < minY) minY = mag.YAxis;
    if (mag.YAxis > maxY) maxY = mag.YAxis;
    digitalWrite(RED, 1);
    digitalWrite(GREEN, 1);
    delay(20);
    digitalWrite(RED, 0);
    digitalWrite(GREEN, 0);

  }

  offX = (maxX + minX) / 2;
  offY = (maxY + minY) / 2;
  compass.setOffset(offX, offY);

  eeprom_update_word(5, offX);
  eeprom_update_word(10, offY);
}
