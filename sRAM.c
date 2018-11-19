#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void SRAM_test(void) {
  volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
  uint16_t ext_ram_size = 0x800;
  uint16_t write_errors = 0;
  uint16_t retrieval_errors = 0;
  printf("Starting SRAM test...\r\n");
  // rand() stores some
  //internal state, so calling this function in a loop will
  // yield different seeds each time (unless srand() is called before this
  //function)
  uint16_t seed = rand();
  // Write phase: Immediately check that the correct value was stored
  srand(seed);
  for (uint16_t i = 0; i < ext_ram_size; i++) {
    uint8_t some_value = rand();
    ext_ram[i] = some_value;
    uint8_t retreived_value = ext_ram[i];
    if (retreived_value != some_value) {
      printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
      write_errors++;
    }
  }
  // Retrieval phase: Check that no values were changed during or after the write
  //phase
  srand(seed);
  // reset the PRNG to the state
  //it had before the write phase
  for (uint16_t i = 0; i < ext_ram_size; i++) {
    uint8_t some_value = rand();
    uint8_t retreived_value = ext_ram[i];
    if (retreived_value != some_value) {
      printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\r\n",i, retreived_value, some_value);
      retrieval_errors++;
    }
  }
  printf("SRAM test completed with \r\n%4d errors in write phase and \r\n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}

void write_to_EEPROM(uint8_t uiAddress, unsigned char ucData){
  cli();
  while(EECR & (1<<EEWE));
  EEAR = uiAddress;
  EEDR = ucData;
  EECR |= (1<<EEMWE);
  EECR |= (1<<EEWE);
  sei();
}
unsigned char read_from_EEPROM(uint8_t uiAddress){
  cli();
  while(EECR & (1<<EEWE));
  EEAR = uiAddress;
  EECR |= (1<<EERE);
  sei();
  return EEDR;
}

void init_highScore(){
  for(uint8_t i = 0; i<24; i++){
    write_to_EEPROM(i, 0);
  }
  //printf("%s\n","Highscores reset" );
}

void saveHighScore(uint8_t score){
  read_data_on_highscore();
  printf("Name:\n\r ");
  unsigned char name[3];
  char temp = ' ';
  uint8_t i = 0;
  while(i<3){
    temp = USART_receive();
    name[i] = temp;
    i++;
    USART_transmit(temp);
  }
  //printf("\n\r%s", "Thats gay" );

  for(uint8_t i = 3; i<4*6; i+=4){
    if(score > read_from_EEPROM(i)){
      //Jump to second last item, right shift this by 4.
      for (uint8_t j=20; j>i-4; j--){
        uint8_t data = read_from_EEPROM(j);
        write_to_EEPROM(j+4, data);
        //USART_transmit((char) j);
        if (j==0){
          break;
        }
      }
      //Insert score into correct place
      write_to_EEPROM(i-3, name[0]);
      write_to_EEPROM(i-2, name[1]);
      write_to_EEPROM(i-1, name[2]);
      write_to_EEPROM(i, score);
      break;
    }
  }
}

void read_data_on_highscore(){
  for (int i = 0; i < 23; i++){
    unsigned char data = read_from_EEPROM(i);
    USART_transmit(data);
  }
}
