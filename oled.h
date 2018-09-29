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
void oled_read_page_sram(uint8_t line);
uint8_t oled_read_d_sram(uint8_t line, uint8_t col);
void oled_write_letter_sram(const char letter, uint8_t fontSize, uint8_t line, uint8_t col);
void oled_write_d_sram(uint8_t data, uint8_t line, uint8_t col);
void oled_init_sram();
void oled_print_sram(const char* data, uint8_t fontSize, uint8_t line, uint8_t col);
void oled_read_screen_sram();
