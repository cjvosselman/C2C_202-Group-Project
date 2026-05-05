//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  TBD
//
//       LAB NAME:  TBD
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code ... *** COMPLETE THIS BASED ON LAB REQUIREMENTS ***
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include "LaunchPad.h"
#include "clock.h"
#include <ti/devices/msp/msp.h>
#include "lcd1602.h"
#include "spi.h"
#include "uart.h"
#include "adc.h"

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void light_read_display();

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define step_size                                                           254
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  I2C_mstr_init(); 
  lcd1602_init();
  leds_init();
  leds_enable();
  ADC1_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);

  lcd_clear();
  light_read_display();

  leds_off();
   
 // Endless loop to prevent program from ending
 while (1);

} /* main */

void light_read_display()
{
  bool done = false;
  uint16_t adc_value = 0;
  uint8_t led_value = 7;
  uint8_t led_idx = 0;
  char adc_text[] = "ADC = ";
  char light_text[] = "Light = ";

  while (!done)
  {
  adc_value = ADC1_in(6);
  led_value = (adc_value/step_size); 
  leds_off();

  for (led_idx = 0; led_idx < led_value; led_idx++)
  {
  led_on(led_idx);  
  }

  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string(adc_text);
  lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_8);
  lcd_write_doublebyte(adc_value);

  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string(light_text);
  lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_13);
  }
}
