#define pinecho  2
#define pintrigger 3
#include <ros.h>
#include <std_msgs/String.h>
ros::NodeHandle  nh;
std_msgs::String sensorData;
ros::Publisher sensorDistance("sensor_distance", &sensorData);
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
  
char dataBuffer[50];
void setup() {
  // put your setup code here, to run once:
  // CONFIGURAR PINES DE ENTRADA Y SALIDA
  pinMode(pinecho, INPUT);
  pinMode(pintrigger, OUTPUT);
  nh.initNode();
  nh.advertise(sensorDistance);
}

void loop() {
  // put your main code here, to run repeatedly:
  int meassure;
  meassure=distance();
  sprintf(dataBuffer,"%d,", meassure);  
  sensorData.data=dataBuffer;
  sensorDistance.publish(&sensorData);
  nh.spinOnce();
  delay(500);
}
