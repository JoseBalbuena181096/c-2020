
volatile unsigned   muestreoActualI=0;
volatile unsigned   muestreoAnteriorI=0;
volatile unsigned   deltaMuestreoI=0;
double frecuencia=0;
double wr=0;
int past=0;
int change=0;
int n=20;
int encoderI=3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(encoderI,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderI), interrupt,FALLING);
}
void interrupt(){
         deltaMuestreoI=muestreoActualI-muestreoAnteriorI;
         frecuencia=1000/((double)deltaMuestreoI);
         wr=((2*3.141516)/n)*frecuencia;
         muestreoAnteriorI=muestreoActualI;
         
}
void loop() {
  // put your main code here, to run repeatedly:
  muestreoActualI=millis();

  change=muestreoActualI-past;
  if(change>=100){
      Serial.println(wr);
    past=muestreoActualI;
    muestreoActualI=0;
    muestreoAnteriorI=0;
    deltaMuestreoI=0;
    frecuencia=0;
    }
}
