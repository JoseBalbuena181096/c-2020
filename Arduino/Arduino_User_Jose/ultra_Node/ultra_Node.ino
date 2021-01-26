#define pinecho 12
#define pintrigger 11
#include<ros.h>
#include<std_msgs/String.h>
//////////////////////////////////
int flag=2;
int distance();
///////////////////////////////////
//Variables 
ros::NodeHandle nh;
std_msgs::String sensorData;
ros::Publisher sensorDistance("sensor_distance",&sensorData);
/////////////////////////////////////////////////

void callback(const std_msgs::String& control_data){
   if(strcmp(control_data.data,"start")){
      flag=1;
    }
    else if(strcmp(control_data.data,"off")){
      flag=0;
      }
}
ros::Subscriber<std_msgs::String> control_sub("control_sensor", callback);

//////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  pinMode(pinecho, INPUT);
  pinMode(pintrigger, OUTPUT);
  nh.initNode();
  nh.advertise(sensorDistance);
  nh.subscribe(control_sub);
}

void loop() {
  int meassure;
  char dataBuffer[20];
  nh.subscribe(control_sub);
  if (flag==1){
      meassure=distance();
      sprintf(dataBuffer,"Distance: %d", meassure);  
      sensorData.data=dataBuffer;
      sensorDistance.publish(&sensorData);
    }
  else if(flag==0){
      sprintf(dataBuffer,"Turn on the sensor"); 
      sensorData.data=dataBuffer;
      sensorDistance.publish(&sensorData);
  }
   
  nh.spinOnce();
  delay(10);
}
////////////////////////////////////////////////////

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
  
