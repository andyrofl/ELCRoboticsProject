#include <QTRSensors.h>
#include <Arduino.h>
#include <MeMegaPi.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// This example is designed for use with six analog QTR sensors. These
// reflectance sensors should be connected to analog pins A0 to A5. The
// sensors' emitter control pin (CTRL or LEDON) can optionally be connected to
// digital pin 2, or you can leave it disconnected and remove the call to
// setEmitterPin().
//
// The setup phase of this example calibrates the sensors for ten seconds and
// turns on the Arduino's LED (usually on pin 13) while calibration is going
// on. During this phase, you should expose each reflectance sensor to the
// lightest and darkest readings they will encounter. For example, if you are
// making a line follower, you should slide the sensors across the line during
// the calibration phase so that each sensor can get a reading of how dark the
// line is and how light the ground is. Improper calibration will result in
// poor readings.
//
// The main loop of the example reads the calibrated sensor values and uses
// them to estimate the position of a line. You can test this by taping a piece
// of 3/4" black electrical tape to a piece of white paper and sliding the
// sensor across it. It prints the sensor values to the serial monitor as
// numbers from 0 (maximum reflectance) to 1000 (minimum reflectance) followed
// by the estimated location of the line as a number from 0 to 5000. 1000 means
// the line is directly under sensor 1, 2000 means directly under sensor 2,
// etc. 0 means the line is directly under sensor 0 or was last seen by sensor
// 0 before being lost. 5000 means the line is directly under sensor 5 or was
// last seen by sensor 5 before being lost.

void move_forward();
void move_backward();
void move_left();
void turn_left();
void turn_right();
void move_right();
void turn_45_deg_to_upper_left();
void turn_45_deg_to_upper_right();
void stop();
void _delay(float seconds);
void _loop() ;
int speedp =20;  //speed in percent power
int potVal;
int angle =0;
Servo Servo1;
QTRSensors qtr;

const uint8_t SensorCount = 6;  //6
uint16_t sensorValues[SensorCount];

MeMegaPiDCMotor motor_1(1);
MeMegaPiDCMotor motor_9(9);
MeMegaPiDCMotor motor_2(2);
MeMegaPiDCMotor motor_10(10);

void move_forward(){

  motor_1.run(speedp / 100.0 * 255);  //50

  motor_9.run(speedp / 100.0 * 255);  //50

  motor_2.run(-speedp / 100.0 * 255);  //-50

  motor_10.run(-speedp / 100.0 * 255);  //-50

}
void move_backward(){

  motor_1.run(-speedp / 100.0 * 255);  //-50

  motor_9.run(-speedp / 100.0 * 255);  //-50

  motor_2.run(speedp / 100.0 * 255);  //50

  motor_10.run(speedp / 100.0 * 255);  //50

}
void move_left(){
  // shift left

  motor_1.run(speedp / 100.0 * 255);  //50

  motor_9.run(-speedp / 100.0 * 255);  //-50

  motor_2.run(-speedp / 100.0 * 255);  //-50

  motor_10.run(speedp / 100.0 * 255);  //50

}
void turn_left(){

  motor_1.run(speedp / 100.0 * 255);  //50

  motor_9.run(speedp / 100.0 * 255);  //50

  motor_2.run(speedp / 100.0 * 255);  //50

  motor_10.run(speedp / 100.0 * 255);  //50

}
void turn_right(){

  motor_1.run(-speedp / 100.0 * 255);  //-50

  motor_9.run(-speedp / 100.0 * 255);  //-50

  motor_2.run(-speedp / 100.0 * 255);  //-50

  motor_10.run(-speedp / 100.0 * 255);  //-50

}
void move_right(){
  // shift right

  motor_1.run(-speedp / 100.0 * 255);  //-50

  motor_9.run(speedp / 100.0 * 255);  //50

  motor_2.run(speedp / 100.0 * 255);  //50

  motor_10.run(-speedp / 100.0 * 255);  //-50

}
void turn_45_deg_to_upper_left(){

  motor_1.run(speedp / 100.0 * 255);  //50

  motor_9.run(-speedp / 100.0 * 255);  //-50

  motor_2.run(0 / 100.0 * 255);  //00

  motor_10.run(0 / 100.0 * 255);  //00

}
void turn_45_deg_to_upper_right(){

  motor_1.run(00 / 100.0 * 255);  //-50

  motor_9.run(00 / 100.0 * 255);  //50

  motor_2.run(speedp / 100.0 * 255);  //00

  motor_10.run(-speedp / 100.0 * 255);  //00

}
void stop(){

  motor_1.run(0 / 100.0 * 255);  //00

  motor_9.run(0 / 100.0 * 255);  //00

  motor_2.run(0 / 100.0 * 255);  //00

  motor_10.run(0 / 100.0 * 255);  //00

}

void _delay(float seconds) {
  if(seconds < 0.0){
    seconds = 0.0;
  }
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime) _loop();
}
void setup() {

 Serial.begin(9600);
 // configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A6, A7, A8, A9, A10, A11}, SensorCount);  //A0, A1, A2, A3, A4, A5
  qtr.setEmitterPin(66);  //2

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

  // analogRead() takes about 0.1 ms on an AVR.
  // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
  // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 200; i++)  // Was 400 samples.
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
 // Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(10);  // was 1000


  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  // DO SOMETHING

  // put your setup code here, to run once:
  Servo1.attach(69);  //The servo is connected on pin D69
potVal= 2;
  Servo1.write(angle);
  delay(2000);
//  Serial.begin(9600);

}

void _loop() {
}

void loop()
{
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
 uint16_t position = qtr.readLineBlack(sensorValues);  // This gets readings for the calibrated min and max values as limits.
// qtr.read(sensorValues);  This will read the raw values of the sensors.
   // print the sensor values as numbers from 0 to 1023, where 0 means maximum
  // reflectance and 1023 means minimum reflectance
 for (uint8_t i = 0; i < SensorCount; i++)
 {
   Serial.print(sensorValues[i]);
   Serial.print('\t');
 }
  Serial.println(position);
  Serial.println();
  delay(20);  // this was a delay of 1000 milliseconds

if(
sensorValues[1]<500 && 
sensorValues[2]<500 && 
sensorValues[3]<500 &&
sensorValues[4]<500) {
move_forward();
Serial.println("move forward");
}
else if(
sensorValues[1]>500 && 
sensorValues[2]>500 && 
sensorValues[3]<500 &&
sensorValues[4]<500) {
move_forward();
Serial.println("move forward");
}
else if(
sensorValues[1]<500 && 
sensorValues[2]<500 && 
sensorValues[3]>500 &&
sensorValues[4]>500) {
turn_right();
// turn_45_deg_to_upper_right();
Serial.println("turn right");
}
else if( 
sensorValues[1]>500 && 
sensorValues[2]<500 && 
sensorValues[3]<500 &&
sensorValues[4]<500) {
turn_left();
Serial.println("turn left");
//turn_45_deg_to_upper_left();
//Serial.println("turn 45 upper left");
}
else if(
sensorValues[1]>500 && 
sensorValues[2]>500 && 
sensorValues[3]>500 &&
sensorValues[4]<500) {
turn_right();
Serial.println("turn right");
//turn_45_deg_to_upper_right();
//Serial.println("turn 45 upper right");
}
else if(
//sensorValues[0]>500 && 
sensorValues[1]>500 && 
sensorValues[2]>500 && 
sensorValues[3]>500 &&
sensorValues[4]>500) {
//sensorValues[5]>500) {
//turn_left();
Serial.println("All black");
//move_backward(); 
//Serial.println("move backward"); 
}
else {
Serial.println("don't know");  
}
 angle=angle+10;
 Servo1.write(angle);
 if(angle>=120){
  angle=0;
 }
}
