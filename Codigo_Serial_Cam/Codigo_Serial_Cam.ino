#include<ros.h>/** Ros library **/
#include <std_msgs/Int16.h>/** Library int16 messages**/
#include <Servo.h> 



Servo basex;

float posicion = 0;

float error= 0;

float control= 0;
float kp= 0.065;


int angulo = 0; 

/******************* Instances ROS ******************/
/******************* ROS Suscriber ******************/ 
ros::NodeHandle nh;
/************ CallBack Funtion to Suscriber *********/

void callbackpos(const std_msgs::Int16& pos){
     posicion = static_cast<float>(pos.data);
     error = (640.0/2.0)-posicion;
     control = kp*error;
     angulo = 90 + static_cast<int>(control);
     if(angulo<0)
       angulo = 0;
     else if(angulo>180)
       angulo = 180;
     basex.write(angulo);
     
}
ros::Subscriber<std_msgs::Int16> posicion_sub("/posicion",callbackpos);


void setup() {
  // put your setup code here, to run once:
  /****************** Initialize Node **************************************************/
  nh.initNode();
  
  nh.subscribe(posicion_sub);
//
  /*************************************************************************************/
 
  basex.attach(9); //******numero de pin****

  
  basex.write(90);

}

void loop() {
  
  nh.spinOnce();
}
