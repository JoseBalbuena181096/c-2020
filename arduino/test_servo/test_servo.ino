#include <Servo.h> 
Servo servoAngle;
void setup() {
  // put your setup code here, to run once:
  servoAngle.attach(3);
  servoAngle.write(60);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  for(int i = 60;i>=0;i--){
        
    servoAngle.write(i);  
    delay(10);
    }

  for(int i = 0;i<120;i++){
        servoAngle.write(i);  
    delay(10);
    
    }
  for(int i = 120;i>=60;i--){
        servoAngle.write(i);  
    delay(10);
    
    }
   */
   servoAngle.write(60);  
}
