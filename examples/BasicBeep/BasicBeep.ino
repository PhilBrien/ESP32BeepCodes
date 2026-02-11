#include <ESP32BeepCodes.h>

ESP32BeepCodes beeper(25);

void setup() {
  beeper.begin();
  beeper.play("..-.-", false);
}

void loop() {
  beeper.update();
}
