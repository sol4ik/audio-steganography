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
/** \file uartdraw.h
 **
 ** A detailed description is available at 
 ** @link UartDrawGroup Functions to draw on Uart Terminal description @endlink
 **
 ** History:
 **   - 2015-17-02  V1.0  Jan Weber  First Version
 *****************************************************************************/

#ifndef __UARTDRAW_H__
#define __UARTDRAW_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UartDrawGroup Functions to draw on Uart Terminal
 **
 ** Provided functions of Functions to draw on Uart Terminal:
 ** 
 ** - UartDraw_Init()
 ** - UartDraw_Deinit()
 ** - UartDraw_reset(void);
 ** - UartDraw_setCursor(uint8_t u8x, uint8_t u8y);
 ** - UartDraw_setBGColor(en_BGColor_t enBGColor);
 ** - UartDraw_setTxtColor(en_TxtColor_t enTxtColor);
 ** - UartDraw_setTxtAttr(en_TxtAttr_t enTxtAttr);
 ** - UartDraw_setAttribute(uint8_t u8Attr);
 ** - UartDraw_printTxtToPos(char_t* pcTxt, en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, boolean_t bDoubleLine, uint8_t u8XPos, uint8_t u8YPos);
 ** - UartDraw_printTxtColor(char_t* pcTxt, en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, boolean_t bDoubleLine);
 ** - UartDraw_printTxt(char_t* pcTxt, boolean_t bDoubleLine);
 ** - UartDraw_printRect(en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, uint8_t u8XPos, uint8_t u8YPos, uint8_t u8SizeHor, uint8_t u8SizeVer, boolean_t bEdges);
 ** - UartDraw_printLine(en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, uint8_t u8XPos, uint8_t u8YPos, uint8_t u8Length, en_LineDirection_t enLineDirection, en_LineEdges_t enLineEdges);
 **   
 ******************************************************************************/
//@{
    
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "base_types.h"
#include "mcu.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
#define POOR_MAN_BORDERS 0 
  
#define ACTIVATE_LINE_MODE              "\033(0"
#define DEACTIVATE_LINE_MODE            "\033(B"
#define RESET                           "\033[2J\033[0;0H"
#define SET_ATTR                        "\033[%um"
#define SET_COLOR                       "\033[%u;%uH"
  
#if (POOR_MAN_BORDERS == 1)
  
#define DIAMOND                         ""
#define CHESSBOARD                      ""
#define HT                              ""
#define FF                              ""
#define CR                              ""      
#define LF                              ""
#define DEGREE                          ""
#define PLUSMINUS                       "+/-"
#define NL                              ""      
#define VT                              ""
#define LINE_EDGE_RIGHT_BOTTOM          "+"
#define LINE_EDGE_RIGHT_TOP             "+"
#define LINE_EDGE_LEFT_TOP              "+"
#define LINE_EDGE_LEFT_BOTTOM           "+"
#define LINE_CROSS                      "+"
#define LINE_HORIZONTAL_TOP             "-"
#define LINE_HORIZONTAL_HALF_TOP        "-"
#define LINE_HORIZONTAL_CENTER          "-"
#define LINE_HORIZONTAL_HALF_BOTTOM     "-"
#define LINE_HORIZONTAL_BOTTOM          "-"
#define LINE_BRANCH_LEFT                "+"
#define LINE_BRANCH_RIGHT               "+"
#define LINE_BRANCH_TOP                 "+"
#define LINE_BRANCH_BOTTOM              "+"
#define LINE_VERTICAL                   "|" 
#define SMALLER_EQUAL                   "<="
#define GREATER_EQUAL                   ">="
//#define PI                              "{"  
#define UNEQUAL                         "!="  
#define POUND                           ""   
#define DOT                             "*"   
  
#else //(POOR_MAN_BORDERS == 1)
  
#define DIAMOND                         "`"
#define CHESSBOARD                      "a"
#define HT                              "b"
#define FF                              "c"
#define CR                              "d"      
#define LF                              "e"
#define DEGREE                          "f"
#define PLUSMINUS                       "g"
#define NL                              "h"      
#define VT                              "i"
#define LINE_EDGE_RIGHT_BOTTOM          "j"
#define LINE_EDGE_RIGHT_TOP             "k"
#define LINE_EDGE_LEFT_TOP              "l"
#define LINE_EDGE_LEFT_BOTTOM           "m"
#define LINE_CROSS                      "n"
#define LINE_HORIZONTAL_TOP             "o"
#define LINE_HORIZONTAL_HALF_TOP        "p"
#define LINE_HORIZONTAL_CENTER          "q"
#define LINE_HORIZONTAL_HALF_BOTTOM     "r"
#define LINE_HORIZONTAL_BOTTOM          "s"
#define LINE_BRANCH_LEFT                "t"
#define LINE_BRANCH_RIGHT               "u"
#define LINE_BRANCH_TOP                 "v"
#define LINE_BRANCH_BOTTOM              "w"
#define LINE_VERTICAL                   "x" 
#define SMALLER_EQUAL                   "y"
#define GREATER_EQUAL                   "z"
//#define PI                              "{"  
#define UNEQUAL                         "|"  
#define POUND                           "}"   
#define DOT                             "~"   
  
#endif // (POOR_MAN_BORDERS == 1)
  
/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
typedef enum en_BGColor
{
  BG_BLACK  = 40u,
  BG_RED    = 41u,
  BG_GREEN  = 42u,
  BG_BROWN  = 43u,
  BG_BLUE   = 44u,
  BG_PURPLE = 45u,
  BG_CYAN   = 46u,
  BG_WHITE  = 47u  
} en_BGColor_t;

typedef enum en_TxtColor
{
  TXT_BLACK  = 30u,
  TXT_RED    = 31u,
  TXT_GREEN  = 32u,
  TXT_BROWN  = 33u,
  TXT_BLUE   = 34u,
  TXT_PURPLE = 35u,
  TXT_CYAN   = 36u,
  TXT_WHITE  = 37u
} en_TxtColor_t;

typedef enum en_TxtAttr
{
  ALL_OFF        =  0u,
  BOLD_ON        =  1u,
  UNDERLINED_ON  =  4u,
  BLINK_ON       =  5u,
  INVERTED_ON    =  7u,
  INVISIBLE_ON   =  8u,
  BOLD_OFF       = 22u,
  UNDERLINED_OFF = 24u,
  BLINK_OFF      = 25u,
  INVERTED_OFF   = 27u,
  INVISIBLE_OFF  = 28u
} en_TxtAttr_t;

typedef enum en_LineEdges
{
  TOP,
  CENTER,
  BOTTOM,
  LEFT,
  RIGHT,
  NONE
} en_LineEdges_t;

typedef enum en_LineDirection
{
  HORIZONTAL,
  VERTICAL
} en_LineDirection_t;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
extern void UartDraw_Init(void);
extern void UartDraw_Deinit(void);
extern void UartDraw_reset(void);
extern void UartDraw_setCursor(uint8_t u8x, uint8_t u8y);
extern void UartDraw_setBGColor(en_BGColor_t enBGColor);
extern void UartDraw_setTxtColor(en_TxtColor_t enTxtColor);
extern void UartDraw_setTxtAttr(en_TxtAttr_t enTxtAttr);
extern void UartDraw_setAttribute(uint8_t u8Attr);
extern void UartDraw_printTxtToPos(char_t* pcTxt, en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, boolean_t bDoubleLine, uint8_t u8XPos, uint8_t u8YPos);
extern void UartDraw_printTxtColor(char_t* pcTxt, en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, boolean_t bDoubleLine);
extern void UartDraw_printTxt(char_t* pcTxt, boolean_t bDoubleLine);
extern void UartDraw_printRect(en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, uint8_t u8XPos, uint8_t u8YPos, uint8_t u8SizeHor, uint8_t u8SizeVer, boolean_t bEdges);
extern void UartDraw_printLine(en_BGColor_t enBGColor, en_TxtColor_t enTxtColor, uint8_t u8XPos, uint8_t u8YPos, uint8_t u8Length, en_LineDirection_t enLineDirection, en_LineEdges_t enLineEdges);

#ifdef __cplusplus
}
#endif

//@} // UartDrawGroup

#endif /* __UARTDRAW_H__*/

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

