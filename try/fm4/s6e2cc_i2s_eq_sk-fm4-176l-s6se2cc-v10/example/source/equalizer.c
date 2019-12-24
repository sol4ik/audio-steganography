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
/** \file equalizer.c
 **
 ** A detailed description is available at 
 ** @link EqualizerGroup - description @endlink
 **
 ** History:
 **   - 2015-12-02  V1.0  Jan Weber  First Version
 *****************************************************************************/
#define __EQUALIZER_C__
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "equalizer.h"
#include "globals.h"

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
 ** \brief Deinitialization Routine of Rqualizer
 **
 ******************************************************************************/
void Equalizer_Init(stc_Equalizer_t* pstcEqualizer)
{
  uint16_t  u16CurValue;
  
  // Calculate the corresponding Q31 Values (from -1 to +1) for each equalizer step:
  for(u16CurValue = 0u; u16CurValue < EQUALIZER_STEPS; u16CurValue++)
  {
    pstcEqualizer->aq31MappingInt8ToQ31[u16CurValue] =  FLOAT_TO_Q31((float32_t)u16CurValue / (EQUALIZER_STEPS - 1u));
  } 
  
  // Initialize all ranges:
  for(u16CurValue = 0u; u16CurValue < EQUALIZER_EQ_RANGE; u16CurValue++)
  {
    Equalizer_Set(pstcEqualizer, (en_EqualizerRange_t)u16CurValue, 0u);
  }
}

/**
 ******************************************************************************
 ** \brief Deinitialization Routine of Rqualizer
 **
 ******************************************************************************/
void Equalizer_Deinit(stc_Equalizer_t* pstcEqualizer)
{
  /* nothing to do here */
}

/**
 ******************************************************************************
 ** \brief Sets the given range enEqRange of the given equalizer structure 
 **        stcEqualizer to the given value i8Value. If the given value is 
 **        out of scope it will be truncated.
 **
 ** \param  [in][out] pstcEqualizer   The equalizer that will be modified.
 ** \param  [in]      enEqRange      The range which is modified
 ** \param  [in]      i8Value        The value which the range is set to.
 **
 ** \return int8_t the actual value the range is set to, truncated if nessesary.
 **
 ******************************************************************************/
int8_t Equalizer_Set(stc_Equalizer_t* pstcEqualizer, 
                     const en_EqualizerRange_t enEqRange, int8_t i8Value)
{
  //If the given value is to big or to small it is truncated.
  if(i8Value > EQUALIZER_STEPS_MAX)
  {
    i8Value = EQUALIZER_STEPS_MAX;
  }
  else if(i8Value < EQUALIZER_STEPS_MIN)
  {
    i8Value = EQUALIZER_STEPS_MIN;
  }
  
  pstcEqualizer->u8EqualizerControl[enEqRange] = (uint8_t)(i8Value + EQUALIZER_STEPS_CENTER - 1); 
  
  return i8Value;  
}

/**
 ******************************************************************************
 ** \brief Processes the given block pq31Block with the aid of the configuration
 **        stored in the given equalizer structure stcEqualizer. 
 **
 ** \param  [in]        pstcEqualizer   The equalizer that is used to process the block
 ** \param  [in][out]   pq31Block       The block that is processed
 **
 ******************************************************************************/
void Equalizer_ProcessBlock(const stc_Equalizer_t* pstcEqualizer, q31_t* pq31Block)
{
  uint16_t u16CurSample;
  q31_t* pq31BlockForward = pq31Block;
  q31_t* pq31BlockBackward = pq31Block + (BUFFER_EQ_SIZE * 2u) - 1u;
  q31_t  q31Temp;
  
  for(u16CurSample = 0u; u16CurSample < (BUFFER_EQ_SIZE); u16CurSample++)
  {      
   q31Temp = *pq31BlockForward;
   *(pq31BlockForward++) = (q31_t) clip_q63_to_q31(((q63_t) (q31Temp) * EQUALIZER_GET_EQ_VALUE(pstcEqualizer, u16CurSample)) >> 31ul);
   q31Temp = *pq31BlockBackward; 
   *(pq31BlockBackward--) = (q31_t) clip_q63_to_q31(((q63_t) (q31Temp) * EQUALIZER_GET_EQ_VALUE(pstcEqualizer, u16CurSample)) >> 31ul);

  }
}

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
