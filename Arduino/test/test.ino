
/****************************************************************************************************/
int sensorPinFront = 5;  // selecciona el pin de entrada para el sensor de audio
int sensorPinBack = 4;  // selecciona el pin de entrada para el sensor de audio
int sensorPinLeft = 3;  // selecciona el pin de entrada para el sensor de audio
int sensorPinRight = 2;  // selecciona el pin de entrada para el sensor de audio

int estate_front_last=HIGH;
int estate_back_last=HIGH;
int estate_left_last=HIGH;
int estate_right_last=HIGH;

unsigned long  time_front=0;
unsigned long time_left=0;
unsigned long  time_right=0;
unsigned long time_back=0;

int sensorStateFront=HIGH;    
int sensorStateBack=HIGH;  
int sensorStateLeft=HIGH;   
int sensorStateRight=HIGH;

char chain[20];
void counter_Time();
void read_sensor();

void setup() {
  // put your setup code here, to run once:
  pinMode(sensorPinFront, INPUT_PULLUP);
  pinMode(sensorPinBack, INPUT_PULLUP);
  pinMode(sensorPinLeft, INPUT_PULLUP);
  pinMode(sensorPinRight, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_sensor();
  counter_time();
  Serial.println("time_back: ");
  Serial.println(time_back);
  Serial.println("time_left: ");
  Serial.println(time_left);
  Serial.println("time_right: ");
  Serial.println(time_right);
  Serial.println("time_front: "); 
  Serial.println(time_front); 
  delay(500);

}
void read_sensor(){
  sensorStateFront = digitalRead(sensorPinFront);
  sensorStateBack = digitalRead(sensorPinBack);
  sensorStateLeft = digitalRead(sensorPinLeft);
  sensorStateRight = digitalRead(sensorPinRight);
  if (mode == INPUT) {
                uint8_t oldSREG = SREG;
                cli();
                *reg &= ~bit;
                *out &= ~bit;
                SREG = oldSREG;
        } else if (mode == INPUT_PULLUP) {
                uint8_t oldSREG = SREG;
                cli();
                *reg &= ~bit;
                *out |= bit;
                SREG = oldSREG;
        } else {
                uint8_t oldSREG = SREG;
                cli();
                *reg |= bit;
                SREG = oldSREG;
        }
}
//************************************************************
void counter_time(){
  if ((estate_front_last==HIGH)&&(sensorStateFront == LOW)){
    time_front=micros();
 }
 
   if ((estate_back_last==HIGH)&&(sensorStateBack == LOW)){
    time_back=micros();
  }
 
   if ((estate_left_last==HIGH)&&(sensorStateLeft == LOW)) {
    time_left=micros();
  }
 
   if ((estate_right_last==HIGH)&&(sensorStateRight == LOW)){
    time_right=micros();
  } 
   estate_left_last=sensorStateLeft;
   estate_right_last=sensorStateRight;    
   estate_back_last=sensorStateBack;
   estate_front_last=sensorStateFront;
}
//*******************************************************************
