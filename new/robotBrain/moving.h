#define INVERT_LEFT false
#define INVERT_RIGHT true

#define MOTOR_SPEED 50
#define MOTOR_K 1

byte maxSpeed = 200;
float startAngle;

void RMotor (int speed) {
  speed = constrain(speed, -255, 255);
  if (INVERT_RIGHT) {
    speed *= -1;
  }
  if (speed < 0) {
    digitalWrite(D1, LOW);
    analogWrite(M1, speed * -1);
  }
  else if (speed > 0) {
    digitalWrite(D1, HIGH);
    analogWrite(M1, 255 - speed);
  }
  else {
    digitalWrite(D1, LOW);
    digitalWrite(M1, LOW);
  }
}

void LMotor (int speed) {
  speed = constrain(speed, -255, 255);
  if (INVERT_LEFT) {
    speed *= -1;
  }
  if (speed < 0) {
    digitalWrite(D2, LOW);
    analogWrite(M2, speed * -1);
  }
  else if (speed > 0) {
    digitalWrite(D2, HIGH);
    analogWrite(M2, 255 - speed);
  }
  else {
    digitalWrite(D2, LOW);
    digitalWrite(M2, LOW);
  }
}

void GoFor() {
  curentData.dist=distanceEarth(hp.lat, hp.lng, curentData.lat, curentData.lng) * 1000;
  if (distanceEarth(hp.lat, hp.lng, curentData.lat, curentData.lng) * 1000 > 25) {
    curentData.sost = GoHome;
  }
  else {
    int tangle = (startAngle - 180) - (curentData.angle - 180);

    if (tangle > 180) {
      tangle = tangle - 360;
    } else if (tangle < -180) {
      tangle = tangle + 360;
    }
    curentData.temp = tangle;

    if (abs(tangle) < 20) {
      RMotor(MOTOR_SPEED);
      LMotor(MOTOR_SPEED);
      return;
    }
    RMotor(MOTOR_SPEED - tangle * MOTOR_K);
    LMotor(MOTOR_SPEED + tangle * MOTOR_K);
  }
}

void GoToBase() {
    curentData.dist=distanceEarth(hp.lat, hp.lng, curentData.lat, curentData.lng) * 1000;
    if (distanceEarth(hp.lat, hp.lng, curentData.lat, curentData.lng) * 1000 < 5) {
    curentData.sost = SBak;
    }
    else {
    float heading = rad2deg(atan2(hp.lat - curentData.lat, hp.lng - curentData.lng));
    float angleP = curentData.angle - heading;


    if (angleP > 180) {
      angleP = angleP - 360;
    } else if (angleP < -180) {
      angleP = angleP + 360;
    }
    curentData.temp = angleP;

    if (abs(angleP) < 20) {
      RMotor(MOTOR_SPEED);
      LMotor(MOTOR_SPEED);
      return;
    }
    RMotor(MOTOR_SPEED - angleP * MOTOR_K);
    LMotor(MOTOR_SPEED + angleP * MOTOR_K);
  }
}
