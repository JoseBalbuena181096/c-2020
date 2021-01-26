#include<ros.h>
#include<std_msgs/String.h>
ros::NodeHandle nh;
const int led=13;
void callback(const std_msgs::String& led){
  if(strcmp(led.data,"ON") == 0){
     digitalWrite(13,HIGH);
     delay(10000);
  }
  else if(strcmp(led.data,"OFF") == 0){
     digitalWrite(13,LOW);
     delay(10000);     
  }
}

ros::Subscriber<std_msgs::String> led_sub("led", callback );

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.subscribe(led_sub);
}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
}
