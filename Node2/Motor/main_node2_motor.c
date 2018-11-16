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
#include "../adc/adc_node2.h"
#include "../USART/USART.h"
#include "../Timer/timer.h"
#include "../Utilities/MCP2515_node2.h"
#include "motor.h"
#include "../Utilities/TWI_Master.h"
#include "../CAN/CAN_node2.h"
#include "../Regulator/regulator.h"

volatile static can_message msg;
static volatile uint8_t receivedMessage;
static volatile uint8_t readEncoderFlag;
static volatile uint32_t scoreCounter = 0;

int main(void){
  USART_init(MYUBRR);
  mcp2515_init();
  can_init();
  can_set_normal_mode();
  timer_pwm_init();
  adc_node2_config();
  TWI_Master_Initialise();
  motor_initialize();
  //Initialize regulator



  /* INTERRUPT ENABLE */
  // Pin input
  DDRB &= ~(1<<PB6);
  // Disable global interrupts
  cli();
  timer_interrupt_for_controller_init();

  //Enable pin change interrupt on pin : PCINT 0:7
  PCICR |= (1<<PCIE0);
  //Setting PCINT6 interrupt
  PCMSK0 |= (1<<PCINT6);
  //Enable global interrupts
  sei();
  //start condition for output
  msg.data[1] = 128;

  uint8_t values[4] = {0,0,0,0};
  int absolutePositionRotation = 0;
  uint8_t absolutePosition = 0;
  double ratio = 255.0/9000;
  uint8_t input = 0;
  uint8_t* dir = 1;
  can_message gameover;
  regulator_data reg;
  while(1){
    switch (msg.id) {
      case 5:
        regulator_init(msg.data[0], msg.data[1], 0, &reg);
        break;
      case 6:
        regulator_init(msg.data[0], msg.data[1], 0, &reg);
        break;
      case 7:
        regulator_init(msg.data[0], msg.data[1], 0, &reg);
        break;
      default:
        //regulator_init(msg.data[0], msg.data[1], 0, &reg);
        break;

    }
    //ONLY PLAY GAME IF SIGNAL IS SENT!
    if(msg.id == 34){
      initial_position();
      scoreCounter = 0;
      //printf("%s\n", "Leggo" );
      while(1){
        uint8_t lost = game_over();
        //printf("%d\n\r", lost );
        if(lost){
          scoreCounter/=(50);
          gameover.id = 1;
          gameover.length = 1;
          gameover.data[0] = scoreCounter;
          printf("%d\n", gameover.id );
          can_message_send(&gameover);
          printf("%s\n","Game over bitch");
          break;
        }
        if(readEncoderFlag){
          readEncoderFlag = 0;
          absolutePositionRotation += read_encoder();
          if(absolutePositionRotation > 9000){
            absolutePositionRotation = 9000;
          }
          else if(absolutePositionRotation < 0){
            absolutePositionRotation = 0;
          }
        }
        absolutePosition = absolutePositionRotation * ratio;
        //printf("Check encoder again: %d\n\r", absolutePositionRotation );
        input = regulator(&dir, msg.data[1], absolutePosition, &reg);
        solenoid_controller(msg.data[2]);
        //printf("%d\n\r", msg.data[0] );
        joystick_to_PWM(msg);
        TWI_motor_control(input, &dir);
        //printf("%s\n","QUE" );
    }
    absolutePositionRotation = 0;
  }


  }
}

ISR(PCINT0_vect){
  //receivedMessage = 1;
  msg = can_message_receive();
  //printf("%d : %d\n\r", msg.data[0],msg.data[1] );
}
ISR(TIMER3_COMPA_vect){
  readEncoderFlag = 1;
  scoreCounter++;
}
