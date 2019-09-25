#include <avr/sleep.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define SENSOR 2
#define LIGHT 3
#define SERVO_A 5
#define SERVO_B 6
#define BIG_SERVO 9

#define THRESHOLD 600
#define DEGREE 30

#define DOOR_DEGREE 90
#define DOOR_DEGREE_FIRST 65

int state = 0;
Servo servo_a;
Servo servo_b;
Servo servo_big;
unsigned long threshold = THRESHOLD;

void setup()
{
    Serial.begin(115200);
    Serial.println("go");
    pinMode(SENSOR, INPUT);
    pinMode(13, OUTPUT);
    servo_a.attach(SERVO_A);
    servo_b.attach(SERVO_B);
    servo_big.attach(BIG_SERVO);
    servo_write_all(0);
    servo_big.write(0);
    delay(1000);
    servo_big.detach();
}

void loop()
{
    arduinoSleep();
}

void arduinoSleep()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    Serial.println("dodo");
    attachInterrupt(0, arduinoWakeUp, RISING);
    delay(1000);
    sleep_mode();
    Serial.println("reveil");
    sleep_disable();
    detachInterrupt(0);
    use_box();
}

void arduinoWakeUp()
{

}

void lightOn()
{
  for (int i = 0; i < 255; i++) {
    analogWrite(LIGHT, i);
    delay(10);
  }  
}

void lightOff()
{
  for (int i = 254; i > -1; i--) {
    analogWrite(LIGHT, i);
    delay(5);
  }  
}

void use_box()
{
  unsigned long now = millis();

  Serial.println(now);
  Serial.println(millis() - now);
  while (digitalRead(2) == HIGH);
  while (millis() - now < threshold) {
    if (digitalRead(2) == HIGH) {
      if (!state)
        open_box();
      else
        close_box();
      state = !state;
    }
  }
}

void open_box()
{
  Serial.println("opening the box");
  open_door();
  lightOn();
  move_planch();
  digitalWrite(13, HIGH);
  Serial.println("box opened");
}

void close_box()
{
  Serial.println("closing the box");
  lightOff();
  move_planch();
  close_door();
  digitalWrite(13, LOW);
  Serial.println("box closed");
}

void open_door() {
  servo_big.attach(9);
  for (int i = 0; i < DOOR_DEGREE_FIRST; i++) {
    servo_write(servo_big, i);
    delay(20);
  }
  for (int i = DOOR_DEGREE_FIRST - 1, t = 20; i < DOOR_DEGREE; i++, t++) {
    servo_write(servo_big, i);
    delay(t);
  }
  servo_big.detach();
}

void close_door() {
  servo_big.attach(9);
  for (int i = DOOR_DEGREE - 1, t = 20; i > 30; i--, t++) {
    servo_write(servo_big, i);
    delay(20);
  }
  for (int i = 30 - 1, t = 20; i > -1; i--, t++) {
    servo_write(servo_big, i);
    delay(t);
  }
  servo_big.detach();

}

void move_planch() {
  if (!state) {
    for (int i = 0, t = 50; i < DEGREE; i++, t++) {
      servo_write_all(i);
      delay(t);
    }
  } else {
    for (int i = DEGREE - 1, t = 10; i > -1; i--, t++) {
      servo_write_all(i);
      delay(t);
    }
  }
}

void servo_write_all(int degree) {
  servo_write(servo_a, degree);
  servo_reverse_write(servo_b, degree);
}

void servo_write(Servo s, int degree) {
  s.writeMicroseconds(map(degree, 0, 179, 500, 2000));
}

void servo_reverse_write(Servo s, int degree) {
  s.writeMicroseconds(map(degree, 0, 179, 2000, 500));
}
