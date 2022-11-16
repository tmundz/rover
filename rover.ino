#include <Arduino.h>


void setup() {
 pinMode(12, OUTPUT);//b1
 pinMode(11, OUTPUT); //b2
 pinMode(10, OUTPUT); //a1
 pinMode(9, OUTPUT); //a2



}

void motorLeftForward(int speed){
 analogWrite(10, speed);
 digitalWrite(9, LOW);
}
void motorLeftReverse(int speed){
 analogWrite(10, LOW);
 digitalWrite(9, speed);
}
void motorRightReverse(int speed){
 analogWrite(12, LOW);
 digitalWrite(11, speed);
}
void motorRightForward(int speed){
 analogWrite(12, speed);
 digitalWrite(11, LOW);
}
void moveForwards(int speed) {
 motorLeftForward(speed);
 motorRightForward(speed);

}
void moveBackwards(int speed) {
 motorLeftReverse(speed);
 motorRightReverse(speed);

}
void moveRight(int speed) {
 motorLeftForward(speed);
 motorRightReverse(speed);

}
void moveLeft(int speed) {
 motorLeftReverse(speed);
 motorRightForward(speed);

}
void coast() {
 digitalWrite(12, LOW);
 digitalWrite(11, LOW);
 digitalWrite(10, LOW);
 digitalWrite(9, LOW);
}
void brake() {
 digitalWrite(12, HIGH);
 digitalWrite(11, HIGH);
 digitalWrite(10, HIGH);
 digitalWrite(9, HIGH); 
}

void loop() {
 /*
 * Check if received data is available and if yes, try to decode it.
 * Decoded result is in the IrReceiver.decodedIRData structure.
 *
 * E.g. command is in IrReceiver.decodedIRData.command
 * address is in command is in IrReceiver.decodedIRData.address
 * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
 */
 moveForwards(150);

}
