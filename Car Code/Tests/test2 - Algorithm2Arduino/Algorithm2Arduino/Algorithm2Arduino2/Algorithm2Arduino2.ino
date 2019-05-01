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
float wallExistDistance = 20;




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
    
    curPos Q[50];
    int Qstart = 0;
    int Qsize = 0;
    
    int visitedS[100];
    int visitedB[100];
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
Update();
}
//LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP








/////////////////////////AAAAAAAAAAAAAAAAAAAAAAAALLLLLLLLLLLLLLLLLLLLLLLLLGGGGGGGGGGGGGGGGGGGGGGGOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void PrintMap()
{
  for(int i = 0; i < myMap[0].length(); i++)
  {
    Serial.println(myMap[i]);
  }
  Serial.println("");
}
void Qclear()
{
  /*
  while(Qcount() > 1)
  {
    Qpop();
  }
  Qclear();*/
  Qstart = 0;
    Qsize = 0;
  for(int i = 0; i < 50; i++)
  {
    Q[i].x = 0; Q[i].y = 0; Q[i].lastDir = "";
    
  }
}
void Qpush(curPos cP)
{
  Q[(Qstart+Qsize)%50] = cP;
  Qsize++;
}
void Qpop()
{
  if(Qsize < 1)
  {
    return;
  }
  Q[Qstart].x = 0; Q[Qstart].y = 0; Q[Qstart].lastDir = "";
  Qstart = (Qstart+1) % 50;
  Qsize--;
}
curPos Qpeek()
{
  return Q[Qstart];
}
int Qcount()
{
  return Qsize;
}
void Awake()
    {
      
        //carPosX = (temp.Length) / 2; carPosY = (temp.Length)/2;
        refreshVS();
        refreshVB();
        
        Xs = carPosX; Ys = carPosY;
        hasReachedDestination = false;
    }
void Start()
{
  if(!hasStarted)
  {
    Awake();
    hasStarted = true;
    delay(startDelay);
  }
}
    void refreshVS()
    {
        for (int i = 0; i < 100; i++)
            visitedS[i] = 0;
    }
    void refreshVB()
    {
        for (int i = 0; i < 100; i++)
            visitedB[i] = 0;
    }
    void ResetMode()//0 none, 1 move, 2 rotation
    {
        movingTimer = 0;
        movementMode = 0;
    }
    void SetMovingMode()//0 none, 1 move, 2 rotation
    {
        movingTimer = movingTimerMax;
        movementMode = 1;
    }
    void SetRotationMode(int dir, float multiplier)//0 none, 1 move, 2 rotation //-1 = left # 1 = right
    {
        rotateDir = dir;
        movingTimer = rotationTimerMax*multiplier;
        movementMode = 2;
    }
    bool isResetMode()//0 none, 1 move, 2 rotation
    {
        return movementMode == 0;
    }
    bool isMovementMode()//0 none, 1 move, 2 rotation
    {
        return movementMode == 1;
    }
    bool isRotationMode()//0 none, 1 move, 2 rotation
    {
        return movementMode == 2;
    }    
int get2DIn1D(int x, int y)
    {
        return x/2 + 10*y/2;
    }
    void MovePhysicallyForward()
    {
        MoveStepForward();
    }
    void RotatePhysically()
    {
        if(rotateDir == 1)
        {
            RotateCar90Right();
        }
        if (rotateDir == -1)
        {
            RotateCar90Left();
        }
    }
    void pickGoToTargetSpot()
    {
        curPos beg;
        beg.x = carPosX; beg.y = carPosY;
        refreshVS();
        Qclear();
        Qpush(beg);
        pickGoToTargetSpotBFS();
    }
    void pickGoToTargetSpotBFS()
    {
      Serial.println(Qcount());   
        if (Qcount() == 0)
        {
          Serial.println("GG");
            return;
        }
        curPos cP = Qpeek();
        Qpop(); 
           
        if (cP.x < 0 || cP.y < 0 || cP.x >= myMap[0].length() || cP.y >= myMap[0].length() || isWall(cP) || myMap[cP.y][cP.x] == '?' || visitedS[get2DIn1D(cP.x, cP.y)] == 1)
        {

          Serial.println("OUT");
            pickGoToTargetSpotBFS();
            return;
        }          
        visitedS[get2DIn1D(cP.x, cP.y)] = 1;
        if(visitedB[get2DIn1D(cP.x, cP.y)] == 1)
        {
          while(Qcount() > 0)
          {
            pickGoToTargetSpotBFS();
          }
          Serial.println("Next0");
            curPos right, down, left, up;
            Serial.println("Next1");
            right.x = cP.x + 2; right.y = cP.y; right.lastDir = "r";
            down.x = cP.x; down.y = cP.y + 2; down.lastDir = "d";
            left.x = cP.x - 2; left.y = cP.y; left.lastDir = "l";
            up.x = cP.x; up.y = cP.y - 2; up.lastDir = "u";
            Serial.println("Next20");
            Qpush(right);
            Serial.println("Next21"); 
            Qpush(down); 
            Serial.println("Next22");
            Qpush(left); 
            Serial.println("Next23");
            Qpush(up);
            Serial.println("Next3");
            pickGoToTargetSpotBFS();
            Serial.println("Next4");
            return;
        }
        Qclear();
        targetX = cP.x; targetY = cP.y;
        Serial.println("WIN");
        return;
    }
     bool isWall(curPos cP)
    {
        //Opposite
        if (cP.lastDir == "" || cP.lastDir.length() == 0)
            return false;
        if(cP.lastDir[cP.lastDir.length()-1] == 'u')
        {
            return myMap[cP.y+1][cP.x] == '1';
        }
        else if (cP.lastDir[cP.lastDir.length() - 1] == 'r')
        {
            return myMap[cP.y][cP.x-1] == '1';
        }
        else if (cP.lastDir[cP.lastDir.length() - 1] == 'd')
        {
            return myMap[cP.y-1][cP.x] == '1';
        }
        else if (cP.lastDir[cP.lastDir.length() - 1] == 'l')
        {
            return myMap[cP.y][cP.x+1] == '1';
        }
        return false;
    }
    void solve(int X, int Y)
    {
        if(X < 0 || Y < 0 || X >= myMap[0].length() || Y >= myMap[0].length() || myMap[Y][X] == '1' || visitedB[get2DIn1D(X, Y)] == 1)
        {
            
            return;
        }

        visitedB[get2DIn1D(X, Y)] = 1;        

        curPos beg;
        beg.x = carPosX; beg.y = carPosY; beg.lastDir = "";
        Qpush(beg);
        refreshVS();        
        instructions = getShortestPath(X, Y);
        instructionIndex = 0;
        isMoving = true;
        ResetMode();
    }
    String getShortestPath(int tX, int tY)
    {
        if (Qcount() == 0)
            return "";
        curPos cP = Qpeek();
        Qpop();
        if (cP.x < 0 || cP.y < 0 || cP.x >= myMap[0].length() || cP.y >= myMap[0].length() || isWall(cP) || myMap[cP.y][cP.x] == '?' || visitedS[get2DIn1D(cP.x, cP.y)] == 1)
        {
            
            return getShortestPath(tX, tY);
        }
        visitedS[get2DIn1D(cP.x, cP.y)] = 1;
        if (cP.x == tX && cP.y == tY)
        {
            Qclear();
            return cP.lastDir;
        }
        while(Qcount() != 0)
        {
            String s = getShortestPath(tX, tY);
            if (s != "")
                return s;
        }
        curPos right, down, left, up;
        right.x = cP.x + 2; right.y = cP.y; right.lastDir = cP.lastDir + "r";
        down.x = cP.x; down.y = cP.y + 2; down.lastDir = cP.lastDir + "d";
        left.x = cP.x - 2; left.y = cP.y; left.lastDir = cP.lastDir + "l";
        up.x = cP.x; up.y = cP.y - 2; up.lastDir = cP.lastDir + "u";
        Qpush(right); Qpush(down); Qpush(left); Qpush(up);

        return getShortestPath(tX, tY);
    }
    bool isWin()
    {
        return isBlack();
    }
    void discover()
    {
        //0 up, 1 right, 2 down, 3 left
        discoverDir(0);
        discoverDir(1);
        discoverDir(2);
        discoverDir(3);
    }
    void discoverDir(int dir)
    {
        char c;
        //beg Determine char
        if(facingDirection == dir) //00 11 22 33
        {
          if(!isWallFront())
          {
            c = '0';
          }
          else
          {
            c = '1';
          }
        }
        if(((facingDirection+1) % 4) == dir) //fd/d 01 12 23 30
        {
          if(!isWallRight())
          {
            c = '0';
          }
          else
          {
            c = '1';
          }
        }
        if(((facingDirection+2) % 4) == dir) //fd/d 02 13 20 31
        {
          if(!isWallBack())
          {
            c = '0';
          }
          else
          {
            c = '1';
          }
        }
        if(((facingDirection+3) % 4) == dir) //fd/d 03 10 21 32
        {
          if(!isWallLeft())
          {
            c = '0';
          }
          else
          {
            c = '1';
          }
        }
        //end Determine char
        if (dir == 0)
        {
            insertInMap(carPosX, carPosY - 1, c);
            if (c != '1')
            {
                insertInMap(carPosX, carPosY - 2, c);
            }
            
        }
        if (dir == 1)
        {
            insertInMap(carPosX + 1, carPosY, c);
            if (c != '1')
            {
                insertInMap(carPosX + 2, carPosY, c);
            }
        }
        if (dir == 2)
        {
            insertInMap(carPosX, carPosY + 1, c);
            if (c != '1')
            {
                insertInMap(carPosX, carPosY + 2, c);
            }
        }
        if (dir == 3)
        {
            insertInMap(carPosX - 1, carPosY, c);
            if (c != '1')
            {
                insertInMap(carPosX - 2, carPosY, c);
            }
        }
    }
    void insertInMap(int x, int y, char c)
    {
        String temp = myMap[y];
        temp[x] = c;
        myMap[y] = temp;
    }
    void setWin()
    {
        
        hasReachedDestination = true;
        refreshVB();
        solve(Xs, Ys);        
    }
    void moveInstruction(char c)
    {
        rotateTo(c);
        //0 up, 1 right, 2 down, 3 left       
        //moveForward();

        if (facingDirection == 0)
        {
            carPosY -= 2;
        }
        if (facingDirection == 1)
        {
            carPosX += 2;
        }
        if (facingDirection == 2)
        {
            carPosY += 2;
        }
        if (facingDirection == 3)
        {
            carPosX -= 2;
        }
    }
    
    void rotateTo(char c)
    {   //0 up, 1 right, 2 down, 3 left
        int cRotation = c == 'u' ? 0 : c == 'r' ? 1 : c == 'd' ? 2 : 3;
        if (facingDirection == cRotation)
        {
            SetMovingMode();
            return;
        }
        if ((facingDirection + 1) % 4 == cRotation)
        {
            rotateRight();
            SetRotationMode(1, 1);
            return;
        }
        if (facingDirection == (cRotation + 1) % 4)
        {
            rotateLeft();
            SetRotationMode(-1, 1);
            return;
        }
        SetRotationMode(1, 2);
        rotateRight();
        rotateRight();
    }
    void rotateLeft()
    {
        facingDirection = (facingDirection + 3)%4;
    }

    void rotateRight()
    {
        facingDirection = (facingDirection + 1) % 4; ;
    }
     void Update()
    {
        if (!hasReachedDestination && !isMoving)
        {            
            discover();
            
            pickGoToTargetSpot(); 
            PrintMap();        
            solve(targetX, targetY);            
        }
        if (!hasReturnedToBeg && isWin() && !isMoving)
        {
            setWin();
            hasReturnedToBeg = true;
            return;
        }
        if (isMoving)
        {            
            if (instructionIndex >= instructions.length())
            {
                ResetMode();
                isMoving = false;
            }
            else
            {
                if (isResetMode())
                {
                    moveInstruction(instructions[instructionIndex]);
                }
                if (movingTimer > 0)
                {
                    if (isMovementMode())
                    {
                        MovePhysicallyForward();
            movingTimer = -1;
                    }
                    if (isRotationMode())
                    {
                        RotatePhysically();
                        if(movingTimer > rotationTimerMax)
                        {
                          RotatePhysically();
                        }
            movingTimer = -1;
                    }
                }
                else
                {
                    if (!isRotationMode())
                    {
                        ResetMode();
                        instructionIndex++;
                    }
                    else
                        SetMovingMode();
                }
            }
        }
    }
/////////////////////////AAAAAAAAAAAAAAAAAAAAAAAALLLLLLLLLLLLLLLLLLLLLLLLLGGGGGGGGGGGGGGGGGGGGGGGOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
bool isWallFront()
{
  ReadFront();
  return GetDistanceFront() < wallExistDistance;
}
bool isWallRight()
{
  ReadRight();
  return GetDistanceRight() < wallExistDistance;
}
bool isWallBack()
{
  ReadBack();
  return GetDistanceBack() < wallExistDistance;
}
bool isWallLeft()
{
  ReadLeft();
  return GetDistanceLeft() < wallExistDistance;
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
  float Distance1_cm = 2000;
  while(Distance1_cm > 1000)
  {
  //////////////Ultra Sonic Front
  digitalWrite(TriggerPin1, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin1, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin1, LOW);           // Trigger pin to HIGH
 
  Duration1 = pulseIn(EchoPin1,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
  Distance1_cm = GetDistanceFront();   // Use function to calculate the distance
  }
   Serial.print("Distance Front = ");             // Output to serial
 Serial.print(Distance1_cm);
 Serial.println(" cm");
 
  delay(sensorsDelay);                             // Wait to do next measurement
}
void ReadRight()
{
  float Distance4_cm = 2000;
  while(Distance4_cm > 1000)
  {
  //////////////Ultra Sonic Right
  digitalWrite(TriggerPin4, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin4, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin4, LOW);           // Trigger pin to HIGH
 
  Duration4 = pulseIn(EchoPin4,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
  Distance4_cm = GetDistanceRight();   // Use function to calculate the distance
  }
  
  Serial.print("Distance Right = ");             // Output to serial
  Serial.print(Distance4_cm);
  Serial.println(" cm");
 
  delay(sensorsDelay);                             // Wait to do next measurement
}
void ReadBack()
{
  float Distance2_cm = 2000;
  while(Distance2_cm > 1000)
  {
  //////////////Ultra Sonic Back
  digitalWrite(TriggerPin2, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin2, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin2, LOW);           // Trigger pin to HIGH
 
  Duration2 = pulseIn(EchoPin2,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
  Distance2_cm = GetDistanceBack();   // Use function to calculate the distance
  }
  
 Serial.print("Distance Back = ");             // Output to serial
 Serial.print(Distance2_cm);
 Serial.println(" cm");
 
  delay(sensorsDelay);                             // Wait to do next measurement
}
void ReadLeft()
{
  float Distance3_cm = 2000;
  while(Distance3_cm > 1000)
  {
  //////////////Ultra Sonic Left
  digitalWrite(TriggerPin3, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin3, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin3, LOW);           // Trigger pin to HIGH
 
  Duration3 = pulseIn(EchoPin3,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
  Distance3_cm = GetDistanceLeft();   // Use function to calculate the distance
  }
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
