#define PIN_TRIG 8

void setup() {


  Serial.begin (9600);
  pinMode(PIN_TRIG, OUTPUT);
  tone(PIN_TRIG,1000);
}

void loop() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);

  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  //  Время задержки акустического сигнала на эхолокаторе.
  // Задержка между измерениями для корректной работы скеча
  delay(10);
}
