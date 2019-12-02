//Cian Costelloe 30/10/19
#include <AccelStepper.h>
#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>
#include <Wire.h>
#include "SparkFun_VL53L1X.h"

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1

int incomingByte = 0; // for incoming serial data
boolean hbd;

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
SFEVL53L1X distanceSensor;

void setup() {
  hbd = false;
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  stepper.setMaxSpeed(3000); // Set the maximum speed and acceleration
  stepper.setAcceleration(8000);

  if (distanceSensor.begin() == 0) //Begin returns 0 on a good init
  {
    //Serial.println("Sensor online!");
  }
}

void loop() {
  //Prox
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.stopRanging();
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW

  if ((distance < 200) && (hbd == false)){
  //if (distance < 200){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    Serial.write(6);
    Serial.println();
    hbd = true;
//    Serial.flush();
  }
  else if((distance >= 200) && (hbd == true)){
    hbd = false;
    //Serial.println("hbd = false");
  }
  
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    //Serial.write(incomingByte);
    //Serial.print(incomingByte);
    //Serial.println();
    //Serial.flush();
  
    if (incomingByte == 49){
      //Left
      Serial.write(3);
      Serial.println();
      Serial.flush();
      stepper.moveTo(500);
      stepper.runToPosition();
    }
    else if (incomingByte == 50){
      //Right
      Serial.write(4);
      Serial.println();
      Serial.flush();
      stepper.moveTo(-500); // Set the target position
      stepper.runToPosition(); // Run to target position with set speed and acceleration/deceleration:
    }
    else if (incomingByte == 51){
      //Center
      Serial.write(5);
      Serial.println();
      Serial.flush();
      stepper.moveTo(0); // Set the target position
      stepper.runToPosition(); // Run to target position with set speed and acceleration/deceleration:
    }
  }
}
