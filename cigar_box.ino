#define PIN_PLANCH 		11
#define PIN_DOOR 		10
#define PIN_TOUCH_1 	7
#define PIN_TOUCH_2 	8
#define PIN_LIGHT		9
#define INTERRUP_PIN    2

#define THRESHOLD       40

#include <Servo.h>
#include <CapacitiveSensor.h>
#include <MedianFilter.h>
#include <avr/sleep.h>

Servo servos_planch;
Servo servo_door;

CapacitiveSensor touch = CapacitiveSensor(PIN_TOUCH_1, PIN_TOUCH_2);
MedianFilter mf(20, 0);

bool opened = false;

void setup() {
    servos_planch.attach(PIN_PLANCH);
    servo_door.attach(PIN_DOOR);
    pinMode(PIN_LIGHT, OUTPUT);
}

void loop() {
    long sensorValue = touch.capacitiveSensor(30);
    mf.in( sensorValue);
    sensorValue = mf.out();
    if (!opened) {
        open_the_box();
    } else {
        close_the_box();
    }
}

void open_the_box() {
    opened = true;
}

void close_the_box() {
    opened = false;
}

void power_down() {
    SMCR != (1<<2); //power down mode
    SMCR != 1; //enable sleep
    __asm__ __volatile("sleep");
}
