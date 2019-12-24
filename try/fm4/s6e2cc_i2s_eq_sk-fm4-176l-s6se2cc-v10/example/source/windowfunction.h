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
/** \file windowfunction.h
 **
 ** A detailed description is available at 
 ** @link WindowFunctionGroup - description @endlink
 **
 ** History:
 **   - 2015-09-02  V1.0  JWe  First Version
 *****************************************************************************/

#ifndef __WINDOWFUNCTION_H__
#define __WINDOWFUNCTION_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup WindowFunctionGroup -
 **
 ** Provided functions of -:
 ** 
 ** - WindowFunction_Init()
 ** - WindowFunction_Deinit()
 **   
 ******************************************************************************/
//@{
    
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "base_types.h"
#include "mcu.h"  
#include "arm_math.h"
#include "globals.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
typedef enum en_WindowType
{
  COSINE
} en_WindowType_t;
  
typedef struct stc_WindowFunction stc_WindowFunction_t;

struct stc_WindowFunction
{
  q31_t   aq31WindowBuf[BUFFER_CHANNEL_COUNT][BUFFER_WINDOW_SIZE];
  q31_t   aq31DeWindowBuf[BUFFER_CHANNEL_COUNT][BUFFER_WINDOW_SIZE];
  q31_t   aq31Edge[BUFFER_WINDOW_SIZE];
  void    (*pFctWindow)(stc_WindowFunction_t*, q31_t*, q31_t*, uint8_t);
  void    (*pFctDeWindow)(stc_WindowFunction_t*, q31_t*, q31_t*, uint8_t); 
};

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
extern void WindowFunction_Init(stc_WindowFunction_t* pstcWindowFunction);
extern void WindowFunction_Deinit(stc_WindowFunction_t* pstcWindowFunction);
extern void WindowFunction_WindowPrepare(stc_WindowFunction_t* pstcWindowFunction, q31_t* pq31InputBuffer, q31_t* pq31OutputBuffer, uint8_t u8Channel);
extern void WindowFunction_WindowFinalize(stc_WindowFunction_t* pstcWindowFunction, q31_t* pq31InputBuffer, q31_t* pq31OutputBuffer, uint8_t u8Channel);

#ifdef __cplusplus
}
#endif

//@} // WindowFunctionGroup

#endif /* __WINDOWFUNCTION_H__*/

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
