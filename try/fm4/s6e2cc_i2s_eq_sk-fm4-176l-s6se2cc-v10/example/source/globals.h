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
/** \file globals.h
 **
 ** A detailed description is available at 
 ** @link GlobalsGroup Stores globals description @endlink
 **
 ** History:
 **   - 2015-06-02  V1.0  Jan Weber  First Version
 *****************************************************************************/

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup GlobalsGroup Stores globals
 **
 ** Provided functions of Stores globals:
 ** 
 **   
 ******************************************************************************/
//@{
    
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "base_types.h"
#include "mcu.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
#define BUFFER_ON          1
#define WINDOWING_ON       1
#define TRANSFORM_ON       1
#define EQUALIZER_ON       1
#define IDLEMEASUREMENT_ON 0
  
#define BUFFER_EQ_SIZE                                            1024u
#define BUFFER_WINDOW_SIZE                                         256u
#if (1 == WINDOWING_ON)
#define BUFFER_IO_SIZE             (BUFFER_EQ_SIZE - BUFFER_WINDOW_SIZE)
#else
#define BUFFER_IO_SIZE                                  (BUFFER_EQ_SIZE)  
#endif
#define BUFFER_COUNT                                                 3u
#define BUFFER_CHANNEL_COUNT                                         2u 
#define BUFFER_CHANNEL_LEFT                                          0u
#define BUFFER_CHANNEL_RIGHT                                         1u
#define BUFFER_EQ_NO_WINDOW   (BUFFER_EQ_SIZE - 2u * BUFFER_WINDOW_SIZE)
  
#define FLOAT_TO_Q31(x) ((q31_t)((x) * 2147483648.0f))
#define Q31_TO_FLOAT(x) ((float32_t) (x) / 2147483648ul)
 
/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

//@} // GlobalsGroup

#endif /* __GLOBALS_H__*/

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
