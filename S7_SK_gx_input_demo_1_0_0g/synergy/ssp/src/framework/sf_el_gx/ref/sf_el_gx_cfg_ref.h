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
* File Name    : sf_el_gx_cfg_ref.h
* Description  : SSP GUIX adaptation framework Configuration file
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup SF_EL_GX
 * @defgroup SF_EL_GX_CFG Build Time Configurations
 * @{
***********************************************************************************************************************/

#ifndef SF_EL_GX_CFG_H
#define SF_EL_GX_CFG_H

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** Specify whether to include code for API parameter checking. Valid settings include:
 *    - BSP_CFG_PARAM_CHECKING_ENABLE : Utilizes the system default setting from bsp_cfg.h
 *    - 1 : Includes parameter checking
 *    - 0 : Compiles out parameter checking
 */
#define SF_EL_GX_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)

/** Specify whether to enable the 2DG hardware acceleration.
 *    - 1 : Enable
 *    - 0 : Disable
 */
#define SF_EL_GX_CFG_RENDERING_SUPPORT_2DG_ENABLE    (0)

/** Specify whether to enable the JPEG hardware acceleration.
 *    - 1 : Enable
 *    - 0 : Disable
 */
#define SF_EL_GX_CFG_RENDERING_SUPPORT_JPEG_ENABLE   (0)

/** Specify whether to enable the direct rendering by JPEG on the frame buffer.
 *    - 1 : Enable direct rendering. Decoded image by JPEG will be rendered on the frame buffer directly. User must be
 *          careful for JPEG hardware limitation for the pixel alignment and the decoded image must not beyond the
 *          width/height of screen. However, users can obtain maximum performance.
 *    - 0 : Disable direct rendering. The work buffer for JPEG is used for JPEG decoding image once and then the image
 *          will be copied to the frame buffer helped by 2DG. Users do not care for the JPEG hardware limitation and
 *          image will be automatically clipped not to over the screen. However, users have to allow the extra bus
 *          bandwidth consumption.
 */
#define SF_EL_GX_CFG_JPEG_DIRECT_DRAW_SUPPORT_ENABLE (0)

/** Specify the size in bytes for the work buffer to be used by JPEG processing. */
#define SF_EL_GX_CFG_JPEG_WORK_BUFFER_SIZE           (1536000)

#endif /* SF_EL_GX_CFG_H */

/*******************************************************************************************************************//**
 * @} (end defgroup SF_EL_GX_CFG)
***********************************************************************************************************************/
