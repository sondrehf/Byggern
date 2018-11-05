#include "timer.h"
#include <stdio.h>
#include "../CAN/CAN_node2.h"
#include "../../usbBoard.h"
#include "../Utilities/MCP2515_node2.h"
#include <avr/io.h>
void timer_init(){
  //Set OC1A to output
  DDRB |= (1<<PB5);

  //Set fast PWM mode and clock prescaling, N=8
  TCCR1A |= (1 << WGM11);
  TCCR1B |=  (1 << WGM12) | (1 <<WGM13);

  //Clock prescaling
  TCCR1B |= (1<<CS11);

  //Non inverting mode
  TCCR1A |= (1<<COM1A1);

  //Decide TOP s.t. We get the desired frequency and prescale factor.
  //N = fclk/(fout*(1+TOP))
  ICR1H = 0x9C;
  ICR1L = 0x3F;



// Default value for duty cycle
  OCR1AH = 0x0B;
  OCR1AL = 0xB8;

//HIGH: 0x1068
//LOW: 0x0708

// Clear counter
  TCNT1H = 0;
  TCNT1L = 0;

}

void joystick_to_PWM(can_message msg){
    //joystick_can_send();
    /*if ((mcp2515_read(MCP_CANINTF) & 0b00000001)) {
      msg = can_message_receive();
    }*/
    unsigned int tempVal = ((4200-1800)/255.0)*msg.data[0] + 1800;
    //printf("%d\n", msg.data[0]);
    //printf("%d\n", tempVal);

    if((tempVal > 4200)){
      OCR1AH = 0x10;
      OCR1AL = 0x68;
    }
    else if((tempVal < 1800)){
      OCR1AH = 0x07;
      OCR1AL = 0xB8;
    } else {
      OCR1AH = tempVal >> 8;
      OCR1AL = tempVal & 0xFF;
      printf("%x, %x\n", OCR1AH,OCR1AL);

    }
}
