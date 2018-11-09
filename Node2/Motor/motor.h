#include <stdio.h>
#include "../CAN/CAN_node2.h"


void TWI_motor_control(uint8_t input, uint8_t* dir);
void motor_initialize();
void initial_position();
int read_encoder();
void solenoid_controller(uint8_t button);
