/*
   Endoder Program
*/
//////////////////////////////////////
///           Macros              ////
//////////////////////////////////////
#define output 3
#define input 2
/////////////////////////////////////
//******Funtions Prototypes  ******//
/////////////////////////////////////

/////////////////////////////////////
void increaseCounter();
/////////////////////////////////////
bool spins_flag(int& number_spins);
/////////////////////////////////////
void get_information(int& number_spins);
/////////////////////////////////////
void restart(int& number_spins, bool& flag, int& counter);
/////////////////////////////////////
///***Global Variables **********////
/////////////////////////////////////
int  counter = 0 ;
bool flag = false;
int number_spins = 0;
long T0 = 0 ;

void setup() {
  // put your setup code here, to run once:
  pinMode(output, OUTPUT);
  pinMode(input, INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(input), increaseCounter, FALLING);
  digitalWrite(output, LOW);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("#################################");
  Serial.println("##       START PROGRAM         ##");
  Serial.println("#################################");
  get_information(number_spins);
  flag = spins_flag(number_spins);
  restart(number_spins, flag, counter);
  delay(5000);
}
///////////////////////////////////
///******Funtions Definions ****///
///////////////////////////////////

void increaseCounter() {
      if ( millis() > T0  + 250)
          {   counter++ ;
              T0 = millis();
          }
}
//////////////////////////////////
bool spins_flag(int& number_spins) {
  /////pulse to spins
  number_spins *= 20;
  int spin_int = counter;
  int diference = 0;
  while (diference <= number_spins-1) {
    diference = counter - spin_int;
    Serial.println("Pulse number: ");
    Serial.println(diference);
    Serial.println("Spin: ");
    Serial.println(diference/20);
    if(diference == number_spins){
        digitalWrite(output, HIGH);
        Serial.println("Motor Stop");
        delay(9000);
        digitalWrite(output, LOW);
        Serial.println("Work finish");
      }
  }
  return true;
}

///////////////////////////////////
void get_information(int& number_spins) {
  Serial.println("Introduce the number of spins");
  if (Serial.available() > 0)
  {
    number_spins = Serial.parseInt();
  }
}
void restart(int& number_spins, bool& flag, int& counter) {
  if (flag) {
    number_spins = 0;
    flag = false;
    counter = 0;
    digitalWrite(output, LOW);
  }
}
