#include "oled.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>           // for _delay_ms()
#include "usbBoard.h"
#include "menu.h"

struct menu_page;

struct menu_page page_init(const char* name, const int* parent, const uint8_t size, const char* options[]){
    struct menu_page page;
    page.name = name; 
    page.size = size; 
    page.parent = parent; 
    for (int i = 0; i < page.size; i++){
        page.options[i] = options[i];
    }
    return page;
}

void OLED_print_arrow(uint8_t row, uint8_t col){
    oled_goto_pos(row, col);
    write_d(0b00011000);
    write_d(0b00011000);
    write_d(0b01111110);
    write_d(0b00111100);
    write_d(0b00011000);
}
void OLED_erase_arrow(uint8_t row, uint8_t col){
    oled_goto_pos(row, col);
    write_d(0b00000000);
    write_d(0b00000000);
    write_d(0b00000000);
    write_d(0b00000000);
    write_d(0b00000000);
}

void update_menu_page(struct menu_page page, enum joystick_direction dir,int position){
    oled_goto_pos(0, 0);//63-(strlen(page.name)/2));
    oled_print(page.name,8);
    for(int i=0; i < page.size ; i++){
        if(i == position){
            OLED_print_arrow(2+(position), 0);
        }
        
        if (dir == UP){
            OLED_erase_arrow(2+position, 0);
        }
        if (dir == DOWN){
            OLED_erase_arrow(2+position, 0);
        }

        oled_goto_page(i+2);
        oled_goto_column(10);
        oled_print(page.options[i], 4);
    }
}

int cursor_counter(struct menu_page page, enum joystick_direction dir, int arrowPos){
    if (dir == UP){
        arrowPos--;
            if((arrowPos)<0){
                arrowPos=page.size-1;
            }
        }
    else if (dir == DOWN){
        arrowPos++;
        if(arrowPos > page.size-1){
            arrowPos = 0;
        }
    }
    return arrowPos; 
}
/*
struct menu_page choose_state(struct* states, int checker){
    for (int i = 0; i < (sizeof(states)/sizeof(struct)); ++i){
        if (i == checker){
            return states[i];
        }
    }
} */


