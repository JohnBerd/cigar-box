#include <avr/sleep.h>

#define SENSOR 2
#define THRESHOLD 400

int state = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(13, OUTPUT);
    pinMode(SENSOR, INPUT);
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

void use_box()
{
  int now = millis();

  while (digitalRead(2) == HIGH);
  while (millis() - now < THRESHOLD) {
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
  delay(2000);
  digitalWrite(13, HIGH);
  Serial.println("box opened");
}

void close_box()
{
  Serial.println("closing the box");
  delay(2000);
  digitalWrite(13, LOW);
  Serial.println("box closed");
}
