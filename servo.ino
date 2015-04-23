/*
TODO:
- Change servo back to using angular values instead of power levels.
- http://www.instructables.com/id/Cheap-2-Way-Bluetooth-Connection-Between-Arduino-a/?ALLSTEPS
*/


#include <SoftwareSerial.h>
#include <Servo.h>

Servo myServo;
char receivedChar; // User's serial input
char stop_rotate; //Input for reading while spinning
int  LED1 = 13; // LED on pin 13
int LED2 = 8; // Second LED on pin 8
SoftwareSerial mySerial(0, 1); // RX, TX
int ccw_count = 0;

// RX on Bluetooth to TCX on Arduino through a voltage divider 50ohm from arduino and 100ohm to ground.
// This is so we can drop the the voltage down to 3.3v (roughly)
// Use blueterm on android to get messages. Does NOT work on IOS.

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  myServo.attach(8);
  myServo.write(0);
 // forward(0);

  // Open serial communications and wait for port to open:
  mySerial.begin(9600);
  mySerial.println("DOORBUSTER project using bluetooth communication");
  delay(1000);
  mySerial.println("Sending a 1 rotates clockwise 180 degrees, then counter clockwise 180 degrees");
  mySerial.println("Sending a 0 during the clockwise rotation will reset back to  0...hopefully");
  delay(1000);
}

void loop() // Event loop runs forever
{
  digitalWrite(8, HIGH);
  // stay here so long as COM port is empty
  while (!mySerial.available());

  receivedChar = mySerial.read();

  delay(1000);
  if ('0' == receivedChar)
  {
    //digitalWrite(LED1, HIGH);
    myServo.write(0);
  }
  else if ('1' == receivedChar)
  {
    receivedChar = 0;
    //digitalWrite(LED1, LOW);
    CW(5);
    //digitalWrite(LED2, HIGH);
    delay(3000);
    
    CCW(5);
    //digitalWrite(LED2, LOW);

  }
  else if ('2' == receivedChar)
  {
    receivedChar = 0;
    //digitalWrite(LED1, LOW);
    CW(5);
    while(receivedChar != '3')
    {
      receivedChar = mySerial.read();
    }
    CCW(5);
  }
  
}

void CW(int speed) // aka clockwise
{
  for(int pos = 0; pos <= 180; pos++) // goes from 0 degrees to 180 degrees
  {                                  // in steps of 1 degree
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(speed);                   // waits 15ms for the servo to reach the position\
    ccw_count = pos;
    stop_rotate = mySerial.read();
    if(stop_rotate == '0')
      break;
  }
  return;
}

void CCW(int speed) // aka counter clockwise
{
  for(int pos = ccw_count; pos >= 0 ; pos--)     // goes from 180 degrees to 0 degrees
  {
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(speed);                       // waits 15ms for the servo to reach the position
    stop_rotate = mySerial.read();
  }
  return;
}

/*
void forward(int powerLevel)
{
  if (300 < powerLevel)
    powerLevel = 300;

  myServo.writeMicroseconds(1500 + powerLevel);
}

void backward(int powerLevel)
{
  if (300 < powerLevel)
    powerLevel = 300;

  myServo.writeMicroseconds(1500 - powerLevel);
}
*/
