/*
 * nova.c
 *
 *  Created on: 17 sep. 2020
 *      Author: ciber
 */
#include<IRremote.h>
#define OUT_LINE   15   //Valor fijo dado cuando se sale de linea-15,15,15,15
///---Define's---///
int MAX_SPEED = 255;  //Velocidad Maxima (maximo valor posible 255)-131,150,230,238,255, 255
int MED_SPEED = 0;   //Velocidad en rectas  (maximo valor posible 255)-70,80,150,168,185,225
float KP = 5;        //Constante P-10,12,23,23,27,33
const long ONE  = 0x00FF629D;
const long TWO = 0x00FF10EF;
const long THREE = 0x00FF5AA5;
const long OFF =0x00FF02FD;
const int RECV_PIN = 11;
//IRrecv irrecv(RECV_PIN);
decode_results results;

enum {HOME,STARTING,RUN} state = HOME;
enum {CENTER,RIGHT,LEFT} out_state = CENTER;
//Global variables//
signed int error_actual = 0;
signed int error_anterior = 0;
signed int speed_1  =0; //Motor 2 - left side
signed int speed_2  =0; //Motor 1 - right side
signed int proporcional = 0;
void setup() {
    // Configuramos pines

    //Enable follower
    //irrecv.enableIRIn();
    //Serial.begin(9600);
     pinMode(3, OUTPUT); // set pins to output
     pinMode(2, OUTPUT);
     pinMode(9, OUTPUT);
     pinMode(8, OUTPUT);
     pinMode(5, OUTPUT);
     pinMode(6, OUTPUT);
     pinMode(10,INPUT_PULLUP);
    //RIGHT
    analogWrite(5,0);
    digitalWrite(3,LOW);//PWM1_1AIA9
    digitalWrite(2,LOW); //PWM1_1AIB8
    //LEFT
    analogWrite(6,0);
    digitalWrite(9,LOW); //PWM2_BIA3
    digitalWrite(8,LOW);  //PWM2_BIB2

}



/*

///Function to read error///
signed int Read_error (void)
{
    int b[8];
    int sum=0;
    signed int error = 0;
    //Lectura de sensores de manera digital
    b[0]=(analogRead(A0)<=550)? 1 : 0;
    b[1]=(analogRead(A1)<=550)? 1 : 0;
    b[2]=(analogRead(A2)<=550)? 1 : 0;
    b[3]=(analogRead(A3)<=550)? 1 : 0;
    b[4]=(analogRead(A4)<=550)? 1 : 0;
    b[5]=(analogRead(A5)<=550)? 1 : 0;
    b[6]=(analogRead(A6)<=550)? 1 : 0;
    b[7]=(analogRead(A7)<=550)? 1 : 0;

    sum=b[0]+b[1]+b[2]+b[3]+b[4]+b[5]+b[6]+b[7];
    if(sum>=5){
      for(int i=0;i<8;i++){
          if(b[i]==0)
            b[i]=1;
          else if(b[i]==1)
            b[i]==0;
        }
      }

   if (b[0]||b[1]||b[2]||b[3]||b[4]||b[5]||b[6]||b[7]) {  //Out line - Black line (si alguno lee linea entra)

      //Negative left sensor
        error = (b[3]) ? (0-1) : error;
        error = (b[2])         ? (0-3) : error;
        error = (b[1])         ? (0-5) : error;
        error = (b[0])         ? (0-7) : error;
        error = (b[3] && b[2]) ? (0-2) : error;
        error = (b[2] && b[1]) ? (0-4) : error;
        error = (b[1] && b[0])   ? (0-6) : error;


        //Positive right sensor
        error = (b[4]) ? 1 : error;
        error = (b[5])         ? (3) : error;
        error = (b[6])         ? (5) : error;
        error = (b[7])         ? (7) : error;
        error = (b[4] && b[5]) ? (2) : error;
        error = (b[5] && b[6]) ? (4) : error;
        error = (b[6] && b[7])   ? (6) : error;

        //Neutral middle sensor
        error = (b[3]&&b[4]) ? 0 : error;

        out_state = ((error <= 1)&&(error >=(0-1)))  ? CENTER : out_state;
        out_state = ((error > 1)&&(error <=7))        ? LEFT : out_state;
        out_state = ((error <(0-1))&&(error >=(0-7))) ? RIGHT: out_state;

        return error;
    }
    else {
       return OUT_LINE;
    }
}

*/
///---Functions---///
///Function to read error///
signed int Read_error (void)
{
    int b[8];
    signed int error = 0;
    //Lectura de sensores de manera digital
    b[0]=(analogRead(A0)>=800)? 1 : 0;
    b[1]=(analogRead(A1)>=800)? 1 : 0;
    b[2]=(analogRead(A2)>=800)? 1 : 0;
    b[3]=(analogRead(A3)>=800)? 1 : 0;
    b[4]=(analogRead(A4)>=800)? 1 : 0;
    b[5]=(analogRead(A5)>=1000)? 1 : 0;
    b[6]=(analogRead(A6)>=800)? 1 : 0;
    b[7]=(analogRead(A7)>=800)? 1 : 0;

   if (b[0]||b[1]||b[2]||b[3]||b[4]||b[5]||b[6]||b[7]) {  //Out line - Black line (si alguno lee linea entra)

      //Negative left sensor
        error = (b[3]) ? (0-1) : error;
        error = (b[2])         ? (0-3) : error;
        error = (b[1])         ? (0-5) : error;
        error = (b[0])         ? (0-7) : error;
        error = (b[3] && b[2]) ? (0-2) : error;
        error = (b[2] && b[1]) ? (0-4) : error;
        error = (b[1] && b[0])   ? (0-6) : error;


        //Positive right sensor
        error = (b[4]) ? 1 : error;
        error = (b[5])         ? (3) : error;
        error = (b[6])         ? (5) : error;
        error = (b[7])         ? (7) : error;
        error = (b[4] && b[5]) ? (2) : error;
        error = (b[5] && b[6]) ? (4) : error;
        error = (b[6] && b[7])   ? (6) : error;

        //Neutral middle sensor
        error = (b[3]&&b[4]) ? 0 : error;

        out_state = ((error <= 1)&&(error >=(0-1)))  ? CENTER : out_state;
        out_state = ((error > 1)&&(error <=7))        ? LEFT : out_state;
        out_state = ((error <(0-1))&&(error >=(0-7))) ? RIGHT: out_state;

        return error;
    }
    else {
       return OUT_LINE;
    }
}
///Function to set speed of right motor///
void Motor_L (signed int speed)
{
   if(!speed) {
    digitalWrite(8, LOW);   //PWM2_BIA3
    digitalWrite(9, LOW); //PWM1_1AIB
     analogWrite(6,0);
   }
   else {
      speed = (speed >= MAX_SPEED) ? MAX_SPEED : speed;

      if (speed >=1){
         digitalWrite(9,LOW); //PWM1_1AIA
         digitalWrite(8,HIGH); //PWM1_1AIB
         analogWrite(6,speed);
      }
      else {
      speed *= (0-1);
       speed = (speed >= MAX_SPEED) ? MAX_SPEED : speed;
         digitalWrite(8,LOW); //PWM1_1AIB
         digitalWrite(9,HIGH); //PWM1_1AIA
         analogWrite(6,speed);

      }
   }
   return;
}
///Function to set speed of left motor///
void Motor_R (signed int speed)
{
   if(!speed) {
    digitalWrite(2, LOW);   //PWM2_BIA3
    digitalWrite(3, LOW);   //PWM2_BIB2
    analogWrite(5,0);
   }
   else {
      speed = (speed >= MAX_SPEED) ? MAX_SPEED : speed;

      if (speed >=1){
        digitalWrite(2, HIGH);   //PWM2_BIB
        digitalWrite(3,LOW);  //PWM2_BIA
        analogWrite(5,speed);
      }
      else {
      speed *= (0-1);
      ////////////////////////
      speed = (speed >= MAX_SPEED) ? MAX_SPEED : speed;
      ///////////////////////////////////
        digitalWrite(3,HIGH); //PWM2_BIA
        digitalWrite(2,LOW);  //PWM2_BIB
        analogWrite(5,speed);
      }
   }
   return;
}

void loop() {
  /*
 if (irrecv.decode(&results))
   {
      //Serial.println(results.value, HEX);
      
      switch (results.value){
        case ONE:
          //Serial.println("ON");
           MAX_SPEED = 255;  //Velocidad Maxima (maximo valor posible 255)-150,230,238,255
           MED_SPEED = 133;   //Velocidad en rectas  (maximo valor posible 255)-80,150,168,185
           KP =3;        //Constante P-12,23,23,27
           state = STARTING;
          break;
         case TWO:
          //Serial.println("ON");
           MAX_SPEED = 255;  //Velocidad Maxima (maximo valor posible 216
           MED_SPEED =133;   //Velocidad en rectas  (maximo valor posible 100
           KP =1;        //Constante 14
           state = STARTING;
          break;
         case THREE:
          //Serial.printl2n("ON");
           MAX_SPEED = 255;  //Velocidad Maxima (maximo valor posible 255)-150,230,238,255
           MED_SPEED = 135;
           KP =1;        //Constante P-12,23,23,27
           state = STARTING;
          break;
        case OFF:
          //Serial.println("OFF");
           state = HOME;
          break;
      }
      
      irrecv.resume();
   }
*/
if(!digitalRead(10)){
   MAX_SPEED = 150;  //Velocidad Maxima (maximo valor posible 255)-150,230,238,255
   MED_SPEED = 75;//80   //Velocidad en rectas  (maximo valor posible 255)-80,150,168,185
   KP = 12.5;        //Constante P-12,23,23,27
   state = STARTING;
   delay(250); 
}
switch (state)
      {
         case HOME:
            Motor_R(0);
            Motor_L(0);
            break;
         case STARTING:
            state = RUN;
            break;

         case RUN:
            //Read real error -10 to 10
            error_actual = Read_error();

            //Out line
            if(error_actual == OUT_LINE) {
               //Out line state machine//
               switch (out_state)
                    {
                        case CENTER:
                            speed_1 = MED_SPEED;
                            speed_2 = MED_SPEED;
                            break;
                        case LEFT:
                            speed_1 = MAX_SPEED;
                            speed_2 = (0-MAX_SPEED);
                            break;
                        case RIGHT: 
                            speed_1 = (0-MAX_SPEED);
                            speed_2 = MAX_SPEED;
                            break;
                    }
            }
            //On line
            else {

                    proporcional = (int)((KP * (float)(error_actual)));


                    speed_1  = MED_SPEED + proporcional;
                    speed_2  = MED_SPEED - proporcional;
            }

            Motor_R(speed_2);
            Motor_L(speed_1);

            break;
      }

}
