#ifndef F_CPU
#define F_CPU 4915200 // or whatever may be your frequency
#endif
#include <avr/io.h>
#include <util/delay.h>          // for _delay_ms()

void extern_mem_init(){
  MCUCR = (1<<SRE);
}
void test_latch(){
  volatile char *ext_ram = (char*) 0x1801;
  ext_ram[0] = 0;
  _delay_ms(200);
  ext_ram[1] = 0;
  //ext_ram = 0x1802;
  //*ext_ram = 0;
  _delay_ms(200);

}
