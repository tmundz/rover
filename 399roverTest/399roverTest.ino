#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);

  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

}

void loop() {
  uint16_t distance = checkDistance();
  Serial.print("passed distance: ");
  Serial.println(distance);

  if(distance < 80){
    coast();
    delay(2000);
    turnLeft(150);
    delay(450);
    coast();
    delay(2000);


    distance = checkDistance();
    if(distance < 80){
      turnRight(150);
      delay(900);
      coast();
      delay(2000);

      if(distance < 80){
        turnRight(150);
        delay(450);
        coast();
        delay(2000);
      }
    }
  } else{
    moveForwards(130);
  }
  moveForwards(130);
}

uint16_t checkDistance(){
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  uint16_t distance = measure.RangeMilliMeter;

  delay(100);
    
  return distance;
}

void motorLeftForward(int speed){
  analogWrite(12, speed);
  digitalWrite(11, LOW);
}

void motorLeftReverse(int speed){
  digitalWrite(12, LOW);
  analogWrite(11, speed);
}

void motorRightForward(int speed){
  digitalWrite(10, speed);
  analogWrite(9, LOW);
}

void motorRightReverse(int speed){
  analogWrite(10, LOW);
  digitalWrite(9, speed);
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
