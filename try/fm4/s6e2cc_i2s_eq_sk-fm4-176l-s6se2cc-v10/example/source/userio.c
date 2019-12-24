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
/** \file userio.c
 **
 ** A detailed description is available at 
 ** @link UserIOGroup Processes user inputs via UART description @endlink
 **
 ** History:
 **   - 2015-16-02  V1.0  Jan Weber  First Version
 *****************************************************************************/
#define __USERIO_C__
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "userio.h"
#include "uart.h"
#include "uartdraw.h"
#include "globals.h"
#if (IDLEMEASUREMENT_ON == 1)
#include "idlemeasurement.h"
#endif
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
#if (IDLEMEASUREMENT_ON == 1)
static uint8_t u8LastMCULoad;
#endif //(IDLEMEASUREMENT_ON == 1)
/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/
static void UserIO_rebuildScreen(stc_UserIO_t* pstcUserIO);  
static void UserIO_updateControlHighlight(stc_UserIO_t* pstcUserIO);
static void UserIO_updateControl(stc_UserIO_t* pstcUserIO);
/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** \brief Initialization Routine
 **
 ******************************************************************************/
void UserIO_Init(stc_UserIO_t* pstcUserIO)
{
  pstcUserIO->enSelectedEqRange = MIDRANGE;
  pstcUserIO->i8EqValues[BASS] = 0;
  pstcUserIO->i8EqValues[MIDRANGE] = 0;
  pstcUserIO->i8EqValues[TREBLE] = 0;
  pstcUserIO->bHasNewOutput = FALSE;
  pstcUserIO->bHasNewInput = FALSE;
  pstcUserIO->u8ESCSequenceDetect = 0u;
  UartDraw_Init();  
  UserIO_rebuildScreen(pstcUserIO);  
}

/**
 ******************************************************************************
 ** \brief Deinitialization Routine
 **
 ******************************************************************************/
void UserIO_Deinit(stc_UserIO_t* pstcUserIO)
{
  /* nothing to do here */
}

/**
 ******************************************************************************
 ** \brief Rebuilds the entire user interface, with the values stored in the
 **        given userio structure pstcUserIO
 **
 ** \param  [in] pstcUserIO      UserIO structure.
 **
 ******************************************************************************/
static void UserIO_rebuildScreen(stc_UserIO_t* pstcUserIO)
{ 
  UartDraw_setTxtAttr(ALL_OFF);
  UartDraw_setTxtColor(TXT_WHITE);
  UartDraw_setBGColor(BG_BLACK);
  UartDraw_reset();   
  UartDraw_printRect(USERIO_BG, USERIO_FG, USERIO_RECT_SIZE, FALSE);
  UartDraw_printRect(BG_WHITE, TXT_BLUE, USERIO_RECT_HEAD_SIZE, FALSE);
  UartDraw_printTxtToPos(USERIO_HEAD, BG_WHITE, TXT_BLUE, FALSE, USERIO_HEAD_POS);
  UartDraw_printTxtToPos(USERIO_FOOT1, USERIO_BG, USERIO_FG, FALSE, USERIO_FOOT1_POS);
  UartDraw_printTxtToPos(USERIO_FOOT2, USERIO_BG, USERIO_FG, FALSE, USERIO_FOOT2_POS);
  UartDraw_printTxtToPos(USERIO_FOOT3, USERIO_BG, USERIO_FG, FALSE, USERIO_FOOT3_POS); 
  UserIO_updateControl(pstcUserIO);
  UserIO_updateControlHighlight(pstcUserIO);
}

/**
 ******************************************************************************
 ** \brief Updates the control line in the user interface, with the values 
 **        stored in the given userio structure pstcUserIO        
 **
 ** \param  [in] pstcUserIO      UserIO structure.
 **
 ******************************************************************************/
static void UserIO_updateControl(stc_UserIO_t* pstcUserIO)
{
  char_t cSign1 = (pstcUserIO->i8EqValues[BASS] > 0)? USERIO_SIGN_POS : (pstcUserIO->i8EqValues[BASS] < 0) ? USERIO_SIGN_NEG : USERIO_SIGN_ZERO;
  char_t cSign2 = (pstcUserIO->i8EqValues[MIDRANGE] > 0)? USERIO_SIGN_POS : (pstcUserIO->i8EqValues[MIDRANGE] < 0) ? USERIO_SIGN_NEG : USERIO_SIGN_ZERO;
  char_t cSign3 = (pstcUserIO->i8EqValues[TREBLE] > 0)? USERIO_SIGN_POS : (pstcUserIO->i8EqValues[TREBLE] < 0) ? USERIO_SIGN_NEG : USERIO_SIGN_ZERO;
  
  uint8_t u8Val1 = (uint8_t)((pstcUserIO->i8EqValues[BASS] >= 0)? pstcUserIO->i8EqValues[BASS] : ((-1) * pstcUserIO->i8EqValues[BASS]));
  uint8_t u8Val2 = (uint8_t)((pstcUserIO->i8EqValues[MIDRANGE] >= 0)? pstcUserIO->i8EqValues[MIDRANGE] : ((-1) * pstcUserIO->i8EqValues[MIDRANGE]));
  uint8_t u8Val3 = (uint8_t)((pstcUserIO->i8EqValues[TREBLE] >= 0)? pstcUserIO->i8EqValues[TREBLE] : ((-1) * pstcUserIO->i8EqValues[TREBLE]));
  
  UartDraw_setCursor(USERIO_CAPTIONS_POS);
  UartDraw_setTxtColor(USERIO_FG);
  UartDraw_setBGColor(USERIO_BG);
  printf(USERIO_CAPTIONS, cSign1, u8Val1, cSign2, u8Val2, cSign3, u8Val3);
}

/**
 ******************************************************************************
 ** \brief Highlights the selected control in the user interface, with the values 
 **        stored in the given userio structure pstcUserIO      
 **
 ** \param  [in] pstcUserIO      UserIO structure.
 **
 ******************************************************************************/
static void UserIO_updateControlHighlight(stc_UserIO_t* pstcUserIO)
{
  char_t acSign[2u] = "-\0"; 
  uint8_t u8Val;

  switch(pstcUserIO->enSelectedEqRange)
  {
  case BASS:
    acSign[0] = (pstcUserIO->i8EqValues[BASS] > 0) ? USERIO_SIGN_POS : (pstcUserIO->i8EqValues[BASS] < 0) ? USERIO_SIGN_NEG : USERIO_SIGN_ZERO;
    u8Val = (uint8_t)((pstcUserIO->i8EqValues[BASS] >= 0)? pstcUserIO->i8EqValues[BASS] : ((-1) * pstcUserIO->i8EqValues[BASS]));
    UartDraw_printTxtToPos(acSign, USERIO_BG_SEL, USERIO_FG_SEL, FALSE, USERIO_SIGN1_POS);
    UartDraw_setCursor(USERIO_VAL1_POS);
    printf("%u", u8Val);
    break;
  case MIDRANGE:
    acSign[0] = (pstcUserIO->i8EqValues[MIDRANGE] > 0) ? USERIO_SIGN_POS : (pstcUserIO->i8EqValues[MIDRANGE] < 0) ? USERIO_SIGN_NEG : USERIO_SIGN_ZERO;
    u8Val = (uint8_t)((pstcUserIO->i8EqValues[MIDRANGE] >= 0)? pstcUserIO->i8EqValues[MIDRANGE] : ((-1) * pstcUserIO->i8EqValues[MIDRANGE]));
    UartDraw_printTxtToPos(acSign, USERIO_BG_SEL, USERIO_FG_SEL, FALSE, USERIO_SIGN2_POS);
    UartDraw_setCursor(USERIO_VAL2_POS);
    printf("%u", u8Val); 
    break;
  case TREBLE:
    acSign[0] = (pstcUserIO->i8EqValues[TREBLE] > 0) ? USERIO_SIGN_POS : (pstcUserIO->i8EqValues[TREBLE] < 0) ? USERIO_SIGN_NEG : USERIO_SIGN_ZERO;
    u8Val = (uint8_t)((pstcUserIO->i8EqValues[TREBLE] >= 0)? pstcUserIO->i8EqValues[TREBLE] : ((-1) * pstcUserIO->i8EqValues[TREBLE]));
    UartDraw_printTxtToPos(acSign, USERIO_BG_SEL, USERIO_FG_SEL, FALSE, USERIO_SIGN3_POS);
    UartDraw_setCursor(USERIO_VAL3_POS);
    printf("%u", u8Val);
    break;
  } 
  
  UartDraw_setCursor(USERIO_CURSOR_PARKING);
}

/**
 ******************************************************************************
 ** \brief Checks whether there is a new input value.        
 **
 ** \param  [in][out] pstcUserIO      UserIO structure.
 **
 ** \return boolean_t  TRUE - new input value / FALSE - no new input value
 **
 ******************************************************************************/
boolean_t UserIO_hasNewInput(stc_UserIO_t* pstcUserIO)
{
    char_t cNewChar;
		#if (IDLEMEASUREMENT_ON == 1)
    uint8_t u8NewMCULoad;    
    
    if(u8LastMCULoad != (u8NewMCULoad = IdleMeasurement_GetCPULoad())) 
    {
      u8LastMCULoad = u8NewMCULoad;
      UartDraw_setCursor(USERIO_MCU_LOAD_POS);
      printf("MCU load: %3u%%     \0", u8LastMCULoad);
      UartDraw_setCursor(USERIO_CURSOR_PARKING);
    }
    #endif    
    
    while(0u != (cNewChar = Uart_GetCharUart0()))
    {
      if('\033' == cNewChar)
      {
        pstcUserIO->u8ESCSequenceDetect = 1u;
      }
      else if(1u == pstcUserIO->u8ESCSequenceDetect)
      {
        if('[' == cNewChar) 
        {
          pstcUserIO->u8ESCSequenceDetect = 2u;
        }
        else
        {
          pstcUserIO->u8ESCSequenceDetect = 0u;
        }
      }
      else if(2u == pstcUserIO->u8ESCSequenceDetect)
      {
        pstcUserIO->u8ESCSequenceDetect = 0u;
        
        switch(cNewChar)
        {
        case 'A':
          pstcUserIO->i8NewValue++;
          pstcUserIO->bHasNewInput = TRUE;
          break;
        case 'B':
          pstcUserIO->i8NewValue--;
          pstcUserIO->bHasNewInput = TRUE;
          break;
        case 'C':
          pstcUserIO->enSelectedEqRange++;
          if(pstcUserIO->enSelectedEqRange > TREBLE)
          {
            pstcUserIO->enSelectedEqRange = TREBLE;
          }
          pstcUserIO->i8NewValue = pstcUserIO->i8EqValues[pstcUserIO->enSelectedEqRange];
          UserIO_updateControl(pstcUserIO);
          UserIO_updateControlHighlight(pstcUserIO);
          break;
        case 'D':
          pstcUserIO->enSelectedEqRange--;
          if(pstcUserIO->enSelectedEqRange == 0xFFu)
          {
            pstcUserIO->enSelectedEqRange = BASS;
          }
          pstcUserIO->i8NewValue = pstcUserIO->i8EqValues[pstcUserIO->enSelectedEqRange];
          UserIO_updateControl(pstcUserIO);
          UserIO_updateControlHighlight(pstcUserIO);
          break;
        }
      }      
    }
    return pstcUserIO->bHasNewInput;
}

/**
 ******************************************************************************
 ** \brief Returns the new input value, if present.        
 **
 ** \param  [in][out]  pstcUserIO   UserIO structure.
 ** \param  [out]      enEqRange    To store the modified range.
 ** \param  [out]      i8Value      To store the value which the range is changed to.
 **
 ** \return boolean_t  TRUE - returned values are valid/ FALSE - returned values 
 **                    are invalid
 **
 ******************************************************************************/
boolean_t UserIO_getNewInput(stc_UserIO_t* pstcUserIO, en_EqualizerRange_t* enEqRange, int8_t* i8Value)
{
  if(pstcUserIO->bHasNewInput)
  {
    *enEqRange = pstcUserIO->enSelectedEqRange;
    *i8Value = pstcUserIO->i8NewValue;
    pstcUserIO->bHasNewInput = FALSE;
    return TRUE;
  }
  return FALSE;
}

/**
 ******************************************************************************
 ** \brief Modifies the values showed to the user.     
 **
 ** \param  [in][out]  pstcUserIO   UserIO structure.
 ** \param  [in]       enEqRange    Range that is modified.
 ** \param  [in]       i8Value      The value which the range is modified to.
 **
 **
 ******************************************************************************/
void UserIO_setNewOutput(stc_UserIO_t* pstcUserIO, const en_EqualizerRange_t enEqRange, const int8_t i8Value)
{
  pstcUserIO->i8EqValues[enEqRange] = i8Value;
  if(pstcUserIO->enSelectedEqRange == enEqRange)
  {
    pstcUserIO->i8NewValue = i8Value;
  }
  UserIO_updateControl(pstcUserIO);
  UserIO_updateControlHighlight(pstcUserIO);
}

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
