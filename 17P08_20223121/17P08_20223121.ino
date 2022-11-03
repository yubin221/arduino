#include <Servo.h>

// Arduino pin assignment

#define PIN_IR 0 // Add IR Sensor Definition Here !!!
#define PIN_LED 9
#define PIN_SERVO 10

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define _DIST_MIN 100
#define _DIST_MAX 250

#define LOOP_INTERVAL 20   // Loop Interval (unit: msec)
#define _EMA_ALPHA 0.2

Servo myservo;
unsigned long last_loop_time;   // unit: msec
float dist, dist_ema, dist_prev = _DIST_MAX;

void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(1000000);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value;
  float duty;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Remove Next line !!!
  a_value = analogRead(PIN_IR);
  dist = (6762.0 / (a_value-9) - 4.0) * 10.0 - 60.0;

  if (dist < _DIST_MIN) {
    dist = dist_prev;           // cut lower than minimum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else if (dist > _DIST_MAX) {
    dist = dist_prev;           // Cut higher than maximum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else {    // In desired Range
    digitalWrite(PIN_LED, 0);       // LED ON  
    dist_prev = dist;
  }

  dist_ema = _EMA_ALPHA * dist + (1 - _EMA_ALPHA ) * dist_ema;

  if(dist_ema <= _DIST_MIN) {
    myservo.writeMicroseconds(_DUTY_MIN);
  }
  else if(dist_ema >= _DIST_MAX) {
    myservo.writeMicroseconds(_DUTY_MAX);
  }
  else {
    duty = 12.3067 * dist_ema - 677.67;
    myservo.writeMicroseconds(duty);
  }

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN: ");    Serial.print(_DIST_MIN);
  Serial.print(", IR: ");   Serial.print(a_value);
  Serial.print(",dist: ");  Serial.print(dist);
  Serial.print(",ema: ");   Serial.print(dist_ema);
  Serial.print(",servo: "); Serial.print(duty);
  Serial.print(",MAX: ");   Serial.print(_DIST_MAX);
  Serial.println("");
}
