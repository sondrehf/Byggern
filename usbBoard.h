#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>

typedef struct joystick_angle {
  uint8_t xRawValue;
  uint8_t yRawValue;
  uint8_t xAngle;
  uint8_t yAngle;
  int xAnglePercent;
  int yAnglePercent;
};

typedef enum joystick_direction {
  LEFT, RIGHT, UP, DOWN, NEUTRAL, ERROR
};

struct joystick_angle calculate_angle();

typedef struct slider_pos {
  uint8_t leftRawValue;
  uint8_t rightRawValue;
  uint8_t leftPercent;
  uint8_t rightPercent;
};

struct slider_pos calculate_slider();
