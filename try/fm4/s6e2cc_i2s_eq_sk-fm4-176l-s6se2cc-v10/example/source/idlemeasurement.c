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
/** \file IdleMeasurement.c
 **
 ** A detailed description is available at 
 ** @link IdleMeasurementGroup Measures the Idle time of the project description @endlink
 **
 ** History:
 **   - 2015-20-02  V1.0  JWe  First Version
 *****************************************************************************/
#define __IDLEMEASUREMENT_C__
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "idlemeasurement.h"

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
static uint32_t u32UpTime;
static uint32_t u32IdleTime;
static boolean_t bIdle;
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
void IdleMeasurement_Init(void)
{
  u32UpTime = 0u;
  u32IdleTime = 0u;
  bIdle = FALSE;
  
  /* Setup MFT FRT interupt for idle measurement */   
  FM4_MFT0_FRT->TCSC0 = 0x00000000ul;
  FM4_MFT0_FRT->TCSA0 = 0x00002010ul;
   
  NVIC_ClearPendingIRQ(FRT0_ZERO_IRQn);
  NVIC_EnableIRQ(FRT0_ZERO_IRQn);
  NVIC_SetPriority(FRT0_ZERO_IRQn, 6u); 

}

/**
 ******************************************************************************
 ** \brief Deinitialization Routine
 **
 ******************************************************************************/
void IdleMeasurement_Deinit(void)
{
  /* nothing to do here */
}


/**
 ******************************************************************************
 ** \brief Call it as soon as the MCU gets idle
 **
 ******************************************************************************/
void IdleMeasurement_Start(void)
{
  bIdle = TRUE;
}

/**
 ******************************************************************************
 ** \brief Call it as soon as the MCU stops to be idle
 **
 ******************************************************************************/
void IdleMeasurement_Stop(void)
{
  bIdle = FALSE;  
}

/**
 ******************************************************************************
 ** \brief Call this to get the MCU load
 **
 ** \return uint8_t cpu load in percentage 
 ******************************************************************************/
uint8_t IdleMeasurement_GetCPULoad()
{  
  return (100u - (u32IdleTime * 100u / u32UpTime)); 
}


/**
 ******************************************************************************
 ** \brief  Interrupt service routine for idle measurement
 ******************************************************************************/
void FRT0_ZERO_IRQHandler(void) 
{

    uint32_t u32IrqMon = FM4_INTREQ->IRQ025MON;    
  
    if( 0x00000001ul == (u32IrqMon & 0x00000001ul) ) 
    {      
      if(TRUE == bIdle)
      {
       u32IdleTime++;
      }
      
      u32UpTime++;  
      
      
      FM4_MFT0_FRT->TCSA0 &= 0x00002000ul;      
    }  
}

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
