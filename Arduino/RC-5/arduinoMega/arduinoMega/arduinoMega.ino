
#include <VirtualWire.h>
const int SW1 = 2;
const int SW2 = 3;
const int SW3 = 4;
const int SW4 = 5;
const int SW5 = 6;
const int LED = 7;
uint8_t data;
uint8_t dataleng=1;
char dato[1];
bool state[5],state_led;
void setup()
{    
    // Se inicializa el RF
    vw_setup(2000); // velocidad: Bits per segundo
    vw_set_tx_pin(8); //Pin 2 como salida para el RF
    vw_set_rx_pin(10);  //Pin 2 como entrada del RF
    vw_rx_start();       // Se inicia como receptor  
    pinMode(SW1, INPUT_PULLUP);
    pinMode(SW2, INPUT_PULLUP);
    pinMode(SW3, INPUT_PULLUP);
    pinMode(SW4, INPUT_PULLUP);
    pinMode(SW5, INPUT_PULLUP);   
    pinMode(LED, OUTPUT);
    state_led = false;   
    digitalWrite(LED,state_led); 
    for(int i=0;i<5;i++){
        state[i]=false;
      }
}

void loop()
{
     if (vw_get_message(&data,&dataleng))
      { 
        if((char)data=='x')
        {
          state_led=!state_led;
          digitalWrite(LED,state_led);
          delay(500);
        }  
       }
      state[0] = digitalRead(SW1);
      state[2] = digitalRead(SW2);
      state[3] = digitalRead(SW3);
      state[4] = digitalRead(SW4);
      state[5] = digitalRead(SW5);
       if(digitalRead(SW1)){
           dato[0] ='a';
           vw_send((uint8_t*)dato,sizeof(dato));
           vw_wait_tx();
        }
       if(digitalRead(SW2)){
           dato[0] ='b';
           vw_send((uint8_t*)dato,sizeof(dato));
           vw_wait_tx();
        }
       if(digitalRead(SW3)){
           dato[0] ='c';
           vw_send((uint8_t*)dato,sizeof(dato));
           vw_wait_tx();
        }
       if(digitalRead(SW4)){
           dato[0] ='d';
           vw_send((uint8_t*)dato,sizeof(dato));
           vw_wait_tx();   
        }
       if(digitalRead(SW5)){
           dato[0] ='e';
           vw_send((uint8_t*)dato,sizeof(dato));
           vw_wait_tx();
        }
       if(!digitalRead(SW1)){
           dato[0] ='A';
           vw_send((uint8_t*)dato,sizeof(dato));
           vw_wait_tx();
        }
       if(!digitalRead(SW2)){
           dato[0] ='B';
           vw_send((uint8_t*)dato,sizeof(dato));
           vw_wait_tx();
        }
       if(!digitalRead(SW3)){
           dato[0] ='C';
           vw_send((uint8_t*)dato,sizeof(dato));
           vw_wait_tx(); 
        }
       if(!digitalRead(SW4)){
           dato[0] ='D';
           vw_send((uint8_t*)dato,sizeof(dato));
           vw_wait_tx();
        }
       if(!digitalRead(SW5)){
           dato[0] ='E';
           vw_send((uint8_t*)dato,sizeof(dato));
           vw_wait_tx();
        }
}

