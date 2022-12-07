#include "Adafruit_VL53L0X.h"
#include <Wire.h>
#include "Adafruit_VEML7700.h"

Adafruit_VEML7700 veml = Adafruit_VEML7700();

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);

  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(10);
  }
  
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    //while(1);
  } else {
    Serial.println("Found a VL53L0X sensor");
  }

  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }else{
    Serial.println("Sensor found");
  }

  Serial.print(F("Gain: "));
  switch (veml.getGain()) {
    case VEML7700_GAIN_1: Serial.println("1"); break;
    case VEML7700_GAIN_2: Serial.println("2"); break;
    case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
    case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
  }

  Serial.print(F("Integration Time (ms): "));
  switch (veml.getIntegrationTime()) {
    case VEML7700_IT_25MS: Serial.println("25"); break;
    case VEML7700_IT_50MS: Serial.println("50"); break;
    case VEML7700_IT_100MS: Serial.println("100"); break;
    case VEML7700_IT_200MS: Serial.println("200"); break;
    case VEML7700_IT_400MS: Serial.println("400"); break;
    case VEML7700_IT_800MS: Serial.println("800"); break;
  }

  veml.setLowThreshold(10000);
  veml.setHighThreshold(20000);
  veml.interruptEnable(true);

}

void loop() {
  uint16_t distance = checkDistance();
   delay(150);
  
  uint16_t reading = veml.readLux();
  buzzerDynamic(reading);

  if(distance < 350){
    coast();
    delay(1000);
    turnLeft(255);
    delay(900);
    coast();
    uint16_t reading = veml.readLux();
    buzzerDynamic(reading);
    delay(2000);


    distance = checkDistance();
    if(distance < 200){
      turnRight(255);
      delay(1800);
      coast();
      uint16_t reading = veml.readLux();
      buzzerDynamic(reading);
      delay(2000);

      if(distance < 200){
        turnRight(255);
        delay(900);
        coast();
        uint16_t reading = veml.readLux();
        buzzerDynamic(reading);
        delay(2000);
      }
    }
  } else{
    moveForwards(255);
    //motorLeftReverse(200);
  }
    //moveForwards(255);
    //motorLeftReverse(200);
   //motorLeftForward(150);
  //motorRightForward(200);
  //motorRightReverse(200);
}

uint16_t checkDistance(){
  VL53L0X_RangingMeasurementData_t measure;
   
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  uint16_t distance = measure.RangeMilliMeter;
    
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
  motorRightForward(speed + 50);
}


void buzzerDynamic(uint16_t reading){
  reading = reading;
  int toneValue = 1400 * reading + 400;
  int delayValue = 0.13896609227 * reading + 49.8610339077;
  tone(7, toneValue);
  delay(delayValue);
  noTone(7);
  delay(delayValue);
  tone(7, toneValue);
  delay(delayValue);
  noTone(7);
}
