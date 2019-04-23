 int i = 3;
int wheelMaxSpeed = 255;
int leftWheelNPin = 5;
int leftWheelPPin = 6;
void LeftWheelSetNegative(int speed)
{
  if(speed == 0)
  {
    digitalWrite(leftWheelNPin, LOW);
  }
  else
  {
    analogWrite(leftWheelNPin, speed);
  }
}
void LeftWheelSetPositive(int speed)
{
  if(speed == 0)
  {
    digitalWrite(leftWheelPPin, LOW);
  }
  else
  {
    analogWrite(leftWheelPPin, speed);
  }
}

void PlayLeftWheel(int speed)
{
  if(speed > 0)
  {
    LeftWheelSetPositive(speed);
    LeftWheelSetNegative(0);
  }
  else
  {
    LeftWheelSetPositive(0);
    LeftWheelSetNegative(-speed);
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(leftWheelNPin, OUTPUT); //negative
  pinMode(leftWheelPPin, OUTPUT); //positive
  pinMode(13,OUTPUT);
  StartingDelay();
  Serial.begin(9600);
}

void loop() {
  //PlayLeftWheel(100);
  Serial.println("digitalRead(5):");
  Serial.println(digitalRead(5));
   Serial.println("digitalRead(6):");
  Serial.println(digitalRead(6));
}

void StartingDelay()
{
  while(i > 0)
  {
  digitalWrite(13, HIGH);
  delay(700);
  digitalWrite(13, LOW);
  delay(300);
  i--;
  }  
}
