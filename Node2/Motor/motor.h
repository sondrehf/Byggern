#include <stdio.h>
#include "../CAN/CAN_node2.h"


void TWI_motor_control(can_message msg, uint8_t input, uint8_t* dir);
void motor_initialize();
void initial_position();
int read_encoder();
