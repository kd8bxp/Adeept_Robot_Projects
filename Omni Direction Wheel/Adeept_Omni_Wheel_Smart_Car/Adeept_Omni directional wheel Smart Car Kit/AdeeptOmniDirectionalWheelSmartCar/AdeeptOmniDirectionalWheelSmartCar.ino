/***********************************************************
File name:  AdeeptOmniDirectionalWheelSmartCar.ino
Description:  
Website: www.adeept.com
E-mail: support@adeept.com
Author: Tom
Date: 2018/04/15 
***********************************************************/
#include <Servo.h>           // call the Servo. H library.
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include "RF24.h"
#define VX_VALUE           (0.5f)   // macro definition
#define VY_VALUE           (sqrt(3)/2.f)      
#define L_value            (20*0.01f)  
#define PIN A4//Lantern pin
#define BRIGHTNESS 50
#define COUNT 170// rudder Angle zero.
#define ADR 90 // maximum Angle
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);//light
RF24 radio(9, 10);            // define the object to control NRF24L01
byte addresses[5] = "6";      // define communication address which should correspond to remote control
int data[9]={512, 512, 0, 0, 1, 1, 512, 512, 512};  // define array used to save the communication data
const int TrigPin = 0; // ultrasonic sending feet.
const int EchoPin = 1; // ultrasonic receiving feet.
int latchPin = 4;// 74HC595 ST_CP (pin 12,latch pin)
int clockPin = 7;//  74HC595  SH_CP (pin 11, clock pin)
int dataPin =A5 ;//  74HC595  DS (pin 14)
int PWMA = 3; //Speed control
int PWMB = 5; //Speed control
int PWMC = 6; //Speed control
int Pin0 = A0;//definition digital A0 pins as pin to control the IN1 (ULN24L01)
int Pin1 = A1;//definition digital A1 pins as pin to control the IN2 (ULN24L01)
int Pin2 = A2;//definition digital A2 pins as pin to control the IN3 (ULN24L01)
int Pin3 = A3;//definition digital A3 pins as pin to control the IN4 (ULN24L01)
int _step = 512; //Stepper motor goes 512 steps
int _speed = 1; //Number of revolutions
int beep=2;//buzzer
float cm;
int val;//Steering gear
int val1=500;;//Reading the variable of left stick x value
int val2=500;;//Reading the variable of left stick y value
int val3=500;;//Read variable of right stick x value
int val4=500;;//Read variable of right stick y value
int val5=1;;//Key A value variable
int val6=1;;//Key B value variable
int val7=0;;//Key C value variable
int val8=1;;//Key D value variable
int data1;//The variable that holds the mapped value
int data2;//The variable that holds the mapped value
int data3;//The variable that holds the mapped value
int data4;//The variable that holds the mapped value
int data5;//The variable that holds the mapped value
int data6;//The variable that holds the mapped value
int ding=0;//Control the variable that the lantern performs once
int dong=0;//Control the variable that the lantern performs once
int dong1=0;//Control the variable that the lantern performs once
int dong2=0;//Control the variable that the lantern performs once
int dong3=0;//Control the variable that the lantern performs once
int deng=0;//Control the variable that the lantern performs once
int  motor_one ;//Motor 1 speed
int  motor_two;//Motor 2 speed
int  motor_there;  //Motor 3 speed
Servo myservo;               // Create a steering gear object
int dat3[11]={0,3,5,6,12,9,10,15}; //×
int dat1[10]={1,2,4,7,8,11,13,14}; //o
int dat2[20]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//Clear screen
int dat5[20]={1,9,14,2,4,5,6,7,10,13};//under
int dat4[20]={1,2,5,6,8,9,10,11,13,14};//on
int dat7[20]={2,4,5,6,7,8,9,10,11,13};//left
int dat6[20]={1,4,5,6,7,8,9,10,11,14};//right
int dat8[20]={1,9,10,2,4,5,6,7,};//Panning
void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();//carried out
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT); //Let all three pins be output state
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(PWMC, OUTPUT);//Output motor PWM
  pinMode(Pin0, OUTPUT);//Set digital A0 port mode, the OUTPUT for the output
  pinMode(Pin1, OUTPUT);//Set digital A1 port mode, the OUTPUT for the output
  pinMode(Pin2, OUTPUT);//Set digital A2 port mode, the OUTPUT for the output
  pinMode(Pin3, OUTPUT);//Set digital A3 port mode, the OUTPUT for the output
  radio.begin();                       // initialize RF24
  radio.setRetries(0, 15);            // set retries times
  radio.setPALevel(RF24_PA_LOW);      // set power
  radio.openReadingPipe(1, addresses);// open delivery channel
  radio.startListening();             // start monitoring
  pinMode(TrigPin, OUTPUT); 
  pinMode(EchoPin, INPUT);
  pinMode(beep,OUTPUT);
  myservo.attach(8);          // Connect the servo on pin 8 to the declared steering gear object
}
void loop() 
{
  receiveData();//Receive data
  if(motor_one>0&&motor_two>0&&motor_there>0){//Determine the direction of motor rotation
    move(3, motor_one,motor_two,motor_there);//Motor 1 Forward Motor 2 Forward Motor 3 Forward
  }else if(motor_one<0&&motor_two>0&&motor_there>0){//Determine the direction of motor rotation
    move(8, motor_one,motor_two,motor_there);//Motor 1 reverse motor 2 forward motor 3 forward
  }else if(motor_one>0&&motor_two<0&&motor_there>0){//Determine the direction of motor rotation
    move(7, motor_one,motor_two,motor_there);//Motor 1 Forward Motor 2 Reverse Motor 3 Forward
  }else if(motor_one>0&&motor_two<0&&motor_there<0){//Determine the direction of motor rotation
    move(5, motor_one,motor_two,motor_there);//Motor 1 Forward Motor 2 Reverse Motor 3 Reverse
  } else if(motor_one<0&&motor_two<0&&motor_there<0){//Determine the direction of motor rotation
    move(4, motor_one,motor_two,motor_there);//Motor 1 Reverse Motor 2 Reverse Motor 3 Revers
  } else if(motor_one<0&&motor_two>0&&motor_there<0){//Determine the direction of motor rotation
    move(6, motor_one,motor_two,motor_there);//Motor 1 Reverse Motor 2 Forward Motor 3 Reverse
  } else if(motor_one<0&&motor_two<0&&motor_there>0){//Determine the direction of motor rotation
    move(10, motor_one,motor_two,motor_there);//Motor 1 Reverse Motor 2 Reverse Motor 3 Forward
  }else if(motor_one>0&&motor_two>0&&motor_there<0){//Determine the direction of motor rotation
    move(9, motor_one,motor_two,motor_there);//Motor 1 Forward Motor 2 Forward Motor 3 Reverse
  }else if(motor_one>0&&motor_two<0&&motor_there==0){//Determine the direction of motor rotation
    move(1, motor_one,motor_two,motor_there);//Motor 1 Forward Motor 2 Reverse Motor 3 Without Rotation
  }else if(motor_one<0&&motor_two>0&&motor_there==0){//Determine the direction of motor rotation
    move(2, motor_one,motor_two,motor_there);//Motor 1 reverse motor 2 forward motor 3 does not turn
  }
  switch(val7){//Button C autopilot
  case 0:if(val3>600&&val4>600){//Pan left 
            data1 = map(val3, 506, 1023, 0,-140 );//Map from 506-1023 to 0- -140
            data2 = map(val4, 505, 1023, 0, 140);//Map from 506-1023 to 0- -140
            Speed_Moto_Control(data1,data2,0);//Motor rotation 
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                  strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                  strip.show();//carried out
              }
              for(int i=0;i<8;i++){
                strip.setPixelColor(dat8[i],strip.Color(150, 150, 0));//Display pattern color
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val3>=0&&val3<406&&val4<405&&val4>=0){//Pan down right
            data1 = map(val3, 506, 0, 0, 140);//Map from 506-0 to 0-140
            data2 = map(val4, 505, 0, 0, -140);//Map from 505-0 to 0-140
            Speed_Moto_Control(data1,data2,0);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<8;i++){
                strip.setPixelColor(dat8[i],strip.Color(150, 150, 0));//Display pattern color
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val4>=0&&val4<406&&val3>600){//Pan left
            data1 = map(val3, 506, 1023, 0,140 );//Map from 506-1023 to 0-140
            data2 = map(val4, 505, 0, 0, 140);//Map from 506-0 to 0-140
            Speed_Moto_Control(data1,data2,0);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<8;i++){
                strip.setPixelColor(dat8[i],strip.Color(150, 150, 0));//Display pattern color
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val4>600&&val3<405&&val3>=0){//Pan down
            data1 = map(val3, 506,0, 0,-140 );//Map from 506-0 to 0- -140
            data2 = map(val4, 505, 1023, 0, -140);//Map from 506-0 to 0- -140
            Speed_Moto_Control(data1,data2,0);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<8;i++){
                strip.setPixelColor(dat8[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val4>400&&val4<=616&&val3>800){//Right stick
            data1 = map(val3, 506,1023,0,0 );//Map from 506-1023 to 0-0
            data2 = map(val4, 505, 605, 255, 0);//Map from 506-605 to 255-0
            Speed_Moto_Control(data1,data2,0);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<10;i++){
                strip.setPixelColor(dat5[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val4>400&&val4<=616&&val3<100){//Right rocker
            data1 = map(val3, 100,0, 0,0 );//Map from 100-0 to 0-0
            data2 = map(val4, 505, 405,-255,0);//Map from 505-405 to -255-0
            Speed_Moto_Control(data1,data2,0);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<10;i++){
                strip.setPixelColor(dat4[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val4>700&&val3>400&&val3<603){//Right stick left
            data1 = map(val3, 405,506, 0,-140 );//Map from 405-506 to 0- -140
            data2 = map(val4, 700, 1023, 0, 0);//Map from 700-1023 to 0-0
            Speed_Moto_Control(data1,data2,0);//Motor rotation
            if(dong==0){
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<8;i++){
                strip.setPixelColor(dat8[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val4<400&&val3>400&&val3<603){//Right stick right
            data1 = map(val3, 406,506, 0,140 );//Map from 406-506 to 0-140
            data2 = map(val4, 100, 0, 0, 0);//Map from 100-0 to 0-0
            Speed_Moto_Control(data1,data2,0);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<8;i++){
                strip.setPixelColor(dat8[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          } else if(val1>700&&val2>400&&val2<603){//Left (swing) left stick
            data3 = 0;//map(val1,  700, 1023, 0,0 );//Map from 700-1023 to 0-0
            data4 = 0;//map(val2, 400, 502, 0, 0);//Map from 400-502 to 0-0
            data5 = map(val1, 700, 1023, 0, 1275);//Map from 700-1023 to 0-1275
            Speed_Moto_Control(data3,data4,data5);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<10;i++){
                strip.setPixelColor(dat7[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val1<400&&val2>400&&val2<603){//Left stick right
            data3 = map(val1,  100, 0, 0,0 );//Map from 100-0 to 0-0
            data4 = map(val2, 400, 502, 0, 0);//Map from 400-502 to 0-0
            data6 = map(val1,  400, 0, 0,-1275 );//Map from 400-0 to 0- -1275
            Speed_Moto_Control(data3,data4,data6);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<10;i++){
                strip.setPixelColor(dat6[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val1>400&&val1<=616&&val2>800){//Left stick
            data3 = map(val1, 400,616, 0,0 );//Map from 400-616 to 0-0
            data4 = map(val2, 800, 1023,0,255);//Map from 100-0 to 0- -255
            Speed_Moto_Control(data3,data4,0);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<10;i++){
                strip.setPixelColor(dat5[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val1>400&&val1<=616&&val2<100){//Left rocker
            data3 = map(val1, 400,616,0,0 );//Map from 400-616 to 0-0
            data4 = map(val2, 100, 0, 0, -255);//Map from 800-1023 to 0-255
            Speed_Moto_Control(data3,data4,0);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<10;i++){
                strip.setPixelColor(dat4[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val1>600&&val2>600){//Left rocker left turn up circle
            data3 = map(val1, 600,1023, 0,-100 );//Map from 600-1023 to 0- -100
            data4 = map(val2, 600, 1023,0,150);//Map from 600-1023 to 0-150
            Speed_Moto_Control(data3,data4,200);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<10;i++){
                strip.setPixelColor(dat5[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val1>=0&&val1<406&&val2>600){//Left rocker right turn up circle
            data3 = map(val1, 400,0, 0,100 );//Map from 400-0 to 0-100
            data4 = map(val2, 600, 1023,0,100);//Map from 600-1023 to 0-100
            Speed_Moto_Control(data3,data4,-100);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<10;i++){
                strip.setPixelColor(dat5[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val1>600&&val2<405&&val2>=0){//Left rocker left down turn circle
            data3 = map(val1, 600,1023, 0,30 );//Map from 600-1023 to 0-30
            data4 = map(val2, 400, 0,0,-150);//Map from 400-0 to 0- -150
            Speed_Moto_Control(data3,data4,-300);//Motor rotation
            if(dong==0){//Judging the status value
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<10;i++){
                strip.setPixelColor(dat4[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else if(val2>=0&&val2<406&&val1<405&&val1>=0){//Left rocker right down turn circle
            data3 = map(val1, 400,0, 0,-30 );//Map from 400-0 to 0- -30
            data4 = map(val2, 400, 0,0,-150);//Map from 400-0 to 0- -150
            Speed_Moto_Control(data3,data4,300);//Motor rotation
            if(dong==0){
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<10;i++){
                strip.setPixelColor(dat4[i],strip.Color(150, 150, 0));//Display patterncolor
                strip.show();//carried out
              }
            }
            dong=1;//State value
            ding=0;//State value
          }else{
            dong=0;//State value
            dong1=0;//State value
            stop();//Stop turning
          }
          if(val1>462&&val1<562&&val2>462&&val2<562&&val3>462&&val3<562&&val4>462&&val4<562){
            if(ding==0){//Judging the status value   
              for(int i=0;i<16;i++){
                strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
                strip.show();//carried out
              }
              for(int i=0;i<8;i++){
                strip.setPixelColor(dat3[i],strip.Color(150, 0, 0));//Display pattern colour
                strip.show();//carried out
              }
            }
            ding=1;//State value
          }else{
            ding=0;//State value
          } 
          break;
  case 1: if(dong1==0){//Judging the status value
            for(int i=0;i<16;i++){
              strip.setPixelColor(dat2[i],strip.Color(0, 0, 0));//Clear screen
              strip.show();//carried out
            }
            for(int i=0;i<8;i++){
              strip.setPixelColor(dat1[i],strip.Color(150, 150, 0));//Display pattern colour
              strip.show();//carried out
            }
          }
          dong1=1;//State value
          ding=0;//State value
          int cd =Ultrasonic();
          if(cd<30){
            Speed_Moto_Control(0,0,255);//Motor rotation
            move(2, motor_one,motor_two,motor_there);//Motor 1 Forward Motor 2 Reverse Motor 3 Without Rotation
            delay(100);
          }else{
            Speed_Moto_Control(0,-150,0);//Motor rotation
            move(1, motor_one,motor_two,motor_there);//Motor 1 Forward Motor 2 Reverse Motor 3 Without Rotation
          } 
          break;
  }
  if(val5==0){//Button A Projector Angle Adjustment
    Speed(5);//Stepper motor speed = 15 fast (note:speed from 1 to 15)
    Step(1);//Stepper motor forward 512 steps ---- 360 angle 
    val5=1; 
  }
  if(val6==0){//Button B buzzer
    tone(beep,400);//Buzzer ring
  }else{
    noTone(beep);//Closed buzzer
  }
  if(val8==0){//Press button D to cast
    delay(500);
    receiveData();//Receive data
    Speed(10);//Stepper motor speed = 10 fast (note:speed from 1 to 15)
    Step(70);//Stepper motor forward 70 steps ---- 360 angle    
    myservo.write(val=COUNT);               // Write the angle to the servo  
    delay(300);                        // Delay 300ms to turn servo to specified position
    myservo.write(val=ADR);               // Write the angle to the servo  
    delay(1000); // Delay 1000ms to the servo 
    Speed(15);//Stepper motor speed = 15 fast (note:speed from 1 to 15)
    Step(442);//Stepper motor forward 442 steps ---- 360 angle    
    val8=1;
  } 
} 
void receiveData(){//Receive function
  if ( radio.available()) { // if receive the data
    while (radio.available()) { // read all the data
      radio.read( data, sizeof(data) );  // read data
    }
    val5=data[2];
    val6=data[3];
    val8=data[5];
    val1=data[0];//Left stick x value
    val2=data[1];//Left stick y value
    val3=data[7];//Right stick x value
    val4=data[8];//Right stick y value
    val7=data[4];
  }
}
void Speed_Moto_Control(float vx,float vy,float vz) //Wheel speed algorithm function
{
  motor_one   = (-VX_VALUE*vx + VY_VALUE*vy + L_value*vz);//Calculate the speed of motor 1
  motor_two   = (-VX_VALUE*vx - VY_VALUE*vy + L_value*vz);//Calculate the speed of motor 2
  motor_there = (vx + L_value*vz);//Calculate the speed of motor 3
}
void move(int motor, int speed,int speed1,int speed2)  //Wheel control function
{
  if (motor == 1){
    byte  high=34;//Motor control high and low
    digitalWrite(latchPin, LOW);// Send High Byte first to the 74HC595 which is farther from Arduino
    shiftOut(dataPin, clockPin, MSBFIRST, high);// After sending the data, pull latch back to high potential
    digitalWrite(latchPin, HIGH);
  }else if(motor == 2){
    byte  high=20;//Motor control high and low
    digitalWrite(latchPin, LOW);// Send High Byte first to the 74HC595 which is farther from Arduino
    shiftOut(dataPin, clockPin, MSBFIRST, high);// After sending the data, pull latch back to high potential
    digitalWrite(latchPin, HIGH);
  }else if(motor == 3){
    byte  high=164;//Motor control high and low
    digitalWrite(latchPin, LOW);// Send High Byte first to the 74HC595 which is farther from Arduino
    shiftOut(dataPin, clockPin, MSBFIRST, high);// After sending the data, pull latch back to high potential
    digitalWrite(latchPin, HIGH);
  }else if(motor == 4){
    byte  high=82;//Motor control high and low
    digitalWrite(latchPin, LOW);// Send High Byte first to the 74HC595 which is farther from Arduino
    shiftOut(dataPin, clockPin, MSBFIRST, high);// After sending the data, pull latch back to high potential
    digitalWrite(latchPin, HIGH);
  }else if(motor == 5){
    byte  high=98;//Motor control high and low
    digitalWrite(latchPin, LOW);// Send High Byte first to the 74HC595 which is farther from Arduino
    shiftOut(dataPin, clockPin, MSBFIRST, high);// After sending the data, pull latch back to high potential
    digitalWrite(latchPin, HIGH);
  }else if(motor == 6){
    byte  high=84;//Motor control high and low
    digitalWrite(latchPin, LOW);// Send High Byte first to the 74HC595 which is farther from Arduino
    shiftOut(dataPin, clockPin, MSBFIRST, high);// After sending the data, pull latch back to high potential
    digitalWrite(latchPin, HIGH);
  }else if(motor == 7){
    byte  high=162;//Motor control high and low
    digitalWrite(latchPin, LOW);// Send High Byte first to the 74HC595 which is farther from Arduino
    shiftOut(dataPin, clockPin, MSBFIRST, high);// After sending the data, pull latch back to high potential
    digitalWrite(latchPin, HIGH);
  }else if(motor == 8){
    byte  high=148;//Motor control high and low
    digitalWrite(latchPin, LOW);// Send High Byte first to the 74HC595 which is farther from Arduino
    shiftOut(dataPin, clockPin, MSBFIRST, high);// After sending the data, pull latch back to high potential
    digitalWrite(latchPin, HIGH);
  }else if(motor == 9){
    byte  high=100;//Motor control high and low
    digitalWrite(latchPin, LOW);// Send High Byte first to the 74HC595 which is farther from Arduino
    shiftOut(dataPin, clockPin, MSBFIRST, high);// After sending the data, pull latch back to high potential
    digitalWrite(latchPin, HIGH);
  }else if(motor == 10){
    byte  high=146;//Motor control high and low
    digitalWrite(latchPin, LOW);// Send High Byte first to the 74HC595 which is farther from Arduino
    shiftOut(dataPin, clockPin, MSBFIRST, high);// After sending the data, pull latch back to high potential
    digitalWrite(latchPin, HIGH);
  }                                       
  analogWrite(PWMA, abs(speed));//Read wheel 1 speed value
  analogWrite(PWMB, abs(speed1));//Read wheel 2 speed value
  analogWrite(PWMC, abs(speed2));//Read wheel 3 speed value
}
void stop()  //Stop function
{
  Speed_Moto_Control(0,0,0);
  byte  high=1;//Motor control high and low
  digitalWrite(latchPin, LOW);// Send High Byte first to the 74HC595 which is farther from Arduino
  shiftOut(dataPin, clockPin, MSBFIRST, high);  // After sending the data, pull latch back to high potential
  digitalWrite(latchPin, HIGH);
}
void colorWipe(uint32_t c, uint8_t wait) 
{
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void rainbow(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {strip.setPixelColor(i, Wheel((i + j) & 255 ));}
    strip.show();delay(wait);
  }
} 
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));}
    strip.show();delay(wait);
  }
}
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
void Speed(int stepperspeed){//set Stepper speed 
    _speed = 15 - stepperspeed;
    if(_speed<1){_speed = 1;}
    if(_speed>15){_speed = 15;}
}
void Step(int _step){//Stepper motor rotation
  if(_step>=0){  // Stepper motor forward
    for(int i=0;i<_step;i++){   
      setStep(1, 0, 0, 1);delay(_speed);setStep(1, 0, 0, 0);delay(_speed);setStep(1, 1, 0, 0);delay(_speed);setStep(0, 1, 0, 0);delay(_speed);
      setStep(0, 1, 1, 0);delay(_speed);setStep(0, 0, 1, 0);delay(_speed);setStep(0, 0, 1, 1);delay(_speed);setStep(0, 0, 0, 1);delay(_speed);
    }
  }else{ // Stepper motor backward
     for(int i=_step;i<0;i++){  
      setStep(0, 0, 0, 1);delay(_speed);setStep(0, 0, 1, 1);delay(_speed);setStep(0, 0, 1, 0);delay(_speed);setStep(0, 1, 1, 0);delay(_speed);
      setStep(0, 1, 0, 0);delay(_speed);setStep(1, 1, 0, 0);delay(_speed);setStep(1, 0, 0, 0);delay(_speed);setStep(1, 0, 0, 1);delay(_speed);
    }
   }
}
void setStep(int a, int b, int c, int d){  
  digitalWrite(Pin0, a);digitalWrite(Pin1, b);     
  digitalWrite(Pin2, c);digitalWrite(Pin3, d);     
}  
float Ultrasonic(){
  digitalWrite(TrigPin, LOW); //Low High Level Sends a Short Pulse to TrigPin
  delayMicroseconds(2); 
  digitalWrite(TrigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(TrigPin, LOW); 
  cm = pulseIn(EchoPin, HIGH) / 58.0; //Convert echo time to cm
  cm = (int(cm * 100.0)) / 100.0; //Keep two decimal places
  return cm;
}
