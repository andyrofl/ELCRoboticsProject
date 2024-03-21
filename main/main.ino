#include <QTRSensors.h>
#include <Arduino.h>
#include <MeMegaPi.h>

//**
  * Spring 2024
  * Engineering Leadership Council
  *
  */

void stop();
void _delay(float seconds);
void _loop() ;
int speedp =20;  //speed in percent power
int potVal;
int angle =0;
Servo Servo1;
QTRSensors qtr;

const uint8_t SensorCount = 1;  //6
uint16_t sensorValues[SensorCount];

MeMegaPiDCMotor motor_1(1);
MeMegaPiDCMotor motor_9(9);
MeMegaPiDCMotor motor_2(2);
MeMegaPiDCMotor motor_10(10);


void setup(){
    Serial.begin(9600);
    // configure the sensors
    //qtr.setTypeAnalog();
    //qtr.setSensorPins((const uint8_t[]){A6, A7, A8, A9, A10, A11}, SensorCount);  //A0, A1, A2, A3, A4, A5
    //qtr.setEmitterPin(66);  //2

    pinMode(LED_BUILTIN, OUTPUT);
    //calibrate_sensors()

    //
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(WGM12);
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS21);
    // DO SOMETHING

    delay(2000);
    //Serial.begin(9600);

}

//performs calibration of QTR sensors
//legacy code from linefollowingdemo, highly subject to change as sensor is integrated into project
void calibrate_sensors(){
    digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

    // analogRead() takes about 0.1 ms on an AVR.
    // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
    // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
    // Call calibrate() 400 times to make calibration take about 10 seconds.
    for (uint16_t i = 0; i < 200; i++)  // Was 400 samples.{
        qtr.calibrate();
    }
    digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

    // print the calibration minimum values measured when emitters were on
    // Serial.begin(9600);
    for (uint8_t i = 0; i < SensorCount; i++){
        Serial.print(qtr.calibrationOn.minimum[i]);
        Serial.print(' ');
    }
    Serial.println();

    // print the calibration maximum values measured when emitters were on
    for (uint8_t i = 0; i < SensorCount; i++){
        Serial.print(qtr.calibrationOn.maximum[i]);
        Serial.print(' ');
        }
        Serial.println();
        Serial.println();
        delay(10);  // was 1000
    }
}

void _loop(){
}

void loop(){

}
