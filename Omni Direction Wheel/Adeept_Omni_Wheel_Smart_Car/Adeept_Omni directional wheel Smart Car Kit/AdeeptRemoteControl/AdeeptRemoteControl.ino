/***********************************************************
File name:  AdeeptRemoteControl.ino
Description:  
Website: www.adeept.com
E-mail: support@adeept.com
Author: Tom
Date: 2018/04/15 
***********************************************************/
#include <SPI.h>
#include "src/RF24/RF24.h"
RF24 radio(9, 10);            // define the object to control NRF24L01
byte addresses[] = "6";       // define communication address which should correspond to remote control
int data[9];                  // define array used to save the communication data
int mode = 0;
int stat=0;
int adc;
const int pot6Pin = 5;        // define R6
const int pot5Pin = 4;        // define R1
const int led1Pin = 6;        // define pin for LED1 which is close to NRF24L01 and used to indicate the state of NRF24L01
const int led2Pin = 7;        // define pin for LED2 which is the mode is displayed in the robotic arm remote control mode  
const int led3Pin = 8;        // define pin for LED3 which is the mode is displayed in the robotic arm auto mode
const int APin = 2;           // define pin for D2
const int BPin = 3;           // define pin for D3
const int CPin = 4;           // define pin for D4
const int DPin = 5;           // define pin for D5

const int u1XPin = 0;      // define pin for direction X of joystick U1
const int u1YPin = 1;      // define pin for direction Y of joystick U1
const int u2XPin = 2;      // define pin for direction X of joystick U2
const int u2YPin = 3;      // define pin for direction Y of joystick U2

void setup() {
  
  radio.begin();                      // initialize RF24
  radio.setRetries(0, 15);            // set retries times
  radio.setPALevel(RF24_PA_LOW);      // set power
  radio.openWritingPipe(addresses);   // open delivery channel
  radio.stopListening();              // stop monitoring
  pinMode(led1Pin, OUTPUT);           // set led1Pin to output mode
  pinMode(led2Pin, OUTPUT);           // set led2Pin to output mode
  pinMode(led3Pin, OUTPUT);           // set led3Pin to output mode
  pinMode(APin, INPUT_PULLUP);        // set APin to output mode
  pinMode(BPin, INPUT_PULLUP);        // set BPin to output mode
  pinMode(CPin, INPUT_PULLUP);        // set CPin to output mode
  pinMode(DPin, INPUT_PULLUP);        // set DPin to output mode  
Serial.begin(9600);
}

void loop() {
  // put the values of rocker, switch and potentiometer into the array
  data[0] = analogRead(u1XPin);
  data[1] = analogRead(u1YPin);
  if(digitalRead(APin)==LOW){  
    delay(10);
    data[2] = digitalRead(APin);
      Serial.println("val");   
    }else{
      data[2] = HIGH;
      }
  data[3]=digitalRead(BPin);
 // data[4]=digitalRead(CPin);
  data[5] = digitalRead(DPin);
  data[6] = analogRead(pot5Pin);
  data[7] = analogRead(u2YPin);
  data[8] = analogRead(u2XPin);
  // send array data. If the sending succeeds, open signal LED
  if (radio.write( data, sizeof(data) ))
  digitalWrite(led1Pin,HIGH);

  // delay for a period of time, then turn off the signal LED for next sending
  delay(2);                       
  digitalWrite(led1Pin,LOW);
  
  if(digitalRead(CPin)==LOW)
  {
   delay(20);
  while(!digitalRead(CPin));
  delay(20);
   stat++;
    if(stat>1)
    {
      stat=0;
      }
  } 
  if(stat==0){//LED display status
    digitalWrite(led2Pin,HIGH);
    digitalWrite(led3Pin,LOW);
    data[4]=0;
  }
  if(stat==1){//LED display status
      digitalWrite(led2Pin,LOW);
      digitalWrite(led3Pin,HIGH);
      data[4]=1;
  }   
}

