//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Casey J Vosselman
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
#include "adc.h"



//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void wait_for_pb_released(uint8_t PB_IDX);
void read_display_soil();

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define wet_stp_size                                                          33
#define dry_state_max                                                         99
#define dry_state_min                                                         66
#define wet_state_max                                                         65
#define wet_state_min                                                         34
#define too_wet_state_max                                                     33
#define too_wet_state_min                                                     0
#define adc_max                                                             2180
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
  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);

  read_display_soil();

  // enter your code here
  

  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */

void read_display_soil()
{

  bool     done      = false;
  uint16_t adc_value = 0;
  uint8_t wet_step = 0;
  lcd_clear();

  while (!done)
  {
    adc_value = ADC0_in(5);
  
    
    lcd_set_ddram_addr(LCD_LINE1_ADDR);

    if ((adc_max-adc_value) >= dry_state_min)
    {
      lcd_write_string("Status: Very Wet");
      
    else if ((adc_max-adc_value) <= too_wet_state_max)
    {
      lcd_write_string("Status: Dry");
    }
    }
    else
    {
      lcd_write_string("Status: Proper Moisture");
      
    }

    lcd_write_string("ADC:");

    lcd_set_ddram_addr(LCD_LINE2_ADDR);

    lcd_write_doublebyte(adc_value);

    if (is_pb_down(PB1_IDX))
    {
      wait_for_pb_released(PB1_IDX);
      done = true;
    }

  }
  lcd_clear();
  lcd_write_string("Program Stopped");
}

//  ----------------------------------------------------------------------------
// DESCRIPTION:
//  Accepts a pushbutton press an input. While the button is down, nothing
//  happens.
//
//  INPUT PARAMETERS:
//  uint8_t value representing a pushbutton
//
//  OUTPUT PARAMETERS:
//  none
//
//  RETURN:
//  none
//  ----------------------------------------------------------------------------
void wait_for_pb_released(uint8_t PB_IDX)
{
  while (is_pb_down(PB_IDX))
    ;
  msec_delay(10);
}
