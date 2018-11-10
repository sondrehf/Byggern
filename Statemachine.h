#include "menu.h"
#include "CAN.h"
#include <stdio.h>


void state_machine(menu_page* page, can_message *msg);
void play_game(can_message*);
