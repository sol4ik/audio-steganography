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
/** \file uartdraw.c
 **
 ** A detailed description is available at 
 ** @link UartDrawGroup Functions to draw on Uart Terminal description @endlink
 **
 ** History:
 **   - 2015-17-02  V1.0  Jan Weber  First Version
 *****************************************************************************/
#define __UARTDRAW_C__
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "uartdraw.h"
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
void UartDraw_Init(void)
{
  Uart_Init();
}

/**
 ******************************************************************************
 ** \brief Deinitialization Routine
 **
 ******************************************************************************/
void UartDraw_Deinit(void)
{
  /* nothing to do here */
}

/**
 ******************************************************************************
 ** \brief Clears the UART Terminal
 **
 ******************************************************************************/
void UartDraw_reset(void)
{
  printf(RESET);
}

/**
 ******************************************************************************
 ** \brief Sets the cursor to the given position
 **
 ** \param  [in] u8x      X coordinate.
 ** \param  [in] u8y      Y coordinate.
 **
 ******************************************************************************/
void UartDraw_setCursor(uint8_t u8x, uint8_t u8y) 
{
  printf(SET_COLOR, u8y, u8x);
}

/**
 ******************************************************************************
 ** \brief Sets the Background color to the given value
 **
 ** \param  [in] enBGColor      Background color.
 **
 ******************************************************************************/
void UartDraw_setBGColor(en_BGColor_t enBGColor) 
{
  UartDraw_setAttribute((uint8_t) enBGColor);
}

/**
 ******************************************************************************
 ** \brief Sets the Foreground color to the given value
 **
 ** \param  [in] enTxtColor      Foreground color.
 **
 ******************************************************************************/
void UartDraw_setTxtColor(en_TxtColor_t enTxtColor)
{
   UartDraw_setAttribute((uint8_t)enTxtColor); 
}

/**
 ******************************************************************************
 ** \brief Sets the given Text Attribute (like bold, underlined ...)
 **
 ** \param  [in] enTxtAttr      Attribute.
 **
 ******************************************************************************/
void UartDraw_setTxtAttr(en_TxtAttr_t enTxtAttr)
{
   UartDraw_setAttribute((uint8_t)enTxtAttr); 
}

/**
 ******************************************************************************
 ** \brief Sends the given Attribute
 **
 ** \param  [in] u8Attr      Attribute.
 **
 ******************************************************************************/
void UartDraw_setAttribute(uint8_t u8Attr)
{
  printf(SET_ATTR, u8Attr);
}

/**
 ******************************************************************************
 ** \brief Prints the given text with the given appearance.
 **
 ** \param  [in] pcTxt       Text that is printed.
 ** \param  [in] enBGColor   The texts background color.
 ** \param  [in] enTxtColor  The texts color.
 ** \param  [in] bDoubleLine Print in two lines.
 ** \param  [in] u8XPos      X coordinate.
 ** \param  [in] u8YPos      Y coordinate.
 **
 ******************************************************************************/
void UartDraw_printTxtToPos(char_t* pcTxt, en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, boolean_t bDoubleLine, uint8_t u8XPos, uint8_t u8YPos)
{
  UartDraw_setCursor(u8XPos, u8YPos);
  UartDraw_printTxtColor(pcTxt, enBGColor, enTxtColor, bDoubleLine);
}

/**
 ******************************************************************************
 ** \brief Prints the given text with the given appearance.
 **
 ** \param  [in] pcTxt       Text that is printed.
 ** \param  [in] enBGColor   The texts background color.
 ** \param  [in] enTxtColor  The texts color.
 ** \param  [in] bDoubleLine Print in two lines.
 **
 ******************************************************************************/
void UartDraw_printTxtColor(char_t* pcTxt, en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, boolean_t bDoubleLine)
{
  UartDraw_setTxtColor(enTxtColor);
  UartDraw_setBGColor(enBGColor);
  UartDraw_printTxt(pcTxt, bDoubleLine);
}

/**
 ******************************************************************************
 ** \brief Prints the given text with the given appearance.
 **
 ** \param  [in] pcTxt       Text that is printed.
 ** \param  [in] enBGColor   The texts background color.
 ** \param  [in] bDoubleLine Print in two lines.
 ******************************************************************************/
void UartDraw_printTxt(char_t* pcTxt, boolean_t bDoubleLine)
{
  if(TRUE == bDoubleLine)
  {
    printf("\033#3%s\r\n\033#4%s", pcTxt, pcTxt);
  }
  else
  {
    printf(pcTxt);
  }
}

/**
 ******************************************************************************
 ** \brief Prints a box with given appearance.
 **
 ** \param  [in] enBGColor   The background color.
 ** \param  [in] enTxtColor  The foreground color.
 ** \param  [in] u8XPos      X coordinate.
 ** \param  [in] u8YPos      Y coordinate.
 ** \param  [in] u8SizeHor   Horizontal size.
 ** \param  [in] u8SizeVer   Vertical size.
 ** \param  [in] bEdges      Draw edge lines.
 **
 ******************************************************************************/
void UartDraw_printRect(en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, uint8_t u8XPos, uint8_t u8YPos, uint8_t u8SizeHor, uint8_t u8SizeVer, boolean_t bEdges)
{
  char_t acSpace[256u];
  uint8_t u8CurPos;
  
  for(u8CurPos = 0u; u8CurPos < u8SizeHor; u8CurPos++)
  {
    acSpace[u8CurPos] = ' ';
  }
  acSpace[u8CurPos] = '\0';
 
  printf(ACTIVATE_LINE_MODE);
  UartDraw_setCursor(u8XPos, u8YPos);
  UartDraw_setTxtColor(enTxtColor);
  UartDraw_setBGColor(enBGColor);
  
  for(u8CurPos = 0u; u8CurPos < u8SizeVer; u8CurPos++)
  {
    UartDraw_setCursor(u8XPos, u8YPos + u8CurPos);
    printf(acSpace);
  } 
  
  if(TRUE == bEdges)
  {
    UartDraw_printLine(enBGColor, enTxtColor,  u8XPos,  u8YPos,  u8SizeHor, HORIZONTAL, TOP);
    UartDraw_printLine(enBGColor, enTxtColor,  u8XPos,  u8YPos + u8SizeVer, u8SizeHor, HORIZONTAL, BOTTOM);   
    UartDraw_printLine(enBGColor, enTxtColor,  u8XPos,  u8YPos,  u8SizeVer, VERTICAL, LEFT);   
    UartDraw_printLine(enBGColor, enTxtColor,  u8XPos + u8SizeHor,  u8YPos,  u8SizeVer, VERTICAL, RIGHT);       
  }

  printf(DEACTIVATE_LINE_MODE);
}

/**
 ******************************************************************************
 ** \brief Prints a line with given appearance.
 **
 ** \param  [in] enBGColor              The background color.
 ** \param  [in] enTxtColor             The foreground color.
 ** \param  [in] u8XPos                 X coordinate.
 ** \param  [in] u8YPos                 Y coordinate.
 ** \param  [in] u8Length               Line length.
** \param   [in] enLineDirection        Line direction: HORIZONTAL, VERTICAL.
 ** \param  [in] enLineEdges            Directions of the corners (to form a box).
 **
 ******************************************************************************/
void UartDraw_printLine(en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, uint8_t u8XPos, uint8_t u8YPos, uint8_t u8Length, en_LineDirection_t enLineDirection, en_LineEdges_t enLineEdges)
{
  uint8_t u8CurPos;
  
  UartDraw_setTxtColor(enTxtColor);
  UartDraw_setBGColor(enBGColor);
  printf(ACTIVATE_LINE_MODE);
  
  if(HORIZONTAL == enLineDirection)
  {
    for(u8CurPos = 0u; u8CurPos < u8Length; u8CurPos++)
    {
      UartDraw_setCursor(u8XPos + u8CurPos, u8YPos);
      if(TOP == enLineEdges)
      {
        printf(ACTIVATE_LINE_MODE LINE_HORIZONTAL_TOP);
      }
      else if (BOTTOM == enLineEdges)
      {
        printf(ACTIVATE_LINE_MODE LINE_HORIZONTAL_BOTTOM);        
      }
      else
      {
        printf(LINE_HORIZONTAL_CENTER);
      }
    }
    
    if(TOP == enLineEdges)
    {
      UartDraw_setCursor(u8XPos, u8YPos);
      printf(LINE_EDGE_LEFT_TOP);
      UartDraw_setCursor(u8XPos + u8Length, u8YPos);
      printf(LINE_EDGE_RIGHT_TOP);      
    }
    else if(CENTER == enLineEdges)
    {
      UartDraw_setCursor(u8XPos, u8YPos);
      printf(LINE_BRANCH_LEFT);
      UartDraw_setCursor(u8XPos + u8Length, u8YPos);
      printf(LINE_BRANCH_RIGHT);      
    }
    else if(BOTTOM == enLineEdges)
    {
      UartDraw_setCursor(u8XPos, u8YPos);
      printf(LINE_EDGE_LEFT_BOTTOM);
      UartDraw_setCursor(u8XPos + u8Length, u8YPos);
      printf(LINE_EDGE_RIGHT_BOTTOM);      
    }
  }
  else if (VERTICAL == enLineDirection)
  {
    for(u8CurPos = 0u; u8CurPos < u8Length; u8CurPos++)
    {
      UartDraw_setCursor(u8XPos, u8YPos + u8CurPos);
      printf(LINE_VERTICAL);
    }
    
    if(LEFT == enLineEdges)
    {
      UartDraw_setCursor(u8XPos, u8YPos);
      printf(LINE_EDGE_LEFT_TOP);
      UartDraw_setCursor(u8XPos, u8YPos + u8Length);
      printf(LINE_EDGE_LEFT_BOTTOM);      
    }
    else if(CENTER == enLineEdges)
    {
      UartDraw_setCursor(u8XPos, u8YPos);
      printf(LINE_BRANCH_LEFT);
      UartDraw_setCursor(u8XPos, u8YPos + u8Length);
      printf(LINE_BRANCH_RIGHT);      
    }
    else if(RIGHT == enLineEdges)
    {
      UartDraw_setCursor(u8XPos, u8YPos);
      printf(LINE_EDGE_RIGHT_TOP);
      UartDraw_setCursor(u8XPos, u8YPos + u8Length);
      printf(LINE_EDGE_RIGHT_BOTTOM);      
    }
  }
  
  printf(DEACTIVATE_LINE_MODE);
}

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
