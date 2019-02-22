//COM 5

#include <Wire.h>
//#include "Globals.h"
#include "Wheels.h"
#include "Arm.h"

using namespace std;

Wheels wheels;
Arm arm;
bool writeWheelParams = true;

void setup() {
  setPinModes();
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveHandler);
  Wire.onRequest(requestHandler);
  Serial.begin(9600);
}

void loop() {
  if (writeWheelParams) {
    wheels.writeParams();
    writeWheelParams = false;
  }
  checkClearErrorStates();
  wheels.updateFeedbackData();
}

void checkClearErrorStates() {
  for (int i = 0; i < NUM_WHEELS; i++) {
    if (wheels.wheelList[i].error) {
      digitalWrite(wheels.wheelList[i].enable_pin, false);
      delay(10);
      digitalWrite(wheels.wheelList[i].enable_pin, true);
    }
  }
}

void receiveHandler(int byteCount) {
  switch(Wire.read()) {
    case 1: setWheelParams(); break;
    case 2: setArmParams(); break;
    default: flushWire(); break;
  }
}

void flushWire() {
  while(Wire.available()) {
    Wire.read();
  }
}

void requestHandler() {
  for (int i = 0; i < NUM_WHEELS; i++) {
//    Wire.write((uint8_t*)&wheels.wheelList[i].actual_speed, 2);
    Wire.write(wheels.wheelList[i].actual_speed);
    Wire.write(wheels.wheelList[i].error);
  }
}

void setWheelParams() {
  if (Wire.available() == 12) { 
    for (int i = 0; i < NUM_WHEELS; i++) {
      wheels.wheelList[i].set_speed = Wire.read();
      wheels.wheelList[i].dir = Wire.read();
    }
    writeWheelParams = true;
  } else {
    flushWire();
    Serial.println("wrong number of bytes sent in: setWheelParams");
  }
}

//    armCommands = bytearray([armMessage.turret_speed, armMessage.turret_dir, shoulder_low, 
//      shoulder_high, elbow_low, elbow_high, armMessage.wrist_speed, 
//      armMessage.wrist_dir, armMessage.hand_speed, armMessage.hand_dir]);

void setArmParams() {
  if (Wire.available() == 10) {
    arm.set_turret_params(Wire.read(), Wire.read()); // params are speed and dir in that order
    arm.set_shoulder_params(Wire.read(), Wire.read()); // params are pos_high_byte and pos_low_byte in that order!
    arm.set_elbow_params(Wire.read(), Wire.read()); // params are pos_high_byte and pos_low_byte in that order!
    arm.set_wrist_params(Wire.read(), Wire.read()); 
    arm.set_hand_params(Wire.read(), Wire.read()); 
  }
  else {
    // maybe notify someone that this failed????
    flushWire();
  }
}


void setPinModes() {
//  pinMode(RIGHT_FRONT_WHEEL_SET_SPEED, OUTPUT);
//  pinMode(RIGHT_FRONT_WHEEL_DIR, OUTPUT);
//  pinMode(RIGHT_FRONT_WHEEL_ENABLE, OUTPUT);
//  pinMode(RIGHT_FRONT_WHEEL_ACTUAL_SPEED, INPUT);
//  pinMode(RIGHT_FRONT_WHEEL_ERROR, INPUT_PULLUP);
//  
//  pinMode(RIGHT_MIDDLE_WHEEL_SET_SPEED, OUTPUT);
//  pinMode(RIGHT_MIDDLE_WHEEL_DIR, OUTPUT);
//  pinMode(RIGHT_MIDDLE_WHEEL_ENABLE, OUTPUT);
//  pinMode(RIGHT_MIDDLE_WHEEL_ACTUAL_SPEED, INPUT);
//  pinMode(RIGHT_MIDDLE_WHEEL_ERROR, INPUT_PULLUP);
//  
//  pinMode(RIGHT_REAR_WHEEL_SET_SPEED, OUTPUT);
//  pinMode(RIGHT_REAR_WHEEL_DIR, OUTPUT);
//  pinMode(RIGHT_REAR_WHEEL_ENABLE, OUTPUT);
//  pinMode(RIGHT_REAR_WHEEL_ACTUAL_SPEED, INPUT);
//  pinMode(RIGHT_REAR_WHEEL_ERROR, INPUT_PULLUP);
//  
//  pinMode(LEFT_FRONT_WHEEL_SET_SPEED, OUTPUT);
//  pinMode(LEFT_FRONT_WHEEL_DIR, OUTPUT);
//  pinMode(LEFT_FRONT_WHEEL_ENABLE, OUTPUT);
//  pinMode(LEFT_FRONT_WHEEL_ACTUAL_SPEED, INPUT);
//  pinMode(LEFT_FRONT_WHEEL_ERROR, INPUT_PULLUP);
//  
//  pinMode(LEFT_MIDDLE_WHEEL_SET_SPEED, OUTPUT);
//  pinMode(LEFT_MIDDLE_WHEEL_DIR, OUTPUT);
//  pinMode(LEFT_MIDDLE_WHEEL_ENABLE, OUTPUT);
//  pinMode(LEFT_MIDDLE_WHEEL_ACTUAL_SPEED, INPUT);
//  pinMode(LEFT_MIDDLE_WHEEL_ERROR, INPUT_PULLUP);
//  
//  pinMode(LEFT_REAR_WHEEL_SET_SPEED, OUTPUT);
//  pinMode(LEFT_REAR_WHEEL_DIR, OUTPUT);
//  pinMode(LEFT_REAR_WHEEL_ENABLE, OUTPUT);
//  pinMode(LEFT_REAR_WHEEL_ACTUAL_SPEED, INPUT);
//  pinMode(LEFT_REAR_WHEEL_ERROR, INPUT_PULLUP);
}
