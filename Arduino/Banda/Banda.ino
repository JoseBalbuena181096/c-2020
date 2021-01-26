//inputs
#define START_B 12
#define STOP_B 13
#define A_P 3
#define A_L 2
#define B_P 5
#define B_L 4
#define C_P 7
#define C_L 6
//outputs
#define ONE_Y1 8
#define ONE_Y2 9
#define TWO_Y1 10
#define THREE_Y1 11

void setup() {
  // put your setup code here, to run once:
  pinMode(START_B,INPUT); 
  pinMode(STOP_B,INPUT); 
  pinMode(A_P,INPUT);
  pinMode(A_L,INPUT);
  pinMode(B_P,INPUT);
  pinMode(B_L,INPUT);
  pinMode(C_P,INPUT);
  pinMode(C_L,INPUT);
  pinMode(ONE_Y1,OUTPUT); 
  pinMode(ONE_Y2,OUTPUT);        
  pinMode(TWO_Y1,OUTPUT);
  pinMode(THREE_Y1,OUTPUT);
  digitalWrite(ONE_Y1,LOW); 
  digitalWrite(ONE_Y2,LOW);
  digitalWrite(TWO_Y1,LOW);  
  digitalWrite(THREE_Y1,LOW);
  Serial.begin(9600);
}

enum{STOP,START,P1,P2,P3,P4}state=STOP; 
int cout2=0,cout3=0;
void loop() {
  // put your main code here, to run repeatedly:
    while(digitalRead(START_B) && state==STOP){
      digitalWrite(ONE_Y1,LOW); 
      digitalWrite(ONE_Y2,LOW); 
      digitalWrite(TWO_Y1,LOW); 
      digitalWrite(THREE_Y1,LOW); 
      if(!digitalRead(START_B) && !digitalRead(A_L) && !digitalRead(B_L) && !digitalRead(C_L)){
          state = START;
          Serial.println("start");
        }
    }
    if(state==START){
        state = P1;
      }
   
    else if(state==P1){
         digitalWrite(ONE_Y1,HIGH); 
         delay(50);
         digitalWrite(ONE_Y1,LOW); 
         while(digitalRead(A_P));
         state=P2;
         cout2=0;
         Serial.println("P1");
      } 
   else if(state==P2){
        while(cout2<3){
            while(digitalRead(B_L));
            digitalWrite(TWO_Y1,HIGH);
            while(digitalRead(B_P));
            delay(3000);
            digitalWrite(TWO_Y1,LOW);
            cout2++;
          }  
         state=P3;
         cout3 = 0;
         Serial.println("P2");
    }
   else if(state==P3){
        while(cout3<2){
            while(digitalRead(C_L));
            digitalWrite(THREE_Y1,HIGH);
            while(digitalRead(C_P));
            delay(4000);
            digitalWrite(THREE_Y1,LOW);
            cout3++;
          }  
         state=P4;
         Serial.println("P3");
    }    
   else if(state==P4){
          while(digitalRead(C_L));
          delay(5000);
          digitalWrite(ONE_Y2,HIGH); 
          delay(50);
          digitalWrite(ONE_Y2,LOW); 
          while(digitalRead(A_L));
          state = STOP;
          Serial.println("P4");
    }
}

