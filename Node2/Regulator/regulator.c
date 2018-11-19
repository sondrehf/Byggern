#include "stdio.h"
#include "regulator.h"
#include "../USART/USART.h"
#include "stdint.h"


/* We can later define what type of controller we need */
void regulator_init(double p_factor, double K_i, uint8_t d_factor, regulator_data* reg){
    // Start values for PID controller
    reg->sumError = 0;
    reg->lastProcessValue = 0;
    // Tuning constants for PID loop
    reg->P_Factor = p_factor;
    reg->I_Factor = K_i;//p_factor*(1/50.0)*(1/T_i);
    reg->D_Factor = d_factor;
    // Limits to avoid overflow
    reg->maxError = 255;        //MAX_INT / (reg->P_Factor + 1);
    reg->maxSumError = 1000;
    //reg->maxSumError = MAX_I_TERM / (reg->I_Factor + 1);
}

/* Have not implemented overflow check */
uint8_t regulator(uint8_t* dir, uint8_t setPoint, uint8_t processValue, regulator_data* regData){
  //Uninverting
  setPoint = 255 - setPoint;
  //Setting deadband
    if (processValue > 255){
      processValue = 255;
    }
    else if(processValue < 5){
      processValue = 5;
    }
    if(setPoint > 248){
      setPoint = 248;
    }
    else if(setPoint < 5){
      setPoint = 5;
    }
    // computing the abs value of error
    uint8_t error = 0;
    if(processValue > setPoint){
        error = processValue - setPoint;
        *dir = -1;
    }
    else{
        error = setPoint - processValue;
        *dir = 1;
    }

    // Calculate P-term
    uint8_t pTerm = regData->P_Factor*error;

    //Calculate I-term
    uint8_t temp = regData->sumError + (-1)*(*dir)*error;
    float iTerm;
    if (temp > regData->maxSumError) {
        iTerm = regData->maxSumError;
        regData->sumError = regData->maxSumError;
    }
    else{
        regData->sumError = temp;
        iTerm = (regData->I_Factor * regData->sumError);
    }
    if(abs(error) < 5){
        iTerm = 0;
    }
    uint16_t inputTemp = pTerm + iTerm/5.0;

    //Float needed to scale down to get the correct number
    uint8_t input = pTerm +iTerm/5.0;
    //overflow protection
    if(inputTemp > 255){
        input = 255;
    }

    return input;
}
