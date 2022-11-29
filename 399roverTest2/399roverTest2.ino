#include "Adafruit_VL53L0X.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)

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
    delay(1);
  }
  
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  
   if (tsl.begin()) 
  {
    Serial.println(F("Found a TSL2591 sensor"));
  } 
  else 
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }
    
  /* Configure the sensor */
  configureSensor();

}

void loop() {
  uint16_t distance = checkDistance();
  Serial.print("passed distance: ");
  Serial.println(distance);
  
  uint16_t reading = simpleRead();
  Serial.print("Reading: ");
  Serial.println(reading);
  buzzerDynamic(reading);


  if(distance < 80){
    coast();
    delay(2000);
    turnLeft(150);
    delay(600);
    coast();
    uint16_t reading = simpleRead();
    Serial.print("Reading: ");
    Serial.println(reading);
    buzzerDynamic(reading);
    delay(2000);


    distance = checkDistance();
    if(distance < 80){
      turnRight(150);
      delay(1200);
      coast();
      uint16_t reading = simpleRead();
      Serial.print("Reading: ");
      Serial.println(reading);
      buzzerDynamic(reading);
      delay(2000);

      if(distance < 80){
        turnRight(150);
        delay(600);
        coast();
        uint16_t reading = simpleRead();
        Serial.print("Reading: ");
        Serial.println(reading);
        buzzerDynamic(reading);
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

void configureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */  
  tsl2591Gain_t gain = tsl.getGain();
  switch(gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));
      break;
  }
  Serial.print  (F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC); 
  Serial.println(F(" ms"));
}

void buzzerDynamic(uint16_t reading){
  reading = reading / 5;
  int toneValue = -0.500277935 * reading + 1100.5002779;
  int delayValue = 0.13896609227 * reading + 49.8610339077;
  tone(7, toneValue);
  delay(delayValue);
  noTone(7);
  delay(delayValue);
  tone(7, toneValue);
  delay(delayValue);
  noTone(7);
}
