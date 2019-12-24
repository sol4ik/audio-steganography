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
/** \file buffer.h
 **
 ** A detailed description is available at 
 ** @link BufferGroup Stores Input and Output Buffers description @endlink
 **
 ** History:
 **   - 2015-06-02  V1.0  Jan Weber  First Version
 *****************************************************************************/

#ifndef __BUFFER_H__
#define __BUFFER_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup BufferGroup Stores Input and Output Buffers
 **
 ** Provided functions of Stores Input and Output Buffers:
 ** 
 ** - Buffers_Init()
 ** - Buffers_Deinit()
 ** - Buffer_isFull(const stc_Buffer_t* pstcBuffer);
 ** - Buffer_isReady(const stc_Buffer_t* pstcBuffer);
 ** - Buffer_isEmpty(const stc_Buffer_t* pstcBuffer);
 ** - Buffer_AddSample(stc_Buffer_t* pstcBuffer, const uint16_t au16Sample[BUFFER_CHANNEL_COUNT]);
 ** - Buffer_PrepareNextBlock(stc_Buffer_t* pstcInputBuffer, stc_BufferEq_t* pstcEqBuffer);
 ** - Buffer_FinalizeNextBlock(stc_Buffer_t* pstcOutputBuffer, stc_BufferEq_t* pstcEqBuffer);
 ** - Buffer_GetNextSample(stc_Buffer_t* pstcOutputBuffer, uint16_t au16Sample[BUFFER_CHANNEL_COUNT]);
 **   
 ******************************************************************************/
//@{
    
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "base_types.h"
#include "mcu.h"
#include "globals.h"
#include "arm_math.h"
#include "windowfunction.h"
  
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
 
/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
typedef enum en_BufferType
{
  INPUTBUFFER, 
  OUTPUTBUFFER
} en_BufferType_t;  

typedef enum en_BufferBlockStatus
{
  INVALID, 
  VALID, 
} en_BufferBlockStatus_t;
  
typedef struct stc_BufferBlock
{
  en_BufferBlockStatus_t  enStatus;
  q31_t                   aq31Buffer[BUFFER_CHANNEL_COUNT][BUFFER_IO_SIZE];
} stc_BufferBlock_t;
   
typedef struct stc_Buffer
{
  en_BufferType_t         enBufferType;
  uint8_t                 u8ReadBlock;
  uint8_t                 u8WriteBlock;      
  uint16_t                u16WriteBlockPos;
  uint16_t                u16ReadBlockPos;
  stc_WindowFunction_t*   pstcWindowFunction;
  stc_BufferBlock_t       astcBlock[BUFFER_COUNT]; 
} stc_Buffer_t;

typedef struct stc_BufferEq
{
  q31_t                   aq31Data[BUFFER_CHANNEL_COUNT][2u * BUFFER_EQ_SIZE];
} stc_BufferEq_t;

typedef enum en_BufferResult
{
  SUCCESSFULL, 
  FAILED_BUFFER_FULL, 
  FAILED_BUFFER_EMPTY, 
  DENIED_WRONG_BUFFERTYPE, 
  NONE
} en_BufferResult_t;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
extern void                     Buffer_Init(stc_Buffer_t* pstcBuffer, en_BufferType_t enBufferType, stc_WindowFunction_t* pstcWindowFunction);
extern void                     Buffer_Deinit(stc_Buffer_t* pstcBuffer);
extern boolean_t                Buffer_isFull(const stc_Buffer_t* pstcBuffer);
extern boolean_t                Buffer_isReady(const stc_Buffer_t* pstcBuffer);
extern boolean_t                Buffer_isEmpty(const stc_Buffer_t* pstcBuffer);
extern en_BufferResult_t        Buffer_AddSample(stc_Buffer_t* pstcBuffer, const uint16_t au16Sample[BUFFER_CHANNEL_COUNT]);
extern en_BufferResult_t Buffer_PrepareNextBlock(stc_Buffer_t* pstcInputBuffer, stc_BufferEq_t* pstcEqBuffer);
extern en_BufferResult_t        Buffer_FinalizeNextBlock(stc_Buffer_t* pstcOutputBuffer, stc_BufferEq_t* pstcEqBuffer);
extern en_BufferResult_t        Buffer_GetNextSample(stc_Buffer_t* pstcOutputBuffer, uint16_t au16Sample[BUFFER_CHANNEL_COUNT]);

#ifdef __cplusplus
}
#endif

//@} // BufferGroup

#endif /* __BUFFER_H__*/

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
