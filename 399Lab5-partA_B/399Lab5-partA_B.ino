#define DECODE_NEC

#include <Arduino.h>

#include "Adafruit_VL53L0X.h"

#define IR_RECEIVE_PIN 5
#include <IRremote.hpp>
#if !defined(STR_HELPER)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#endif

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 

}

void loop() {
  Serial.println("Step");
  if (IrReceiver.decode()) {
    
        IrReceiver.resume(); // Enable receiving of the next value
         
        if (IrReceiver.decodedIRData.command == 0x5) {
          Serial.println("Forward");
          coast();
        } else if (IrReceiver.decodedIRData.command == 0x0) {
          Serial.println("Backwards");
          coast();
        } else if (IrReceiver.decodedIRData.command == 0x8) {
          Serial.println("Left");
          coast();
        } else if (IrReceiver.decodedIRData.command == 0x1) {
          Serial.println("Right");
          coast();
        } else if (IrReceiver.decodedIRData.command == 0x4) {
          Serial.println("Coast");
          coast();
        }
        
   }
   Serial.println(lox.readRange());
   if(lox.readRange() <= 60){
     turnLeft(140);
     delay(800);
     coast();

     Serial.println(lox.readRange());
     if(Serial.println(lox.readRange()) <= 60){
      turnRight(140);
      delay(1600);
      coast();
     }
   }
}

int fracTo8Bit(float value){
  int speed = value * 255;
  speed = floor(speed);

  return speed;
}

void motorLeftForward(int speed){
  analogWrite(10, speed);
  digitalWrite(9, LOW);
}

void motorLeftReverse(int speed){
  digitalWrite(9, LOW);
  analogWrite(10, speed);
}

void motorRightForward(int speed){
  digitalWrite(11, LOW);
  analogWrite(12, speed);
}

void motorRightReverse(int speed){
  analogWrite(11, speed);
  digitalWrite(12, LOW);
}

void coast(){
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
}

void brake(){
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
}

void turnRight(int speed){
  motorLeftForward(speed);
  motorRightReverse(speed);
}

void turnLeft(int speed){
  motorLeftReverse(speed);
  motorRightForward(speed);
}

void moveBackwards(int speed){
  motorLeftReverse(speed);
  motorRightReverse(speed);
}

void moveForwards(int speed){
  motorLeftForward(speed);
  motorRightForward(speed);
}
