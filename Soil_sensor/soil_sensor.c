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
#include "lcd1602.h"
#include "spi.h"
#include "uart.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include "LaunchPad.h"
#include "clock.h"
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types
typedef struct
{
  char     name[10]; // Name of preset
  uint16_t light;    // light level in ...
  uint16_t duration; // Duration in milliseconds
} preset_struct_t;

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  lcd1602_init();

  // enter your code here
  run_monitoring_system();

  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */

// -----------------------------------------------------------------------------
// DESCRIPTION
//    This function writes a string to the Serial terminal
//
//
// INPUT PARAMETERS:
//    string    - A pointer (address) to the null-terminated string to be
//                written to the serial terminal
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void UART_write_string(const char *string)
{
  // for each character in string, write it to the UART module
  while (*string != '\0')
  {
    UART_out_char(*string++);
  } /* while */

} /* UART_write_string */

