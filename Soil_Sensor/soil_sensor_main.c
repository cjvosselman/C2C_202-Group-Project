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
#include "adc.h"
#include "clock.h"
#include "lcd1602.h"
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void read_display_soil();
void enable_adc_avgs();
bool soil_status(uint16_t adc_value);
uint16_t average_adc_values();
void sys_tick_init(uint32_t period);
void interrupt_sequence();
void dry_soil(uint16_t adc_value);
void wet_soil(uint16_t adc_value);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define adc12_max 4096
#define dry_soil_threshold 3200 // Dry is above | Wet is below
#define wet_soil_threshold 2800
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool adc_ready = false;

// Define a structure to hold different data types

int main(void) {
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  I2C_mstr_init();
  lcd1602_init();
  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);
  sys_tick_init(200000);
  enable_adc_avgs();
  read_display_soil();

  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */

void read_display_soil() {

  bool done = false;
  uint16_t adc_reading = average_adc_values();
  bool soil_state = soil_status(adc_reading);
  bool new_soil_state;
  bool initial_reading = true;
  while (!done) 
  {
    if (adc_ready) {
      adc_reading = average_adc_values();
      new_soil_state = soil_status(adc_reading);
      adc_ready = false;
    }

    if ((soil_state != new_soil_state) || initial_reading) {
      soil_state = new_soil_state;
      if (soil_state == true) {
      dry_soil(adc_reading);
      } else {
      wet_soil(adc_reading);
      }
      initial_reading = false;
    }
  }
}

void enable_adc_avgs() // The adc collects 16 samples and averages them together
{
  ADC0->ULLMEM.MEMCTL[0] = ADC12_MEMCTL_AVGEN_ENABLE;
  ADC0->ULLMEM.CTL1 = ADC12_CTL1_AVGN_AVG_16 | ADC12_CTL1_AVGD_SHIFT4;
}

// This function collects 10 of the averaged values and averages them to create
// less noise Smooths out data Updates global variable determining whether or
// not soil is dry
uint16_t average_adc_values() {

  uint8_t idx = 0;
  uint8_t max_idx = 10;
  uint8_t i = 0;
  uint16_t sum_adc_val = 0;
  uint16_t soil_read[max_idx];
  uint16_t soil_value = 0;

  for (idx = 0; idx < max_idx; idx++) {
    soil_read[idx] = ADC0_in(5);
  }
  for (i = 0; i < max_idx; i++) {
    sum_adc_val += soil_read[i];
  }
  soil_value = sum_adc_val / max_idx;

  return soil_value;
}

void dry_soil(uint16_t adc_value) 
{
  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Status:DRY");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("MOISTURE:LOW ");
}

void wet_soil(uint16_t adc_value) 
{
  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Status:WET");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("MOISTURE:HIGH");

}

bool soil_status(uint16_t adc_value) {

if (adc_value > dry_soil_threshold)
{
  return true;
} 
else {
  return false;
}

}

// SysTick Interrupt Service Routine
void SysTick_Handler(void) { adc_ready = true; } /* SysTick_Handler */