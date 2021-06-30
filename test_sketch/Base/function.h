
void GoToBaseBtn() {
  if (DEBUG) {
    Serial.println("GoToBaseBtn");
  }
  Serial.print('A');
}

void EmergencyStop() {
  if (DEBUG) {
    Serial.println("EmergencyStop");
  }
  Serial.print('E');
}

void RightUp() {
  Serial.print('R');
}

void RightDown() {
  Serial.print('D');
}

void RightStop() {
  Serial.print('I');
}

void LeftUp() {
  Serial.print('L');
}

void LeftDown() {
  Serial.print('U');
}

void LeftStop() {
  Serial.print('J');
}

void StandartMoving() {
  Serial.print('Z');
}
//HOPE YOU STAY - LAELAND

void getInfo() {
  Serial.print('S');
  delay(10);
  int num = 0;
  int oldNum = -1;
  while (Serial.available() > 0) {
    if (num == 0 && oldNum != num) {
      nextion.print("t3.txt=\"");
      oldNum = num;
    }
    else if (num == 1 && oldNum != num) {
      nextion.print("t5.txt=\"");
      oldNum = num;
    }
    else if (num == 2 && oldNum != num) {
      nextion.print("t7.txt=\"");
      oldNum = num;
    }

    char c = Serial.read();
    if (c == ' ') {
      num++;
      nextion.print('"');
      nextion.write(0xFF);
      nextion.write(0xFF);
      nextion.write(0xFF);
    }
    else if (c == ';') {
      nextion.print('"');
      nextion.write(0xFF);
      nextion.write(0xFF);
      nextion.write(0xFF);
      return;
    } else {
      nextion.print(c);
    }
  }
}
