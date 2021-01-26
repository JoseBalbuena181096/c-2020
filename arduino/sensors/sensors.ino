#define EchoFront 12
#define TrigFront 11
#define EchoBack 8
#define TrigBack 7
#define EchoRight 10
#define TrigRight 9

int distance_f = 0;

int distance_r = 0;

int distance_b = 0;

void setup() {
  // put your setup code here, to run once:

   pinMode(TrigFront, OUTPUT);
   pinMode(EchoFront, INPUT);
   pinMode(TrigBack, OUTPUT);
   pinMode(EchoBack, INPUT);
   pinMode(TrigRight, OUTPUT);
   pinMode(EchoRight, INPUT);
   Serial.begin(9600);

}
void loop() {
  // put your main code here, to run repeatedly: 
   distance_f = distance(TrigFront,EchoFront);
   Serial.print("front: ");
   Serial.println(distance_f);
   delay(500); 
   distance_b = distance(TrigBack,EchoBack);
   Serial.print("back: ");
   Serial.println(distance_b); 
   delay(500); 
   distance_r = distance(TrigRight,EchoRight);
   Serial.print("right: ");
   Serial.println(distance_r);
   delay(500);  
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
