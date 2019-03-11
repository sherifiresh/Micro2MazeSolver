bool f = false;
int i = 15;
void setup() {
  // put your setup code here, to run once:
  pinMode(8, OUTPUT);
  pinMode(13,INPUT);
}

void loop() {
  if(i > 0)
  {
  digitalWrite(8, HIGH);
  delay(100);
  digitalWrite(8, LOW);
  delay(100);
  i--;
  }

}
