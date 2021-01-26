#define MAX_SPEED 16000
#define MB1 8
#define MA1 7
#define MPWM1 10
/******************* ROS Libraries ***********************/
#include<ros.h>/** Ros library **/
#include <std_msgs/Int16.h>/** Library int16 messages**/ 
/*********************************************************/
/****************** GLOBAL VARIABLE **********************/

int motor_speed=0;
bool start_motor = false;

/******************* Instances ROS *****************************************************/
/******************* ROS Suscriber *****************************************************/
ros::NodeHandle nh;
/************ CallBack Funtion to Suscriber ********************************************/
void callback_speed(const std_msgs::Int16& speed_message){
     start_motor = true;
     motor_speed = static_cast<int>(speed_message.data);
}

ros::Subscriber<std_msgs::Int16> speed_sub("/speed_car",callback_speed);

void setup() {
  // put your setup code here, to run once:
  /****************** Initialize Node **************************************************/
  nh.initNode();
  /****************** Suscriber to "/isdf/isdf_ss" topic *******************************/
  nh.subscribe(speed_sub);
  /******************Init pins to H Bridge ********************************************/
  pinMode(MA1, OUTPUT);//Motor B1
  pinMode(MB1, OUTPUT);//Motor A1
  //pinMode(MPWM1, OUTPUT);//Motor1 PWM 
  // PB1 and PB2 is now an output 
  DDRB |= (1 << DDB2);
  cli();
  // set TOP to 16000 
  ICR1 = 0x3E80;
  // set PWM for 0 duty cycle @ 16000 values
  OCR1B = 0;
  // set none-inverting modeset correct phase PWM mode using ICR1 as TOP START the timer with no prescaler 
  TCCR1A &= 0b00000000;
  TCCR1B &= 0b00000000;
  TCCR1B |= 0b00010001;
  TCCR1A |= 0b10100010;
  sei();
}


void loop() {
  // put your main code here, to run repeatedly:
    if(start_motor){
      motor(motor_speed);  
    }
 nh.spinOnce();
}

void motor(int speed_motor)
{  
   if(!speed_motor) {
       OCR1B = 0;  
   }
   else {
      speed_motor = (speed_motor >= MAX_SPEED) ? MAX_SPEED : speed_motor;
      if (speed_motor >=1){
      OCR1B = speed_motor;
      digitalWrite(MA1, LOW);
      digitalWrite(MB1, HIGH);
      }
      else {
      speed_motor *= (0-1);
      speed_motor = (speed_motor >= MAX_SPEED) ? MAX_SPEED : speed_motor;
      OCR1B = speed_motor;
      digitalWrite(MA1, HIGH);
      digitalWrite(MB1, LOW);
      }
   }
   return;
}
