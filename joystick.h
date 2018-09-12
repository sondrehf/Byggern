#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>

typedef struct joystick_angle {
  uint8_t xAngle;
  uint8_t yAngle;
  int xAnglePercent;
  int yAnglePercent;

};

typedef enum joystick_direction {
  LEFT, RIGHT, UP, DOWN, NEUTRAL, ERROR
};

struct joystick_angle calculate_angle();
