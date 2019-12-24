/*******************************************************************************
* \file		mcu.h
* 
* \version  1.0
*
* \date		7/29/2016
*
* \brief	Contains references to all device-specific header files.
*
********************************************************************************
* Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
* CYPRESS PROVIDES THIS SOFTWARE "AS IS" AND MAKES NO WARRANTY
* OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS SOFTWARE,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
* PURPOSE.
*******************************************************************************/

#ifndef __MCU_H__
#define __MCU_H__

/*******************************************************************************
* Device-specific include file
*******************************************************************************/
#ifndef _S6E2CCXH_H_
  #include "s6e2ccxh.h"
#endif

/*******************************************************************************
* System device-specific configuration include file
*******************************************************************************/
#ifndef _SYSTEM_S6E2CC_H_
  #include "system_s6e2cc.h"
#endif

/*******************************************************************************
* Device-specific GPIO macro include file
*******************************************************************************/
#ifndef __GPIO_S6E2CCXH_H__
  #include "gpio_s6e2ccxh.h"
#endif

#endif  /* __MCU_H__ */
