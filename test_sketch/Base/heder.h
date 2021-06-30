void GoToBaseBtn();
void EmergencyStop();
void ToControl();
void RightUp();
void RightDown();
void RightStop();
void LeftUp();
void LeftDown();
void LeftStop();
void StandartMoving();

byte bufer[4];

struct FUNCTIONS {
  char id[3];
  void (*func)();
};

struct FUNCTIONS id_func[] = {
  {{0x0, 0x03, 0x01}, &GoToBaseBtn},
  {{0x0, 0x08, 0x01}, &EmergencyStop},
  {{0x0, 0x05, 0x01}, &EmergencyStop},
  {{0x01, 0x03, 0x01}, &RightUp},
  {{0x01, 0x04, 0x01}, &RightDown},
  {{0x01, 0x04, 0x01}, &RightStop},
  {{0x01, 0x01, 0x01}, &LeftUp},
  {{0x01, 0x02, 0x01}, &LeftDown},
  {{0x01, 0x06, 0x01}, &LeftStop},
  {{0x01, 0x07, 0x01}, &StandartMoving},
};

void sendComandToNextion(char const* cmd) {
  nextion.print(cmd);
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);
}

void textToNextion(char* reg, char* data) {
  char nextionBuf[100];
  strcpy(nextionBuf, reg);
  strcat(nextionBuf, ".txt=\"");
  strcat(nextionBuf, data);
  strcat(nextionBuf, "\"");

  if (DEBUG) {
    Serial.println(nextionBuf);
  }
  sendComandToNextion(nextionBuf);
}




bool diff(struct FUNCTIONS arr) {
  bool flag = true;
  for (byte i = 0; i < 3; i++) {
    flag = flag && (bufer[i] == arr.id[i]);
  }
  return flag;
}

void logBuf() {
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < 8; j++) {
      if (DEBUG) {
        Serial.print(bufer[i] >> (7 - j) & 0x01);
      }
    }
    if (DEBUG) {
      Serial.print(' ');
    }
  }
  if (DEBUG) {
    Serial.print('\n');
  }
}



void myLisn() {
  byte readNum = -1;

  while (nextion.available() > 0) {
    char c = nextion.read();
    if (readNum == 3) {
      readNum = -1;
      bufer[3] = 1;
      return;
    }
    if (c == 0b01100101 && readNum == -1) {
      readNum = 0;
      continue;
    }
    if (readNum == -1)
      continue;

    bufer[readNum] = c;
    readNum++;
    delay(1);
  }
}


void findComand() {

  for (byte i = 0; i < sizeof(id_func) / sizeof(FUNCTIONS); i++) {
    if (diff(id_func[i])) {
      id_func[i].func();
      logBuf();
      return;
    }
  }

  if (DEBUG) {
    Serial.println("Uncnown comand");
  }
  logBuf();

}
