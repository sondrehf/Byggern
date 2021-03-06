#ifndef menu_h
#define menu_h
#include "oled.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>          
#include "usbBoard.h"

typedef enum State state;

typedef struct Menu_page menu_page;

struct Menu_page{
    int id;
    int size;
    char* name;
    menu_page* parent;
    menu_page* options[6];
};

menu_page page_init(const char* name, const menu_page* parent, const uint8_t size, const menu_page** options, const int id);
void update_menu_page(menu_page page, enum joystick_direction dir,int position, menu_page** options);

menu_page menu_initialize();

#endif
