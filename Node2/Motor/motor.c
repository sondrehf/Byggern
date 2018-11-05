#include "../Utilities/TWI_Master.h"
#include "../../usbBoard.h"
#include "../CAN/CAN_node2.h"
#include "avr/io.h"
#include "motor.h"

void motor_initialize(){
  //Select direction of motor direction pin and motor enable pin
  DDRH |= (1<<PH4) | (1<<PH1);
  //Set motor enable bit
  PORTH = (1<<PH4);
}

void TWI_motor_control(can_message msg){
  unsigned char msg_array[2];
  msg_array[0] = 0x50; //Address is zero, and the write bit is LSB.
  msg_array[1] = 0; //Deciding which output that transmits from DAC.
  if (msg.data[1] > 140){
    PORTH |= (1<<PH1);
    msg_array[2] = msg.data[1];
  }
  else if (msg.data[1] < 115){
    msg_array[2] = 255 - msg.data[1];
    PORTH &= ~(1<<PH1);
  }
  else{
    msg_array[2] = 0;
  }

  TWI_Start_Transceiver_With_Data(msg_array, 3);
}
