#include "menu.h"
#include "CAN.h"
#include "sRAM.h"
#include "usbBoard.h"
#include <avr/interrupt.h>

#ifndef Statemachine_h
#define Statemachine_h

void state_machine(menu_page* page);
void play_game();
#endif
