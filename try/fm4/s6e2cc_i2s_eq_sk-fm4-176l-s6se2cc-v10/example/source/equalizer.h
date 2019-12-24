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
/** \file equalizer.h
 **
 ** A detailed description is available at 
 ** @link EqualizerGroup - description @endlink
 **
 ** History:
 **   - 2015-12-02  V1.0  Jan Weber  First Version
 *****************************************************************************/

#ifndef __EQUALIZER_H__
#define __EQUALIZER_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup EqualizerGroup -
 **
 ** Provided functions of equalizer:
 ** 
 ** - Equalizer_Init()
 ** - Equalizer_Deinit()
 ** - int8_t Equalizer_Set(stc_Equalizer_t* pstcEqualizer, const en_EqualizerRange_t enEqRange, int8_t i8Value);
 ** - void Equalizer_ProcessBlock(const stc_Equalizer_t* pstcEqualizer, q31_t* pq31Block);
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

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
#define EQUALIZER_EQ_RANGE                                          3u
#define EQUALIZER_BASS_MIN                                          0u
#define EQUALIZER_BASS_MAX                                          8u    //<-- Bass/Midrange band frequency bound (adjust here)
#define EQUALIZER_MIDRANGE_MIN                (EQUALIZER_BASS_MAX + 1u)
#define EQUALIZER_MIDRANGE_MAX                                     32u    //<-- Treble/Midrange band frequency bound  (adjust here)
#define EQUALIZER_TREBLE_MIN              (EQUALIZER_MIDRANGE_MAX + 1u)
#define EQUALIZER_TREBLE_MAX                      (BUFFER_EQ_SIZE - 1u)
#define EQUALIZER_STEPS                                            17u
#define EQUALIZER_STEPS_CENTER                  (EQUALIZER_STEPS/2u+1u)
#define EQUALIZER_STEPS_MAX         ((int8_t)(EQUALIZER_STEPS_CENTER-1))
#define EQUALIZER_STEPS_MIN        (-(int8_t)(EQUALIZER_STEPS_CENTER-1))
  
#define EQUALIZER_GET_EQ_VALUE(eqInstance, x) (x > EQUALIZER_MIDRANGE_MAX ? eqInstance->aq31MappingInt8ToQ31[eqInstance->u8EqualizerControl[TREBLE]] : x > EQUALIZER_BASS_MAX ? eqInstance->aq31MappingInt8ToQ31[eqInstance->u8EqualizerControl[MIDRANGE]] : eqInstance->aq31MappingInt8ToQ31[eqInstance->u8EqualizerControl[BASS]])

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
typedef enum en_EqualizerRange
{
  BASS, 
  MIDRANGE, 
  TREBLE
} en_EqualizerRange_t;
   
typedef struct stc_Equalizer
{
  uint8_t u8EqualizerControl[EQUALIZER_EQ_RANGE];
  q31_t   aq31MappingInt8ToQ31[EQUALIZER_STEPS];
} stc_Equalizer_t;
/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
extern void Equalizer_Init(stc_Equalizer_t* pstcEqualizer);
extern void Equalizer_Deinit(stc_Equalizer_t* pstcEqualizer);
extern int8_t Equalizer_Set(stc_Equalizer_t* pstcEqualizer, const en_EqualizerRange_t enEqRange, int8_t i8Value);
extern void Equalizer_ProcessBlock(const stc_Equalizer_t* pstcEqualizer, q31_t* pq31Block);

#ifdef __cplusplus
}
#endif

//@} // EqualizerGroup

#endif /* __EQUALIZER_H__*/

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

