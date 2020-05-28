void setup() 
{
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4,INPUT);
  Serial.begin(9600);
  int val = 0;

}

void loop() 
{
  // put your main code here, to run repeatedly:
int  val = digitalRead(4);
  Serial.println(val);
  delay(10);
  if(val==1)
  {
   digitalWrite(LED_BUILTIN, HIGH);  
  }
  else
  {
   digitalWrite(LED_BUILTIN, LOW);  
  }
  
  
}
