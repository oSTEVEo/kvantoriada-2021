#define Trig 8
#define Echo 9

#define AG0 A2        //Charge

#define D1 4          // Направление вращение двигателя 1
#define M1 5          // ШИМ вывод для управления двигателем 1
#define D2 7          // Направление вращение двигателя 2
#define M2 6          // ШИМ вывод для управления двигателем 2

#define CONV 11       // Направление вращение двигателя 1 
#define CEP1 13       // Направление вращение двигателя 2
#define CEP2 12       // ШИМ вывод для управления двигателем 2

#define BACHOK 11
#define LOAD 8
#define UNLOAD 10

const int open_ = 30;
const int closed = 70;

struct HomePosition {
  float lat;
  float lng;
};

struct  data {
  float lat;
  float lng;
  int sost;
  int term;
  int bak;
  int carg;
  float temp;
  int dist;
  float angle;
};

enum Sosts {
  AutoMode = 1,
  GoHome,
  AlramStop,
  SBak
};

struct data curentData;
struct HomePosition hp;

uint8_t gpCouter;

char buf[12];
int pointer = 0;
int sost = 0;
bool startFlag  = false;

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
  }
}

void readGps() {
  if (ss.available() > 0) {
    char c = ss.read();

    if (startFlag) {
      if ((c >= '0' && c <= '9') || c == '.' || c == '-') {
        buf[pointer] = c;
        pointer++;
      } else if (c == ' ') {
        if (sost == 0) {
          buf[pointer] = '\0';
          curentData.lat = atof(buf);
          pointer = 0;
          sost++;
        } else if (sost == 1) {
          buf[pointer] = '\0';
          curentData.lng = atof(buf);
          pointer = 0;
          sost++;
        } else if (sost == 2) {
          buf[pointer] = '\0';
          curentData.angle = atof(buf);
          pointer = 0;
          sost = 0;
          startFlag = false;
          gpCouter++;
        }
      }
    } else if (c == '~') {
      startFlag = true;
    }
  }
}

void sendData() {
  Serial.print('S');
  Serial.print(curentData.dist);
  Serial.print('X');
  printFloat(curentData.lng, 1, 9, 6);
  Serial.print('X');
  printFloat(curentData.lat, 1, 9, 6);
  Serial.print('X');
  
  Serial.print(curentData.sost);
  Serial.print('X');
  Serial.print(curentData.term);
  Serial.print('X');
  Serial.print(curentData.bak);
  Serial.print('X');
  Serial.print(curentData.carg);
  Serial.print('X');
  Serial.print(curentData.angle);
  Serial.print(';');
}

//GPS-----------------------------
#include <math.h>
#define earthRadiusKm 6371.0

// This function converts decimal degrees to radians
float deg2rad(float deg) {
  return (deg * M_PI / 180);
}

//  This function converts radians to decimal degrees
float rad2deg(float rad) {
  return (rad * 180 / M_PI);
}

double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r) / 2);
  v = sin((lon2r - lon1r) / 2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}
//GPS-----------------------------
