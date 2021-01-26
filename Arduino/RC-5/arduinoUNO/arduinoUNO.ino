#include <VirtualWire.h>


const int LED1 = 2;
const int LED2 = 3;
const int LED3 = 4;
const int LED4 = 5;
const int LED5 = 6;
bool states_leds[5];
char data[1];
void setup()
{
    // Se inicializa el RF
    vw_setup(2000);  // velocidad: Bits per segundo
    vw_set_rx_pin(8);  //Pin 2 como entrada del RF
    vw_set_tx_pin(10);
    vw_rx_start();       // Se inicia como receptor
    
    pinMode(LED1, OUTPUT);   
    digitalWrite(LED1, false);
    pinMode(LED2, OUTPUT);   
    digitalWrite(LED2, false);
    pinMode(LED3, OUTPUT);   
    digitalWrite(LED3, false);
    pinMode(LED4, OUTPUT);   
    digitalWrite(LED4, false);
    pinMode(LED5, OUTPUT);   
    digitalWrite(LED5, false);
    for(int i=0;i<5;i++){
        states_leds[5] = false;
      } 
}

void loop()
{
    uint8_t dato;
    uint8_t datoleng=1;
    //verificamos si hay un dato valido en el RF
    
    if (vw_get_message(&dato,&datoleng))
    {
       if((char)dato=='a')
        {
          data[0] ='x';
          vw_send((uint8_t*)data,sizeof(data));
          vw_wait_tx();  
          digitalWrite(LED1,true);
        }
        else if((char)dato=='b')
        {
          data[0] ='x';
          vw_send((uint8_t*)data,sizeof(data));
          vw_wait_tx();  
          digitalWrite(LED2,true);
        }
        else if((char)dato=='c')
        {
          data[0] ='x';
          vw_send((uint8_t*)data,sizeof(data));
          vw_wait_tx();  
          digitalWrite(LED3,true);
        }   
        else if((char)dato=='d')
        {
          data[0] ='x';
          vw_send((uint8_t*)data,sizeof(data));
          vw_wait_tx();  
          digitalWrite(LED4, true);
        }
        else if((char)dato=='e')
        {
          data[0] ='x';
          vw_send((uint8_t*)data,sizeof(data));
          vw_wait_tx();  
          digitalWrite(LED5,true);
        }
        else if((char)dato=='A')
        {
          data[0] ='x';
          vw_send((uint8_t*)data,sizeof(data));
          vw_wait_tx();  
          digitalWrite(LED1,false);    
        }
        else if((char)dato=='B')
        {
          data[0] ='x';
          vw_send((uint8_t*)data,sizeof(data));
          vw_wait_tx();  
          digitalWrite(LED2,false);
        }
        else if((char)dato=='C')
        {
          data[0] ='x';
          vw_send((uint8_t*)data,sizeof(data));
          vw_wait_tx();    
          digitalWrite(LED3,false);
        }   
        else if((char)dato=='D')
        {
          data[0] ='x';
          vw_send((uint8_t*)data,sizeof(data));
          vw_wait_tx();  
          digitalWrite(LED4, false);
        }
        else if((char)dato=='E')
        {
          data[0] ='x';
          vw_send((uint8_t*)data,sizeof(data));
          vw_wait_tx();
          digitalWrite(LED5, false);
        }  
    }
}
