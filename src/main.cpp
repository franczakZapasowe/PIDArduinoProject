#include <Arduino.h>
#include "PID.h"

constexpr  unsigned long time_frame = 20000;
unsigned long last_time = 0;
PIDController pid(1.0f, 0.5f, 0.1f, -45.0f, 45.0f);
void setup() {

}
void loop() {
    unsigned long now = micros();
    if (now - last_time >= time_frame) {
        float dt = (now- last_time);
        last_time += time_frame; //zeby miec pewnosc ze nie ma mikroopznienia
        float u = pid.calculate(15.0f,10.0f,dt/1000000.0f);
    }
}

