#include <Servo.h>  //Servo library

Servo name_servo; //initialize a servo object for the connected servo int servo_position =0;
void setup()

{

name_servo.attach(9); // attach the signal pin of servo to pin9 of arduino

}

void loop()

{

for (servo_position=0;servo_position<=180;servo_position +=1){ name_servo.write(servo_position);

delay(10);

}

for(servo_position=180; servo_position>=0;servo_position-=1)

{

name_servo.write(servo_position);

}

delay(10);

}
