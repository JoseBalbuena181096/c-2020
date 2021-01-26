/*
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */
 #include<ros.h>
 #include<std_msgs/Empty.h>
 //ROS handle
 ros::NodeHandle nh;
 //Funtion callback
 /*
  * callback function will be executed for any new command received
  * 
  *When a message arrived, if the LED is ON it will be turned OFF, 
  *and if it is OFF it will be turned ON
  */
 void messageCb(const std_msgs::Empty& toggle_msg){
  digitalWrite(13,HIGH-digitalRead(13));//blink the led 
  }  
  //We subscribe to the topic toogle_led with the message std::msgs
  ros::Subscriber<std_msgs::Empty>sub("toogle_led",&messageCb);
void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  // put your main code here, to run repeatedly:
nh.spinOnce();
delay(1);
}
