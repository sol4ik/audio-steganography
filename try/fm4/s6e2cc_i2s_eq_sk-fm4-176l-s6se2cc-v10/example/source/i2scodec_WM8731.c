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

/*****************************************************************************/
/** \file i2s_codec_WM8731.c
 **
 ** I2S codec support functions
 **
 ** History:
 ** - 2014-09-15  1.0   CNo  First version
 ** - 2014-09-17  1.1   MWi  Continued work
 ** - 2014-10-30  1.2   JWi  Modify for WM8731
 ** - 2014-11-05  1.3   MWi  I2scodec_SetHpVolume() added, some codec values
 **                          changed
 *****************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "i2scodec_WM8731.h"
#include "i2c.h"

/******************************************************************************/
/* Local function prototypes                                                  */
/******************************************************************************/

/**
 ******************************************************************************
 ** I2S codec  I2C defines
 ******************************************************************************/
#define WM8731_ADDRESS             (0x1A)       // WM8731 chip address on I2C bus

#define WM8731_REG_LLINE_IN        (0x00)       // Left Channel Line Input Volume Control
#define WM8731_REG_RLINE_IN        (0x01)       // Right Channel Line Input Volume Control
#define WM8731_REG_LHPHONE_OUT     (0x02)       // Left Channel Headphone Output Volume Control
#define WM8731_REG_RHPHONE_OUT     (0x03)       // Right Channel Headphone Output Volume Control
#define WM8731_REG_ANALOG_PATH     (0x04)       // Analog Audio Path Control
#define WM8731_REG_DIGITAL_PATH    (0x05)       // Digital Audio Path Control
#define WM8731_REG_PDOWN_CTRL      (0x06)       // Power Down Control Register
#define WM8731_REG_DIGITAL_IF      (0x07)       // Digital Audio Interface Format
#define WM8731_REG_SAMPLING_CTRL   (0x08)       // Sampling Control Register
#define WM8731_REG_ACTIVE_CTRL     (0x09)       // Active Control
#define WM8731_REG_RESET           (0x0F)       // Reset register

#define WM8731_R0_LRINBOTH      (0 << 6)
#define WM8731_R2_LRHPBOTH      (0 << 8)

/**
 **************************************************************************************************
 ** \brief Function write word of data to WM8731
 ** \param [in]  address   register address
 ** \param [in]  cmd       data
**************************************************************************************************/
void WM8731_CMD(char address, unsigned int cmd)
{

  // B[15:9] Are Control Address Bits
  // B[8:0]  Are Control Data Bits

  I2cStart(WM8731_ADDRESS << 1u );               // Start I2C2 module
  I2cSendByte((unsigned char)((address << 1u) | ((cmd >> 8u) & 0x1u)));  // Write register address
  I2cSendByte((unsigned char)(cmd & 0xFFu));     // Write command
  I2cWriteStop();                               // Stop I2C2 module
}
/**
 ******************************************************************************
 ** \brief Initialize the external I²S codec WM8731 via I2C
 **
 ** \return   Ok   Coded initialized
 ******************************************************************************/
en_result_t I2scodec_Init(void)
{
    InitI2c2();
    
    WM8731_CMD(WM8731_REG_RESET,         0x00u);     // Reset module  
    WM8731_CMD(WM8731_REG_LHPHONE_OUT,   0x79u);     // Left headphone out settings
    WM8731_CMD(WM8731_REG_RHPHONE_OUT,   0x79u);     // Right headphone out settings
    WM8731_CMD(WM8731_REG_LLINE_IN,     0x117u);     // Left & Right line in to 0dB & unmute    
    WM8731_CMD(WM8731_REG_ANALOG_PATH,   0x10u);     // Analog paths
    WM8731_CMD(WM8731_REG_DIGITAL_PATH,  0x06u);     // Digital paths
    WM8731_CMD(WM8731_REG_PDOWN_CTRL,    0x00u);     // Power down control
    WM8731_CMD(WM8731_REG_DIGITAL_IF,    0x42u);     // DI2S mode, 16 bits, master
    WM8731_CMD(WM8731_REG_SAMPLING_CTRL, 0x00u);     // Sampling control: 256fs @ 48kHz  
    WM8731_CMD(WM8731_REG_ACTIVE_CTRL,   0x01u);     // Activate module
    
    bFM4_GPIO_DDR3_PC = 1u;         // Output
    bFM4_GPIO_PDOR3_PC = 0u;        // Analog switch to DACLRC
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set headphone volume of I²S codec WM8731 via I2C
 **
 ** \return   Ok   Coded initialized
 ******************************************************************************/
en_result_t I2scodec_SetHpVolume(uint8_t u8LeftVol, uint8_t u8RightVol)
{
  if(u8LeftVol == u8RightVol)
  {
     WM8731_CMD(WM8731_REG_LHPHONE_OUT, ((u8LeftVol & 0x7Fu) | (1u << WM8731_R2_LRHPBOTH)));
  }
  else
  {
      WM8731_CMD(WM8731_REG_LHPHONE_OUT, u8LeftVol & 0x7Fu);
      WM8731_CMD(WM8731_REG_RHPHONE_OUT, u8RightVol  & 0x7Fu);
  }
 
  
  return Ok;
}
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
