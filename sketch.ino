int oneturn = 360;  //seconds for onehour turn
int timeout = 2900;  //seconds until breakdown
#define evening_stopper 2
#define morning_stopper 3
#define moto 16
#define revers 17
#define power 15

int expected_position = 0;
int real_position = 0;
int clocker = 0;
#include <Wire.h>
#include <DS3231.h>
RTClib myRTC;

int getTime() { //method to recieve {int hours} from clocks
  DateTime current = myRTC.now();
  return current.hour();
}

void setup() {
  pinMode(morning_stopper, INPUT);
  pinMode(evening_stopper, INPUT);
  pinMode(moto, OUTPUT);
  pinMode(revers, OUTPUT);
  pinMode(power, OUTPUT);
  digitalWrite(moto, HIGH);
  digitalWrite(revers, LOW);
  digitalWrite(power, LOW);

  while (digitalRead(morning_stopper) != LOW && clocker < (timeout * 10)) {
    delay(100);   //Go to the morning stopper
    clocker++;
  }
  digitalWrite(revers, HIGH);
  digitalWrite(power, HIGH);
}

void loop() {
  clocker = 0;
  int difference = 0;

  switch (getTime()) {
    case 10:
      expected_position = 1;
      break;
    case 11:
      expected_position = 2;
      break;
    case 12:
      expected_position = 3;
      break;
    case 13:
      expected_position = 4;
      break;
    case 14:
      expected_position = 5;
      break;
    case 15:
      expected_position = 6;
      break;
    case 16:
      expected_position = 7;
      break;
    case 21:
      expected_position = 0;
      break;
  }

  difference = expected_position - real_position;
  real_position = expected_position;

  if (difference == 0) {
    delay(10000);
  } else if (difference > 0) {
    int steps = oneturn * 10 * difference;
    while (steps > 0) {
      if (digitalRead(evening_stopper) != LOW) {
        digitalWrite(power, LOW);
        digitalWrite(moto, LOW);
        delay(100);
        steps--;
      }
    }
    digitalWrite(power, HIGH);
    digitalWrite(moto, HIGH);

  } else {
    while (digitalRead(evening_stopper) != LOW && clocker < timeout * 10) {
      digitalWrite(power, LOW);
      digitalWrite(moto, LOW);
      clocker++;
    }
    digitalWrite(power, HIGH);
    digitalWrite(moto, HIGH);
  }
}
