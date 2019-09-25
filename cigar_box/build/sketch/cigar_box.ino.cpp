#line 1 "/root/mygit/cigar-box/cigar_box/cigar_box.ino"
#line 1 "/root/mygit/cigar-box/cigar_box/cigar_box.ino"
#include <avr/sleep.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define SENSOR 2
#define LIGHT 3
#define SERVO_A 5
#define SERVO_B 6
#define BIG_SERVO 13

#define THRESHOLD 600
#define DEGREE 30

int state = 0;
Servo servo_a;
Servo servo_b;
unsigned long threshold = THRESHOLD;

void setup()
{
    Serial.begin(11500);
    pinMode(13, OUTPUT);
    pinMode(SENSOR, INPUT);
    servo_a.attach(SERVO_A);
    servo_b.attach(SERVO_B);
    servo_write_all(0);
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
    delay(10);
  }  
}

void use_box()
{
  int now = millis();

  Serial.println(now);
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
  move_planch();
  lightOn();
  digitalWrite(13, HIGH);
  Serial.println("box opened");
}

void close_box()
{
  Serial.println("closing the box");
  move_planch();
  lightOff();
  digitalWrite(13, LOW);
  Serial.println("box closed");
}

void move_planch() {
  if (!state) {
    for (int i = 0; i < DEGREE; i++) {
      servo_write_all(i);
      delay(10);
    }
  } else {
    for (int i = DEGREE - 1; i > -1; i--) {
      servo_write_all(i);
      delay(10);
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

