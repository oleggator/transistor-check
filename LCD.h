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
//--------------------------------------------------------------------------------
//  K. Chen
//  Texas Instruments Inc.
//  Ver 2.00 - March 2012
//  Built with IAR Embedded Workbench Version: 5.40.3 & Code Composer Studio V5.1
//--------------------------------------------------------------------------------

#ifndef BOARD_LCD_H
#define BOARD_LCD_H
#include  <msp430xG46x.h>

typedef unsigned char UInt8;

// LCD Constants
#define LCD_NUM_DIGITS  7                   // Number of digits on LCD
#define LCD_MEM_OFFSET  2                   // Offset from LCDMEM[0]
#define LCD_MEM_LOC     11                  // Num of LCDMEM[] locations used

// LCD Segments
#define LCD_A    BIT0
#define LCD_B    BIT1
#define LCD_C    BIT2
#define LCD_D    BIT3
#define LCD_E    BIT6
#define LCD_F    BIT4
#define LCD_G    BIT5
#define LCD_H    BIT7


// Display Symbol
#define LCD_SYM_DOL      0x01
#define LCD_SYM_ERR      0x02
#define LCD_SYM_MINUS    0x04
#define LCD_SYM_MEM      0x08
#define LCD_SYM_ENV      0x10
#define LCD_SYM_TX       0x20
#define LCD_SYM_RX       0x40
#define LCD_SYM_OFF      0x80

extern void dispSymbol(UInt8 mask);


//
// Initialize LCD
//
extern void initLCD_A(void);


//
// Clear LCD display
//
extern void clrLCD(void);

//
// Display character on LCD
//
//   pos - character position on LCD
//   index - index into LCD_Char_Map[] array
//
extern void dispChar(UInt8 pos, UInt8 index);

extern void displayPass(void);
extern void displayFail(void);
extern void displayWait(void);


extern const UInt8 LCD_MAX_CHARS;



#endif // BOARD_LCD_H
