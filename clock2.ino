#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>

#define clockPin   3 //595's clock
#define latchPin   4 //595's latch
#define enablePin  5 //595's output enable (control the brightness of LEDs)
#define dataPin    6 //595's data input
#define buzzerPin  7 //buzzer

tmElements_t tm;

void setup() {               

  Serial.begin(9600);

  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(enablePin, OUTPUT);  
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin,HIGH);
  analogWrite(enablePin,245);

  onAll();
  delay(300);
  offAll();
  delay(300);
  onAll();
  delay(300);
  offAll();
  delay(300);

}

void loop() {
  
  byte brightness;
  
  brightness=map(analogRead(A3),320,1024,230,254); //check the environment brightness
  analogWrite(5,brightness); 
  
  RTC.read(tm);
/*  
  Serial.print("  ");
  Serial.print(tm.Hour);
  Serial.print("  ");
  Serial.print(tm.Minute);
  Serial.print("  ");
  Serial.println(tm.Second);
*/

  onHour();
  delay(794);

  onFlag();
  delay(5);

  onMinute();
  delay(200);
  
  
  //check when to turn on the buzzer
  if(tm.Hour==7&&tm.Minute<20){
    digitalWrite(7,LOW);
    delay(100);
    digitalWrite(7,HIGH);
  }

}

//trun on all LED
void onAll(){

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b11111111);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b11111111);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b11111111);
  digitalWrite(latchPin, HIGH);

}

//trun off all LED
void offAll(){

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  digitalWrite(latchPin, HIGH);

}

//turn on the flags
void onFlag(){

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00010001);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00010001);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00010001);
  digitalWrite(latchPin, HIGH);

}


void onHour(){

  byte H=0;
  bitSet(H,tm.Hour%8);

  digitalWrite(latchPin, LOW);
  
  if(tm.Hour/8==0){
    
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    shiftOut(dataPin, clockPin, MSBFIRST, H);
    
  }  
  else if(tm.Hour/8==1){

    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    shiftOut(dataPin, clockPin, MSBFIRST, H);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    
  }   
  else if(tm.Hour/8==2){
    
    shiftOut(dataPin, clockPin, MSBFIRST, H);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    
  }
  
  digitalWrite(latchPin, HIGH);

}

void onMinute(){
  byte M=0;
  byte a=tm.Minute;

  a*=2;
  a/=5;

  digitalWrite(latchPin, LOW);

  if(a<4){

    bitSet(M,a+4);

    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    shiftOut(dataPin, clockPin, MSBFIRST, M);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    
  }

  else if(a>=4&&a<12){

    bitSet(M,a-4);

    shiftOut(dataPin, clockPin, MSBFIRST, M);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    
  } 
  
  else if(a>=12&&a<20){

    bitSet(M,a-12);

    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);      
    shiftOut(dataPin, clockPin, MSBFIRST, M);
    
  }

  else if(a>=20&&a<24){

    bitSet(M,a-20);

    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    shiftOut(dataPin, clockPin, MSBFIRST, M);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);      

  }

  digitalWrite(latchPin, HIGH);
}






