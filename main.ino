#include <Servo.h>
#include <PID_v1.h>
#include "AnalogPin.h"



//Define your Touch screen connections
const byte cornerPins[] = {2, 3, 4, 5};
// Central sensor pin
const byte sensePin = A0;
AnalogPin Reader(A0);
//Define your screen resolution as per your Touch screen (Max: 1024)
#define Xresolution 261 //128
#define Yresolution 198 //64


double SetpointX, InputX, OutputX;
double SetpointY, InputY, OutputY;
double Kp1=0.09, Ki1=0.00005, Kd1=0.05;
double Kp2=0.02, Ki2=0.00005, Kd2=0.05;
PID myPIDX(&InputX, &OutputX, &SetpointX, Kp1, Ki1, Kd1, DIRECT);
PID myPIDY(&InputY, &OutputY, &SetpointY, Kp2, Ki2, Kd2, DIRECT);



Servo XServo;
Servo YServo;
double ServoAngleX,ServoAngleY;

int X,Y;

void setup()
{
  SetpointX = 0;
  SetpointY = 0;
  myPIDX.SetOutputLimits(-20,20);
  myPIDY.SetOutputLimits(-20,20);

   Serial.begin(9600);
   Serial1.begin(9600);
  XServo.attach(8);
  YServo.attach(9);
  myPIDX.SetMode(AUTOMATIC);
  myPIDY.SetMode(AUTOMATIC);
  Reader.setNoiseThreshold(10);
  for(int i=0; i<4; i++){
    pinMode(cornerPins[i], OUTPUT);
  }
  
}


double IdleXAngle = 109.5;
double IdleYAngle = 85.5;
int BTimer;
bool _timeout = false;
void loop()
{

  KoordinatFonk();
 
  Timeout();
  if(!_timeout)
  ServoPIDKontrol();

 // Serial.println(_timeout);
  
  if(BTimer == 100)
  {
  BTimer = 0;
  LCDKoordinatGoster();
  }
  BTimer++;
  delay(1);
}

void ServoPIDKontrol()
{
  InputX = X;
  InputY = Y;


    myPIDX.Compute();
    myPIDY.Compute();

  ServoAngleX= OutputX + IdleXAngle;
  ServoAngleY=-1*OutputY + IdleYAngle;


    YServo.write(ServoAngleY);
    XServo.write(ServoAngleX);

}
void KoordinatFonk()
{
  
  // Set the left-hand corner pins LOW and the right-hand corner pins HIGH
  digitalWrite(cornerPins[0], HIGH);
  digitalWrite(cornerPins[1], HIGH);
  digitalWrite(cornerPins[2], LOW);
  digitalWrite(cornerPins[3], LOW);
  // Read the sensor value in the x gradient

delayMicroseconds(110);

  
  
  int TestX = Reader.read()-493;


//   Set the bottom corner pins LOW and the top corner pins HIGH
  digitalWrite(cornerPins[0], LOW);
  
  digitalWrite(cornerPins[1], HIGH);

  digitalWrite(cornerPins[2], HIGH);

  digitalWrite(cornerPins[3], LOW);
 
  // Read the sensor value in the y gradient
  delayMicroseconds(110);
  int TestY = Reader.read()-493;
  
  if(!(abs(TestY-Y) > 30 && !_timeout && abs(TestX-X) > 30))
  {
    Y = TestY;
    X = TestX;
  }
  // Display the co-ordinate value obtained
  Serial.print(X);  
  Serial.print(", ");
  Serial.println(Y);

}

void LCDKoordinatGoster()
{

  Serial1.println(X);
  delay(2);
  Serial1.println(Y); 
}

int LastX;
int LastY;
int TimeoutTreshold = 50;
int TimeoutCounter = 0; 

void Timeout()
{
  if(abs(LastX-X) < 15 && abs(LastY-Y) < 15)
    {
      TimeoutCounter++;

      if(TimeoutCounter >= TimeoutTreshold)
      {
        XServo.write(IdleXAngle);
        YServo.write(IdleYAngle);
        Serial.print("True\n");
        _timeout =true;
        
      }
    }
  else
    {
      TimeoutCounter = 0;
      LastX = X;
      LastY = Y;  
      _timeout = false;
    }
    
}
