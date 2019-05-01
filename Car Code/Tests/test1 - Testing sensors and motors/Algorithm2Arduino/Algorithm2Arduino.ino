#include <Queue.h>

const int rotSpeedR = 109;
const int rotSpeedL = 106;
const int rotTimeR = 270;
const int rotTimeL = 274;

const int moveLSpeed = 95;
const int moveRSpeed = 100;
const int moveTime = 675;

const int sensorsDelay = 0;
const int stepDelay = 2000;
const int startDelay = 3000;

bool hasStarted = false;
int Black;



///////////////////////////////////////////////////ALGORITHM/////////////////////////////////////////////////////////////////////////
struct curPos
    {
        public:
        int x, y;
        String lastDir;
    };
    String myMap[19] = {
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "?????????e?????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????"};
    int carPosX = 9, carPosY = 9;
    Queue<int> QX;
    Queue<int> QY;
    Queue<char[]> QlastDir;
    int * visitedS;
    int * visitedB;
    int facingDirection = 0; //0 up, 1 right, 2 down, 3 left
    bool hasReachedDestination = false;
    int targetX = 0, targetY = 0;
    String instructions;
    bool isMoving = false;
    int instructionIndex;
    float movingTimer = 0;
    float movingTimerMax = 1;
    float rotationTimerMax = 1;
    int rotateDir = 0; //-1 = left # 1 = right
    int movementMode; //0 none, 1 move, 2 rotation
    float moveSpeed = 0.1f;
    float rotateSpeed = 1;
    int Xs, Ys;
    bool hasReturnedToBeg = false;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////out 1 w 2 beta3 el right motor
/////////////out 3 w 4 beta3 el left motor

//For first motor (Left)
const int pwm1 = 9  ;  
const int in_1 = 8 ;
const int in_2 = 11 ;
//  

//For second motor (Right)
const int pwm2 = 10 ;  
const int in_3 = 12 ;
const int in_4 = 13 ;
//

//////////////////Ultra Sonic Front`
const int TriggerPin1 = A0;      //Trig pin
const int EchoPin1 = A1;         //Echo pin
float Duration1 = 0;
//////////////////

//////////////////Ultra Sonic Back
const int TriggerPin2 = 5;      //Trig pin
const int EchoPin2 = 4;         //Echo pin
float Duration2 = 0;
//////////////////

//////////////////Ultra Sonic Left
const int TriggerPin3 = 7;      //Trig pin
const int EchoPin3 = 6;         //Echo pin
float Duration3 = 0;
//////////////////

//////////////////Ultra Sonic Right
const int TriggerPin4 = 3;      //Trig pin
const int EchoPin4 = 2;         //Echo pin
float Duration4 = 0;
//////////////////

/////////////////Black Color Sensor
const int pinBlack= A2;
/////////////////

void setup() {
  // put your setup code here, to run once:
/////////////Motor 1 (Left)
pinMode(pwm1,OUTPUT) ;   //we have to set PWM pin as output
pinMode(in_1,OUTPUT) ;  //Logic pins are also set as output
pinMode(in_2,OUTPUT) ;
/////////////

/////////////Motor 2 (Right)
pinMode(pwm2,OUTPUT) ;   //we have to set PWM pin as output
pinMode(in_3,OUTPUT) ;  //Logic pins are also set as output
pinMode(in_4,OUTPUT) ;
/////////////

/////////////Ultra Sonic Front
// put your setup code here, to run once:
  pinMode(TriggerPin1,OUTPUT);  // Trigger is an output pin
  pinMode(EchoPin1,INPUT);      // Echo is an input pin
  Serial.begin(9600);          // Serial Output
////////////

/////////////Ultra Sonic Back
// put your setup code here, to run once:
  pinMode(TriggerPin2,OUTPUT);  // Trigger is an output pin
  pinMode(EchoPin2,INPUT);      // Echo is an input pin
////////////

/////////////Ultra Sonic Left
// put your setup code here, to run once:
  pinMode(TriggerPin3,OUTPUT);  // Trigger is an output pin
  pinMode(EchoPin3,INPUT);      // Echo is an input pin
////////////

/////////////Ultra Sonic Right
// put your setup code here, to run once:
  pinMode(TriggerPin4,OUTPUT);  // Trigger is an output pin
  pinMode(EchoPin4,INPUT);      // Echo is an input pin
////////////

/////////////////Black Color Sensor
  pinMode(pinBlack,INPUT);
/////////////////
}










//LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
void loop() {
  Start();
  ReadSensors();

float f=GetDistanceFront();
/*
if(f<15)
{
  float L=GetDistanceLeft();
  float R=GetDistanceRight();
  if(L<R)
  {
   RotateCar90Right();
  }
  else if(L>R)
  {
   RotateCar90Left();
  }
}
else
{
  MoveStepForward();
}*/

MoveStepForward();
}
//LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP

void Start()
{
  if(!hasStarted)
  {
    hasStarted = true;
    delay(startDelay);
  }
}

void ReadSensors()
{
  ReadFront();
ReadBack();
ReadLeft();
ReadRight();
ReadBlack();
}

void MoveStepForward()
{
  
  SetWheelLeft(moveLSpeed);
  SetWheelRight(moveRSpeed);
  delay(moveTime);
  SetWheelLeft(0);
  SetWheelRight(0);
  delay(stepDelay);
  
}
void ReadBlack()
{
  /////////////////Black Color Sensor
Black=!digitalRead(pinBlack);
Serial.print("Black Found");
Serial.print("    ");
Serial.print(Black);
Serial.print("\n");
delay(sensorsDelay);
/////////////////
}
bool isBlack()
{
  return Black == 1;
}
void ReadFront()
{
  //////////////Ultra Sonic Front
  digitalWrite(TriggerPin1, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin1, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin1, LOW);           // Trigger pin to HIGH
 
  Duration1 = pulseIn(EchoPin1,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
  float Distance1_cm = GetDistanceFront();   // Use function to calculate the distance
 
   Serial.print("Distance Front = ");             // Output to serial
 Serial.print(Distance1_cm);
 Serial.println(" cm");
 
  delay(sensorsDelay);                             // Wait to do next measurement
}
void ReadRight()
{
  //////////////Ultra Sonic Right
  digitalWrite(TriggerPin4, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin4, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin4, LOW);           // Trigger pin to HIGH
 
  Duration4 = pulseIn(EchoPin4,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
  float Distance4_cm = GetDistanceRight();   // Use function to calculate the distance
 
  Serial.print("Distance Right = ");             // Output to serial
  Serial.print(Distance4_cm);
  Serial.println(" cm");
 
  delay(sensorsDelay);                             // Wait to do next measurement
}
void ReadBack()
{
  //////////////Ultra Sonic Back
  digitalWrite(TriggerPin2, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin2, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin2, LOW);           // Trigger pin to HIGH
 
  Duration2 = pulseIn(EchoPin2,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
  float Distance2_cm = GetDistanceBack();   // Use function to calculate the distance
 
 Serial.print("Distance Back = ");             // Output to serial
 Serial.print(Distance2_cm);
 Serial.println(" cm");
 
  delay(sensorsDelay);                             // Wait to do next measurement
}
void ReadLeft()
{
  //////////////Ultra Sonic Left
  digitalWrite(TriggerPin3, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin3, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin3, LOW);           // Trigger pin to HIGH
 
  Duration3 = pulseIn(EchoPin3,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
  float Distance3_cm = GetDistanceLeft();   // Use function to calculate the distance
 
  Serial.print("Distance Left = ");             // Output to serial
  Serial.print(Distance3_cm);
  Serial.println(" cm");
 
  delay(sensorsDelay);                             // Wait to do next measurement
}
////////////////Right Wheel Movement
void SetWheelRight(int s)
{
  if(s>0)
  {
    //For Clock wise motion 
    digitalWrite(in_3,LOW) ;
    digitalWrite(in_4,HIGH) ;
    analogWrite(pwm2,s) ;
    return;
  }
  else if(s<0)
  {
    //For Anti-Clock wise motion 
    digitalWrite(in_3,HIGH) ;
    digitalWrite(in_4,LOW) ;
    analogWrite(pwm2,-s) ;
    return;
  }
     else if(s==0)
  {
    //For Stopping
    digitalWrite(in_3,LOW) ;
    digitalWrite(in_4,LOW) ;
    analogWrite(pwm2,0) ;
    return;
  }
  return;
}
////////////////
////////////////Left Wheel Movement
void SetWheelLeft(int s)
{
  if(s>0)
  {
    //For Clock wise motion 
    digitalWrite(in_1,LOW) ;
    digitalWrite(in_2,HIGH) ;
    analogWrite(pwm1,s) ;
    return;
  }
  else if(s<0)
  {
    //For Anti-Clock wise motion
    digitalWrite(in_1,HIGH) ;
    digitalWrite(in_2,LOW) ;
    analogWrite(pwm1,-s) ;
    return;
  }
    else if(s==0)
  {
    //For Stopping
    digitalWrite(in_1,LOW) ;
    digitalWrite(in_2,LOW) ;
    analogWrite(pwm1,0) ;
    return;
  }
  return;
}
////////////////
/////////////////Rotation 90 to the left
void RotateCar90Left()
{
 
int s=-rotSpeedL;
int s1=rotSpeedL;
SetWheelLeft(s);
SetWheelRight(s1);
delay(rotTimeL);
SetWheelLeft(0);
SetWheelRight(0);
delay(stepDelay);

}
////////////////
/////////////////Rotation 90 to the right
void RotateCar90Right()
{
  
int s=rotSpeedR;
int s1=-rotSpeedR;
SetWheelLeft(s);
SetWheelRight(s1);
delay(rotTimeR);
SetWheelLeft(0);
SetWheelRight(0);
delay(stepDelay);
}
////////////////
/////////////////Rotation 180
void RotateCar180()
{
  
int s=-rotSpeedL;
int s1=rotSpeedL;
SetWheelLeft(s);
SetWheelRight(s1);
delay(rotTimeL*2);
SetWheelLeft(0);
SetWheelRight(0);
delay(stepDelay);

}
////////////////
/////////////////Ultra Sonic Front
float GetDistanceFront()
{
  float time = Duration1;
  float DistanceCalc;                      // Calculation variable
   
  DistanceCalc = ((time * 0.034) / 2);     // Actual calculation in cm
    //DistanceCalc = time / 74 / 2;         // Actual calculation in inches
  return DistanceCalc;                    // return calculated value
}
/////////////////
/////////////////Ultra Sonic Back
float GetDistanceBack()
{
  float time = Duration2;
  float DistanceCalc;                      // Calculation variable
   
  DistanceCalc = ((time * 0.034) / 2);     // Actual calculation in cm
    //DistanceCalc = time / 74 / 2;         // Actual calculation in inches
  return DistanceCalc;                    // return calculated value
}
/////////////////
/////////////////Ultra Sonic Left
float GetDistanceLeft()
{
  float time = Duration3;
  float DistanceCalc;                      // Calculation variable
   
  DistanceCalc = ((time * 0.034) / 2);     // Actual calculation in cm
    //DistanceCalc = time / 74 / 2;         // Actual calculation in inches
  return DistanceCalc;                    // return calculated value
}
/////////////////
/////////////////Ultra Sonic Right
float GetDistanceRight()
{
  float time = Duration4;
  float DistanceCalc;                      // Calculation variable
   
  DistanceCalc = ((time * 0.034) / 2);     // Actual calculation in cm
    //DistanceCalc = time / 74 / 2;         // Actual calculation in inches
  return DistanceCalc;                    // return calculated value
}
/////////////////
