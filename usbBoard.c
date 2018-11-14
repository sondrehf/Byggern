#include "usbBoard.h"
#include "adc.h"
#include "CAN.h"


uint8_t get_y_raw_value(){
  return get_channel_value(0);
}
uint8_t get_x_raw_value(){
  return get_channel_value(1);
}

struct joystick_angle calculate_angle(){
  struct joystick_angle pos;
  pos.yRawValue = get_y_raw_value();
  pos.xRawValue = get_x_raw_value();

  pos.xAngle = (uint8_t)pos.xRawValue*(180.0/255);
  pos.yAngle = (uint8_t)pos.yRawValue*(180.0/255);
  //printf("X-Angle: %d\n\r, Y-Angle: %d\n\r", b.xAngle, b.yAngle);

  pos.xAnglePercent = (int)((pos.xRawValue*(100.0/255)-50)*2);
  pos.yAnglePercent = (int)((pos.yRawValue*(100.0/255)-50)*2);
  //printf("X-Angle: %d \n\r, Y-Angle: %d \n\r", b.xAnglePercent, b.yAnglePercent);

  return pos;
}

enum joystick_direction get_joystick_direction(struct joystick_angle angle){

  if((angle.xRawValue < 132 && angle.xRawValue > 124) && (angle.yRawValue < 132 && angle.yRawValue > 124)){
    return NEUTRAL;
  }
  else if (angle.xRawValue < 60 && (angle.yRawValue <= 230 && angle.yRawValue >= 60)){
    return LEFT;
  }
  else if (angle.xRawValue > 210 && (angle.yRawValue <= 230 && angle.yRawValue >= 60)){
      return RIGHT;
  }
  else if (angle.yRawValue > 170 && (angle.xRawValue >= 0 && angle.xRawValue <= 255)) {
    return UP;
  }

  else if (angle.yRawValue < 70 && (angle.xRawValue <= 255 && angle.xRawValue >= 0)) {
    return DOWN;
  }
  return ERROR;
}

//Function for retrieval of left slider position
uint8_t get_left_value(){
  return get_channel_value(2);
}

//Right slider position
uint8_t get_right_value(){
  return get_channel_value(3);
}

struct slider_pos calculate_slider(){
  struct slider_pos pos;
  pos.leftRawValue = get_left_value();
  pos.rightRawValue = get_right_value();

  pos.leftPercent = pos.leftRawValue*(100.0/255);
  pos.rightPercent = pos.rightRawValue*(100.0/255);

  return pos;
}

void print_dir(enum joystick_direction dir){
  switch (dir) {
  case NEUTRAL:
    printf("Neutral\t\t");
    break;
  case UP:
    printf("UP\t\t");
    break;
  case DOWN:
    printf("DOWN\t\t");
    break;
  case RIGHT:
    printf("RIGHT\t\t");
    break;
  case LEFT:
    printf("LEFT\t\t");
    break;
  case ERROR:
    printf("ERROR\t\t");
    break;
  default:
    break;
}
}

void motor_input_can_send(){
  can_message msg;
  msg.id = 0;
  msg.length = 3;
  //Joystick x position fetch
  msg.data[0] = get_x_raw_value();

  //Fetch right slider position
  msg.data[1] = get_right_value();
  //Button
  msg.data[2] = !get_button_value();
  can_message_send(&msg);


}


/*
struct joystick_angle pos = calculate_angle();
struct slider_pos slider = calculate_slider();
enum joystick_direction dir = get_joystick_direction(pos);
//printf("x: %d, y: %d \t\t", pos.xRawValue, pos.yRawValue);

switch (dir) {
  case NEUTRAL:
    printf("Neutral\t\t");
    break;
  case UP:
    printf("UP\t\t");
    break;
  case DOWN:
    printf("DOWN\t\t");
    break;
  case RIGHT:
    printf("RIGHT\t\t");
    break;
  case LEFT:
    printf("LEFT\t\t");
    break;
  case ERROR:
    printf("ERROR\t\t");
    break;
  default:
    break;
}uint8_t get_right_value(){
  return get_channel_value(3);
}

printf("Left: %d, Right: %d \n\r", slider.leftPercent, slider.rightPercent);
*/
