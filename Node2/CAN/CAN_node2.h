#ifndef CAN_h
#define CAN_h


typedef struct Can_message can_message;

struct Can_message{
  unsigned int id;
  uint8_t length;
  uint8_t data[8]; // 8 is max size for data;
};

void can_init();
uint8_t can_message_send(can_message* msg);
can_message can_message_receive();
uint8_t can_int_vect();
void can_set_normal_mode();

#endif
