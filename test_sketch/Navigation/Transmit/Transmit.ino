#define PIN_ECHO1 A3
#define PIN_ECHO2 A4

#define LEFT 5
#define RIGHT 6

void setup() {
  Serial.begin (9600);

  pinMode(PIN_ECHO1, INPUT);
  pinMode(PIN_ECHO2, INPUT);
  pinMode(LEFT, OUTPUT);
  pinMode(RIGHT, OUTPUT);
}

float a1 = 0;
float a2 = 0;

void loop() {
  float s1 = 0;
  float s2 = 0;

  for (int i = 0; i < 400; i++) {
    s1 = s1 + analogRead(PIN_ECHO1);
    s2 = s2 + analogRead(PIN_ECHO2);
    delayMicroseconds(5);
  }

  //Время задержки акустического сигнала на эхолокаторе.
  a1 = a1 * 0.9 + (s1 / 400.0f) * 0.1;
  a2 = a2 * 0.9 + (s2 / 400.0f) * 0.1;

  Serial.print(a1);
  Serial.print("\t");
  Serial.println(a2);

  digitalWrite(LEFT, LOW);
  digitalWrite(RIGHT, LOW);

  if (abs(a1 - a2) < 3 && abs(a1 - a2) > 0) {
    digitalWrite(LEFT, HIGH);
    digitalWrite(RIGHT, HIGH);
  } else {
    if (a1 > a2) {
      digitalWrite(RIGHT, HIGH);
    } else if (a1 < a2) {
      digitalWrite(LEFT, HIGH);
    }
  }

  delay(5);
}
