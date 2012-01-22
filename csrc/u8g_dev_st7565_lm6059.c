/*

  u8g_dev_st7565_lm6059.c (Adafruit display)

  Universal 8bit Graphics Library
  
  Copyright (c) 2011, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
  
*/

#include "u8g.h"

#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8

/* init sequence from https://github.com/adafruit/ST7565-LCD/blob/master/ST7565/ST7565.cpp */
u8g_pgm_uint8_t u8g_dev_st7565_lm6059_init_seq[] = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  U8G_ESC_RST(15),           /* do reset low pulse with (15*16)+2 milliseconds (=maximum delay)*/

  0x0a2,		                /* 0x0a2: LCD bias 1/9 (suggested for the LM6063), 0x0a3: Used by Adafruit */
  /* the LM6059 vs LM6063, ADC and SHL have inverted settings */
  0x0a0,		                /* 0x0a1: ADC set to normal (suggested for the LM6059), 0x0a0: Used by Adafruit -> normal mode */
  0x0c8,                            /* common output mode: set scan direction normal operation/SHL Select, 0x0c0 --> SHL = 0, normal, 0x0c8 --> SHL = 1 */
  0x040,		                /* set display start line */
  
  0x028 | 0x04,                 /* power control: turn on voltage converter */
  U8G_ESC_DLY(50),         /* delay 50 ms */

  0x028 | 0x06,                 /* power control: turn on voltage regulator */
  U8G_ESC_DLY(50),         /* delay 50 ms */
  
  0x028 | 0x07,                 /* power control: turn on voltage follower */
  U8G_ESC_DLY(50),         /* delay 50 ms */

  0x026,		                /* set V0 voltage resistor ratio to 6 (Adafruit Value, no info from LM6063 Manual) */
  
  0x0a6,                           /* display normal, bit val 0: LCD pixel off. */
  
  0x081,		                /* set contrast */
  0x018,		                /* contrast value*/
  
  /*0x0ac,*/		                /* indicator */
  /*0x000,*/		                /* disable */
  
  0x0af,		                /* display on */

  U8G_ESC_DLY(100),       /* delay 100 ms */
  0x0a5,		                /* display all points, ST7565 */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  0x0a4,		                /* normal display */
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

u8g_pgm_uint8_t u8g_dev_st7565_lm6059_data_start[] = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x010,		/* set upper 4 bit of the col adr to 0 */
  0x000,		/* set lower 4 bit of the col adr to 0 */      
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_st7565_lm6059_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_st7565_lm6059_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_st7565_lm6059_data_start);    
        u8g_WriteByte(u8g, dev, 0x0b0 | pb->p.page); /* select current page (ST7565R) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
          return 0;
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_st7565_lm6059_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_st7565_lm6059_fn, u8g_com_arduino_sw_spi_fn);
U8G_PB_DEV(u8g_dev_st7565_lm6059_hw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_st7565_lm6059_fn, u8g_com_arduino_hw_spi_fn);


