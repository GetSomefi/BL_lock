#include <Servo.h>

#include<SoftwareSerial.h>

#define rxPin 11//these are in a wrong way lol
#define txPin 10

/* Create object named bt of the class SoftwareSerial */ 
SoftwareSerial bt(rxPin,txPin); /* (Rx White,Tx Black) */	
int looped=0;
char myChar;
int code = 0;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int min = pos;
int max = 90;
int step = 1;

void setup() {
  Serial.begin(9600);	/* Define baud rate for serial communication */
  Serial.println("Serial init");

  delay(2000);

  pinMode(rxPin, INPUT);  
  pinMode(txPin, OUTPUT); 
  
  Serial.println("BT should be connected next " __DATE__ );
  bt.begin(9600);  /* Define baud rate for software serial communication */
  bt.println("BT init");

  //myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

char Comp(char This) {
    if( This != 49 ){ 
      return(0);
    }else{
      return(1);
    }
}

void loop() {
    looped++;
    //Serial.println("BT is NOT available [" + String(looped) + "]");
    if ( bt.available() ){
      myChar = bt.read();
      Serial.write(myChar);
      if (Comp(myChar)==1) {
         Serial.println("Correct");
         bt.write("Ok");

         runServo();
      }else{
        Serial.println("Incorrect");
      }
      
    } 
}

int runServo(){
  myservo.attach(9);
  for (pos = min; pos <= max; pos += step) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(step*15);                       // waits 15ms for the servo to reach the position
  }
  delay(4000);
  for (pos = max; pos >= min; pos -= step) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(step*15);                       // waits 15ms for the servo to reach the position
  }
  delay(200);
  myservo.detach();
  return(1);
}
