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

/*****************************************************************************/
/** \file i2s_codec_WM8731.c
 **
 ** I2S codec support functions
 **
 ** History:
 ** - 2014-09-15  1.0   CNo  First version
 ** - 2014-09-xx  1.1   MWi  Continued work
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

/*R0 LEFT LINE IN*/
#define WM8731_R0_LINVOL        (1 << 0)
#define WM8731_R0_LINVOL1       (1 << 1)
#define WM8731_R0_LINVOL2       (1 << 2)
#define WM8731_R0_LINVOL3       (1 << 3)
#define WM8731_R0_LINVOL4       (1 << 4)
#define WM8731_R0_LINMUTE       (1 << 5)
#define WM8731_R0_LRINBOTH      (0 << 6)

/*R1 RIGHT LINE IN*/
#define WM8731_R1_RINVOL        (1 << 0)
#define WM8731_R1_RINVOL1       (1 << 1)
#define WM8731_R1_RINVOL2       (1 << 2)
#define WM8731_R1_RINVOL3       (1 << 3)
#define WM8731_R1_RINVOL4       (1 << 4)
#define WM8731_R1_RINMUTE       (1 << 5)
#define WM8731_R1_RLINBOTH      (0 << 6)

/*R2 LEFT HEADPHONE OUT*/
#define WM8731_R2_LHPVOL        (1 << 0)
#define WM8731_R2_LHPVOL1       (1 << 1)
#define WM8731_R2_LHPVOL2       (1 << 2)
#define WM8731_R2_LHPVOL3       (1 << 3)
#define WM8731_R2_LHPVOL4       (1 << 4)
#define WM8731_R2_LHPVOL5       (1 << 5)
#define WM8731_R2_LHPVOL6       (1 << 6)
#define WM8731_R2_LZCEN         (0 << 7)
#define WM8731_R2_LRHPBOTH      (0 << 8)

/*R3 RIGHT HEADPHONE OUT*/
#define WM8731_R3_RHPVOL        (1 << 0)
#define WM8731_R3_RHPVOL1       (1 << 1)
#define WM8731_R3_RHPVOL2       (1 << 2)
#define WM8731_R3_RHPVOL3       (1 << 3)
#define WM8731_R3_RHPVOL4       (1 << 4) 
#define WM8731_R3_RHPVOL5       (1 << 5)
#define WM8731_R3_RHPVOL6       (1 << 6)  //111111 +6dB
#define WM8731_R3_RZCEN         (0 << 7)  //Disable Zero Cross detect
#define WM8731_R3_RLHPBOTH      (0 << 8)  //Disable Simultaneous Load

/*R4 ANALOGUE AUDIO PATH CONTROL*/
#define WM8731_R4_MICBOOST      (1 << 0)
#define WM8731_R4_MUTEMIC       (1 << 1)
#define WM8731_R4_INSEL         (0 << 2)
#define WM8731_R4_BYPASS        (0 << 3)
#define WM8731_R4_DACSEL        (1 << 4)
#define WM8731_R4_SIDETONE      (0 << 5)
#define WM8731_R4_SIDEATT0      (0 << 6)
#define WM8731_R4_SIDEATT1      (0 << 7)

/*R5 DIGITAL AUDIO PATH CONTROL*/
#define WM8731_R5_ADCHPD        (0 << 0)
#define WM8731_R5_DEEMPH0       (0 << 2)
#define WM8731_R5_DEEMPH1       (0 << 2)
#define WM8731_R5_DACMU         (0 << 3)
#define WM8731_R5_HPOR          (0 << 3)

/*R6 POWER DOWN CONTROL*/
#define WM8731_R6_LINEINPD      (1 << 0)
#define WM8731_R6_MICPD         (1 << 1)
#define WM8731_R6_ADCPD         (1 << 2)
#define WM8731_R6_DACPD         (0 << 3)
#define WM8731_R6_OUTPD         (0 << 4)
#define WM8731_R6_OSCPD         (0 << 5)
#define WM8731_R6_CLKOUTPD      (0 << 6)
#define WM8731_R6_POWEROFF      (0 << 7)

/*R7 DIGITAL AUDIO INTERFACE CONTROL*/
#define WM8731_R7_FORMAT0       (0 << 0)
#define WM8731_R7_FORMAT1       (1 << 1)
#define WM8731_R7_IWL0          (0 << 2)
#define WM8731_R7_IWL1          (0 << 3)
#define WM8731_R7_LRP           (0 << 4)
#define WM8731_R7_LRSWAP        (0 << 5)
#define WM8731_R7_MS            (0 << 6)
#define WM8731_R7_BCLKINV       (0 << 7)

/*R8 SAMPLING CONTROL*/
#define WM8731_R8_USBNORMAL     (0 << 0)
#define WM8731_R8_BOSR          (0 << 1)
#define WM8731_R8_SR0           (0 << 2)
#define WM8731_R8_SR1           (0 << 3)
#define WM8731_R8_SR2           (0 << 4)
#define WM8731_R8_SR3           (1 << 5)
#define WM8731_R8_CLKIDIV       (0 << 6)
#define WM8731_R8_CLKODIV       (0 << 7)

/*R9 ACTIVE CONTROL*/
#define WM8731_R9_ACTIVATE        (1 << 0)
#define WM8731_R9_0               (0 << 1)
#define WM8731_R9_1               (0 << 2)
#define WM8731_R9_2               (0 << 3)
#define WM8731_R9_3               (0 << 4)
#define WM8731_R9_4               (0 << 5)
#define WM8731_R9_5               (0 << 6)
#define WM8731_R9_6               (0 << 7)
#define WM8731_R9_7               (0 << 8)

#define WM8731_R9_DEACTIVATE      (0 << 0)

/*R15 RESET CONTROL*/
#define WM8731_RESET             (0 << 0)

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

  I2cStart(WM8731_ADDRESS << 1 );               // Start I2C2 module
  I2cSendByte((unsigned char)((address << 1) | ((cmd>>8) & 0x1)));  // Write register address
  I2cSendByte((unsigned char)(cmd & 0xFF));     // Write command
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
    
    WM8731_CMD(WM8731_REG_RESET,         0x00);     // Reset module  
    WM8731_CMD(WM8731_REG_LHPHONE_OUT,   0x5F);     // Left headphone out settings
    WM8731_CMD(WM8731_REG_RHPHONE_OUT,   0x5F);     // Right headphone out settings
    WM8731_CMD(WM8731_REG_ANALOG_PATH,   0x10);     // Analog paths
    WM8731_CMD(WM8731_REG_DIGITAL_PATH,  0x06);     // Digital paths
    WM8731_CMD(WM8731_REG_PDOWN_CTRL,    0x00);     // Power down control
    WM8731_CMD(WM8731_REG_DIGITAL_IF,    0x42);     // DI2S mode, 16 bits, master
    WM8731_CMD(WM8731_REG_SAMPLING_CTRL, 0x00);     // Sampling control: 256fs @ 48kHz  
    WM8731_CMD(WM8731_REG_ACTIVE_CTRL,   0x01);     // Activate module
    
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
en_result_t I2scodec_SetHpVolume(uint8_t u8LeftVol, uint8_t RightVol)
{
  WM8731_CMD(WM8731_REG_LHPHONE_OUT, u8LeftVol & 0x7Fu);
  WM8731_CMD(WM8731_REG_RHPHONE_OUT, RightVol  & 0x7Fu); 
  
  return Ok;
}
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
