int led_left = 8;
int led_right = 9;
int led_close = 10;
int led_far = 11;
int left = 3;
int right = 4;
int distance = A0;

void setup() {                
  pinMode(led_left, OUTPUT);
  pinMode(led_right, OUTPUT);
  pinMode(led_close, OUTPUT);
  pinMode(led_far, OUTPUT);
  pinMode(left, INPUT);
  pinMode(right, INPUT);
}

void loop() {
  if (digitalRead(left)==0)
    digitalWrite(led_left,LOW);
  else
    digitalWrite(led_left,HIGH);
  
  if (digitalRead(right)==0)
    digitalWrite(led_right,LOW);
  else
    digitalWrite(led_right,HIGH);
  
  if (analogRead(distance)>0.5)
    digitalWrite(led_close,LOW);
  else
    digitalWrite(led_close,HIGH);
  
  if (analogRead(distance)>1.5)
    digitalWrite(led_far,LOW);
  else
    digitalWrite(led_far,HIGH);
 
  delay(100);               // wait for a second
}
