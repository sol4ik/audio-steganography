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
/** \file userio.h
 **
 ** A detailed description is available at 
 ** @link UserIOGroup Processes user inputs via UART description @endlink
 **
 ** History:
 **   - 2015-16-02  V1.0  Jan Weber  First Version
 *****************************************************************************/

#ifndef __USERIO_H__
#define __USERIO_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UserIOGroup Processes user inputs via UART
 **
 ** Provided functions of Processes user inputs via UART:
 ** 
 ** - UserIO_Init(stc_UserIO_t* pstcUserIO)
 ** - UserIO_Deinit(stc_UserIO_t* pstcUserIO)
 ** - boolean_t UserIO_hasNewInput(stc_UserIO_t* pstcUserIO)
 ** - boolean_t UserIO_getNewInput(stc_UserIO_t* pstcUserIO, en_EqualizerRange_t* enEqRange, int8_t* i8Value)
 ** - UserIO_setNewOutput(stc_UserIO_t* pstcUserIO, const en_EqualizerRange_t enEqRange, const int8_t i8Value)
 **   
 ******************************************************************************/
//@{
  
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "base_types.h"
#include "mcu.h"
#include "equalizer.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
#define USERIO_HEAD             "SK-FM4-176L Equalizer"
#define USERIO_BG               BG_BLUE
#define USERIO_FG               TXT_WHITE
#define USERIO_BG_SEL           BG_BLACK
#define USERIO_FG_SEL           TXT_WHITE
#define USERIO_FOOT1            "HINT: Use left and right keys to navigate"
#define USERIO_FOOT2            "      and up and down keys to modify the"
#define USERIO_FOOT3            "      values."
#define USERIO_CAPTIONS         "BASS  %c%d      MIDRANGE  %c%d      TREBLE  %c%d  "
#define USERIO_CAPTIONS_POS     5u, 5u
#define USERIO_FOOT1_POS        5u, 8u
#define USERIO_FOOT2_POS        5u, 9u
#define USERIO_FOOT3_POS        5u, 10u
#define USERIO_HEAD_POS         17u, 3u
#define USERIO_VAL1_POS         12u, 5u  
#define USERIO_VAL2_POS         30u, 5u
#define USERIO_VAL3_POS         46u, 5u
#define USERIO_SIGN1_POS        11u, 5u
#define USERIO_SIGN2_POS        29u, 5u
#define USERIO_SIGN3_POS        45u, 5u
#define USERIO_SIGN_POS         '+'
#define USERIO_SIGN_NEG         '-'
#define USERIO_SIGN_ZERO        ' '
#define USERIO_CURSOR_PARKING   0u, 15u
#define USERIO_MCU_LOAD_POS     5u, 14u
#define USERIO_RECT_SIZE        3u, 3u, 50u, 10u
#define USERIO_RECT_HEAD_SIZE   3u, 3u, 50u, 1u
   
/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
typedef struct stc_UserIO  
{
  en_EqualizerRange_t enSelectedEqRange;
  int8_t              i8NewValue;
  boolean_t           bHasNewInput;
  boolean_t           bHasNewOutput;
  int8_t              i8EqValues[EQUALIZER_EQ_RANGE]; 
  uint8_t             u8ESCSequenceDetect;
} stc_UserIO_t;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
extern void      UserIO_Init(stc_UserIO_t* pstcUserIO);
extern void      UserIO_Deinit(stc_UserIO_t* pstcUserIO);
extern boolean_t UserIO_hasNewInput(stc_UserIO_t* pstcUserIO);
extern boolean_t UserIO_getNewInput(stc_UserIO_t* pstcUserIO, en_EqualizerRange_t* enEqRange, int8_t* i8Value);
extern void      UserIO_setNewOutput(stc_UserIO_t* pstcUserIO, const en_EqualizerRange_t enEqRange, const int8_t i8Value);

#ifdef __cplusplus
}
#endif

//@} // UserIOGroup

#endif /* __USERIO_H__*/

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
