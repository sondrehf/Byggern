#ifndef F_CPU
#define F_CPU 16000000
#endif

#include "../Utilities/TWI_Master.h"
#include "../../usbBoard.h"
#include "../CAN/CAN_node2.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../Timer/timer.h"
#include "avr/io.h"
#include "motor.h"

static volatile int encoderValue;
#define DIR PH1
#define EN PH4
#define OE PH5
#define SEL PH3
#define RST PH6
void motor_initialize(){
  //Select direction of motor direction pin and motor enable pin
  DDRH |= (1<<EN) | (1<<DIR);
  //Select direction of pins for the encoder: !OE, SEL, RST
  DDRH |= (1<<OE) | (1<<SEL) | (1<<RST);
  //
  DDRK = 0;
  //Set motor enable bit
  PORTH = (1<<EN);
}

void TWI_motor_control(can_message msg, uint8_t input, uint8_t* dir){
  unsigned char msg_array[3];
  msg_array[0] = 0x50; //Address is zero, and the write bit is LSB.
  msg_array[1] = 0; //Deciding which output that transmits from DAC.
  if (*dir == 1){
    PORTH |= (1<<DIR);
    msg_array[2] = input;
  }
  else{
    msg_array[2] = input;
    PORTH &= ~(1<<DIR);
  }
  //else{
  

  TWI_Start_Transceiver_With_Data(msg_array, 3);
}

int read_encoder(){
  //Setting reset high to use encoder as indicator of motor rotation
  int totalValue = 0;
  uint8_t highValue, lowValue = 0;
  //Enable output and get high byte
  PORTH &= ~(1<<OE);
  PORTH &= ~(1<<SEL);
  _delay_us(20);
  highValue = PINK;
  //get low byte
  PORTH |= (1<<SEL);
  _delay_us(20);
  lowValue = PINK;
  //Toggle RST
  PORTH &= ~(1<<RST);
  _delay_us(20);
  PORTH |= (1<<RST);
  PORTH |= (1<<OE);
  totalValue = (highValue << 8)+lowValue;
  return totalValue;
}

void initial_position(){
  //Drive a certain dir
  PORTH &= ~(1<<DIR);
  unsigned char msg_array[3] = {0x50,0,130};
  TWI_Start_Transceiver_With_Data(msg_array,3);
  _delay_ms(3000);
  //reset encoder again
  PORTH &= ~(1<<RST);
  _delay_us(20);
  PORTH |= (1<<RST);
}

/* --------POSITION CONTROLLER ------------*/
//TOTAL AMOUNT OF ROTATIONS FROM LEFT TO RIGHT 8730
//int error_calc()
uint8_t motor_controller_PD(uint8_t r){
  double T = 1/50;
  int e;
  e = r-encoderValue;

}
ISR(TIMER3_COMPA_vect){
  //encoderValue = read_encoder();
  //printf("%d\n",encoderValue );
}
