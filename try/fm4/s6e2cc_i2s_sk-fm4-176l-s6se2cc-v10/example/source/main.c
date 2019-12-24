/*******************************************************************************
* Copyright (C) 2014 Spansion LLC. All Rights Reserved. 
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
/** \file main.c
 **
 ** Main Module
 **
 ** \brief This example plays the "Pixie Dust" sound of the Android mobile.
 **        The audio codec WM8731 on the SK-FM4-176L-S6SE2CC board is used as
 **        it plays the sound, if the user push button is pressed.
 **        The sound is played at 48 kHz sampling rate.
 **        Both line out and headphones out connectors are unmuted to hear the
 **        sound.
 **
 ** History:
 **   - 2014-12-18  1.0  MWi  First version.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "i2scodec_WM8731.h"
#include "pixiedust_sound_i2s.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('define')                             */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions                                               */
/*****************************************************************************/
const uint16_t *pu16SoundData;
boolean_t bPlaySoundActive = FALSE;

/**
 ******************************************************************************
 ** \brief  Initialization of I2S Clock
 **
 ** \return  int8_t  0: Ok, -1: Error
 ******************************************************************************/
static int8_t I2sClockInit(void)
{
  uint32_t u32TimeOut = 1000000ul;
  
  // I2S Clock off
  FM4_I2SPRE->ICCR_f.ICEN = 0u; 
  
  // Clock off within time out?
  while((0u != u32TimeOut) && (1u == FM4_I2SPRE->ICCR_f.ICEN))
  {
    u32TimeOut--;
  }
  
  // Too long waited?
  if (0u == u32TimeOut)
  {
    return -1;
  }
  
  // Switch off PLL
  FM4_I2SPRE->IPCR1_f.IPLLEN = 0u;
  
  // Set I2SMCLOCK to output
  FM4_I2SPRE->ICCR_f.ICSEL = 1u;
  
  // Set PLL stabilization wait time to 426 us
  FM4_I2SPRE->IPCR2_f.IPOWT = 4u;
  
  // Overclock PLL for SGTL5000 minimum input frequency of 8 MHz
  FM4_I2SPRE->IPCR3_f.IPLLK = 1u;
  FM4_I2SPRE->IPCR4_f.IPLLN = 31u;
  FM4_I2SPRE->IPCR5_f.IPLLM = 7u;
  
  FM4_I2SPRE->IPINT_ENR_f.IPCSE = 0u; // No interrupt used

  // Enable PLL
  FM4_I2SPRE->IPCR1_f.IPLLEN = 1u;
    
  u32TimeOut = 10000000ul;
  
  // Wait for PLL stabilization
  while((0u != u32TimeOut) && (0u == FM4_I2SPRE->IP_STR_f.IPRDY))
  {
    u32TimeOut--;
  }
  
  // Too long waited?
  if (0u == u32TimeOut)
  {
    // Disable PLL
    FM4_I2SPRE->IPCR1_f.IPLLEN = 0u;
    
    return -1;
  }
  
  // Clock on
  FM4_I2SPRE->ICCR_f.ICEN = 1u;
  
  return 0;
}

/**
 ******************************************************************************
 ** \brief  Initialization of I2S to 44.1 kHz sampling rate 16 bit per channel
 **         PCM A mode.
 **
 ** \return  int8_t  0: Ok, -1: Error
 ******************************************************************************/
static int8_t I2sInit(void)
{
  // Disable I2C channel for configuration
  FM4_I2S0->OPRREG_f.START = 0u;     

  // Clock divider configuration: Bypass: Use SGTL5000 clock as is
  FM4_I2S0->CNTREG_f.CKRT  =  0u;
  
  // No overhead bits in corresponding sub frame channel
  FM4_I2S0->CNTREG_f.OVHD  =  0u;
  
  // No mask bit
  FM4_I2S0->CNTREG_f.MSKB  =  0u;
  
  // Slave mode 
  FM4_I2S0->CNTREG_f.MSMD  =  0u;
  
  // Subframe 0 only
  FM4_I2S0->CNTREG_f.SBFN  =  0u;
    
  // FIFO two 16bit half words
  FM4_I2S0->CNTREG_f.RHLL  =  1u; 
  
  // I2SMCLK to output (for SGTL5000)
  FM4_I2S0->CNTREG_f.ECKM  =  1u;
  
  // No bit extension
  FM4_I2S0->CNTREG_f.BEXT  =  0u; 
  
  // Free run mode
  FM4_I2S0->CNTREG_f.FRUN  =  1u; 
  
  // Shifting order: MSB first
  FM4_I2S0->CNTREG_f.MLSB  =  0u; 
  
  // Sampling point of data reception (not used for TX only)
  FM4_I2S0->CNTREG_f.SMPL  =  0u; 
  
  // Clock polarity (not used for TX only)
  FM4_I2S0->CNTREG_f.CPOL  =  0u; 
  
  // I2S word select 1st bit before frame
  FM4_I2S0->CNTREG_f.FSPH  =  0u; 
  
  // I2SWS: L/R
  FM4_I2S0->CNTREG_f.FSLN  =  1u; 
  
  // I2SWS at '0'
  FM4_I2S0->CNTREG_f.FSPL  =  1u;
  
  // Configuration for Sub Frame 0
  FM4_I2S0->MCR0REG_f.S0CHN  =  1u;      // 1 channel sub frame 
  FM4_I2S0->MCR0REG_f.S0CHL  =  31u;     // 32 bit length 
  FM4_I2S0->MCR0REG_f.S0WDL  =  15u;     // 16 bit word length 

  // Configuration for Sub Frame 1
  FM4_I2S0->MCR0REG_f.S1CHN  =  1u;      // 1 channel sub frame 
  FM4_I2S0->MCR0REG_f.S1CHL  =  31u;     // 32 bit length 
  FM4_I2S0->MCR0REG_f.S1WDL  =  15u;     // 16 bit word length 
  
  // Set active channels for Sub Frame 0   
  FM4_I2S0->MCR1REG  =  3ul;             // Enable channel 0 and 1
  
  // Set active channels for Sub Frame 1   
  FM4_I2S0->MCR2REG  =  3ul;             // Enable channel 0 and 1
  
  // Interrupts (0 = do not mask interrupt (enable), 1 = mask interrupt (disable)
  FM4_I2S0->INTCNT_f.TXUD0M = 0u;        // Tx FIFO 0 underflow enable interrupt
  FM4_I2S0->INTCNT_f.TXUD1M = 0u;        // Tx FIFO 1 underflow enable interrupt
  FM4_I2S0->INTCNT_f.TBERM  = 1u;        // No Tx block size error interrupt
  FM4_I2S0->INTCNT_f.FERRM  = 1u;        // No Tx Frame error interrupt
  FM4_I2S0->INTCNT_f.TXOVM  = 1u;        // No Tx FIFO overflow interrupt
  FM4_I2S0->INTCNT_f.TXFDM  = 1u;        // No Tx DMA
  FM4_I2S0->INTCNT_f.TXFIM  = 0u;        // Tx FIFO interrupt
  FM4_I2S0->INTCNT_f.RBERM  = 1u;        // No Rx block size error interrupt
  FM4_I2S0->INTCNT_f.RXUDM  = 1u;        // No Rx FIFO overflow interrupt
  FM4_I2S0->INTCNT_f.RXOVM  = 1u;        // No Rx FIFO overflow interrupt
  FM4_I2S0->INTCNT_f.RXFDM  = 1u;        // No Rx DMA
  FM4_I2S0->INTCNT_f.EOPM   = 1u;        // No EOPI interrupt
  FM4_I2S0->INTCNT_f.RXFIM  = 1u;        // No Rx FIFO interrupt
  
  // Tx FIFO thresholds
  FM4_I2S0->INTCNT_f.TFTH = 0x00u;  // Immediately (no FIFO used)
  FM4_I2S0->INTCNT_f.RFTH = 0x00u;  // Immediately (no FIFO used)
  
  // No packet receive completion timer
  FM4_I2S0->INTCNT_f.RPTMR = 0u;
  
  // Enable I2S Transmitter
  FM4_I2S0->OPRREG_f.TXENB = 1u;
  FM4_I2S0->CNTREG_f.TXDIS = 0u;
      
  // Disable I2S Receiver
  FM4_I2S0->OPRREG_f.RXENB = 0u;
  FM4_I2S0->CNTREG_f.RXDIS = 1u;
  
  // Globally enable I2S interrupt
  NVIC_ClearPendingIRQ(PRGCRC_I2S_IRQn);
  NVIC_EnableIRQ(PRGCRC_I2S_IRQn);
  NVIC_SetPriority(PRGCRC_I2S_IRQn, 8u);  
  
  return 0;
}

/**
 ******************************************************************************
 ** \brief  Interrupt service routine for I2S
 **
 ** This function calculates a triangular wave form for the ADC of the
 ** SGTL5000 and fills the I2S transmission FIFO.
 ******************************************************************************/
void PRGCRC_I2S_IRQHandler(void)
{
  uint32_t u32IrqMon = FM4_INTREQ->IRQ117MON;
  uint16_t u16OutLeft;  
  uint16_t u16OutRight;
  
  if (0x000000001ul == (u32IrqMon & 0x000000001ul))
  {
    if (TRUE == bPlaySoundActive)
    {
      u16OutLeft =  *pu16SoundData++;
      u16OutRight = *pu16SoundData++;

      if (0xFFFFu == *pu16SoundData)     // End of sound reached?
      {
        bPlaySoundActive = FALSE;
      }
      else
      {
        FM4_I2S0->TXFDAT = u16OutLeft | (u16OutRight << 16);
      }
    }
    else
    {
        FM4_I2S0->TXFDAT = 0x80008000ul;  // Play silence
    }
  }
  else
  {
    __NOP();    // Ghost interrupt (should never happen in this example)
  }
}

/**
 ******************************************************************************
 ** \brief  Main function of project for MB9B560R series.
 **
 ******************************************************************************/
int32_t main(void) 
{
  bFM4_CLK_GATING_CKEN2_I2SCK0 = 1u; // enable I2S0 clock peripheral
  
  FM4_GPIO->EPFR24 |= (1ul << 2ul);  // I2S0-MCLK pin to output
  bFM4_GPIO_PFR5_PD = 1u;
        
  FM4_GPIO->EPFR24 |= (1ul << 10ul); // I2S0-DO pin to output
  bFM4_GPIO_PFR5_PE = 1u;
          
  FM4_GPIO->EPFR24 |= (1ul << 4ul);  // I2S0-CK pin to input/output (input used)
  bFM4_GPIO_PFR3_P1 = 1u;
        
  FM4_GPIO->EPFR24 |= (1ul << 6ul);  // I2S0-WS pin to input/output (input used)
  bFM4_GPIO_PFR5_PF = 1u;
  
  bFM4_GPIO_DDR2_P0 = 0u;            // User button to input
  
  // Initialize I2S clock
  if (0u == I2sClockInit())
  {
    // Initialize I2S
    if (0u == I2sInit())
    {
      // Initialization of SGTL5000
      if (0u == I2scodec_Init())
      {
        FM4_I2S0->OPRREG_f.START  = 1u;  // Start of I2S

        while(1)
        {       
          // If center button pressed and no active playing => Start to play sound
          if ((FALSE == bPlaySoundActive) && (0u == bFM4_GPIO_PDIR2_P0))
          {
            // Initialize buffer index
            pu16SoundData = &au16PixieDustSoundI2s[0];
            bPlaySoundActive = TRUE;
          }
        } // while(1)
      } // if (0u == I2scodec_Init())
    } // if (0u == I2sInit()) 
  } // if (0u == I2sClockInit())
}

/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
