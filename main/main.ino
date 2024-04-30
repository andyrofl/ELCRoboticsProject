#include <QTRSensors.h>
#include <Arduino.h>
#include <MeMegaPi.h>
#include <SoftwareSerial.h>

//
//Spring 2024
//Engineering Leadership Council
//

//define functions
void calibrate_sensors();
void drive_motors(int left, int right);
int get_line_sensor_values();

//define constants and global variables
const uint8_t SensorCount = 8;  //6
uint16_t sensorValues[SensorCount];

int EMITTOR_PIN = A15;//check what pin the IR sensor is on
int tracking_value = 50;//for one sensor, ideally track to 50% of a range of 0-100


//define objects
MeMegaPiDCMotor motor_left(9);//left side?
MeMegaPiDCMotor motor_right(1);//right side?. refactor both with clear names
QTRSensors qtr;


//
//This is the main setup function and is run once on startup
// 
//
void setup(){
    Serial.begin(9600);
    delay(1000);
    Serial.println("serial communication started");
    // configure the sensors
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]){A14, A13, A12, A5, A4, A3, A2, A1}, SensorCount);  //A0, A1, A2, A3, A4, A5
    qtr.setEmitterPin(EMITTOR_PIN);  //2

    pinMode(LED_BUILTIN, OUTPUT);
    
    calibrate_sensors();
    //perform any other motor or sensor initialization

    
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(WGM12);
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS21);
  
}

//
//This is the main loop function and is run until terminated after the startup function has begun
//
//
void loop(){
    Serial.println("entered main loop");
    Serial.println("passed line reading function");
    while(true){//until terminated
        //get_line_sensor_values();
        int sensor_values = get_line_sensor_values();
        //if(approaching_river(sensor_values))
        //    then cross river or initiate dump procedure and continue to next iteration of loop
        //motor_power = calculate_motor_power(sensor_values)
        if(sensor_values == 0){
          drive_motors(40, 40);
        }
        else if(sensor_values >3500){
            drive_motors(40, 100);
        }
        else if(sensor_values <3500){
          drive_motors(100, 40);
        }
        else{
           drive_motors(100, 100);
        }
        //check for presence of popsicle stick and perform appropriate action
    }
}

//performs calibration of QTR sensors
// legacy code from linefollowingdemo, highly subject to change as sensor is integrated into project
void calibrate_sensors(){
    digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

    // analogRead() takes about 0.1 ms on an AVR.
    // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
    // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
    // Call calibrate() 400 times to make calibration take about 10 seconds.
    for (uint16_t i = 0; i < 200; i++){  // Was 400 samples.{
        qtr.calibrate();
    }
    digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

    // print the calibration minimum values measured when emitters were on
    // Serial.begin(9600);
    for (int i = 0; i < SensorCount; i++){
        Serial.print(qtr.calibrationOn.minimum[i]);
        Serial.print(' ');
    }
    Serial.println();

    // print the calibration maximum values measured when emitters were on
    for (uint8_t i = 0; i < SensorCount; i++){
        Serial.print(qtr.calibrationOn.maximum[i]);
        Serial.print(' ');
        
        Serial.println();
        Serial.println();
        delay(10);  // was 1000
    }
}

//drive both motors with the given input speeds
void drive_motors(int left, int right){
    motor_left.run(-left);
    motor_right.run(right);
    //300 fast
}

//get sensor values. will need to return an array later
int get_line_sensor_values(){
    qtr.read(sensorValues);
     for (uint8_t i = 0; i<SensorCount; i++){
       //Serial.print(sensorValues[i]);
       //Serial.print('\t');

    }
    int position = qtr.readLineBlack(sensorValues);
    Serial.println(position);
    
    Serial.println();

      //delay(100);

    return position;
}
