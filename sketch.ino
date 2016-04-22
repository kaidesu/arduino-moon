#include <Time.h>

#define TIME_MSG_LEN 11
#define TIME_HEADER 'T'
#define TIME_REQUEST 7

// Assign LEDs to pins on Arduino board.
int led1 = 2;
int led2 = 3;
int led3 = 4;
int led4 = 5;
int led5 = 6;
int led6 = 7;

// Setup datetime variables.
int y, m, d, h;

// Defines LED brightness level, 0 is off and 255 is full bright.
int I = 255;

void setup() {
  setTime(8, 10, 0, 22, 4, 2016);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);

  Serial.begin(9600);

  Serial.println("Arduino Moon ready.");
}

void loop() {
  y = year();
  m = month();
  d = day();
  h = hour();

  setMoonPhase(y, m, d);
  delay(3000);
}

void lightLEDs(int L6, int L5, int L4, int L3, int L2, int L1) {
  analogWrite(led1, L1);
  analogWrite(led2, L2);
  analogWrite(led3, L3);
  analogWrite(led4, L4);
  analogWrite(led5, L5);
  analogWrite(led6, L6);
}

void setMoonPhase(int Y, int M, int D) {
  double AG, IP;
  byte phase;

  long YY, MM, K1, K2, K3, JD;

  // Calculate Julian date
  YY = Y - floor((12 - M) / 10);
  MM = M + 9;

  if (MM >= 12) {
    MM = MM - 12;
  }

  K1 = floor(365.25 * (YY + 4712));
  K2 = floor(30.6 * MM + 0.5);
  K3 = floor(floor((YY / 100) + 49) * 0.75) - 38;

  JD = K1 + K2 + D + 59;

  if (JD > 2299160) {
    JD = JD - K3;
  }

  IP = normalize((JD - 2451550.1) / 29.530588853);
  AG = IP * 29.53;

  Serial.println(AG);

  if (AG < 1.20369) {
    lightLEDs(0, 0, 0, 0, 0, 0);
  } else if (AG < 3.61108) {
    lightLEDs(0, 0, 0, 0, 0, I);
  } else if (AG < 6.01846) {
    lightLEDs(0, 0, 0, 0, I, I);
  } else if (AG < 8.42595) {
    lightLEDs(0, 0, 0, I, I, I);
  } else if (AG < 10.83323) {
    lightLEDs(0, 0, I, I, I, I);
  } else if (AG < 13.24062) {
    lightLEDs(0, I, I, I, I, I);
  } else if (AG < 15.64800) {
    lightLEDs(I, I, I, I, I, I);
  } else if (AG < 18.05539) {
    lightLEDs(I, I, I, I, I, 0);
  } else if (AG < 20.46277) {
    lightLEDs(I, I, I, I, 0, 0);
  } else if (AG < 22.87016) {
    lightLEDs(I, I, I, 0, 0, 0);
  } else if (AG < 25.27754) {
    lightLEDs(I, I, 0, 0, 0, 0);
  } else if (AG < 27.68493) {
    lightLEDs(I, 0, 0, 0, 0, 0);
  } else {
    lightLEDs(0, 0, 0, 0, 0, 0);
  }
}

double normalize(double v) {
  v = v - floor(v);

  if (v < 0) {
    v = v + 1;
  }

  return v;
}
