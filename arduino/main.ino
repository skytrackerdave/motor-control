//RS-485 Modbus Slave (Arduino UNO)

//Circuit Digest


#include<ModbusRtu.h>       //Library for using Modbus in Arduino

#include<LiquidCrystal.h>   //Library for using 16x2 LCD display

#include <Servo.h>          //Library for using Servo Motor


#define led1 3              //Define as 2 led1 

#define led2 5              //Define as 5 led2
#define enable A0
#define cw 2
#define pulsec 7
bool forward = true;
bool firstStart = true;
bool slow = false;


LiquidCrystal lcd(8,9,10,11,12,13);   //initizlize lcd object with pins (RS,E,D4,D5,D6,D7) for class liquid crystal

Servo servo;                          //Initilize servo object for class Servo

Modbus bus;                          //Define Object bus for class modbus 

uint16_t modbus_array[] = {0,0,0,0};    //Array initilized with three 0 values
long var = 0;
int valcount = 0;
int pcount =  0;
//int pulsec = 0;
int lastval = 1;
int pwm = 0;
int location = 0;
                      

void setup()

{

  digitalWrite(enable,HIGH);
  digitalWrite(cw,LOW);
 
  //val = digitalRead(pulsec);
  //digitalWrite(A1,HIGH);
  //digitalWrite(enable,HIGH);

  
  lcd.begin(16,2);                //Lcd set in 16x2 mode

  lcd.print("RS-485 Modbus");     //Welcome Message

  lcd.setCursor(0,1); 

  lcd.print("Arduino Slave");

  //delay(5000);

  lcd.clear();
 

  

  pinMode(led1,OUTPUT);           //Led1 set as OUTPUT

  pinMode(led2,OUTPUT);           //Led2 set as OUTPUT
  pinMode(cw, OUTPUT);
  //pinMode(enable, OUTPUT);
  pinMode(pulsec, INPUT);


  //servo.attach(6);                //Servo PWM pin 6

  bus = Modbus(1,1,4);            //Modbus slave ID as 1 and 1 connected via RS-485 and 4 connected to DE & RE pin of RS-485 Module 

  bus.begin(9600);                //Modbus slave baudrate at 9600
  
  
}


void loop()

//

{
   //digitalWrite(led2,HIGH);
   valcount = digitalRead(pulsec);
   if (valcount < 1 && lastval > 0){
        if (forward == true){
          pcount = pcount + 1;
          } else { pcount = pcount - 1;
        }
        
        //lcd.print(pcount); 
   
        //lcd.setCursor(0,1);        
        //lcd.print("Count:");
        //lcd.print(String(pcount) + "," + String(location));
        //lcd.print("     ");
        
   }
        //lcd.setCursor(0,1);        
        //lcd.print("Count:");
        //lcd.print(String(pcount) + "," + String(location));
        //lcd.print("     ");
   
   lastval = valcount; 
   if (pcount == location){modbus_array[1] = 0;} 
   bus.poll(modbus_array,sizeof(modbus_array)/sizeof(modbus_array[0]));       //Used to receive or write value from Master 
   if ((int)modbus_array[2] > 32767)
   { location = modbus_array[2] - 65536;}
   else {location = modbus_array[2];}


   modbus_array[3] = pcount;
   //int holdingRegisterWrite(0, pcount);
   //int coilWrite(0, pcount);


  //long pwm = modbus_array[2]; //Depends upon value in modbus_array[1] written by Master Modbus
  int ad = (float)modbus_array[1] / 8 + 1;
  if ((int) location > pcount) 
     {pwm = modbus_array[1];
     digitalWrite(cw,LOW);
     forward = true;
     modbus_array[0] = 1;
     pwm = modbus_array[1];
     analogWrite(3, pwm);
     digitalWrite(led2,HIGH);
     
     if (location - pcount < ad)
     {modbus_array[1] = (int)modbus_array[1]/2;
     }
     
     
     
     
     
  } else if ((int) location < pcount){
    modbus_array[0] = 0;
    forward = false;
    digitalWrite(cw,HIGH);
    pwm = modbus_array[1];
    analogWrite(3, pwm);
    digitalWrite(led2,LOW); 
    if (pcount == location){modbus_array[1] = 1;} 
    
    if (pcount - location < ad)
     {modbus_array[1] = (int)modbus_array[1]/2;
     }
    
    /**
    
    if (((float) pcount/ (float) modbus_array[2]) > 0.6)
     {modbus_array[1] = modbus_array[1] * 0.6;
     } 
   **/
  } else {analogWrite(3,0);}


  /**
  //lcd.print(pwm); 
  if (modbus_array[0] == 1){
     forward = true;
     digitalWrite(cw,LOW);
  }else{
     forward = false;
     digitalWrite(cw,HIGH);
  }
  **/


  
  

}
