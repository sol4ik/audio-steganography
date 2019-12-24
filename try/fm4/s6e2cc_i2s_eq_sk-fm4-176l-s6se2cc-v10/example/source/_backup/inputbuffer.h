/*******************************************************************************
* Copyright (C) 2013 Spansion LLC. All Rights Reserved. 
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
/** \file buffers.h
 **
 ** A detailed description is available at 
 ** @link BuffersGroup Stores Input and Output Buffers description @endlink
 **
 ** History:
 **   - 2015-06-02  V1.0  Jan Weber  First Version
 *****************************************************************************/

#ifndef __BUFFERS_H__
#define __BUFFERS_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup BuffersGroup Stores Input and Output Buffers
 **
 ** Provided functions of Stores Input and Output Buffers:
 ** 
 ** - Buffers_Init()
 ** - Buffers_Deinit()
 **   
 ******************************************************************************/
//@{

/**
 ******************************************************************************    
 ** \page buffers_module_includes Required includes in main application
 ** \brief Following includes are required
 ** @code   
 ** #include "buffers.h"   
 ** @endcode
 **
 ******************************************************************************/
    
/**
 ****************************************************************************** 
 ** \page buffers_module_init Example: Initialization
 ** \brief Following initialization is required 
 **
 ** @code
 ** Buffers_Init();   
 ** @endcode
 **
 ******************************************************************************/
    
    
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "base_types.h"
#include "mcu.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
#define INPUTBUFFER_SIZE                    2048
#define INPUTBUFFER_WINDOW_SIZE              256
 
/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
typedef struct stc_InputBuffer
{
  boolean_t      bTopValid;
  boolean_t      bTailValid;                                               
  uint16_t       u16WritePos;
  q31_t          q31BufferLeft[INPUTBUFFER_SIZE];
  q31_t          q31BufferRight[INPUTBUFFER_SIZE];
  stcWindowFkt_t stcWindowFkt;
} stc_InputBuffer_t

typedef enum en_BufferResult
{
  SUCCESSFULL, FAILED_BUFFER_FULL, FAILED_BUFFER_EMPTY
} en_BufferResult_t

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
extern void                     InputBuffer_Init(stc_InputBuffer_t* stcInputBuffer);
extern void                     InputBuffer_Deinit(stc_InputBuffer_t* stcInputBuffer);
extern boolean_t                InputBuffer_isFilled();
extern en_BufferResult_t        InputBuffer_AddSample(stc_InputBuffer_t* stcInputBuffer, const uint16_t u16sampleLeft, const uint16_t u16sampleRight);
extern en_BufferResult_t        InputBuffer_GetNextBuffer(stc_InputBuffer_t* stcInputBuffer, q31_t* pq31BufferStart, uint16_t* pu16BufferLength);

#ifdef __cplusplus
}
#endif

//@} // BuffersGroup

#endif /* __BUFFERS_H__*/

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

