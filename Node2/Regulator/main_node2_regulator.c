#include <stdio.h>
#include "regulator.h"
#include "../USART/USART.h"

#ifndef F_CPU
#define F_CPU 16000000
#endif

#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

int main(){
    USART_init(MYUBRR);
    regulator_data reg;
    regulator_init(1, 0, 0, &reg);
    uint8_t value = regulator(128, 0, &reg);
    while (1){
    }
    return 0;
}
