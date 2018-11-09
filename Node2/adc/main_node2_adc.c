#ifndef F_CPU
#define F_CPU 16000000
#endif

#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include "../USART/USART.h"
#include "adc_node2.h"
#include <util/delay.h>           // for _delay_ms()

int main(){
    adc_config();
    USART_init(MYUBRR);
    uint8_t counter = 0;
    while(1){
      counter = score_counter(counter);

    }
    return 0;
}
