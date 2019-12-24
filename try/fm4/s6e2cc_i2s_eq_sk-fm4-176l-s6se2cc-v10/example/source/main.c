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
/** \file main.c
 **
 ** Main Module
 **
 ** \brief This example implements an software equalizer.
 **        The audio codec WM8731 on the SK-FM4-176L-S6SE2CC board is used 
 **        to read the line-in and play the processed signal at the headphone
 **        connector.
 **        The sound is recorded and played at 48 kHz sampling rate.
 **        
 **
 ** History:
 **   - 2014-12-18  1.0  Jan Weber  First version.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "globals.h"
#include "mcu.h"
#include "i2scodec_WM8731.h"
#include "buffer.h"
#include "windowfunction.h"
#include "equalizer.h"
#include "userio.h"
#include "arm_math.h"
#if (1 == IDLEMEASUREMENT_ON)
#include "idlemeasurement.h"
#endif

/*****************************************************************************/
/* Local pre-processor symbols/macros ('define')                             */
/*****************************************************************************/
#define CFFT_FORWARD         0u
#define CFFT_INVERSE         1u
#define CFFT_NO_BIT_REVERSE  0u
#define CFFT_BIT_REVERSE     1u   

/*****************************************************************************/
/* Global variable definitions                                               */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
//static stc_Equalizer_t         stcEqualizer;
#if (BUFFER_ON == 1)
static stc_Buffer_t            stcOutputBuffer;
static stc_Buffer_t            stcInputBuffer;
static stc_WindowFunction_t    stcWindowFunction;
static stc_BufferEq_t          stcBufferEq;
static boolean_t               bOutputReady = FALSE;
#if (EQUALIZER_ON == 1)
static stc_Equalizer_t         stcEqualizer;
static stc_UserIO_t            stcUserIO;
static en_EqualizerRange_t     enEqRange;
static int8_t                  i8EqValue;  
#endif
#endif


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
  
  // Overclock PLL for WM8731 minimum input frequency of 8 MHz
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

  // Clock divider configuration: Bypass: Use WM8731 clock as it is
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
  
  // I2SMCLK to output (for WM8731)
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
  FM4_I2S0->INTCNT_f.RXFIM  = 0u;        // Rx FIFO interrupt
  
  // Tx FIFO thresholds
  FM4_I2S0->INTCNT_f.TFTH = 0x00u;  // Immediately (no FIFO used)
  FM4_I2S0->INTCNT_f.RFTH = 0x00u;  // Immediately (no FIFO used)
  
  // No packet receive completion timer
  FM4_I2S0->INTCNT_f.RPTMR = 0u;
  
  // Enable I2S Transmitter
  FM4_I2S0->OPRREG_f.TXENB = 1u;
  FM4_I2S0->CNTREG_f.TXDIS = 0u;
      
  // Disable I2S Receiver
  FM4_I2S0->OPRREG_f.RXENB = 1u;
  FM4_I2S0->CNTREG_f.RXDIS = 0u;
  
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
 ** This function reads the Rx-FIFO of the I2S to the Input Buffer and 
 ** writes the Tx-FIFO  of the I2S peripheral.
 **
 ******************************************************************************/
void PRGCRC_I2S_IRQHandler(void)
{

  uint32_t u32IrqMon = FM4_INTREQ->IRQ117MON;
  uint32_t u32I2SStatus = FM4_I2S0->STATUS;
  uint16_t au16Sample[BUFFER_CHANNEL_COUNT];
  uint32_t u32I2SData;
  
  if (0x000000001ul == (u32IrqMon & 0x000000001ul))
  {
#if (BUFFER_ON == 1)
    if(TRUE == bOutputReady) // on startup: wait until the output buffer is filled entirely.
    {
    // check whether TX-FIFO should be refilled: 
    if(0u != (FM4_I2S0->STATUS & (1ul << 17ul)))
    {
      while(((FM4_I2S0->STATUS & 0xFF00u) >> 8u) < 66u)
      {
        
        if(SUCCESSFULL == Buffer_GetNextSample(&stcOutputBuffer, &au16Sample[0u]))
        {
          FM4_I2S0->TXFDAT = ((((uint32_t)au16Sample[BUFFER_CHANNEL_LEFT]) << 16u) | au16Sample[BUFFER_CHANNEL_RIGHT]) ^ 0x80008000ul;
        }
        else
        {
          break;
        } 
      } 
    }
    }
    else
    {
      FM4_I2S0->TXFDAT = 0x80008000ul;  // Play silence
      if(TRUE == Buffer_isFull(&stcOutputBuffer))
      {
        bOutputReady = TRUE;
      }
    }
#endif    

    //check whether RX-FIFO should be read-out: 
    if(0u != (FM4_I2S0->STATUS & (1u << 16u)))
    {
      while(((FM4_I2S0->STATUS & 0xFFu) > 0u) 
#if (BUFFER_ON == 1)       
            && (FALSE == Buffer_isFull(&stcInputBuffer))
#endif            
              )
      {
        u32I2SData = FM4_I2S0->RXFDAT;
        au16Sample[BUFFER_CHANNEL_LEFT] = (u32I2SData >> 16ul) & 0x0000FFFFul; 
        au16Sample[BUFFER_CHANNEL_RIGHT] = u32I2SData & 0x0000FFFFul;

#if (BUFFER_ON == 0)        
        FM4_I2S0->TXFDAT = ((((uint32_t)au16Sample[BUFFER_CHANNEL_LEFT]) << 16ul) | au16Sample[BUFFER_CHANNEL_RIGHT]) ^ 0x80008000ul;
#else
        if(SUCCESSFULL != Buffer_AddSample(&stcInputBuffer, &au16Sample[0u]))
        {
          break;
        }
#endif
      }
    }
  }
  else
  {
    __NOP();    // Ghost interrupt (should never happen in this example)
  }
}

#if (1 == BUFFER_ON && 1 == EQUALIZER_ON)
void HandleUserIO(void)
{
  if(TRUE == UserIO_hasNewInput(&stcUserIO))
  {
    UserIO_getNewInput(&stcUserIO, &enEqRange, &i8EqValue);
    i8EqValue = Equalizer_Set(&stcEqualizer, enEqRange, i8EqValue);
    UserIO_setNewOutput(&stcUserIO, enEqRange, i8EqValue);
  }  
}
#endif

/**
 ******************************************************************************
 ** \brief  Main function of project for MB9B560R series.
 **
 ******************************************************************************/
int32_t main(void) 
{  
#if (BUFFER_ON == 1 && TRANSFORM_ON == 1)
	arm_cfft_radix4_instance_q31  stcCfftInstance;
	uint8_t                       u8CurChannel; 
#endif  // (BUFFER_ON == 1 && TRANSFORM_ON == 1)
	
#if (1 == IDLEMEASUREMENT_ON)
  IdleMeasurement_Init();  
#endif
#if (BUFFER_ON == 1) 
  #if (WINDOWING_ON == 1)
		WindowFunction_Init(&stcWindowFunction);
  #endif // (WINDOWING_ON == 1)
  Buffer_Init(&stcInputBuffer, INPUTBUFFER, &stcWindowFunction);
  Buffer_Init(&stcOutputBuffer, OUTPUTBUFFER, &stcWindowFunction);
  #if (EQUALIZER_ON == 1) 
  Equalizer_Init(&stcEqualizer);
  UserIO_Init(&stcUserIO);
  #endif // (EQUALIZER_ON == 1) 
#endif // (BUFFER_ON == 1) 
 
  bFM4_CLK_GATING_CKEN2_I2SCK0 = 1u; // enable I2S0 clock peripheral
  
  FM4_GPIO->EPFR24 |= (1ul << 2ul);  // I2S0-MCLK pin to output
  bFM4_GPIO_PFR5_PD = 1u;
        
  FM4_GPIO->EPFR24 |= (1ul << 10ul); // I2S0-DO pin to output
  bFM4_GPIO_PFR5_PE = 1u;
  
  FM4_GPIO->EPFR24 |= (1ul << 8ul); // I2S0-DI pin to input
  bFM4_GPIO_PFR3_P0 = 1u;
          
          
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
      // Initialization of WM8731
      if (0u == I2scodec_Init())
      {         
        
        FM4_I2S0->OPRREG_f.START  = 1u;  // Start of I2S
        
        while(1u)
        {
#if (BUFFER_ON == 1)   

          while(SUCCESSFULL != Buffer_PrepareNextBlock(&stcInputBuffer, &stcBufferEq))
          {
#if (EQUALIZER_ON == 1)
            HandleUserIO();
#endif
          }
          
#if (IDLEMEASUREMENT_ON == 1)
          IdleMeasurement_Stop(); 
#endif         
          
          
#if (TRANSFORM_ON == 1)         
          for(u8CurChannel = 0u; u8CurChannel < BUFFER_CHANNEL_COUNT; u8CurChannel++)
          {
            if(ARM_MATH_SUCCESS == arm_cfft_radix4_init_q31(&stcCfftInstance, BUFFER_EQ_SIZE, CFFT_FORWARD, CFFT_BIT_REVERSE))
            {
              arm_cfft_radix4_q31(&stcCfftInstance,&stcBufferEq.aq31Data[u8CurChannel][0u]);
#if (EQUALIZER_ON == 1)
              Equalizer_ProcessBlock(&stcEqualizer, &stcBufferEq.aq31Data[u8CurChannel][0u]);
#endif // (EQUALIZER_ON == 1)
              if(ARM_MATH_SUCCESS == arm_cfft_radix4_init_q31(&stcCfftInstance, BUFFER_EQ_SIZE, CFFT_INVERSE, CFFT_BIT_REVERSE))
              {
                arm_cfft_radix4_q31(&stcCfftInstance,&stcBufferEq.aq31Data[u8CurChannel][0u]);
              }
            }
          }
#endif // (TRANSFROM_ON == 1) 
                
#if (IDLEMEASUREMENT_ON == 1)
          IdleMeasurement_Start();
#endif           
          
          
          while(SUCCESSFULL != Buffer_FinalizeNextBlock(&stcOutputBuffer, &stcBufferEq))
          {
#if (EQUALIZER_ON == 1)
            HandleUserIO();
#endif
          }
#endif // (BUFFER_ON == 1)
          
        } // while(1)
      } // if (0u == I2scodec_Init())
    } // if (0u == I2sInit()) 
  } // if (0u == I2sClockInit())
}

/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
