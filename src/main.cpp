#include <Arduino.h>
#include <Servo.h>
#include "PID.h"

// do pomiaru czasu w loop - pid
constexpr unsigned long time_frame = 20000;
unsigned long last_time = 0;

// do pomiaru czasu w loop - czujnik
constexpr unsigned long time_frame_trigger = 60000;
unsigned long last_time_trigger = 0;

//dla funkcji echoInterrupt
volatile unsigned long ping;
volatile unsigned long czasJednejFali;
volatile float odlegoscWCm;

//piny
constexpr int TRIGGER_PIN = 4;  // czujnik wyślij ping
constexpr int ECHO_PIN = 2;     // w momencie wyslania pingu zmienia stan naprocie echo na wysoki
constexpr int SERWO_PIN = 9;

//
PIDController pid(1.0f, 0.5f, 0.1f, -45.0f, 45.0f);

//serwomechanizm
Servo servo;
constexpr int bazowe_ustawienie_serwo = 90;

void echoInterrupt() {
    if (digitalRead(ECHO_PIN) == HIGH) {
        ping = micros();
    }else {
        unsigned long now = micros();
        czasJednejFali = now - ping;
        odlegoscWCm = (czasJednejFali * 0.0343) *0.5f;  //droga w cm, czas w mikrosekundach i predkosc dzwieku 34300 cm/s
    }
}


void setup() {
    pinMode(TRIGGER_PIN,OUTPUT);
    pinMode(ECHO_PIN,INPUT);
    attachInterrupt(digitalPinToInterrupt(ECHO_PIN),echoInterrupt, CHANGE);
    servo.attach(SERWO_PIN);
    servo.write(bazowe_ustawienie_serwo);
}

void loop() {
    unsigned long now = micros();
    if (now - last_time_trigger >= time_frame_trigger) {
        digitalWrite(TRIGGER_PIN,LOW);
        delayMicroseconds(2);
        digitalWrite(TRIGGER_PIN,HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIGGER_PIN,LOW);
        last_time_trigger+=time_frame_trigger;
    }

    if (now - last_time >= time_frame) {
        float dt = (now- last_time);
        last_time += time_frame; //zeby miec pewnosc ze nie ma mikroopznienia
        noInterrupts();
        float pv = odlegoscWCm;
        interrupts();
        float u = pid.calculate(15.0f,pv,dt/1000000.0f);

        int real_output_servo = u + bazowe_ustawienie_serwo ; 
        if (real_output_servo > 135) real_output_servo = 135;
        else if (real_output_servo < 45 ) real_output_servo = 45;
        servo.write(real_output_servo);
    }
}

