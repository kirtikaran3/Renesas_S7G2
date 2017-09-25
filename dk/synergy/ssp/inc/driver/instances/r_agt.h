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
 * File Name    : r_agt.h
 * Description  : Asynchronous General Purpose Timer (AGT) Module instance header file.
 **********************************************************************************************************************/



#ifndef R_AGT_H
#define R_AGT_H


/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_agt_cfg.h"
#include "r_timer_api.h"
#include "../../../ssp/src/driver/r_agt/hw/hw_agt_private.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* Leading zeroes removed to avoid coding standards violation. */
#define AGT_CODE_VERSION_MAJOR ((uint8_t)1)
#define AGT_CODE_VERSION_MINOR ((uint8_t)0)

#define AGT_MAX_CH             AGT_HW_MAX_CH

/** Maximum number of clock counts in 16 bit timer. */
#define AGT_MAX_CLOCK_COUNTS        (0xFFFFULL)
#define AGT_MILLISECONDS_PER_SECOND (1000ULL)
#define AGT_MICROSECONDS_PER_SECOND (1000000ULL)
#define AGT_NANOSECONDS_PER_SECOND  (1000000000ULL)
#define AGT_KHZ_TO_HZ               (1000ULL)

#define AGT_32BIT_MAX_VALUE         (0xFFFFFFFFULL)

/** Macro for error logger. */
#ifndef AGT_ERROR_RETURN
#define AGT_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), "agt", &s_agt_version)
#endif

/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup AGT AGT
 * @brief Driver for the Asynchronous General Purpose Timer (AGT).
 *
 * @section AGT_SUMMARY Summary
 * Extends @ref TIMER_API.
 *
 * HAL High-Level Driver for accessing and configuring AGT timer modes.
 *
 * The AGT timer functions are used by the Timer to provide timer services.
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** AGT extension data structure.*/
typedef struct st_timer_on_agt_cfg
{
    agt_count_source_t  count_source;  ///< AGT channel clock source. Valid values are: AGT_CLOCK_PCLKB, AGT_CLOCK_LOCO, AGT_CLOCK_FSUB
    bool  agto_output_enabled;         ///< AGTO pin is enabled for output compare (true, false)
    bool  agtio_output_enabled;        ///< AGTIO pin is enabled for output compare (true, false)
    bool  output_inverted;             ///< Output inverted (true, false)
} timer_on_agt_cfg_t;

/*******************************************************************************************************************//**
 * @} (end defgroup AGT)
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const timer_api_t g_timer_on_agt;
/** @endcond */

#endif // R_AGT_H


