#define pinecho  12
#define pintrigger 11
#include<ros.h>
#include<std_msgs/String.h>
bool flag=true;
int distance(){
  digitalWrite(pintrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pintrigger, HIGH);
  // EL PULSO DURA AL MENOS 10 uS EN ESTADO ALTO
  delayMicroseconds(10);
  digitalWrite(pintrigger, LOW);
  // MEDIR EL TIEMPO EN ESTADO ALTO DEL PIN "ECHO" EL PULSO ES PROPORCIONAL A LA DISTANCIA MEDIDA
  int tiempo = pulseIn(pinecho, HIGH);
  // LA VELOCIDAD DEL SONIDO ES DE 340 M/S O 29 MICROSEGUNDOS POR CENTIMETRO
  // DIVIDIMOS EL TIEMPO DEL PULSO ENTRE 58, TIEMPO QUE TARDA RECORRER IDA Y VUELTA UN CENTIMETRO LA ONDA SONORA
  int distancia = tiempo / 58;
  return distancia;
  }
  
void callback(const std_msgs::String& control_message){
  if(strcmp(control_message.data,"START") == 0){
    flag=true;
  }
  else if(strcmp(control_message.data,"OFF") == 0){
     flag=false;
  }
}

//////////////////////////////////////////////////////////////////////////
ros::NodeHandle  nh;
std_msgs::String sensorData;
ros::Publisher sensorDistance("sensor_distance", &sensorData);
char dataBuffer[50];
std_msgs::String control_message;
ros::Subscriber<std_msgs::String> control_sub("control", callback);
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  pinMode(pinecho, INPUT);
  pinMode(pintrigger, OUTPUT);
  nh.initNode();
  nh.subscribe(control_sub);
  nh.advertise(sensorDistance);
}

void loop() {
  // put your main code here, to run repeatedly:
  int meassure;
  meassure=distance();
  sprintf(dataBuffer,"Distance: %d", meassure);  
  
  if(flag==true){
            sensorData.data=dataBuffer;
            sensorDistance.publish(&sensorData);
    }
  else if(flag==false){
            sensorData.data=dataBuffer;
            sensorDistance.publish(&sensorData);
   }
  nh.spinOnce();
  delay(10);
}
