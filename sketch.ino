int oneturn = 360;  //seconds for onehour turn
int timeout = 2900;  //seconds until breakdown
bool serial = false;  //developer mode
#define evening_stopper 2
#define morning_stopper 3
#define moto 16
#define revers 17
#define power 15

int expected_position = 0;
int real_position = 0;
int clocker = 0;
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

int getTime() { //method to recieve {int hours} from clocks
  tmElements_t tm;
  RTC.read(tm);
  return tm.Hour;
}

void setup() {
  if (serial) {
    Serial.begin(9600);
  }
  Serial.println("Setup started");
  pinMode(morning_stopper, INPUT);
  pinMode(evening_stopper, INPUT);
  pinMode(moto, OUTPUT);
  pinMode(revers, OUTPUT);
  pinMode(power, OUTPUT);
  digitalWrite(moto, HIGH);
  digitalWrite(revers, LOW);
  digitalWrite(power, LOW);

  Serial.println("Entering the loop for revers");
  while (digitalRead(morning_stopper) != LOW && clocker < (timeout * 10)) {
    delay(100);   //Go to the morning stopper
    clocker++;
    Serial.print("Reversing: ");
    Serial.print(clocker);
    Serial.print("/");
    Serial.println(timeout * 10);
  }
  digitalWrite(revers, HIGH);
  digitalWrite(power, HIGH);
  Serial.println("Setup is done successfully");
}

void loop() {
  Serial.println("New loop began");
  clocker = 0;
  int difference = 0;
  expected_position = 0;
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
    case 17:
      expected_position = 7;
      break;
    case 18:
      expected_position = 7;
      break;
    case 19:
      expected_position = 7;
      break;
    case 20:
      expected_position = 7;
      break;
    case 21:
      expected_position = 0;
      break;
  }
  Serial.print("Current time is ");
  Serial.println(getTime());

  difference = expected_position - real_position;
  real_position = expected_position;

  if (difference == 0) {
    Serial.println("So idling");
    delay(10000);
  } else if (difference > 0) {
    Serial.print("Ongoint to evening ");
    Serial.print(difference);
    Serial.println( " step(s)");
    int steps = oneturn * 10 * difference;
    while (steps > 0) {
      if (digitalRead(evening_stopper) != LOW) {
        digitalWrite(power, LOW);
        digitalWrite(moto, LOW);
        delay(100);
        Serial.print("Progress: ");
        Serial.println(steps);
        steps--;
      }
    }
    digitalWrite(power, HIGH);
    digitalWrite(moto, HIGH);

  } else {
    Serial.println("Going to the morning");
    while (digitalRead(evening_stopper) != LOW && clocker < timeout * 10) {
      digitalWrite(power, LOW);
      digitalWrite(moto, LOW);
      clocker++;
      Serial.print("Progress: ");
      Serial.println(clocker);
    }
    digitalWrite(power, HIGH);
    digitalWrite(moto, HIGH);
  }
  Serial.println("Cycle has been finished successfully");
}
