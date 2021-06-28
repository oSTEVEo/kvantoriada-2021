#include "util.h"

void setup() {
  Serial.begin(9600);
  Serial.println(distanceEarth(12, 12, 14, 14));
}

void loop() {
}
