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
/** \file i2c.c 
 **
 ** \brief Low level I2C functions
 **
 ** History:
 **   - 2014-11-21  1.0  MWi    First version.
 **
 ******************************************************************************/
#include "i2c.h"

/**
 ******************************************************************************
 **  \brief    Initialize MFS2 to I2C mode
 ******************************************************************************/
void InitI2c2(void)
{
  
  // Init I2C ports
  //FM4_GPIO->EPFR07 = 0x00280000ul;
   FM4_GPIO->EPFR07 &= 0xFF00FFFFul;
   FM4_GPIO->EPFR07 |= 0x00280000ul;

  bFM4_GPIO_PFR3_PA = 1u;
  bFM4_GPIO_PFR3_PB = 1u;
      
  FM4_GPIO->PZR3_f.PA = 1u;  // SDA to pseudo open drain

  // Initialize MFS2 I2C mode
  FM4_MFS2->BGR  = 100000000ul / 400000ul - 1ul;  // 400k Bit/s @ 100 MHz
  FM4_MFS2->SMR  = 0x80u;      // operation mode 4, no interrupts, no test
                                //   mode, no wake up
  FM4_MFS2->SCR |= 0x80u;      // Reset MFS2 (UPCL = 1)
  

}

/**
 ******************************************************************************
 **  \brief       Start of I2C Communication with 1st Data Byte
 **
 **  \param [in]  u8Data  Data Byte
 **
 **  \retval      0    0k
 **  \retval      -1   Time-out Error
 **  \retval      -2   Bus Error
 **  \retval      -3   Ack Error
 **  \retval      -5   Trx Error
 ******************************************************************************/
int16_t I2cStart(uint8_t u8Data)
{ 
  uint16_t u16ErrorCount = 0;
  uint16_t u16Timeout;
  
  FM4_MFS2->ISBA = 0x00u;
  FM4_MFS2->ISMK = 0x80u;        // ISMK_EN = 1
  
  FM4_MFS2->TDR  = u8Data;       // Send data to slave (address)
  FM4_MFS2->IBCR = 0x80u;        // MSS = 1: Master Mode
  
  do
  {
    u16Timeout = I2C_TIME_OUT_INT;
    
    while((!(FM4_MFS2->IBCR & 0x01u)) // Wait for transmission complete via INT flag
          && (u16Timeout))
    {                                  
      u16Timeout--;
    }
    
    if (0 == u16Timeout)
    {
      return -1;
    }
    
    if (FM4_MFS2->IBCR & 0x02u)     // BER == 1? ->  error
    {
      return -2;
    }
    
    if (!(FM4_MFS2->IBCR & 0x40u))  // ACT == 0? ->  error
    {
      return -3;
    }
    
    // MSS is set ...
  
    if (FM4_MFS2->IBSR & 0x40u)  // RACK == 1? ->  busy or error
    {
      FM4_MFS2->IBCR = 0xE0u;  // MMS = CSS = ACKE = 1: Try restart (if busy)
      u16ErrorCount++;
      
      if (u16ErrorCount > I2C_TIME_OUT)
      {
        return -1;        // too much retrials
      }
    }
    else
    {
      u16ErrorCount = 0u; 
    }
    
  }while (u16ErrorCount);

  if (!(FM4_MFS2->IBSR & 0x10u))   // TRX == 0? ->  error
  {
     return -5;
  }
  
  return 0;
}

/**
 ******************************************************************************
 **  \brief        Send I2C Byte
 **
 **  \param [in]   u8Data   Byte Data
 **
 **  \retval      0    0k
 **  \retval      -1   Time-out Error
 **  \retval      -2   Bus Error
 **  \retval      -3   Ack Error
 **  \retval      -4   Rack Error
 **  \retval      -5   Trx Error
 ******************************************************************************/
int16_t I2cSendByte(uint8_t u8Data)
{
  uint16_t u16Timeout;
  
  FM4_MFS2->TDR = u8Data;  // Send data to EEPROM
  FM4_MFS2->IBCR = 0xB0u;   // WSEL = 1, ACKE = 1, Clear INT flag
  
  u16Timeout = I2C_TIME_OUT_INT;
  
  while((!(FM4_MFS2->IBCR & 0x01u)) // Wait for transmission complete via INT flag
        && (u16Timeout))
  {                                  
    u16Timeout--;
  }
  
  if (0 == u16Timeout)
  {
    return -1;
  }
   
  if (FM4_MFS2->IBCR & 0x02u)     // BER == 1? ->  error
  {
    return -2;
  }
  
  if (!(FM4_MFS2->IBCR & 0x40u))  // ACT == 0? ->  error
  {
    return -3;
  }
  
  // MSS is set, no reserved address
  
  if (FM4_MFS2->IBSR & 0x40u)     // RACK == 1? ->  error
  {
    return -4;
  }
  
  if (!(FM4_MFS2->IBSR & 0x10u))  // TRX == 0? ->  error
  {
    return -5;
  }
  
  return 0;
}

/**
 ******************************************************************************
 **  \brief        Continue I2C Byte (Iteration)
 **
 **  \param [in]   u8Data   Byte Data
 **
 **  \retval      0    0k
 **  \retval      -1   Time-out Error
 **  \retval      -2   Bus Error
 **  \retval      -3   Ack Error
 **  \retval      -4   Rack Error
 **  \retval      -5   Trx Error
 ******************************************************************************/
int16_t I2cContinueSendByte(uint8_t u8Data)
{
  uint16_t u16Timeout;
  
  FM4_MFS2->TDR = u8Data;  // Send data to EEPROM
  FM4_MFS2->IBCR = 0xE0u;  // Set continuous mode MSS = SCC = ACKE = 1

  u16Timeout = I2C_TIME_OUT_INT;
  
  while((!(FM4_MFS2->IBCR & 0x01u)) // Wait for transmission complete via INT flag
        && (u16Timeout))
  {                                  
    u16Timeout--;
  }
  
  if (0 == u16Timeout)
  {
    return -1;
  }
  
  if (FM4_MFS2->IBCR & 0x02u)     // BER == 1? ->  error
  {
    return -1;
  }
  
  if (!(FM4_MFS2->IBCR & 0x40u))  // ACT == 0? ->  error
  {
    return -3;
  }
  
  // MSS is set, no reserved address
  
  if (FM4_MFS2->IBSR & 0x40u)     // RACK == 1? ->  error
  {
    return -4;
  }

  if (u8Data & 0x01u)              // Read = 0, Write = 1
  {
    if (FM4_MFS2->IBSR & 0x10u)   // TRX != (data & 0x01) ? ->  error
    {    
      return -5;
    }
  }
  else
  {
    if (!(FM4_MFS2->IBSR & 0x10u)) // TRX == (data & 0x01) ? ->  error
    {
      return -5; 
    }
  }
  
  return 0;
}

/**
 ******************************************************************************
 **  \brief    Stop I2C Write Communication
 **
 **  \param    None
 **
 **  \return   Nothing
 ******************************************************************************/
void I2cWriteStop(void)
{
  volatile uint32_t u32Wait = 1000;      // value for no optimization
  
  FM4_MFS2->IBCR = 0x20u;   // MMS = CSS = INT = 0, ACKE = 1

  while(u32Wait--)
  {}
}

/**
 ******************************************************************************
 **  \brief    Stop I2C Read Communication
 **
 **  \param    None
 **
 **  \return   Nothing
 ******************************************************************************/
void I2cReadStop(void)
{
  volatile uint32_t u32Wait = 1000;      // value for no optimization
  
  FM4_MFS2->IBCR = 0x01u;    // INT = 1, MMS = ACKE = 0
  //FM4_MFS2->IBCR_f.MSS = 0u;
  //FM4_MFS2->IBCR_f.ACKE = 1u;
  //FM4_MFS2->IBCR_f.INT = 0u;
  
  while(u32Wait--)
  {}
}

/**
 ******************************************************************************
 **  \brief     Receive I2C Byte
 **
 **  \param     None
 **
 **  \return    Received Data (positive value)
 **  \return    -1 = Time-out Error
 **  \return    -2 = Bus Error
 **  \return    -3 = Ack Error
 **  \return    -4 = Rack Error
 **  \return    -5 = Trx Error
 ******************************************************************************/
int16_t I2cReceiveByte(void)
{ 
  uint8_t  u8Data;
  uint16_t u16ErrorCount = 1;
  uint16_t u16Timeout;
  
  do
  {
    if ((!(FM4_MFS2->IBSR & 0x80u)) && (u16ErrorCount != 1))  // FBT = 1
    {
      u8Data = FM4_MFS2->RDR;
      u16ErrorCount = 0u;
    }
    else
    {
      u16ErrorCount++;
      if (u16ErrorCount > I2C_TIME_OUT) // too much retrails
      {
        return -1; 
      }
      
      FM4_MFS2->IBCR = 0xB0u;   // MMS = 1, WSEL = 1, ACKE = 1, Clear INT flag
      //FM4_MFS2->IBCR = 0xA0u;   // MMS = 1, WSEL = 1, ACKE = 1, Clear INT flag
      
      u16Timeout = I2C_TIME_OUT_INT;
      
      while((!(FM4_MFS2->IBCR & 0x01u)) // Wait for transmission complete via INT flag
            && (u16Timeout))
      {                                  
        u16Timeout--;
      }
      
      if (0u == u16Timeout)
      {
        return -1;
      }
      
      if (FM4_MFS2->IBCR & 0x02u)      // BER == 1? ->  error
      {
        return -2;
      }
      
      if (!(FM4_MFS2->IBCR & 0x40u))   // ACT == 0? ->  error
      {
        return -3;
      }
      
      // MSS is set, no reserved address
      
      if (FM4_MFS2->IBSR & 0x40u)      // RACK == 1? ->  error
      {
        return -4;
      }
      
      if (FM4_MFS2->IBSR & 0x10u)      // TRX == 1? ->  error
      {
        return -5;
      }
    }
  }while(u16ErrorCount);
  
  return (int16_t)u8Data;
}

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
