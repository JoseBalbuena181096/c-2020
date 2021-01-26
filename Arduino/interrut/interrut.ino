int contador = 0;
int n = contador ;
long T0 = 0 ;  
void setup()
   {   Serial.begin(9600);
       attachInterrupt( 0, ServicioBoton, FALLING);
   }
void loop()
   {
 if (n != contador)
           {   Serial.println(contador);
               n = contador ;
           }
   }

void ServicioBoton() 
   {    
          if ( millis() > T0  + 250)
          {   contador++ ;
              T0 = millis();
          }
   }
