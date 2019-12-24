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
/** \file buffer.c
 **
 ** A detailed description is available at 
 ** @link InputBufferGroup Stores Input Buffers description @endlink
 **
 ** History:
 **   - 2015-06-02  V1.0  Jan Weber  First Version
 *****************************************************************************/
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "buffer.h"
#include "windowfunction.h"

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
 ** \brief Initialization Routine of buffer
 **
 ******************************************************************************/
void Buffer_Init(stc_Buffer_t* pstcBuffer, en_BufferType_t enBufferType, stc_WindowFunction_t* pstcWindowFunction)
{
  pstcBuffer->enBufferType = enBufferType;
  pstcBuffer->u8WriteBlock = 0u;
  pstcBuffer->u8ReadBlock = 0u;
  pstcBuffer->u16WriteBlockPos = 0u;
  pstcBuffer->u16ReadBlockPos = 0u;
  pstcBuffer->pstcWindowFunction = pstcWindowFunction;
}

/**
 ******************************************************************************
 ** \brief Deinitialization Routine of buffer
 **
 ******************************************************************************/
void Buffer_Deinit(stc_Buffer_t* pstcBuffer)
{
  /* nothing to do here. */
}

/**
 ******************************************************************************
 ** \brief Checks whether the given Buffer structure has at least one filled block.
 **
 ** \param  [in] pstcBuffer      The Buffer that will be checked.
 **
 ** \return boolean_t TRUE - At least one block is full / FALSE - No block full.
 **
 ******************************************************************************/
boolean_t Buffer_isFull(const stc_Buffer_t* pstcBuffer)
{
  return (VALID == pstcBuffer->astcBlock[pstcBuffer->u8WriteBlock].enStatus);
}

/**
 ******************************************************************************
 ** \brief Checks whether the given Buffer structure is completly filled
 **
 ** \param  [in] pstcBuffer      The Buffer that will be checked.
 **
 ** \return boolean_t TRUE - Buffer is filled / FALSE - Buffer is not filled.
 **
 ******************************************************************************/
boolean_t Buffer_isReady(const stc_Buffer_t* pstcBuffer)
{
  uint8_t u8curBuffer;
  
  for(u8curBuffer = 0u; u8curBuffer < BUFFER_COUNT; u8curBuffer++)
  {
    if(INVALID == pstcBuffer->astcBlock[pstcBuffer->u8WriteBlock].enStatus)
    {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 ******************************************************************************
 ** \brief Checks whether the given Buffer structure is empty or not.
 **
 ** \param  [in] pstcBuffer      Buffer structure that will be checked.
 **
 ** \return boolean_t TRUE - Buffer is empty / FALSE - Buffer is not empty.
 **
 ******************************************************************************/
boolean_t Buffer_isEmpty(const stc_Buffer_t* pstcBuffer)
{
  return (INVALID == pstcBuffer->astcBlock[pstcBuffer->u8ReadBlock].enStatus);
}

/**
 ******************************************************************************
 ** \brief Adds the given sample au16Sample[] to the given buffer pstcBuffer
 **
 ** \param  [in][out] pstcBuffer   The buffer to which the samples are added.
 ** \param  [in]      au16Sample[] array of all channels of the sample that is added.        
 **
 ** \return en_BufferResult_t shows whether the adding was successfull or not.
 **
 ******************************************************************************/
en_BufferResult_t Buffer_AddSample(stc_Buffer_t* pstcBuffer, 
                                        const uint16_t au16Sample[BUFFER_CHANNEL_COUNT])
{
  uint8_t u8CurChannel;
  
  if(INPUTBUFFER != pstcBuffer->enBufferType)
  {
    return DENIED_WRONG_BUFFERTYPE;
  }
  
  /* if the buffer is full, stop here... */
  if(TRUE == Buffer_isFull(pstcBuffer))
  {
    return FAILED_BUFFER_FULL;
  }
  
  /* Copy sample values to input buffer */
  for(u8CurChannel = 0u; u8CurChannel < BUFFER_CHANNEL_COUNT; u8CurChannel++)
  {
    
    pstcBuffer->astcBlock[pstcBuffer->u8WriteBlock].
      aq31Buffer[u8CurChannel][pstcBuffer->u16WriteBlockPos] = 
          ((((au16Sample[u8CurChannel] & 0x8000u) > 0u) ? (q31_t) (au16Sample[u8CurChannel] | 0xFFFF0000ul) : (q31_t) au16Sample[u8CurChannel]) 
#if (TRANSFORM_ON == 1)
            << 12ul
#endif
           );
  }
  
  /* update write position cursor */
  (pstcBuffer->u16WriteBlockPos)++;  
  if(BUFFER_IO_SIZE == pstcBuffer->u16WriteBlockPos)
  {
    pstcBuffer->astcBlock[pstcBuffer->u8WriteBlock].enStatus = VALID;
    pstcBuffer->u16WriteBlockPos = 0u;
    pstcBuffer->u8WriteBlock++;
    if(BUFFER_COUNT == pstcBuffer->u8WriteBlock)
    {
      pstcBuffer->u8WriteBlock = 0u;
    }
  }
  
  return SUCCESSFULL;
}

/**
 ******************************************************************************
 ** \brief Transfers the next filled block from the given buffer structure 
 **        pstcInputBuffer to the given equalizer buffer structure pstcEqBuffer. 
 **        The samples are proccessed by the window function attached to 
 **        pstcInputBuffer  before they are saved.       
 **
 ** \param  [in][out]  pstcInputBuffer    InputBuffer which the next full block is read from.
 ** \param  [in][out]  pstcEqBuffer       EqualizerBuffer which the block is written to.
 **
 ** \return en_BufferResult_t shows whether the buffer preparation was successfull or not.
 **
 ******************************************************************************/
en_BufferResult_t Buffer_PrepareNextBlock(stc_Buffer_t* pstcInputBuffer, 
                                       stc_BufferEq_t* pstcEqBuffer)
{ 
#if (0 == WINDOWING_ON)
  q31_t* pq31InputBuffer;  
  q31_t* pq31OutputBuffer;
  uint16_t u16CurSample;
#endif
  uint8_t  u8CurChannel;
  
  if(INPUTBUFFER != pstcInputBuffer->enBufferType)
  {
    return DENIED_WRONG_BUFFERTYPE;
  }
  
  if(TRUE == Buffer_isEmpty(pstcInputBuffer))
  {
    return FAILED_BUFFER_EMPTY;   
  }
  
#if (1 == WINDOWING_ON)
  for(u8CurChannel = 0u; u8CurChannel < BUFFER_CHANNEL_COUNT; u8CurChannel++)
  {
        pstcInputBuffer->pstcWindowFunction->pFctWindow(pstcInputBuffer->pstcWindowFunction,
                                                        &pstcInputBuffer->
                                                          astcBlock[pstcInputBuffer->u8ReadBlock].
                                                            aq31Buffer[u8CurChannel][0u],
                                                        &pstcEqBuffer->
                                                            aq31Data[u8CurChannel][0u],
                                                            u8CurChannel);
  }
#else  
  for(u8CurChannel = 0u; u8CurChannel < BUFFER_CHANNEL_COUNT; u8CurChannel++)
  {
      pq31InputBuffer = &pstcInputBuffer->astcBlock[pstcInputBuffer->u8ReadBlock].
                                                            aq31Buffer[u8CurChannel][0u];  
      pq31OutputBuffer = &pstcEqBuffer->aq31Data[u8CurChannel][0u];      
    
      for(u16CurSample = 0u; u16CurSample < BUFFER_EQ_SIZE; u16CurSample++)
      {
        *pq31OutputBuffer++ = *pq31InputBuffer++;
        *(pq31OutputBuffer++) = 0u; // set imag to zero
      }

  }
#endif
    
  /* update read position cursor */
  pstcInputBuffer->astcBlock[pstcInputBuffer->u8ReadBlock].enStatus = INVALID;
  pstcInputBuffer->u8ReadBlock++;  
  if(BUFFER_COUNT == pstcInputBuffer->u8ReadBlock)
  {
    pstcInputBuffer->u8ReadBlock = 0u;
  }
  
  return SUCCESSFULL;
}

/**
 ******************************************************************************
 ** \brief Transfers the block from the given equalizer buffer structure 
 **        pstcEqBuffer to the given output buffer structure pstcOutputBuffer. 
 **        The samples are proccessed by the window function attached to 
 **        pstcOutputBuffer  before they are saved.       
 **
 ** \param  [in][out]  pstcOutputBuffer   OutputBuffer which the next full block is written to.
 ** \param  [in][out]  pstcEqBuffer       EqualizerBuffer which the block is read to.
 **
 ** \return en_BufferResult_t shows whether the buffer preparation was successfull or not.
 **
 ******************************************************************************/
en_BufferResult_t Buffer_FinalizeNextBlock(stc_Buffer_t* pstcOutputBuffer, stc_BufferEq_t* pstcEqBuffer)
{
#if (0 == WINDOWING_ON)
  q31_t* pq31InputBuffer;  
  q31_t* pq31OutputBuffer;
  uint16_t u16CurSample;
#endif
  uint8_t  u8CurChannel;  
  
  if(OUTPUTBUFFER != pstcOutputBuffer->enBufferType)
  {
    return DENIED_WRONG_BUFFERTYPE;
  }
  
  if(TRUE == Buffer_isFull(pstcOutputBuffer))
  {
    return FAILED_BUFFER_FULL;   
  }  

#if (1 == WINDOWING_ON)
  for(u8CurChannel = 0u; u8CurChannel < BUFFER_CHANNEL_COUNT; u8CurChannel++)
  {
    pstcOutputBuffer->
      pstcWindowFunction->pFctDeWindow(pstcOutputBuffer->pstcWindowFunction, 
                                       &(pstcEqBuffer->aq31Data[u8CurChannel][0u]), 
                                       &(pstcOutputBuffer->astcBlock[pstcOutputBuffer->
                                         u8WriteBlock].aq31Buffer[u8CurChannel][0u]),
                                       u8CurChannel);

  } 
#else
  for(u8CurChannel = 0u; u8CurChannel < BUFFER_CHANNEL_COUNT; u8CurChannel++)
  {
       pq31OutputBuffer = &pstcOutputBuffer->astcBlock[pstcOutputBuffer->u8WriteBlock].
                                                            aq31Buffer[u8CurChannel][0u];  
       pq31InputBuffer = &pstcEqBuffer->aq31Data[u8CurChannel][0u];      
    
      for(u16CurSample = 0u; u16CurSample < BUFFER_EQ_SIZE; u16CurSample++)
      {
        *pq31OutputBuffer++ = *pq31InputBuffer++;
        (pq31InputBuffer++); // skip imag
      }
  }
#endif
  /* update write position cursor */
  pstcOutputBuffer->astcBlock[pstcOutputBuffer->u8WriteBlock].enStatus = VALID;
  pstcOutputBuffer->u8WriteBlock++;  
  if(BUFFER_COUNT == pstcOutputBuffer->u8WriteBlock)
  {
    pstcOutputBuffer->u8WriteBlock = 0u;
  } 
  
  return SUCCESSFULL;
}

/**
 ******************************************************************************
 ** \brief Adds the next sample from the given output buffer pstcOutputBuffer
 **        to the given sample au16Sample[].
 **
 ** \param  [in][out] pstcBuffer   The buffer which the samples is added from.
 ** \param  [out]     au16Sample[] array of all channels for the sample.        
 **
 ** \return en_BufferResult_t shows whether the adding was successfull or not.
 **
 ******************************************************************************/
en_BufferResult_t Buffer_GetNextSample(stc_Buffer_t* pstcOutputBuffer, uint16_t au16Sample[BUFFER_CHANNEL_COUNT])
{
  uint8_t u8CurChannel;
  
  if(OUTPUTBUFFER != pstcOutputBuffer->enBufferType)
  {
    return DENIED_WRONG_BUFFERTYPE;
  }
  
  if(TRUE == Buffer_isEmpty(pstcOutputBuffer))
  {
    return FAILED_BUFFER_EMPTY;   
  }
  
  for(u8CurChannel = 0u; u8CurChannel < BUFFER_CHANNEL_COUNT; u8CurChannel++)
  {
    au16Sample[u8CurChannel] = (pstcOutputBuffer->astcBlock[pstcOutputBuffer->u8ReadBlock].
      aq31Buffer[u8CurChannel][pstcOutputBuffer->u16ReadBlockPos] & 0x0000FFFF);
  }
  
  pstcOutputBuffer->u16ReadBlockPos++;
  if(BUFFER_IO_SIZE == pstcOutputBuffer->u16ReadBlockPos)
  {
    pstcOutputBuffer->astcBlock[pstcOutputBuffer->u8ReadBlock].enStatus = INVALID; 
    pstcOutputBuffer->u16ReadBlockPos = 0u;
    pstcOutputBuffer->u8ReadBlock++;
    if(BUFFER_COUNT == pstcOutputBuffer->u8ReadBlock)
    {
      pstcOutputBuffer->u8ReadBlock = 0u;
    }
  }
  
  return SUCCESSFULL;
}

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
