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
/** \file windowfunction.c
 **
 ** A detailed description is available at 
 ** @link WindowFunctionGroup - description @endlink
 **
 ** History:
 **   - 2015-09-02  V1.0  JWe  First Version
 *****************************************************************************/
#define __WINDOWFUNCTION_C__
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "arm_math.h" 
#include <math.h>
#include "windowfunction.h"
#include "globals.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/
#define WINDOW_PI                     3.14159265359f
#define WINDOW_OFFSET                           1.0f
#define WINDOW_ATTENUATION                      0.5f   

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
 ** \brief Initialization Routine of Window Function
 **
 ******************************************************************************/
void WindowFunction_Init(stc_WindowFunction_t* pstcWindowFunction)
{
  float32_t f32CurEdgeValue;
  const float32_t f32MaxEdgeValue = (float32_t)(BUFFER_WINDOW_SIZE - 1u);
  
  memset(&pstcWindowFunction->aq31WindowBuf[0u][0u], 0u, sizeof(pstcWindowFunction->aq31WindowBuf));
  memset(&pstcWindowFunction->aq31DeWindowBuf[0u][0u], 0u, sizeof(pstcWindowFunction->aq31DeWindowBuf));
  
  //Calculation of the cosine window
  for(f32CurEdgeValue = 0.0f; f32CurEdgeValue <= f32MaxEdgeValue; f32CurEdgeValue++)
  {
    pstcWindowFunction->aq31Edge[(uint16_t)f32CurEdgeValue] = 
      FLOAT_TO_Q31(WINDOW_ATTENUATION * 
                   (WINDOW_OFFSET - cos(f32CurEdgeValue / 
                                        f32MaxEdgeValue * WINDOW_PI)));
  }
  
  pstcWindowFunction->pFctWindow = &WindowFunction_WindowPrepare;
  pstcWindowFunction->pFctDeWindow = &WindowFunction_WindowFinalize;  
}

/**
 ******************************************************************************
 ** \brief Deinitialization Routine of Window Function
 **
 ******************************************************************************/
void WindowFunction_Deinit(stc_WindowFunction_t* pstcWindowFunction)
{
  /* nothing to do here */
}

/**
 ******************************************************************************
 ** \brief Windows the input buffer pq31InputBuffer with the given 
 **        window function pstcWindowFunction and writes the result 
 **        to pq31OutputBuffer. (For more details see Appnote)
 **
 ** \param  [in][out] pstcWindowFunction  Window function structure.
 ** \param  [in] pq31InputBuffer          input buffer.
 ** \param  [in] pq31OutputBuffer         output buffer.
 ** \param  [in] u8Channel                the channel to which the given 
 **                                       buffer belongs to.
 **
 ******************************************************************************/
void WindowFunction_WindowPrepare(stc_WindowFunction_t* pstcWindowFunction, q31_t* pq31InputBuffer, q31_t* pq31OutputBuffer, uint8_t u8Channel)
{
  uint16_t       u16CurSample;
  
  /* fill the beginning of the output buffer with the buffered part */  
  for(u16CurSample = 0u; u16CurSample < BUFFER_WINDOW_SIZE; u16CurSample++)
  {
    
    *(pq31OutputBuffer++) = pstcWindowFunction->aq31WindowBuf[u8Channel][u16CurSample]; 
    *(pq31OutputBuffer++) = 0u; // set imag to zero
  }

  /* fill the main part with the input values */
  for(u16CurSample = 0u; u16CurSample < (BUFFER_EQ_NO_WINDOW); u16CurSample++)
  { 
    *pq31OutputBuffer++ = *pq31InputBuffer++;
    *(pq31OutputBuffer++) = 0u; // set imag to zero   
  }
  
  /* copy the end of the inputbuffer to the outputbuffer and in the window prepare buffer */
  for(u16CurSample = 0u; u16CurSample < BUFFER_WINDOW_SIZE; u16CurSample++)
  {
    pstcWindowFunction->aq31WindowBuf[u8Channel][u16CurSample] = *(pq31InputBuffer);
    *(pq31OutputBuffer++) = *(pq31InputBuffer++); 
    *(pq31OutputBuffer++) = 0u; // set imag to zero
  }
}

/**
 ******************************************************************************
 ** \brief DeWindows the input buffer pq31InputBuffer with the given 
 **        window function pstcWindowFunction and writes the result 
 **        to pq31OutputBuffer. (For more details see Appnote)
 **
 ** \param  [in][out] pstcWindowFunction  Window function structure.
 ** \param  [in]      pq31InputBuffer     input buffer.
 ** \param  [out]     pq31OutputBuffer    output buffer.
 ** \param  [in]      u8Channel           the channel to which the given 
 **                                       buffer belongs to.
 **
 ******************************************************************************/
void WindowFunction_WindowFinalize(stc_WindowFunction_t* pstcWindowFunction, q31_t* pq31InputBuffer, q31_t* pq31OutputBuffer, uint8_t u8Channel)
{
  uint16_t       u16CurSample;
  q31_t          q31Temp;
  const uint16_t u16MaxNoWindow = BUFFER_EQ_SIZE - 2u * BUFFER_WINDOW_SIZE;
  
  /* fill the beginning of the output buffer with the buffered falling edge 
     and the rising edge of the input buffer.  */
  for(u16CurSample = 0u; u16CurSample < BUFFER_WINDOW_SIZE; u16CurSample++)
  {
    q31Temp = (q31_t) clip_q63_to_q31(((q63_t) 
      (pstcWindowFunction->aq31Edge[u16CurSample]) * (*pq31InputBuffer++)) >> 31ul); 
    
    *(pq31OutputBuffer++) = (q31_t) clip_q63_to_q31((q63_t) 
      q31Temp + pstcWindowFunction->aq31DeWindowBuf[u8Channel][u16CurSample]);
    
      pq31InputBuffer++; // skip imag part
  }                                          
  
  /* fill the main part with the input values */
  for(u16CurSample = 0u; u16CurSample < u16MaxNoWindow; u16CurSample++)
  {
    *(pq31OutputBuffer++) = *(pq31InputBuffer++);
    pq31InputBuffer++; // skip imag part
  }
  
  /* copy the falling edge of the input buffer to the aq31DeWindowBuf 
     buffer */
  for(u16CurSample = 0u; u16CurSample < BUFFER_WINDOW_SIZE; u16CurSample++)
  {    
    pstcWindowFunction->aq31DeWindowBuf[u8Channel][u16CurSample] = (q31_t) clip_q63_to_q31(((q63_t) 
      (pstcWindowFunction->aq31Edge[BUFFER_WINDOW_SIZE - 1u - u16CurSample]) * (*pq31InputBuffer++)) >> 31ul); 
    
    pq31InputBuffer++; // skip imag part
  }
}

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
