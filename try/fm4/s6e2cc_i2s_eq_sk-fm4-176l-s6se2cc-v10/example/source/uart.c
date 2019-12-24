/*******************************************************************************
* Copyright (C) 2015 Spansion LLC. All Rights Reserved. 
*
* This software is owned and published by: 
* Spansion LLC, 915 DeGuigne Dr. Sunnyvale, CA  94088-3453 ("Spansion").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with Spansion 
* components. This software is licensed by Spansion to be adapted only 
* for use in systems utilizing Spansion components. Spansion shall not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein.  Spansion is providing this software "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the software.  
*
* SPANSION MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS), 
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING, 
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED 
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED 
* WARRANTY OF NONINFRINGEMENT.  
* SPANSION SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, 
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT 
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, 
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR 
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, 
* SAVINGS OR PROFITS, 
* EVEN IF SPANSION HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED 
* FROM, THE SOFTWARE.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Disclaimer and Copyright notice must be 
* included with each copy of this software, whether used in part or whole, 
* at all times.  
*/
/******************************************************************************/
/** \file uart.c
 **
 ** A detailed description is available at 
 ** @link UartGroup This file provides basic uart functions description @endlink
 **
 ** History:
 **   - 2015-13-02  V1.0  Jan Weber  First Version
 *****************************************************************************/
#define __UART_C__
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "mcu.h"
#include "uart.h"
#include <stdio.h>

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** \brief Initialization Routine
 **
 ******************************************************************************/
void Uart_Init(void)
{
  // Initialize ports for MFS0
  FM4_GPIO->PFR2   = 0x06u;                     // P21>SIN0_0, P22>SOT0_0
  FM4_GPIO->EPFR07 &= 0xFFFFFF0Ful;
  FM4_GPIO->EPFR07 |= 0x00000040ul;
  FM4_GPIO->ADE &= ~(1ul << 31ul);              // switch off AN31 for SOT0_0
  
  // Initialize MFS to UART asynchronous mode
  FM4_MFS0->SMR =  0x01u;                       // serial data output enable, operation mode 0
  FM4_MFS0->SCR =  0x80u;                       // MFS reset
  FM4_MFS0->SSR =  0x00u;                       // 0x80 = clear receive error flag
  FM4_MFS0->ESCR = 0x00u;     
  
  FM4_MFS0->FCR1 = 0x00u;                       //
  FM4_MFS0->FCR0 = 0x0Au;                       // Enable Receive FIFO
 
  FM4_MFS0->BGR = 100000000ul / 115200ul - 1ul; // 115.2kbaud @ 100 MHz
 
  FM4_MFS0->SCR |= 0x03u;                       // RX, TX enable  
}

/**
 ******************************************************************************
 ** \brief Deinitialization Routine
 **
 ******************************************************************************/
void Uart_Deinit(void)
{
  /* Nothing to do here */
}

/**
**************************************************************************
** \brief Sends a single character via UART0. Blocks until the character
** was sent
**
** \param  u8Char The character to be sent
** \return nothing
**************************************************************************/
void Uart_PutCharUart0(const uint8_t u8Char)
{
  while (!(FM4_MFS0->SSR & (1ul << 1ul)));   // wait for transmit buffer empty 
  FM4_MFS0->TDR = u8Char;                   // put u8Char into buffer
  
  if ('\n' == u8Char)
  {
    while (!(FM4_MFS0->SSR & (1ul << 1ul))); // wait for transmit buffer empty 
    FM4_MFS0->TDR = '\r';                   // put '\r' into buffer
  }
}

/**
**************************************************************************
** \brief  Writes a \0 terminated string to UART0. Blocks until the entire
** string was written.
**
** \param  none
** \return nothing
**************************************************************************/
void Uart_PutStringUart0(const uint8_t *pu8Buffer)              
{
  while (*pu8Buffer != '\0')
  { 
    Uart_PutCharUart0(*pu8Buffer++);        // send every char of string
  }
}

/**
**************************************************************************
** \brief  Reads a single character from UART0. Non-blocking.
**
** \param  none
** \return The received character, 0x00 -> nothing received, 0xFF -> error
**************************************************************************/
uint8_t Uart_GetCharUart0(void)   
{
  uint8_t u8Char;
  
  if ((FM4_MFS0->SSR & 0x38u) != 0u)  // Check for errors PE, ORE, FRE
  {
    FM4_MFS0->SSR |= (1ul << 7u);      // Clear error flags 
    u8Char = FM4_MFS0->RDR;         // Flush RDR
    return 0xFFu;
  }
  else if (FM4_MFS0->FBYTE2 > 0u)
  {
    u8Char = FM4_MFS0->RDR;
    
    if ('\r' == u8Char)
    {
      u8Char = '\n';
    }
    return (u8Char);
    
  }
  else
  {
    return 0u;
  }
}

/**
******************************************************************************
** \brief  Overloads system function to let printf() write to UART0
**
** \param  handle  Standard interface
** \param  pu8Buf  Print buffer
** \param  u8Size  Print buffer size
** \return u8Size  Size of actual print
******************************************************************************/
uint8_t __write(int handle, const unsigned char *pu8Buf, uint8_t u8Size)
{
  int32_t i32CharToWrite = 0u;
  for(i32CharToWrite = 0u; i32CharToWrite < u8Size; i32CharToWrite++)
  {
    while (!(FM4_MFS0->SSR & (1ul << 1ul)))
      ;                                     // wait for transmit buffer to be empty
	  FM4_MFS0->RDR = pu8Buf[i32CharToWrite];  // put ch into tx buffer
  }
  return u8Size;
}


/* *
******************************************************************************
** \brief  some systems need __putchar() to be overloaded instead of __write()
** to enable printf() to output strings to UART0. In that case, you can use this code.
**
** \param  standard interface
** \return size
****************************************************************************** /
int __putchar(char ch)
{
   while (!(FM4_MFS0->SSR & (1u << 1ul)))
      ;                                     // wait for transmit buffer to be empty
	  FM4_MFS0->RDR = ch;                // put ch into tx buffer
    return ch;
}
*/

/**
******************************************************************************
** \brief  Overloads system function to let scanf() read from UART0
**
** \param  handle   Standard interface
** \param  pu8Buf   Print buffer
** \param  u8Size   Print buffer size
** \return u8Size   Size of actual print
******************************************************************************/
uint8_t __read(int handle, unsigned char *pu8Buf, uint8_t u8Size)
{
  uint8_t u8Index;
  for (u8Index = 0u; u8Index < u8Size; ++u8Index)
  {
    while(!(FM4_MFS0->SSR & (1ul << 2ul)));			/* wait for data in buffer */
      pu8Buf[u8Index] = FM4_MFS0->RDR;
  }
  return u8Size;
}

#if !__CC_ARM
/**
******************************************************************************
** \brief  Nonblocking getchar()
**
** \return u8Char   input character, 0 if nothing was input
******************************************************************************/
uint8_t getch(void)
{
  uint8_t cInputChar = 0u;
  if (FM4_MFS0->SSR & (1ul << 2ul))
  {
    cInputChar = getchar();
  }
  return cInputChar;
}
#endif

#if  __CC_ARM
// If Keil uVision is used with standard library files like stdio.h and strings.h,
// a special feature by the name of "semihosting" must be considered.
// See http://infocenter.arm.com/help/topic/com.arm.doc.dui0349c/Ciheeaja.html for more information.
// This example is based on C:\Keil\ARM\Examples\Hello
#pragma import(__use_no_semihosting)

typedef struct __FILE {int handle;} FILE;
FILE __stdout;

int fputc(int ch, FILE *f) {
  return (__write(0u, (const unsigned char*)&ch, 1u));
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return 0u;
}

void _ttywrch(int ch) {
  __write(0u, (const unsigned char*)&ch, 1u);
}

void _sys_exit(int return_code) {
	while(1u)  /* endless loop */
        {
        }
}

#endif  //__CC_ARM

//@} // UartGroup

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
