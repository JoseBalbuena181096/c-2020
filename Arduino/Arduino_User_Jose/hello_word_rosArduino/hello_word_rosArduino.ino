                               /*
 * rosserial Publiher Example
 * Prints "Hello Word"
 * Jose Balbuena
*/
//Include libreries
#include<ros.h>
#include<std_msgs/String.h>

//Ros handle
ros::NodeHandle nh;
//Create message ot the type string
std_msgs::String str_msg;
//Create a publisher of topic chatter 
/*
 * chatter publisher object that publishes
 * on the topic /chatter topic
 */
ros::Publisher chatter("chatter",&str_msg);
//Create the message string
char hello[13]="Hello world!";
/*
 * This ROS node in Arduino will simpy
 * publish a String
 * message "Hello world!"
 * like in talker application
 */

void setup() {
  // put your setup code here, to run once:
  /*
   * Initialization of the node
   * must be the setup() method
   */
  nh.initNode();
  //Advertise the publisher to the master_node 
  nh.advertise(chatter);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Equal the str_msg.data to the string hello
  str_msg.data=hello;
  //Publish the message throught the topic chatter
  chatter.publish(&str_msg);
  nh.spinOnce();
  delay(1000);
}
