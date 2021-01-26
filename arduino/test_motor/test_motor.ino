#define MAX_SPEED 16000
#define MB1 4
#define MA1 9
#define MPWM1 10


void setup() {

  /******************Init pins to H Bridge ********************************************/
  pinMode(MA1, OUTPUT);//Motor B1
  pinMode(MB1, OUTPUT);//Motor A1
  pinMode(MPWM1, OUTPUT);//Motor1 PWM 

}


void loop() {
  // put your main code here, to run repeatedly:
       analogWrite(MPWM1,150);
       digitalWrite(MA1, HIGH);
      digitalWrite(MB1, LOW);  
}
