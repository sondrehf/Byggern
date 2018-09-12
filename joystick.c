#include "joystick.h"
#include "adc.h"

uint8_t get_y_raw_value(){
  return get_channel_value(0);
}
uint8_t get_x_raw_value(){
  return get_channel_value(1);
}

struct joystick_angle calculate_angle(){
  uint8_t yRawValue = get_y_raw_value();
  uint8_t xRawValue = get_x_raw_value();
  struct joystick_angle pos;
  pos.xAngle = (uint8_t)xRawValue*(180.0/255);
  pos.yAngle = (uint8_t)yRawValue*(180.0/255);
  //printf("X-Angle: %d\n\r, Y-Angle: %d\n\r", b.xAngle, b.yAngle);

  pos.xAnglePercent = (int)((xRawValue*(100.0/255)-50)*2);
  pos.yAnglePercent = (int)((yRawValue*(100.0/255)-50)*2);
  //printf("X-Angle: %d \n\r, Y-Angle: %d \n\r", b.xAnglePercent, b.yAnglePercent);

  return pos;
}

enum joystick_direction get_joystick_direction(struct joystick_angle angle){
  if ((angle.xAnglePercent < 4 && angle.xAnglePercent >-4)
  && (angle.yAnglePercent < 4 && angle.yAnglePercent >-4)) {
    return NEUTRAL;
  }
  else if ((angle.yAnglePercent >= 0) && (angle.xAnglePercent >= -50 && angle.xAnglePercent <= 50)) {
    return UP;
  }
  else if ((angle.yAnglePercent <= 0) && (angle.xAnglePercent >= -50 && angle.xAnglePercent <= 50)) {
    return DOWN;
  }
  else if ((angle.xAnglePercent >= 0) && (angle.yAnglePercent >= -50 && angle.yAnglePercent <= 50)) {
    return RIGHT;
  }
  else if ((angle.xAnglePercent <= 0) && (angle.yAnglePercent >= -50 && angle.yAnglePercent <= 50)) {
    return LEFT;
  }
  return ERROR;
}
