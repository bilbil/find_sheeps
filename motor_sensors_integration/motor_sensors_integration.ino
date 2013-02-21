#include <motor.h>
#include <Arduino.h>

motor myMotor;

#define GRIDSENS_BACK_RIGHT  4
#define GRIDSENS_BACK_LEFT   5
#define GRIDSENS_FRONT_RIGHT 6
#define GRIDSENS_FRONT_LEFT  7


int left;
int right;
  
void setup()
{
  pinMode(GRIDSENS_BACK_RIGHT, INPUT);
  pinMode(GRIDSENS_BACK_LEFT, INPUT);
  pinMode(GRIDSENS_FRONT_RIGHT, INPUT);
  pinMode(GRIDSENS_FRONT_LEFT, INPUT);
  myMotor = motor();
  
}

void loop()
{ 
    delay(500);
  
//    myMotor.motorStart(true,75,75);
//  
//    delay(2000);
//    
//    myMotor.motorStop();
    
    while(1)
    {
       left=digitalRead(GRIDSENS_FRONT_LEFT);
       right=digitalRead(GRIDSENS_FRONT_RIGHT);
       if (left==0 && right==0)
       {
           myMotor.motorStart(true,100,100);
       }
       else if (left==1 && right==0)
       {
            myMotor.motorStart(true,0,100);
       }
       else if (left==0 && right==1)
       {
           myMotor.motorStart(true,100,0);
       }
       else
       {
           if (digitalRead(GRIDSENS_BACK_LEFT)==0 && digitalRead(GRIDSENS_BACK_RIGHT)==0)
           {
                myMotor.motorStart(true,100,100);
           }
           else if(digitalRead(GRIDSENS_BACK_LEFT)==0 && digitalRead(GRIDSENS_BACK_RIGHT)==1)
           {  
                myMotor.motorStart(true,100,0);
           }
           else if(digitalRead(GRIDSENS_BACK_LEFT)==1 && digitalRead(GRIDSENS_BACK_RIGHT)==0)
           {
                myMotor.motorStart(true,0,100);
           }
       }
       
       delay(50);
       myMotor.motorStop();
       delay(500);
       
    }
}
