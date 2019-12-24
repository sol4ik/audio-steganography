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
/** \file Buffers.c
 **
 ** A detailed description is available at 
 ** @link BuffersGroup Stores Input and Output Buffers description @endlink
 **
 ** History:
 **   - 2015-06-02  V1.0  Jan Weber  First Version
 *****************************************************************************/
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "buffers.h"

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
static uint8_t InputBuffer_getCurrentSector(stc_InputBuffer_t* pstcInputBuffer);

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
void InputBuffer_Init(stc_InputBuffer_t* pstcInputBuffer)
{
  pstcInputBuffer->bTopValid = FALSE;
  pstcInputBuffer->bTailValid = FALSE
  pstcInputBuffer->u16WritePos = 0;
}

/**
 ******************************************************************************
 ** \brief Deinitialization Routine
 **
 ******************************************************************************/
void InputBuffer_Deinit(stc_InputBuffer_t* pstcInputBuffer)
{
  /* nothing to do here. */
}

/**
 ******************************************************************************
 ** \brief Checks whether the given InputBuffer is full or not.
 **
 ** \param  [in] pstcInputBuffer      InputBuffer that will be checked.
 **
 ** \return boolean_t TRUE - Buffer is full / FALSE - Buffer is not full.
 **
 ******************************************************************************/
boolean_t InputBuffer_isFilled(stc_InputBuffer_t* pstcInputBuffer)
{
  if(((pstcInputBuffer->u16WritePos < (INPUTBUFFER_SIZE / 2u)) && 
      pstcInputBuffer->bTopValid) && 
     ((pstcInputBuffer->u16WritePos >= ((INPUTBUFFER_SIZE / 2u)) - 
                                         INPUTBUFFER_WINDOW_SIZE) && 
      pstcInputBuffer->bTailValid) &&
     ((pstcInputBuffer->u16WritePos >= (INPUTBUFFER_SIZE - 
                                        INPUTBUFFER_WINDOW_SIZE)) && 
      pstcInputBuffer->bTopValid))
  {
    return TRUE;
  }
  return FALSE;  
}

/**
 ******************************************************************************
 ** \brief Returns the sector of the given InputBuffer the u16WritePos points on.
 **
 ** \param  [in] pstcInputBuffer      InputBuffer that will be checked.
 **
 ** \return uint8_t Number of the sector the u16WritePos points on
 **
 ******************************************************************************/
static uint8_t InputBuffer_getCurrentSector(stc_InputBuffer_t* pstcInputBuffer)
{
    if((pstcInputBuffer->u16WritePos >= INPUTBUFFER_WINDOW_SIZE) && 
     (pstcInputBuffer->u16WritePos < (INPUTBUFFER_SIZE / 2u)))
    {
      if(pstcInputBuffer->u16WritePos >= (INPUTBUFFER_SIZE / 2u - 
                                          INPUTBUFFER_WINDOW_SIZE))
      {
        return 3u;
      }
      else
      {
        return 2u;
      }
    }
    else if((pstcInputBuffer->u16WritePos >= (INPUTBUFFER_WINDOW_SIZE + 
                                              INPUTBUFFER_SIZE / 2u)))
    {
      if((pstcInputBuffer->u16WritePos >= (INPUTBUFFER_SIZE - 
                                           INPUTBUFFER_WINDOW_SIZE)))
      {
        return 6u;
      }
      else
      {
        return 5u;
      }      
    }
}

/**
 ******************************************************************************
 ** \brief Returns the sector of the given InputBuffer the u16WritePos points on.
 **
 ** \param  [in][out] pstcInputBuffer  InputBuffer to which the sample will be added.
 ** \param  [in]      u16sampleLeft    Sample of the left channel that will be added.  
 ** \param  [in]      u16sampleRight    Sample of the right channel that will be added.      
 **
 ** \return en_BufferResult_t shows whether the adding was successfull or not.
 **
 ******************************************************************************/
en_BufferResult_t InputBuffer_AddSample(stc_InputBuffer_t* pstcInputBuffer, 
                                    const uint16_t u16sampleLeft, const uint16_t u16sampleRight)
{
  
  /* if the buffer is full, stop here... */
  if(InputBuffer_isFilled())
  {
    return FAILED_BUFFER_FULL;
  }
  
  /*  determine current writing position in buffer, 
      and if data have to be duplicated due to windowing */
  switch(InputBuffer_getCurrentSector(pstcInputBuffer))
  {
  case 3u:
    pstcInputBuffer->q31BufferLeft[pstcInputBuffer->u16WritePos] = 
        pstcInputBuffer->stcWindowFct->pWindow(u16sampleLeft, 
                                               (pstcInputBuffer->u16WritePos + 
                                                INPUTBUFFER_WINDOW_SIZE));
    
    pstcInputBuffer->q31BufferRight[pstcInputBuffer->u16WritePos] =
        pstcInputBuffer->stcWindowFct->pWindow(u16sampleRight, 
                                               (pstcInputBuffer->u16WritePos + 
                                                INPUTBUFFER_WINDOW_SIZE));
    
  case 4u:
    pstcInputBuffer->q31BufferLeft[pstcInputBuffer->u16WritePos] = 
      pstcInputBuffer->stcWindowFct->pWindow(u16sampleLeft, 
                                             pstcInputBuffer->u16WritePos);
    
    pstcInputBuffer->q31BufferRight[pstcInputBuffer->u16WritePos] = 
      pstcInputBuffer->stcWindowFct->pWindow(u16sampleRight, 
                                             pstcInputBuffer->u16WritePos);    
    
    break;
    
  case 5u:
    pstcInputBuffer->q31BufferLeft[pstcInputBuffer->u16WritePos] = 
        pstcInputBuffer->stcWindowFct->pWindow(u16sampleLeft, 
                                               (pstcInputBuffer->u16WritePos - 
                                                (INPUTBUFFER_SIZE - 
                                                 INPUTBUFFER_WINDOW_SIZE)));
    
    pstcInputBuffer->q31BufferRight[pstcInputBuffer->u16WritePos] =
        pstcInputBuffer->stcWindowFct->pWindow(u16sampleRight, 
                                               (pstcInputBuffer->u16WritePos - 
                                                (INPUTBUFFER_SIZE - 
                                                 INPUTBUFFER_WINDOW_SIZE)));
  case 6u:
    
    pstcInputBuffer->q31BufferLeft[pstcInputBuffer->u16WritePos] = 
      pstcInputBuffer->stcWindowFct->pWindow(u16sampleLeft, 
                                             (pstcInputBuffer->u16WritePos - 
                                              INPUTBUFFER_SIZE / 2u));
    
    pstcInputBuffer->q31BufferRight[pstcInputBuffer->u16WritePos] = 
      pstcInputBuffer->stcWindowFct->pWindow(u16sampleRight, 
                                             (pstcInputBuffer->u16WritePos - 
                                              INPUTBUFFER_SIZE / 2u));
    
    break;
  default: /* should not happen */
    return FAILED_BUFFER_FULL;    
  }
  
  pstcInputBuffer->u16WritePos++;
  
  if((INPUTBUFFER_SIZE/2u) == pstcInputBuffer->u16WritePos)
  {
    pstcInputBuffer->u16WritePos = (INPUTBUFFER_SIZE/2u) + INPUTBUFFER_WINDOW_SIZE;
    pstcInputBuffer->bTopValid = TRUE;
  }
  else if((INPUTBUFFER_SIZE) == pstcInputBuffer->u16WritePos)
  {
    pstcInputBuffer->u16WritePos = INPUTBUFFER_WINDOW_SIZE;
    pstcInputBuffer->bTailValid = TRUE;
  }  
  
  return SUCCESSFULL;
}

/**
 ******************************************************************************
 ** \brief Returns the next full buffer from the given InputBuffer
 **
 ** \param  [in][out] pstcInputBuffer  InputBuffer from which the next full buffer is returned
 ** \param  [in]      pq31BufferStart  Start position of the full buffer.  
 ** \param  [in]      pu16BufferLength Length of the full buffer.      
 **
 ** \return en_BufferResult_t shows whether the buffer fetching was successfull or not.
 **
 ******************************************************************************/
en_BufferResult_t InputBuffer_GetNextBuffer(stc_InputBuffer_t* pstcInputBuffer, 
                                        q31_t* pq31BufferStart, 
                                        uint16_t* pu16BufferLength)
{ 
  
  /* Check whether one of the input buffers is ready to be proccessed, 
     also make sure that the write Cursor points on the other half to ensure that
     the older part is returned. */
  
  if(pstcInputBuffer->bTopValid && pstcInputBuffer->u16WritePos >= 
     (INPUTBUFFER_SIZE / 2u))
  {
    pq31BufferStart = 0u;
    pu16BufferLength = INPUTBUFFER_SIZE / 2u;
    pstcInputBuffer->bTopValid = FALSE;    
    return SUCCESSFULL;
  }
  else if(pstcInputBuffer->bTailValid && pstcInputBuffer->u16WritePos < 
          (INPUTBUFFER_SIZE / 2u) )
  {
    pq31BufferStart = INPUTBUFFER_SIZE / 2u;
    pu16BufferLength = INPUTBUFFER_SIZE / 2u;
    pstcInputBuffer->bTopValid = FALSE;  
    return SUCCESSFULL;
  }  
  return FAILED_BUFFER_EMPTY;  
}

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
