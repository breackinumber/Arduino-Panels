#include <Wire.h>
#include <DS3231.h>
RTClib myRTC;
int expected_position = 0;
int real_position = 0;

int alltime = 1500;

#define evening_stopper 0
#define morning_stopper 0
#define moto 0
#define revers 0
#define power 0

void setup() {
  pinMode(morning_stopper, INPUT);
  pinMode(evening_stopper, INPUT);
  pinMode(moto, OUTPUT);
  pinMode(revers, OUTPUT);
  pinMode(power, OUTPUT);
  digitalWrite(moto, HIGH);
  digitalWrite(revers, HIGH);
  digitalWrite(power, HIGH);


}

void loop() {
switch(getTime()){
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
  }

  int difference = expected_position - real_position;
  real_position = expected_position;

  if(difference>0){
    int steps = alltime * 10 * difference;
    while (steps>0){
      if(digitalRead(evening_stopper)!=LOW){
        digitalWrite(power, LOW);
        digitalWrite(moto, LOW);
        delay(100);
        steps--;
      }
      }
    digitalWrite(power, HIGH);
    digitalWrite(moto, HIGH);
    difference = 0;
    }

}

int getTime() {
  DateTime current = myRTC.now();
  return current.hour();
}
