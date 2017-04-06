//Time Declerations
#include <FlexiTimer2.h>
#include "eceRoverMath.h"
float seconds = 0;
float deltaT = 1.0/5000;

//Motor Declerations
#define outputA A0
#define outputB A1
float counter = 0;
float revs;
float rpm;
float realAngle = 0;
float counterValue = 0;
float counterAngle = 0;
const float maxAngle = 360;
const float scaleFactor = 13.8461538;
int aState;
int aLastState;
int set1 = 3;
int set2 = 4;
int enable = 5;

//Angular Velocity Declerations
double oldTheta = 0;
double newTheta = 0;
double oldThetaM = 0;
double newThetaM = 0;
float angVelMain;
float angVelMotor;

void flash(){
  setMotor(0);
  getEncoder();
  getAngMotor();
  //getAngVelMotor();
}

void setup(){

  Serial.begin(250000);

// Timer Setup
  FlexiTimer2::set(1, deltaT, flash); // call every 500 1ms "ticks"
  //FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
  FlexiTimer2::start();

// Motor/Encoder Setup
  pinMode(outputA,INPUT);
  pinMode(outputB,INPUT);
  pinMode(set1, OUTPUT);
  pinMode(set2, OUTPUT);
  pinMode(enable, OUTPUT);
  aLastState = digitalRead(outputA);
}

void loop(){
  float seconds = micros()*0.000001;
  Serial.print(seconds);
  Serial.print("\t");
  Serial.print(counter);
  Serial.print("\t");
  Serial.println(counterAngle);
//  Serial.print("\t");
//  Serial.print(revs);
//  Serial.print("\t");
//  Serial.println(angVelMotor);
}

void setMotor(int val){
  digitalWrite(set1, LOW);
  digitalWrite(set2, HIGH);
  analogWrite(enable, val);
}

void getEncoder(){
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState){
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState) {
      counter ++;
    } else {
      counter --;
    }
  }
  counterValue = counter;
  aLastState = aState; // Updates the previous state of the outputA with the current state
}

void getAngMotor(){
  counterAngle = scaleFactor*counterValue;
  realAngle = scaleFactor*counterValue;
  revs = realAngle/maxAngle;
  if(counterAngle >= maxAngle || counterAngle <= -maxAngle){
    int revsOver = (int) counterAngle/maxAngle;
    int revsOverDeg = maxAngle*revsOver;
    counterAngle = counterAngle - revsOverDeg;
  }
}

void getAngVelMotor(){
  deltaT = 0.001;
  newThetaM = counterAngle;
  angVelMotor = (newThetaM - oldThetaM)/deltaT;
  //rpm = revs / 60;
  oldThetaM = newThetaM;
}
