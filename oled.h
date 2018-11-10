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

/* --------------------SRAM MEMORY TO OLED FROM HERE DOWN ----------------------------------*/
// Line is page
// Row is each line on the OLED. A total of 64 rows
void oled_read_page_sram(uint8_t line);
uint8_t oled_read_d_sram(uint8_t line, uint8_t col);
void oled_write_letter_sram(const char letter, uint8_t fontSize, uint8_t line, uint8_t col);
void oled_write_d_sram(uint8_t data, uint8_t line, uint8_t col);
void oled_clear_sram();
void oled_print_sram(const char* data, uint8_t fontSize, uint8_t line, uint8_t col);
void oled_read_screen_sram();

void oled_write_pixel_sram(uint8_t row, uint8_t col);
void oled_write_circle_sram(uint8_t line, uint8_t col);
void oled_animation_circle_horizontal_sram(uint8_t line, uint8_t* distanceFromStart, uint8_t* sign);
void oled_write_platform_vertical_sram(uint8_t line, uint8_t col, uint8_t height);
void oled_write_platform_horizontal_sram(uint8_t line, uint8_t col, uint8_t height);
void oled_animation_shoot_ball_sram(uint8_t line, uint8_t startHeightPlat, uint8_t* distanceFromStart, uint8_t* sign);

/* --------------------Timer interrupt ----------------------------------*/
void timer_interrupt_for_oled_init();
