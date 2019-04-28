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

void loop() {
  // put your main code here, to run repeatedly:

//////////////Ultra Sonic Front
// put your main code here, to run repeatedly:
  digitalWrite(TriggerPin1, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin1, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin1, LOW);           // Trigger pin to HIGH
 
  Duration1 = pulseIn(EchoPin1,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
 
//////////////

//////////////Ultra Sonic Back
// put your main code here, to run repeatedly:
  digitalWrite(TriggerPin2, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin2, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin2, LOW);           // Trigger pin to HIGH
 
  Duration2 = pulseIn(EchoPin2,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
///////////////
//////////////Ultra Sonic Left
// put your main code here, to run repeatedly:
  digitalWrite(TriggerPin3, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin3, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin3, LOW);           // Trigger pin to HIGH
 
  Duration3 = pulseIn(EchoPin3,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
//////////////
//////////////Ultra Sonic Right
// put your main code here, to run repeatedly:
  digitalWrite(TriggerPin4, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin4, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin4, LOW);           // Trigger pin to HIGH
 
  Duration4 = pulseIn(EchoPin4,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds

//////////////

/////////////////Black Color Sensor
int Black=digitalRead(pinBlack);
////////////////
delay(500);
float f=GetDistanceFront(Duration1);
if (f>30)
{
SetWheelLeft(75);
SetWheelRight(75);
delay(500);
SetWheelLeft(0);
SetWheelRight(0);
}
else 
{
  
  float L=GetDistanceLeft(Duration3);
  float R=GetDistanceRight(Duration4);
  if(L<R)
  {
   RotateCar90Right();
  }
  else if(L>R)
  {
   RotateCar90Left();
  }
}
delay(500);
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
int s=-100;
int s1=100;
SetWheelLeft(s);
SetWheelRight(s1);
delay(275);
SetWheelLeft(0);
SetWheelRight(0);
}
////////////////
/////////////////Rotation 90 to the right
void RotateCar90Right()
{
int s=100;
int s1=-100;
SetWheelLeft(s);
SetWheelRight(s1);
delay(275);
SetWheelLeft(0);
SetWheelRight(0);
}
////////////////
/////////////////Rotation 180
void RotateCar180()
{
int s=-100;
int s1=100;
SetWheelLeft(s);
SetWheelRight(s1);
delay(540);
SetWheelLeft(0);
SetWheelRight(0);
}
////////////////
/////////////////Ultra Sonic Front
float GetDistanceFront(float time)
{
  float DistanceCalc;                      // Calculation variable
   
  DistanceCalc = ((time * 0.034) / 2);     // Actual calculation in cm
    //DistanceCalc = time / 74 / 2;         // Actual calculation in inches
  return DistanceCalc;                    // return calculated value
}
/////////////////
/////////////////Ultra Sonic Back
float GetDistanceBack(float time)
{
  float DistanceCalc;                      // Calculation variable
   
  DistanceCalc = ((time * 0.034) / 2);     // Actual calculation in cm
    //DistanceCalc = time / 74 / 2;         // Actual calculation in inches
  return DistanceCalc;                    // return calculated value
}
/////////////////
/////////////////Ultra Sonic Left
float GetDistanceLeft(float time)
{
  float DistanceCalc;                      // Calculation variable
   
  DistanceCalc = ((time * 0.034) / 2);     // Actual calculation in cm
    //DistanceCalc = time / 74 / 2;         // Actual calculation in inches
  return DistanceCalc;                    // return calculated value
}
/////////////////
/////////////////Ultra Sonic Right
float GetDistanceRight(float time)
{
  float DistanceCalc;                      // Calculation variable
   
  DistanceCalc = ((time * 0.034) / 2);     // Actual calculation in cm
    //DistanceCalc = time / 74 / 2;         // Actual calculation in inches
  return DistanceCalc;                    // return calculated value
}
/////////////////
