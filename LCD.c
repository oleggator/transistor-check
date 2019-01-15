/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//---------------------------------------------------------------------------------
//  K. Chen
//  Texas Instruments Inc.
//  Ver 2.00 - March 2012
//  Built with IAR Embedded Workbench Version: 5.40.3 & Code Composer Studio V5.1
//---------------------------------------------------------------------------------

#include "LCD.h"


// LCD Segment Mapping
const UInt8 LCD_Char_Map[] =
{
  LCD_A+LCD_B+LCD_C+LCD_D+LCD_E+LCD_F,        // '0' or 'O'
  LCD_B+LCD_C,                                // '1' or 'I'
  LCD_A+LCD_B+LCD_D+LCD_E+LCD_G,              // '2' or 'Z'
  LCD_A+LCD_B+LCD_C+LCD_D+LCD_G,              // '3'
  LCD_B+LCD_C+LCD_F+LCD_G,                    // '4' or 'y'
  LCD_A+LCD_C+LCD_D+LCD_F+LCD_G,              // '5' or 'S'
  LCD_A+LCD_C+LCD_D+LCD_E+LCD_F+LCD_G,        // '6' or 'b'
  LCD_A+LCD_B+LCD_C,                          // '7'
  LCD_A+LCD_B+LCD_C+LCD_D+LCD_E+LCD_F+LCD_G,  // '8' or 'B'
  LCD_A+LCD_B+LCD_C+LCD_F+LCD_G,              // '9' or 'g'
  LCD_A+LCD_B+LCD_C+LCD_E+LCD_F+LCD_G,        // 'A'
  LCD_A+LCD_D+LCD_E+LCD_F,                    // 'C'
  LCD_B+LCD_C+LCD_D+LCD_E+LCD_G,              // 'd'
  LCD_A+LCD_D+LCD_E+LCD_F+LCD_G,              // 'E'
  LCD_A+LCD_E+LCD_F+LCD_G,                    // 'F'
  LCD_B+LCD_C+LCD_E+LCD_F+LCD_G,              // 'H'
  LCD_B+LCD_C+LCD_D+LCD_E,                    // 'J'
  LCD_D+LCD_E+LCD_F,                          // 'L'
  LCD_A+LCD_B+LCD_E+LCD_F+LCD_G,              // 'P'
  LCD_B+LCD_C+LCD_D+LCD_E+LCD_F,               // 'U'
  LCD_G
};

const UInt8 LCD_MAX_CHARS = (sizeof(LCD_Char_Map)/sizeof(UInt8));


//
// Initialize LCD_A
//
void initLCD_A(void)
{
  // Clear LCD memory
  clrLCD();

  // Configure COM0-COM3 and R03-R33 pins
  P5SEL |= (BIT4 | BIT3 | BIT2);
  P5DIR |= (BIT4 | BIT3 | BIT2);

  // Configure LCD_A
  LCDACTL = LCDFREQ_128 | LCDMX1 | LCDMX0 | LCDSON | LCDON;
  LCDAPCTL0 = LCDS4 | LCDS8 | LCDS12 | LCDS16 | LCDS20 | LCDS24;
  LCDAPCTL1 = 0;
  LCDAVCTL0 = LCDCPEN;
  LCDAVCTL1 = VLCD_2_60;
}


//
// Clear LCD
//
void clrLCD(void)
{
  int i;

  for(i = LCD_MEM_OFFSET; i < (LCD_MEM_OFFSET+LCD_MEM_LOC);  i++)
  {
    LCDMEM[i] = 0;
  }
}

//
// Display character on LCD
//
void dispChar(UInt8 pos, UInt8 index)
{
  LCDMEM[pos + LCD_MEM_OFFSET] &= ~LCD_Char_Map[8];

  if( pos < LCD_NUM_DIGITS )
  {
    if( index < LCD_MAX_CHARS )
    {
      LCDMEM[pos + LCD_MEM_OFFSET] |= LCD_Char_Map[index];
    }
  }
}

void displayPass(void) {
    dispChar(3, 18);    // P
    dispChar(2, 10);    // A
    dispChar(1, 5);     // S
    dispChar(0, 5);     // S
}

void displayFail(void) {
    dispChar(3, 14);    // F
    dispChar(2, 10);    // A
    dispChar(1, 1);     // I
    dispChar(0, 17);    // L
}

void displayWait(void) {
    dispChar(3, 20);    // -
    dispChar(2, 20);    // -
    dispChar(1, 20);    // -
    dispChar(0, 20);    // -
}
