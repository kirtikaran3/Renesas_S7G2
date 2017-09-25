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
 * File Name    : r_dtc.h
 * Description  : DTC extension of transfer interface.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup DTC DTC
 * @brief Driver for the Data Transfer Controller (DTC).
 *
 * @section DTC_SUMMARY Summary
 * Extends @ref TRANSFER_API.
 *
 * The Data Transfer Controller allows data transfers to occur in place of or in addition to any interrupt. It does not
 * support data transfers using software start.
 *
 * @note The transfer length is limited to 256 (8 bits) in ::TRANSFER_MODE_BLOCK and ::TRANSFER_MODE_REPEAT.
 * @{
 **********************************************************************************************************************/

#ifndef R_DTC_H
#define R_DTC_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_transfer_api.h"
#include "r_dtc_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define DTC_CODE_VERSION_MAJOR (1)
#define DTC_CODE_VERSION_MINOR (2)

/** Length limited to 256 transfers for repeat and block mode */
#define DTC_REPEAT_BLOCK_MAX_LENGTH (0x100)

/** Length limited to 1024 transfers for normal mode */
#define DTC_NORMAL_MAX_LENGTH (0x400)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** DTC extension of the transfer interface. */

/* --------------------  Begin section using anonymous unions  ------------------- */
#if defined(__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined(__ICCARM__)
#pragma language=extended
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
#pragma warning 586
#else // if defined(__CC_ARM)
#warning Not supported compiler type
#endif // if defined(__CC_ARM)

/** DTC Registers. Same as ::transfer_info_t, but uses register names.
 *  Provided as service to typecast ::transfer_info_t.
 */
typedef struct st_dtc_reg
{
	///* Mode registers */
    struct
    {
        uint32_t : 16;
        ///* Mode register B */
        union
        {
            uint8_t  MRB; ///< Mode Register B
            ///* MRB bits  */
            struct
            {
                uint8_t        : 2;
                uint8_t  DM    : 2;  ///< Transfer Destination Address mode
                uint8_t  DTS   : 1;  ///< DTC Transfer Mode Select
                uint8_t  DISEL : 1;  ///< DTC Interrupt Select
                uint8_t  CHNS  : 1;  ///< DTC Chain Transfer Select
                uint8_t  CHNE  : 1;  ///< DTC CHain Transfer Enable
            }  MRB_b;
        };
        ///* Mode register A */
        union
        {
            uint8_t  MRA; ///< Mode Register A
            ///* MRA bits  */
            struct
            {
                uint8_t     : 2;
                uint8_t  SM : 2;     ///< Transfer Source Address mode
                uint8_t  SZ : 2;     ///< DTC Data Transfer Size
                uint8_t  MD : 2;     ///< DTC Transfer Mode Select
            }  MRA_b;
        };
    };

    void * volatile  SAR;  ///< Source address register
    void * volatile  DAR;  ///< Destination address register
    ///* Transfer count registers */
    struct
    {
        volatile uint16_t  CRB; ///< Transfer count register B
        ///* Transfer count register A */
        union
        {
            uint16_t  CRA;  ///< Transfer count register A
            ///* bits */
            struct
            {
                uint8_t  CRAL;    ///< Transfer counter A lower register
                uint8_t  CRAH;    ///< Transfer counter B upper register
            }  CRA_b;
        };
    };
} dtc_reg_t;

/* --------------------  End of section using anonymous unions  ------------------- */
#if defined(__CC_ARM)
#pragma pop
#elif defined(__ICCARM__)
/* leave anonymous unions enabled */
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
#pragma warning restore
#else // if defined(__CC_ARM)
#warning Not supported compiler type
#endif // if defined(__CC_ARM)

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const transfer_api_t g_transfer_on_dtc;
/** @endcond */

#endif /* R_DTC_H */

/*******************************************************************************************************************//**
 * @} (end defgroup DTC)
 **********************************************************************************************************************/
