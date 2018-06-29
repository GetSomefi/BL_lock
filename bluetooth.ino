#include <Servo.h>
#include <SoftwareSerial.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define rxPin 11//these are in a wrong way lol
#define txPin 10

/* tutorial https://youtu.be/xVC0X_PE_XE */
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

/* Create object named bt of the class SoftwareSerial */ 
SoftwareSerial bt(rxPin,txPin); /* (Rx White,Tx Black) */	
int looped=0;
char myChar;
int code = 0;
String pinCode;

int redPin = 12;                  // Red LED connected to digital pin 12
int greenPin = 13;                // Green LED connected to digital pin 13


Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int min = pos;
int max = 125;
int step = 1;

int lcdLoopCount = 0;

void setup() {
  //Wire.begin();
  lcd.begin(16,2);
  
  lcd.clear();
  lcd.home ();// go home
  lcd.print("THE LOCK 2");
  lcd.setCursor(0,1);// second line
  lcd.print("starting...");
  
  pinMode(redPin, OUTPUT);        // sets the digital pin as output
  pinMode(greenPin, OUTPUT);      // sets the digital pin as output
  
  Serial.begin(9600);	/* Define baud rate for serial communication */
  Serial.println("Serial init");

  delay(2000);

  pinMode(rxPin, INPUT);  
  pinMode(txPin, OUTPUT); 
  
  bt.begin(9600);  /* Define baud rate for software serial communication */
  delay(1000);
  Serial.println("BT should be connected next " __DATE__ );
  bt.println("init");

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
    //scanI2C(); //needs Wire.begin on setup
    looped++;
    digitalWrite(greenPin, HIGH);

    lcdStartingScreen();
    
    //Serial.println("Loop" + String(looped));
    if ( bt.available() ){
      //myChar = bt.read();
      digitalWrite(greenPin, LOW);
      pinCode = bt.readString();
      delay(1000);
      Serial.println("Pin code");
      Serial.println(pinCode);
      bt.write("Code received");

      //Serial.write(myChar);
      if (pinCode=="905623") {
        lcd.home ();// go home
        lcd.print("ACCESS GRANTED!");
        lcd.setCursor(0,1);// second line
        lcd.print("opening...");
        lcdLoopCount = 0;
        
        Serial.println("Correct");
        bt.write("ok");
        
        runServo();
      }else{
        lcd.home ();// go home
        lcd.print("ACCESS DENIED!");
        lcd.setCursor(0,1);// second line
        lcd.print("Please try again");
        
        Serial.println("Incorrect");
        bt.write("error");
        runIncorrect();

        delay(1000);
        lcd.clear();
        lcd.home ();// go home
        lcd.print("---THE LOCK 2---");
        lcd.setCursor(0,1);// second line
        lcd.print("Ready");
        lcdLoopCount = 0;
      }

    } 
}

int runIncorrect(){
    digitalWrite(redPin, HIGH);
    delay(800);
    digitalWrite(redPin, LOW);
    delay(100);
    digitalWrite(redPin, HIGH);
    delay(800);
    digitalWrite(redPin, LOW);
    delay(100);
    digitalWrite(redPin, HIGH);
    delay(800);
    digitalWrite(redPin, LOW);
    delay(100);
    digitalWrite(redPin, HIGH);
    delay(800);
    digitalWrite(redPin, LOW); 
}

int runServo(){
  digitalWrite(greenPin, HIGH);
  myservo.attach(9);
  for (pos = min; pos <= max; pos += step) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(step*15);                       // waits 15ms for the servo to reach the position
  }
  bt.write("open");
  runCorrect();
  bt.write("closing");
  digitalWrite(redPin, HIGH);
  for (pos = max; pos >= min; pos -= step) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(step*15);                       // waits 15ms for the servo to reach the position
  }
  delay(200);
  myservo.detach();
  digitalWrite(redPin, LOW);
  bt.write("locked");
  return(1);
}

int runCorrect(){
  lcd.clear();
  lcd.home ();// go home
  lcd.print("Lock open for");
  lcd.setCursor(0,1);// second line
  lcd.print("4s");
        
  digitalWrite(greenPin, LOW);
  delay(100);
  digitalWrite(greenPin, HIGH);
  delay(900);
  
  lcd.clear();
  lcd.home ();// go home
  lcd.print("Lock open for");  
  lcd.setCursor(0,1);// second line
  lcd.print("3s");
  
  digitalWrite(greenPin, LOW);
  delay(100);
  digitalWrite(greenPin, HIGH);
  delay(900);

  lcd.clear();
  lcd.home ();// go home
  lcd.print("Lock open for");
  lcd.setCursor(0,1);// second line
  lcd.print("2s");
  
  digitalWrite(greenPin, LOW);
  delay(100);
  digitalWrite(greenPin, HIGH);
  delay(900);

  lcd.clear();
  lcd.home ();// go home
  lcd.print("Lock open for");
  lcd.setCursor(0,1);// second line
  lcd.print("1s");
  
  digitalWrite(greenPin, LOW);
  delay(100);
  digitalWrite(greenPin, HIGH);
  delay(900);
  digitalWrite(greenPin, LOW);

  lcd.clear();
  lcd.home ();// go home
  lcd.print("ATTENTION");
  lcd.setCursor(0,1);// second line
  lcd.print("Lock now closing");
}

int lcdStartingScreen(){
  if(lcdLoopCount==0){
    lcdLoopCount = 1;
    lcd.clear();
    lcd.home ();// go home
    lcd.print("---THE LOCK 2---");
    lcd.setCursor(0,1);// second line
    lcd.print("Ready");
  }
}

int scanI2C(){
  //this should give 0x27
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}
