#ifndef F_CPU
#define F_CPU 16000000
#endif

#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


#include <stdio.h>
#include <util/delay.h>           // for _delay_ms()
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../USART/USART.h"
#include "../Timer/timer.h"
#include "../Utilities/MCP2515_node2.h"
#include "motor.h"
#include "../Utilities/TWI_Master.h"
#include "../CAN/CAN_node2.h"
#include "../Regulator/regulator.h"

volatile static can_message msg;

int main(void){
  USART_init(MYUBRR);
  mcp2515_init();
  can_init();
  can_set_normal_mode();
  timer_pwm_init();
  timer_interrupt_for_controller_init();
  TWI_Master_Initialise();
  motor_initialize();
  //Initialize regulator
  regulator_data reg;
  regulator_init(1.2, 1, 0, &reg);


  /* INTERRUPT ENABLE */
  // Pin input
  DDRB &= ~(1<<PB6);
  // Disable global interrupts
  cli();
  //Enable pin change interrupt on pin : PCINT 0:7
  PCICR |= (1<<PCIE0);
  //Setting PCINT6 interrupt
  PCMSK0 |= (1<<PCINT6);
  //Enable global interrupts
  sei();
  initial_position();
  //start condition for output
  msg.data[1] = 128;


  int absolutePositionRotation = 0;
  uint8_t absolutePosition = 0;
  double ratio = 255.0/8900;
  uint8_t input = 0;
  uint8_t* dir = 1;
  while(1){
    absolutePositionRotation += read_encoder();
    absolutePosition = absolutePositionRotation * ratio;

    input = regulator(&dir, msg.data[1], absolutePosition, &reg);


    TWI_motor_control(input, &dir);


    solenoid_controller(msg.data[2]);
    joystick_to_PWM(msg);

  }
}

ISR(PCINT0_vect){
  uint8_t interrupt = mcp2515_read(MCP_CANINTF);
  uint8_t RX0_flag = interrupt & 0b00000001;
  //check to see if received data.
  if (RX0_flag){
    msg = can_message_receive();

  }
}
