/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : hw_fmi_private.h
 * Description  : FMI
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup FMI
 * @{
 **********************************************************************************************************************/

#ifndef HW_FMI_PRIVATE_H
#define HW_FMI_PRIVATE_H

#if defined(__GNUC__) && defined(__ARM_EABI__)
#define INLINE_ATTRIBUTE __attribute__((always_inline))
#else
#define INLINE_ATTRIBUTE
#endif

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_fmi.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum
{
	FMI_PRODUCT_INFORMATION,
	FMI_IP_INFORMATION,
	FMI_SW_PROVISIONING,
	FMI_TYPE_COUNT
} fmi_types_t;

typedef struct
{
	fmi_header_t header;
	uint32_t offset[FMI_TYPE_COUNT];
} fmi_table_t;

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/

__STATIC_INLINE uint32_t HW_FMI_RecordLocate(fmi_types_t type) INLINE_ATTRIBUTE;

#endif /* HW_FMI_PRIVATE_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup FMI)
 **********************************************************************************************************************/
