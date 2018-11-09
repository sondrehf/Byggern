#ifndef regulator_h
#define regulator_h


typedef struct Regulator_data regulator_data;

struct Regulator_data{
   // Last process value, used to find derivative of process value.
  uint8_t lastProcessValue;
  // Summation of errors, used for integrate calculations
  uint32_t sumError;
  // The Proportional tuning constant, multiplied with SCALING_FACTOR
  uint8_t P_Factor;
  // The Integral tuning constant, multiplied with SCALING_FACTOR
  uint8_t I_Factor;
  // The Derivative tuning constant, multiplied with SCALING_FACTOR
  uint8_t D_Factor;
  // Maximum allowed error, avoid overflow
  uint8_t maxError;
  // Maximum allowed sumerror, avoid overflow
  uint16_t maxSumError;

};

void regulator_init(double p_factor, double K_i, uint8_t d_factor, regulator_data* reg);
uint8_t regulator(uint8_t* dir, uint8_t setPoint, uint8_t processValue, regulator_data* regData);

#endif