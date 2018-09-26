#include <stdio.h>

void oled_init();
void oled_reset();
void write_c(uint8_t command);
void write_d(uint8_t data);
void oled_write_letter_P(uint8_t letter, uint8_t fontSize);
void oled_goto_column(uint8_t column);
void oled_goto_line(uint8_t line);
void oled_goto_page(uint8_t page);
void oled_print(const char* data, uint8_t fontSize);
void oled_goto_pos(uint8_t row, uint8_t col);