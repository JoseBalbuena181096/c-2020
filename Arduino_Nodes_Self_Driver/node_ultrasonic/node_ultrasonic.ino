#define EchoFront 12
#define TrigFront 11
#define EchoBack 8
#define TrigBack 7
#define EchoRight 10
#define TrigRight 9

#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/UInt8.h>
#include <Servo.h> 
Servo servoAngle;

int angle_sp = 85;
int angle_now = 90;
int angle_last = 120;

int distance_f=0;
int distance_b=0;
int distance_r=0;

ros::NodeHandle nh;
/************ CallBack Funtion to Suscriber ********************************************/
void callback_angle(const std_msgs::UInt8& angle_message){
     //start_servo = true; 
     angle_now = static_cast<int>(angle_message.data);
     if(angle_last>angle_now){
         angle_sp = (5*angle_now/6);
         if(angle_sp<0)
          angle_sp = 0;
         else if(angle_sp>=130)
          angle_sp = 130;
      }
      else{
        angle_sp = (17*angle_now/18);
         if(angle_sp<=0)
          angle_sp = 0;
         else if(angle_sp>=130)
          angle_sp = 130;
      }
      
     angle_last = angle_now;
     servoAngle.write(angle_sp);
}

std_msgs::Int16 front_msg;
ros::Publisher pub_front("front_distance", &front_msg);

std_msgs::Int16 back_msg;
ros::Publisher pub_back("back_distance", &back_msg);

std_msgs::Int16 right_msg;
ros::Publisher pub_right("right_distance", &right_msg);

ros::Subscriber<std_msgs::UInt8> angle_sub("/angle_car",callback_angle);

void setup() {
  // put your setup code here, to run once:
   nh.initNode();
   nh.advertise(pub_front);
   nh.advertise(pub_back);
   nh.advertise(pub_right);
   nh.subscribe(angle_sub);
   pinMode(TrigFront, OUTPUT);
   pinMode(EchoFront, INPUT);
   pinMode(TrigBack, OUTPUT);
   pinMode(EchoBack, INPUT);
   pinMode(TrigRight, OUTPUT);
   pinMode(EchoRight, INPUT);
   servoAngle.attach(3);
   servoAngle.write(60);//init in the center position
}
void loop() {
  // put your main code here, to run repeatedly:
   distance_f = distance(TrigFront,EchoFront);
   front_msg.data =  distance_f;
   pub_front.publish(&front_msg);
   distance_b = distance(TrigBack,EchoBack);
   back_msg.data =  distance_b;
   pub_back.publish(&back_msg);   
   distance_r = distance(TrigRight,EchoRight);
   right_msg.data =  distance_r;
   pub_right.publish(&right_msg);   
   nh.spinOnce();
}

int distance(int TriggerPin, int EchoPin){
   long duration, distanceCm;
   digitalWrite(TriggerPin, LOW);  
   delayMicroseconds(4);
   digitalWrite(TriggerPin, HIGH);  
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);
   duration = pulseIn(EchoPin, HIGH); 
   distanceCm = duration * 10 / 292/ 2;
   return distanceCm;
}
