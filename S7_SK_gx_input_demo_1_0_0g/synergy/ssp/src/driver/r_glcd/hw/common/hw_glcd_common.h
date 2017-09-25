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
 * File Name    : hw_glcd_common.h
 * Description  : GLCD HAL module, LLD functions
 **********************************************************************************************************************/

#ifndef HW_GLCD_COMMON_H
#define HW_GLCD_COMMON_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* Register bit definition for BUS arbitration control */
#define BUSSCNTHMI_ARBMET_RB (0x1U)

/* Registers for Background Frame Control Block */
#define BG_PERI_FV_MASK  (0x7FFU)
#define BG_PERI_FH_MASK  (0x7FFU)
#define BG_SYNC_VP_MASK  (0xFU)
#define BG_SYNC_HP_MASK  (0xFU)
#define BG_VSIZE_VP_MASK (0x7FFU)
#define BG_VSIZE_VW_MASK (0x7FFU)
#define BG_HSIZE_HP_MASK (0x7FFU)
#define BG_HSIZE_HW_MASK (0x7FFU)

/* Register bit definition for Graphics Frame Control Block */
#define GRn_FLM3_LNOFF_MASK   (0xFFFFU)
#define GRn_FLM3_FLNUM_MASK   (0x3FFU)
#define GRn_FLM4_FLOFF_MASK   (0xFFFFFFU)
#define GRn_FLM5_LNNUM_MASK   (0x7FFU)
#define GRn_FLM5_DATANUM_MASK (0xFFFFU)

#define GRn_AB1_DISPSEL_MASK  (0x3U)
#define GRn_AB2_GRCVS_MASK    (0x7FFU)
#define GRn_AB2_GRCVW_MASK    (0x7FFU)
#define GRn_AB3_GRCHS_MASK    (0x7FFU)
#define GRn_AB3_GRCHW_MASK    (0x7FFU)
#define GRn_AB4_ARCVS_MASK    (0x7FFU)
#define GRn_AB4_ARCVW_MASK    (0x7FFU)
#define GRn_AB5_ARCHS_MASK    (0x7FFU)
#define GRn_AB5_ARCHW_MASK    (0x7FFU)
#define GRn_AB6_ARCCOEF_MASK  (0x1FFU)
#define GRn_AB6_ARCRATE_MASK  (0xFFU)
#define GRn_AB7_ARCDEF_MASK   (0xFFU)
#define GRn_CLUTINT_LINE_MASK (0x7FFU)

/* Register bit definition for Output Control Block */
#define GAMX_LUTX_GAIN_MASK            (0x7FFU)
#define GAMX_AREAX_MASK                (0x3FFU)

#define OUT_SET_FRQSEL_NO_DIVISION     (0)
#define OUT_SET_FRQSEL_QUATER_DIVISION (2)

#define OUT_BRIGHT1_BRTG_MASK          (0x3FFU)
#define OUT_BRIGHT2_BRTB_MASK          (0x3FFU)
#define OUT_BRIGHT2_BRTR_MASK          (0x3FFU)

#define OUT_CONTRAST_CONTG_MASK        (0xFFU)
#define OUT_CONTRAST_CONTB_MASK        (0xFFU)
#define OUT_CONTRAST_CONTR_MASK        (0xFFU)

#define TCON_TIM_HALF_MASK             (0x7FFU)
#define TCON_TIM_OFFSET_MASK           (0x7FFU)

#define TCON_STHx1_HS_MASK             (0x7FFU)
#define TCON_STHx1_HW_MASK             (0x7FFU)
#define TCON_STVx1_VS_MASK             (0x7FFU)
#define TCON_STVx1_VW_MASK             (0x7FFU)

#define SYSCNT_PANEL_CLK_DCDR_MASK     (0x3FU)

/** MSTP register accessor macro  */
#define MODULE_STOP ((R_MSTP_Type *) R_MSTP)
#define MSTP_GLCD   (1U << 4)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * General setting, Provides power to the GLCD module.
 * @param[in] none
 * @retval    none
 * @note
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_PowerOn (void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
    MODULE_STOP->MSTPCRC &= (~MSTP_GLCD);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);
}  /* End of function HW_GLCD_PowerOn() */

/*******************************************************************************************************************//**
 * General setting, Removes power from the GLCD module.
 * @param[in] none
 * @retval    none
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_PowerOff (void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
    MODULE_STOP->MSTPCRC |= MSTP_GLCD;
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);
}  /* End of function HW_GLCD_PowerOff() */

/*******************************************************************************************************************//**
 * General setting, Negates reset signal to GLCDC hardware.
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_ReleaseSoftwareReset (void)
{
    R_GLCDC->BACKGROUND.CONTROL_b.SWRST = 1;        /**< releases software reset. */
}  /* End of function HW_GLCD_ReleaseSoftwareReset() */

/*******************************************************************************************************************//**
 * General setting, Asserts signal to GLCDC hardware.
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_SoftwareReset (void)
{
    R_GLCDC->BACKGROUND.CONTROL_b.SWRST = 0;        /**< applies software reset. */
}  /* End of function HW_GLCD_SoftwareReset() */

/*******************************************************************************************************************//**
 * CLUT setting, Updates data in the entry of CLUT0.
 * @param[in] frame  Number of graphics layer
 * @param[in] entry  Number of CLUT entry
 * @param[in] data   Data to be updated
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_CLUT0Set (uint32_t frame, uint32_t entry, uint32_t data)
{
    if (0 == frame)
    {
        R_GLCDC->L1CLUT0[entry].ARGB = data;
    }
    else
    {
        R_GLCDC->L2CLUT0[entry].ARGB = data;
    }
}  /* End of function HW_GLCD_CLUT0Set() */

/*******************************************************************************************************************//**
 * CLUT setting, Updates data in the entry of CLUT1.
 * @param[in] frame  Number of graphics layer
 * @param[in] entry  Number of CLUT entry
 * @param[in] data   Data to be updated
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_CLUT1Set (uint32_t frame, uint32_t entry, uint32_t data)
{
    if (0 == frame)
    {
        R_GLCDC->L1CLUT1[entry].ARGB = data;
    }
    else
    {
        R_GLCDC->L2CLUT1[entry].ARGB = data;
    }
}  /* End of function HW_GLCD_CLUT1Set() */

/*******************************************************************************************************************//**
 * Background screen block setting, Sets the update flag for all the control blocks in GLCDC. Actual register update
 *  starts at very next Vsync timing.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_InternalRegisterReflect (void)
{
    R_GLCDC->BACKGROUND.CONTROL_b.UPDATE = 1;       /**< enables internal register value reflection. */
}  /* End of function HW_GLCD_InternalRegisterReflect() */

/*******************************************************************************************************************//**
 * Background screen block setting, Enables the background screen generation block.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_BackgroundOperationEnable (void)
{
    /**< enables background plane operation and internal register value reflection. */
    R_GLCDC->BACKGROUND.CONTROL_b.UPDATE = 1;
    R_GLCDC->BACKGROUND.CONTROL_b.EN     = 1;
}  /* End of function HW_GLCD_BackgroundOperationEnable() */

/*******************************************************************************************************************//**
 * Background screen block setting, Disables the background screen generation block.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_BackgroundOperationDisable (void)
{
    /**< disables background plane operation */
    R_GLCDC->BACKGROUND.CONTROL_b.EN = 0;
}  /* End of function HW_GLCD_BackgroundOperationDisable() */

/*******************************************************************************************************************//**
 * Background screen block setting, Checks the register update status for all the control blocks in GLCDC.
 * @param     void
 * @retval    true : Register update for all the blocks is underway / false : Register update is not underway
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_IsInternalRegisterReflecting (void)
{
    return R_GLCDC->BACKGROUND.CONTROL_b.UPDATE;            /**< checks status of internal register value reflection. */
}  /* End of function HW_GLCD_IsInternalRegisterReflecting() */

/*******************************************************************************************************************//**
 * Background screen block setting, Checks whether the background screen generation block is activated or not.
 * @param     void
 * @retval    true : BG block is in active status / false : BG block is not in active status.
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_IsBackgroundOperationing (void)
{
    return R_GLCDC->BACKGROUND.MONITOR_b.EN;                        /**< checks status of background plane operation. */
}  /* End of function HW_GLCD_IsBackgroundOperationing() */

/*******************************************************************************************************************//**
 * Output control block setting, Sets the phase of the output signals as rising edge.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_LCDOUT_PhaseSyncToRisingEdge (void)
{
    /** OUT_CLKPHASE (LCDC clock source is PLCKA)
     * b8    LCDEDGE   = 0(LCDOUT output phase is synchronized with the rising edge of OUTCLK_OUT)
     */
    R_GLCDC->OUTPUT.CLKPHASE_b.DATA = 0;
}  /* End of function HW_GLCD_LCDOUT_PhaseSyncToRisingEdge() */

/*******************************************************************************************************************//**
 * Output control block setting, Sets the phase of the output signals as falling edge.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_LCDOUT_PhaseSyncToFallingEdge (void)
{
    /** OUT_CLKPHASE (LCDC clock source is PLCKA)
     * b8    LCDEDGE   = 1(LCDOUT output phase is synchronized with the falling edge of OUTCLK_OUT)
     */
    R_GLCDC->OUTPUT.CLKPHASE_b.DATA = 1;
}  /* End of function HW_GLCD_LCDOUT_PhaseSyncToFallingEdge() */

/*******************************************************************************************************************//**
 * Output control block setting, Sets the TCON output phase as the rising edge of OUTCLK_OUT
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON_PhaseSyncToRisingEdge (void)
{
    /** OUT_CLKPHASE (LCDC clock source is PLCKA)
     * b6    TCON0EDGE = 0(TCON0 output phase is synchronized with the rising edge of OUTCLK_OUT)
     * b5    TCON1EDGE = 0(TCON1 output phase is synchronized with the rising edge of OUTCLK_OUT)
     * b4    TCON2EDGE = 0(TCON2 output phase is synchronized with the rising edge of OUTCLK_OUT)
     * b3    TCON3EDGE = 0(TCON3 output phase is synchronized with the rising edge of OUTCLK_OUT)
     */
    R_GLCDC->OUTPUT.CLKPHASE_b.TCON0 = 0;
    R_GLCDC->OUTPUT.CLKPHASE_b.TCON1 = 0;
    R_GLCDC->OUTPUT.CLKPHASE_b.TCON2 = 0;
    R_GLCDC->OUTPUT.CLKPHASE_b.TCON3 = 0;
}  /* End of function HW_GLCD_TCON_PhaseSyncToRisingEdge() */

/*******************************************************************************************************************//**
 * Output control block setting, Sets the TCON output phase as the falling edge of OUTCLK_OUT
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON_PhaseSyncToFallingEdge (void)
{
    /** OUT_CLKPHASE (LCDC clock source is PLCKA)
     * b6    TCON0EDGE = 1(TCON0 output phase is synchronized with the falling edge of OUTCLK_OUT)
     * b5    TCON1EDGE = 1(TCON1 output phase is synchronized with the falling edge of OUTCLK_OUT)
     * b4    TCON2EDGE = 1(TCON2 output phase is synchronized with the falling edge of OUTCLK_OUT)
     * b3    TCON3EDGE = 1(TCON3 output phase is synchronized with the falling edge of OUTCLK_OUT)
     */
    R_GLCDC->OUTPUT.CLKPHASE_b.TCON0 = 1;
    R_GLCDC->OUTPUT.CLKPHASE_b.TCON1 = 1;
    R_GLCDC->OUTPUT.CLKPHASE_b.TCON2 = 1;
    R_GLCDC->OUTPUT.CLKPHASE_b.TCON3 = 1;
}  /* End of function HW_GLCD_TCON_PhaseSyncToFallingEdge() */

/*******************************************************************************************************************//**
 * Output control block setting, Sets the order of color correction processing to brightness and contrast first, gamma
 *  correction second.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_CorrectionProcOrderBrightnessContrastFirst (void)
{
    /** OUT_CLKPHASE (LCDC clock source is PLCKA)
     * b12   FRONTGAM  = 0(Brightness/contrast correction is followed by gamma correction.)
     */
    R_GLCDC->OUTPUT.CLKPHASE_b.FRONTGAM = 0;
}  /* End of function HW_GLCD_CorrectionProcOrderBrightnessContrastFirst() */

/*******************************************************************************************************************//**
 * Output control block setting, Sets the order of color correction processing to gamma correction first, brightness and
 * contrast second.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_CorrectionProcOrderGammaFirst (void)
{
    /** OUT_CLKPHASE (LCDC clock source is PLCKA)
     * b12   FRONTGAM  = 1(Gamma correction is followed by Brightness/contrast correction)
     */
    R_GLCDC->OUTPUT.CLKPHASE_b.FRONTGAM = 1;
}  /* End of function HW_GLCD_CorrectionProcOrderGammaFirst() */

/*******************************************************************************************************************//**
 * Output control setting, Sets output bit order in RGB I/F as big endian (ascending order).
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputInterfaceBigEndianSet (void)
{
    R_GLCDC->OUTPUT.INTERFACE_b.ENDIAN = 1;
}  /* End of function HW_GLCD_OutputInterfaceBigEndianSet() */

/*******************************************************************************************************************//**
 * Output control setting, Sets output bit order in RGB I/F as little endian (descending order).
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputInterfaceLittleEndianSet (void)
{
    R_GLCDC->OUTPUT.INTERFACE_b.ENDIAN = 0;
}  /* End of function HW_GLCD_OutputInterfaceLittleEndianSet() */

/*******************************************************************************************************************//**
 * Output control setting, Outputs pixel in the order of BGR.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputInterfaceSwapOn (void)
{
    R_GLCDC->OUTPUT.INTERFACE_b.SWAP = 1;
}  /* End of function HW_GLCD_OutputInterfaceSwapOn() */

/*******************************************************************************************************************//**
 * Output control setting, Outputs pixel in the order of RGB.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputInterfaceSwapOff (void)
{
    R_GLCDC->OUTPUT.INTERFACE_b.SWAP = 0;
}  /* End of function HW_GLCD_OutputInterfaceSwapOff() */

/*******************************************************************************************************************//**
 * Output control setting, Sets output color format.
 * @param[in] format  Color format
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputInterfaceFormatSet (glcd_output_interface_format_t format)
{
    R_GLCDC->OUTPUT.INTERFACE_b.FORMAT = format;
}  /* End of function HW_GLCD_OutputInterfaceFormatSet() */

/*******************************************************************************************************************//**
 * Output control setting, Sets pixel clock normal frequency (1:1 to dot clock).
 * @param[in] void
 * @retval    void
 * @note pixel clock is GLCD internal signal but not a dot clock.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputInterfaceClockFreqNoDivision (void)
{
    R_GLCDC->OUTPUT.INTERFACE_b.FRQSEL = OUT_SET_FRQSEL_NO_DIVISION;
}  /* End of function HW_GLCD_OutputInterfaceClockFreqNoDivision() */

/*******************************************************************************************************************//**
 * Output control setting, Sets pixel clock quarter frequency than dot clock. This setting is for the serial 8-bit RGB
 *  interface mode.
 * @param[in] void
 * @retval    void
 * @note pixel clock is GLCD internal signal but not a dot clock.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputInterfaceClockFreqQuaterDivision (void)
{
    R_GLCDC->OUTPUT.INTERFACE_b.FRQSEL = OUT_SET_FRQSEL_QUATER_DIVISION;
}  /* End of function HW_GLCD_OutputInterfaceClockFreqQuaterDivision() */

/*******************************************************************************************************************//**
 * Output control setting, Sets brightness correction value for green color.
 * @param[in] value  Color collection value for green color (0-255).
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputBrightnessCorrectionGreen (uint16_t value)
{
    R_GLCDC->OUTPUT.BRIGHT1_b.GREEN = value & OUT_BRIGHT1_BRTG_MASK;
}  /* End of function HW_GLCD_OutputBrightnessCorrectionGreen() */

/*******************************************************************************************************************//**
 * Output control setting, Sets brightness correction value for blue color.
 * @param[in] value  Color collection value for blue color (0-255).
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputBrightnessCorrectionBlue (uint16_t value)
{
    R_GLCDC->OUTPUT.BRIGHT2_b.BLUE = value & OUT_BRIGHT2_BRTB_MASK;
}  /* End of function HW_GLCD_OutputBrightnessCorrectionBlue() */

/*******************************************************************************************************************//**
 * Output control setting, Sets brightness correction value for red color.
 * @param[in] value  Color collection value for red color (0-255).
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputBrightnessCorrectionRed (uint16_t value)
{
    R_GLCDC->OUTPUT.BRIGHT2_b.RED = value & OUT_BRIGHT2_BRTR_MASK;
}  /* End of function HW_GLCD_OutputBrightnessCorrectionRed() */

/*******************************************************************************************************************//**
 * Output control setting, Sets contrast correction value for green color.
 * @param[in] value  Color collection value for green color (0-255).
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputContrastCorrectionGreen (uint16_t value)
{
    R_GLCDC->OUTPUT.CONTRAST_b.GREEN = value & OUT_CONTRAST_CONTG_MASK;
}  /* End of function HW_GLCD_OutputContrastCorrectionGreen() */

/*******************************************************************************************************************//**
 * Output control setting, Sets contrast correction value for blue color.
 * @param[in] value  Color collection value for blue color (0-255).
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputContrastCorrectionBlue (uint16_t value)
{
    R_GLCDC->OUTPUT.CONTRAST_b.BLUE = value & OUT_CONTRAST_CONTB_MASK;
}  /* End of function HW_GLCD_OutputContrastCorrectionBlue() */

/*******************************************************************************************************************//**
 * Output control setting, Sets contrast correction value for red color.
 * @param[in] value  Color collection value for red color (0-255).
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputContrastCorrectionRed (uint16_t value)
{
    R_GLCDC->OUTPUT.CONTRAST_b.RED = value & OUT_CONTRAST_CONTR_MASK;
}  /* End of function HW_GLCD_OutputContrastCorrectionRed() */

/*******************************************************************************************************************//**
 * Output control setting, Sets dithering mode.
 * @param[in] mode   GLCD_DITHERING_MODE_TRANCATE(No dithering (trancate)) | GLCD_DITHERING_MODE_ROUND_OFF (Dithering
 *                    with round off) | GLCD_DITHERING_MODE_2X2PATTERN(Dithering with 2x2 pattern)
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_DitheringModeSet (glcd_dithering_mode_t mode)
{
    R_GLCDC->OUTPUT.DITHER_b.MODE = mode;
}  /* End of function HW_GLCD_DitheringModeSet() */

/*******************************************************************************************************************//**
 * Output control setting, Sets dithering format.
 * @param[in] format  GLCD_DITHERING_OUTPUT_FORMAT_RGB888(Dithering output format RGB888) |
 *                    GLCD_DITHERING_OUTPUT_FORMAT_RGB666(Dithering output format RGB666) |
 *                    GLCD_DITHERING_OUTPUT_FORMAT_RGB565(Dithering output format RGB565)
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputDitherFormatSet (glcd_dithering_output_format_t format)
{
    R_GLCDC->OUTPUT.DITHER_b.FORMAT = format;
}  /* End of function HW_GLCD_OutputDitherFormatSet() */

/*******************************************************************************************************************//**
 * Output control setting, Sets dithering pattern A for dithering with 2x2 pattern.
 * @param[in] pattern  00|01|10|11, for detail, see the description of OUT_PDTHA register
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputDitherPatternASet (glcd_dithering_pattern_t pattern)
{
    R_GLCDC->OUTPUT.DITHER_b.PATTERN_A = pattern;
}  /* End of function HW_GLCD_OutputDitherPatternASet() */

/*******************************************************************************************************************//**
 * Output control setting, Sets dithering pattern B for dithering with 2x2 pattern.
 * @param[in] pattern  00|01|10|11, for detail, see the description of OUT_PDTHA register
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputDitherPatternBSet (glcd_dithering_pattern_t pattern)
{
    R_GLCDC->OUTPUT.DITHER_b.PATTERN_B = pattern;
}  /* End of function HW_GLCD_OutputDitherPatternBSet() */

/*******************************************************************************************************************//**
 * Output control setting, Sets dithering pattern C for dithering with 2x2 pattern.
 * @param[in] pattern  00|01|10|11, for detail, see the description of OUT_PDTHA register
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputDitherPatternCSet (glcd_dithering_pattern_t pattern)
{
    R_GLCDC->OUTPUT.DITHER_b.PATTERN_C = pattern;
}  /* End of function HW_GLCD_OutputDitherPatternCSet() */

/*******************************************************************************************************************//**
 * Output control setting, Sets dithering pattern D for dithering with 2x2 pattern.
 * @param[in] pattern  00|01|10|11, for detail, see the description of OUT_PDTHA register
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputDitherPatternDSet (glcd_dithering_pattern_t pattern)
{
    R_GLCDC->OUTPUT.DITHER_b.PATTERN_D = pattern;
}  /* End of function HW_GLCD_OutputDitherPatternDSet() */

/*******************************************************************************************************************//**
 * Output control setting, Enables output control block register update.
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_OutputControlBlockUpdate (void)
{
    R_GLCDC->OUTPUT.CONTROL_b.UPDATE = 1;
}  /* End of function HW_GLCD_OutputControlBlockUpdate() */

/*******************************************************************************************************************//**
 * Output control setting, Checks whether output control block register updating is underway or not.
 * @param[in] void
 * @retval    true : Register update for output control block is underway / false : Register update is not underway
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_IsOutputControlBlockUpdating (void)
{
    return R_GLCDC->OUTPUT.CONTROL_b.UPDATE;      /**< checks status of internal register value reflection. */
}  /* End of function HW_GLCD_IsOutputControlBlockUpdating() */

/*******************************************************************************************************************//**
 * TCON setting, Sets the Hsync signal delay cycle against the internal HS signal.
 * @param[in] delay  Number of cycle which Hysnc signal starts to assert.
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_HsyncDelaySet (uint32_t delay)
{
    /** TCON Reference Timing Setting Register (TCON_TIM)
     * b10:b0  OFFSET[10:0] (Horizontal synchronization signal generation reference timing)
     */
    R_GLCDC->TCON.OFFSET_b.HSYNC = delay & TCON_TIM_OFFSET_MASK;
}  /* End of function HW_GLCD_HsyncDelaySet() */

/*******************************************************************************************************************//**
 * TCON setting, Sets the Vsync signal delay cycle against the internal HS signal.
 * @param[in] delay  Number of cycle which Hysnc signal starts to assert.
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_VsyncDelaySet (uint32_t delay)
{
    /** TCON Reference Timing Setting Register (TCON_TIM)
     * b26:b16 HALF[10:0] (Vertical synchronization signal generation change timing)
     */
    R_GLCDC->TCON.OFFSET_b.VSYNC = delay & TCON_TIM_HALF_MASK;
}  /* End of function HW_GLCD_VsyncDelaySet() */

/*******************************************************************************************************************//**
 * TCON setting, Sets Hsync signal position.
 * @param[in] position  Hsync beginning position [cycle]
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON_HSyncPostion_Set (uint16_t position)
{
    R_GLCDC->TCON.HSYNCA_REG1_b.START = position & TCON_STHx1_HS_MASK;
}  /* End of function HW_GLCD_TCON_HSyncPostion_Set() */

/*******************************************************************************************************************//**
 * TCON setting, Sets Hsync signal width.
 * @param[in] width  Hsync width [cycle]
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON_HSyncWidth_Set (uint16_t width)
{
    R_GLCDC->TCON.HSYNCA_REG1_b.SIZE = width & TCON_STHx1_HW_MASK;
}  /* End of function HW_GLCD_TCON_HSyncWidth_Set() */

/*******************************************************************************************************************//**
 * TCON setting, Sets start position of horizontal data enable(DE) signal assertion.
 * @param[in] position   Horizontal Data Enable begin position [cycle]
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON_HDataEnablePostion_Set (uint16_t position)
{
    R_GLCDC->TCON.HSYNCB_REG1_b.START = position & TCON_STHx1_HS_MASK;
}  /* End of function HW_GLCD_TCON_HDataEnablePostion_Set() */

/*******************************************************************************************************************//**
 * TCON setting, Sets width of horizontal data enable(DE) signal assertion.
 * @param[in] width   Horizontal DE width [cycles]
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON_HDataEnableWidth_Set (uint16_t width)
{
    R_GLCDC->TCON.HSYNCB_REG1_b.SIZE = width & TCON_STHx1_HW_MASK;
}  /* End of function HW_GLCD_TCON_HDataEnableWidth_Set() */

/*******************************************************************************************************************//**
 * TCON setting, Sets Vsync signal position.
 * @param[in] position  Vsync beginning position [line]
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON_VSyncPostion_Set (uint16_t position)
{
    R_GLCDC->TCON.VSYNCA_REG1_b.START = position & TCON_STVx1_VS_MASK;
}  /* End of function HW_GLCD_TCON_VSyncPostion_Set() */

/*******************************************************************************************************************//**
 * TCON setting, Sets Vsync signal width.
 * @param[in] width  Vsync width [lines]
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON_VSyncWidth_Set (uint16_t width)
{
    R_GLCDC->TCON.VSYNCA_REG1_b.SIZE = width & TCON_STVx1_VW_MASK;
}  /* End of function HW_GLCD_TCON_VSyncWidth_Set() */

/*******************************************************************************************************************//**
 * TCON setting, Sets start position of vertical data enable(DE) signal assertion.
 * @param[in] position  Vertical Data Enable begin position [line]
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON_VDataEnablePostion_Set (uint16_t position)
{
    R_GLCDC->TCON.VSYNCB_REG1_b.START = position & TCON_STVx1_VS_MASK;
}  /* End of function HW_GLCD_TCON_VDataEnablePostion_Set() */

/*******************************************************************************************************************//**
 * TCON setting, Sets width of vertical data enable(DE) signal assertion.
 * @param[in] width  Vertical DE width [lines]
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON_VDataEnableWidth_Set (uint16_t width)
{
    R_GLCDC->TCON.VSYNCB_REG1_b.SIZE = width & TCON_STVx1_VW_MASK;
}  /* End of function HW_GLCD_TCON_VDataEnableWidth_Set() */

/*******************************************************************************************************************//**
 * TCON setting, Sets the polarity of VSYNCA signal as non-inverting.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_VSYNCA_NotInvert (void)
{
    R_GLCDC->TCON.VSYNCA_REG2_b.INV = 0;
}  /* End of function HW_GLCD_VSYNCA_NotInvert() */

/*******************************************************************************************************************//**
 * TCON setting, Inverts the polarity of VSYNCA signal.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_VSYNCA_Invert (void)
{
    R_GLCDC->TCON.VSYNCA_REG2_b.INV = 1;
}  /* End of function HW_GLCD_VSYNCAVSYNCArt() */

/*******************************************************************************************************************//**
 * Internal signal selection for TCON0
 * @param[in] signal  GLCD_TCONSIG_STVA_VS | GLCD_TCONSIG_STVB_VE| GLCD_TCONSIG_STHA_HS | GLCD_TCONSIG_STHB_HE |
 * GLCD_TCONSIG_DE
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON0_Select (glcd_tcon_signal_select_t signal)
{
    R_GLCDC->TCON.VSYNCA_REG2_b.TCONN_SELECT = signal;
}  /* End of function HW_GLCD_TCON0_Select() */

/*******************************************************************************************************************//**
 * TCON setting, Sets the polarity of TCON0 signal as non-inverting.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_VSYNCB_NotInvert (void)
{
    R_GLCDC->TCON.VSYNCB_REG2_b.INV = 0;
}  /* End of function HW_GLCD_VSYNCB_NotInvert() */

/*******************************************************************************************************************//**
 * TCON setting, Inverts the polarity of VSYNCB signal.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_VSYNCB_Invert (void)
{
    R_GLCDC->TCON.VSYNCB_REG2_b.INV = 1;
}  /* End of function HW_GLCD_VSYNCB_Invert() */

/*******************************************************************************************************************//**
 * Internal signal selection for TCON1
 * @param[in] signal  GLCD_TCONSIG_STVA_VS | GLCD_TCONSIG_STVB_VE| GLCD_TCONSIG_STHA_HS | GLCD_TCONSIG_STHB_HE |
 * GLCD_TCONSIG_DE
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON1_Select (glcd_tcon_signal_select_t signal)
{
    R_GLCDC->TCON.VSYNCB_REG2_b.TCONN_SELECT = signal;
}  /* End of function HW_GLCD_TCON1_Select() */

/*******************************************************************************************************************//**
 * TCON setting, Sets the polarity of HSYNCA signal as non-inverting.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_HSYNCA_NotInvert (void)
{
    R_GLCDC->TCON.HSYNCA_REG2_b.INV = 0;
}  /* End of function HW_GLCD_HSYNCA_NotInvert() */

/*******************************************************************************************************************//**
 * TCON setting, Inverts the polarity of HSYNCA signal.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_HSYNCA_Invert (void)
{
    R_GLCDC->TCON.HSYNCA_REG2_b.INV = 1;
}  /* End of function HW_GLCD_HSYNCA_Invert() */

/*******************************************************************************************************************//**
 * Internal signal selection for TCON2
 * @param[in] signal  GLCD_TCONSIG_STVA_VS | GLCD_TCONSIG_STVB_VE| GLCD_TCONSIG_STHA_HS | GLCD_TCONSIG_STHB_HE |
 * GLCD_TCONSIG_DE
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON2_Select (glcd_tcon_signal_select_t signal)
{
    R_GLCDC->TCON.HSYNCA_REG2_b.TCONN_SELECT = signal;
}  /* End of function HW_GLCD_TCON2_Select() */

/*******************************************************************************************************************//**
 * TCON setting, Sets the polarity of HSYNCB signal as non-inverting.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_HSYNCB_NotInvert (void)
{
    R_GLCDC->TCON.HSYNCB_REG2_b.INV = 0;
}  /* End of function HW_GLCD_HSYNCB_NotInvert() */

/*******************************************************************************************************************//**
 * TCON setting, Inverts the polarity of HSYNCB signal.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_HSYNCB_Invert (void)
{
    R_GLCDC->TCON.HSYNCB_REG2_b.INV = 1;
}  /* End of function HW_GLCD_HSYNCB_Invert() */

/*******************************************************************************************************************//**
 * TCON setting, Internal signal selection for DE
 * @param[in] signal  GLCD_TCONSIG_STVA_VS | GLCD_TCONSIG_STVB_VE| GLCD_TCONSIG_STHA_HS | GLCD_TCONSIG_STHB_HE |
 * GLCD_TCONSIG_DE
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_TCON3_Select (glcd_tcon_signal_select_t signal)
{
    R_GLCDC->TCON.HSYNCB_REG2_b.TCONN_SELECT = signal;
}  /* End of function HW_GLCD_TCON3_Select() */

/*******************************************************************************************************************//**
 * TCON setting, Sets the polarity of DE signal as non-inverting.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_DE_NotInvert (void)
{
    R_GLCDC->TCON.DATA_ENABLE_b.INV = 0;
}  /* End of function HW_GLCD_DE_NotInvert() */

/*******************************************************************************************************************//**
 * TCON setting, Inverts the polarity of DE signal.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_DE_Invert (void)
{
    R_GLCDC->TCON.DATA_ENABLE_b.INV = 1;
}  /* End of function HW_GLCD_DE_Invert() */

/*******************************************************************************************************************//**
 * Background screen generation block setting, Sets HSIN(BG_PERI.FH) value
 * @param[in] Number of horizontal cycles (24 to 1024)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_BGplaneHorizontalCycleSet (uint16_t cycles)
{
    R_GLCDC->BACKGROUND.PERIOD_b.HORIZONTAL = cycles & BG_PERI_FH_MASK;
}  /* End of function HW_GLCD_BGplaneHorizontalCycleSet() */

/*******************************************************************************************************************//**
 * Background screen generation block setting, Sets VSIN(BG_PERI.FV) value
 * @param[in] Number of vertical lines (20 to 1024)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_BGplaneVerticalCycleSet (uint16_t cycles)
{
    R_GLCDC->BACKGROUND.PERIOD_b.VERTICAL = cycles & BG_PERI_FV_MASK;
}  /* End of function HW_GLCD_BGplaneVerticalCycleSet() */

/*******************************************************************************************************************//**
 * Background screen generation block setting, Sets the 1st edge position of HSIN
 * @param[in] Number of horizontal cycle (1 to 15)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_BGplaneHorizontalSyncStartPositionSet (uint16_t cycles)
{
    R_GLCDC->BACKGROUND.SYNC_POSITION_b.HORIZONTAL = cycles & BG_SYNC_HP_MASK;
}  /* End of function HW_GLCD_BGplaneHorizontalSyncStartPositionSet() */

/*******************************************************************************************************************//**
 * Background screen generation block setting, Sets the 1st edge position of VSIN
 * @param[in] Number of horizontal cycle (1 to 15)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_BGplaneVerticalSyncStartPositionSet (uint16_t cycles)
{
    R_GLCDC->BACKGROUND.SYNC_POSITION_b.VERTICAL = cycles & BG_SYNC_VP_MASK;
}  /* End of function HW_GLCD_BGplaneVerticalSyncStartPositionSet() */

/*******************************************************************************************************************//**
 * Background screen generation block setting, Sets start position of image frame (horizontal)
 * @param[in] Number of horizontal cycles (6 to 1006)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_BGplaneHorizontalStartPositionSet (uint16_t cycles)
{
    R_GLCDC->BACKGROUND.HORIZONTAL_b.START = cycles & BG_HSIZE_HP_MASK;
}  /* End of function HW_GLCD_BGplaneHorizontalStartPositionSet() */

/*******************************************************************************************************************//**
 * Background screen generation block setting, Sets width of image frame (horizontal)
 * @param[in] Number of horizontal cycles (16 to 1008)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_BGplaneHorizontalWidthSet (uint16_t cycles)
{
    R_GLCDC->BACKGROUND.HORIZONTAL_b.SIZE = cycles & BG_HSIZE_HW_MASK;
}  /* End of function HW_GLCD_BGplaneHorizontalWidthSet() */

/*******************************************************************************************************************//**
 * Background screen generation block setting, Sets start position of image frame (vertical)
 * @param[in] Number of vertical lines (3 to 1007)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_BGplaneVerticalStartPositionSet (uint16_t cycles)
{
    R_GLCDC->BACKGROUND.VERTICAL_b.START = cycles & BG_VSIZE_VP_MASK;
}  /* End of function HW_GLCD_BGplaneVerticalStartPositionSet() */

/*******************************************************************************************************************//**
 * Background screen generation block setting, Sets width of image frame (vertical)
 * @param[in] Number of vertical lines (16 to 1008)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_BGplaneVerticalWidthSet (uint16_t cycles)
{
    R_GLCDC->BACKGROUND.VERTICAL_b.SIZE = cycles & BG_VSIZE_VW_MASK;
}  /* End of function HW_GLCD_BGplaneVerticalWidthSet() */

/*******************************************************************************************************************//**
 * Background screen generation block setting, Sets background color of image frame
 * @param[in] Number of vertical lines (16 to 1008)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_BGplaneColorSet (display_color_t color)
{
    R_GLCDC->BACKGROUND.FILL_COLOR_b.BLUE  = color.byte.b;
    R_GLCDC->BACKGROUND.FILL_COLOR_b.GREEN = color.byte.g;
    R_GLCDC->BACKGROUND.FILL_COLOR_b.RED   = color.byte.r;
}  /* End of function HW_GLCD_BGplaneColorSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets the base address of graphics Frame N
 * @param[in] frame  Number of graphics IF blocks
 * @param[in] base   Base address of graphics frame, this have to be aligned to 64bytes
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneBaseAddress (uint32_t frame, uint32_t base)
{
    R_GLCDC->LAYER[frame].FRAME2_b.BASE = base;
}  /* End of function HW_GLCD_GRplaneBaseAddress() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets the base address of graphics Frame N
 * @param[in] frame  Number of graphics IF blocks
 * @param[in] offset Line offset[bytes], this have to be aligned to 64bytes
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneLineOffsetSet (uint32_t frame, uint32_t offset)
{
    /* Graphics 3 Frame Buffer Control Register 3 (GR1_FLM3)
     * b31:b16 LNOFF[15:0] Line Offset Address for Accessing Graphics Data
     */
    R_GLCDC->LAYER[frame].FRAME3_b.LINE_OFFSET = offset & GRn_FLM3_LNOFF_MASK;
}  /* End of function HW_GLCD_GRplaneLineOffsetSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets the base address of graphics Frame N
 * @param[in] frame  Number of graphics IF blocks
 * @param[in] number Frame number for accessing graphics data (1 to 256 frames)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneFrameNumberSet (uint32_t frame, uint32_t number)
{
    /* Graphics 3 Frame Buffer Control Register 3 (GR1_FLM3)
     * b9:b0   FLNUM[9:0]  Frame Number for Accessing Graphics Data
     */
    R_GLCDC->LAYER[frame].FRAME3_b.NUMBER = number & GRn_FLM3_FLNUM_MASK;
}  /* End of function HW_GLCD_GRplaneFrameNumberSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets frame offset address for accessing Graphics Data
 * @param[in] frame   Number of graphics IF blocks
 * @param[in] address Frame offset for accessing graphics data, have to be aligned to a 64-byte boundary
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneFrameOffsetAddressSet (uint32_t frame, uint32_t address)
{
    /* Graphics 4 Frame Buffer Control Register 4 (GR1_FLM4)
     * b23:b0 FLOFF[23:0] Line Offset Address for Accessing Graphics Data
     */
    R_GLCDC->LAYER[frame].FRAME4_b.OFFSET = address & GRn_FLM3_FLNUM_MASK;
}  /* End of function HW_GLCD_GRplaneFrameOffsetAddressSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets number of lines per frame for accessing graphics data (frame buffer data)
 * @param[in] frame   Number of graphics IF blocks
 * @param[in] vsize  Number of vertical lines (16 to 1024)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneLineNumberSet (uint32_t frame, uint32_t vsize)
{
    R_GLCDC->LAYER[frame].FRAME5_b.LINE_COUNT = vsize & GRn_FLM5_LNNUM_MASK;
}  /* End of function HW_GLCD_GRplaneLineNumberSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets number of data transfer times per line for accessing graphics data
 * (frame buffer data), where one transfer is defined as 16-beat burst access (64-byte boundary)
 * @param[in] frame     Number of graphics IF blocks
 * @param[in] transsize Number of data transfer times per line for accessing graphics data , have to be aligned to a
 *                      64-byte boundary
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneFrameHtransSizeSet (uint32_t frame, uint32_t transsize)
{
    R_GLCDC->LAYER[frame].FRAME5_b.LINE_BURSTS = transsize & GRn_FLM5_DATANUM_MASK;
}  /* End of function HW_GLCD_GRplaneFrameHtransSizeSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets Data format for accessing graphics data
 * @param[in] frame   Number of graphics IF blocks
 * @param[in] format  Color format of the input graphics data
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneFormatSet (uint32_t frame, glcd_input_interface_format_t format)
{
    R_GLCDC->LAYER[frame].FRAME6_b.FORMAT = format & 0x7U;
}  /* End of function HW_GLCD_GRplaneFormatSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Enables data read access from graphics data IF to AHB bus.
 * @param[in] frame   Number of graphics IF blocks
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneFrameEnable (uint32_t frame)
{
    R_GLCDC->LAYER[frame].READ_b.ENABLE = 1;
}  /* End of function HW_GLCD_GRplaneFrameEnable() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Disables data read access from graphics data IF to AHB bus.
 * @param[in] frame   Number of graphics IF blocks
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneFrameDisable (uint32_t frame)
{
    R_GLCDC->LAYER[frame].READ_b.ENABLE = 0;
}  /* End of function HW_GLCD_GRplaneFrameDisable() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Checks whether or not data read access from graphics data IF to AHB bus is enabled.
 * @param[in] frame   Number of graphics IF blocks
 * @retval    true : Data read for GRn block is enabled / false : Data read for GRn block is disabled.
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_IsGRplaneFrameEnabled (uint32_t frame)
{
    return R_GLCDC->LAYER[frame].READ_b.ENABLE;
}  /* End of function HW_GLCD_IsGRplaneFrameEnabled() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets the background color for the graphics layer out of the rectangular area.
 * @param[in] frame   Number of graphics IF blocks
 * @param[in] color   RGB Color of the background color in the input graphic frame
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneBGColorSet (uint32_t frame, display_color_t color)
{
    R_GLCDC->LAYER[frame].FILL_COLOR_b.BLUE  = color.byte.g;
    R_GLCDC->LAYER[frame].FILL_COLOR_b.GREEN = color.byte.b;
    R_GLCDC->LAYER[frame].FILL_COLOR_b.RED   = color.byte.r;
}  /* End of function HW_GLCD_GRplaneBGColorSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets the number for line detection
 * @param[in] line    Number of line which happens the line detection interrupt.
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GR2planeLineDetectionNumberSet (uint32_t line)
{
    R_GLCDC->LAYER[1].CLUT_INT_b.LINE = line & GRn_CLUTINT_LINE_MASK;
}  /* End of function HW_GLCD_GR2planeLineDetectionNumberSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets the number for line detection
 * @param[in] frame   Number of graphics IF blocks
 * @retval    true : Fading control is underway for GRn block / false : Fading control is not underway
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_GRplaneIsFadeInOutInProgress (uint32_t frame)
{
    return R_GLCDC->LAYER[frame].MONITOR_b.ARCST;
}  /* End of function HW_GLCD_GRplaneIsFadeInOutInProgress() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Selects what is displayed on the graphics layer.
 * @param[in] frame   Number of graphics IF blocks
 * @param[in] plane   Graphics plane setting (DISPLAY_PLANE_TRANSPARENT(1;Lower-layer graphics displayed)
 *                                           |DISPLAY_PLANE_NON_TRANSPARENT(2;Current graphics displayed)
 *                                           |DISPLAY_PLANE_BLEND_ON_LOWER_LAYER(3;Graphics layers are blended)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneAlphaBlendingPlaneSet (uint32_t frame, uint32_t plane)
{
    R_GLCDC->LAYER[frame].GRAPHICS_b.DISPLAY = plane & GRn_AB1_DISPSEL_MASK;
}  /* End of function HW_GLCD_GRplaneAlphaBlendingPlaneSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Enables rectangular alpha blending in the specified layer
 * @param[in] frame   Number of graphics IF blocks
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneRectangularAlphaBlendingEnable (uint32_t frame)
{
    R_GLCDC->LAYER[frame].GRAPHICS_b.ALPHA_RECTANGLE = 1;
}  /* End of function HW_GLCD_GRplaneRectangularAlphaBlendingEnable() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Disables rectangular alpha blending in the specified layer
 * @param[in] frame   Number of graphics IF blocks
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneRectangularAlphaBlendingDisable (uint32_t frame)
{
    R_GLCDC->LAYER[frame].GRAPHICS_b.ALPHA_RECTANGLE = 0;
}  /* End of function HW_GLCD_GRplaneRectangularAlphaBlendingDisable() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Enables boarder line on rectangular alpha blending area in the specified layer
 * @param[in] frame   Number of graphics IF blocks
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneRectangularAlphaBlendingBoarderEnable (uint32_t frame)
{
    R_GLCDC->LAYER[frame].GRAPHICS_b.BORDER_ON = 1;
}  /* End of function HW_GLCD_GRplaneRectangularAlphaBlendingBoarderEnable() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Disables boarder line on rectangular alpha blending area in the specified layer
 * @param[in] frame   Number of graphics IF blocks
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneRectangularAlphaBlendingBoarderDisable (uint32_t frame)
{
    R_GLCDC->LAYER[frame].GRAPHICS_b.BORDER_ON = 0;
}  /* End of function HW_GLCD_GRplaneRectangularAlphaBlendingBoarderDisable() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets vertical start position of graphics plane
 * @param[in] frame      Number of graphics IF blocks
 * @param[in] position   Number of start line for the specified graphics plane against the background screen
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneAlphaBlendingVerticalStartPositionSet (uint32_t frame, uint32_t position)
{
    R_GLCDC->LAYER[frame].VERTICAL_b.START = position & GRn_AB2_GRCVS_MASK;
}  /* End of function HW_GLCD_GRplaneAlphaBlendingVerticalStartPositionSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets vertical width of graphics plane
 * @param[in] frame      Number of graphics IF blocks
 * @param[in] width      Vertical width (height) of graphics plane
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneAlphaBlendingVerticalWidthSet (uint32_t frame, uint32_t width)
{
    R_GLCDC->LAYER[frame].VERTICAL_b.SIZE = width & GRn_AB2_GRCVW_MASK;
}  /* End of function HW_GLCD_GRplaneAlphaBlendingVerticalWidthSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets horizontal start position of graphics plane
 * @param[in] frame      Number of graphics IF blocks
 * @param[in] position   Number of start cycle in a line for the specified graphics plane against the background screen
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneAlphaBlendingHorizontalStartPositionSet (uint32_t frame, uint32_t position)
{
    R_GLCDC->LAYER[frame].HORIZONTAL_b.START = position & GRn_AB3_GRCHS_MASK;
}  /* End of function HW_GLCD_GRplaneAlphaBlendingHorizontalStartPositionSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets horizontal width of graphics plane
 * @param[in] frame      Number of graphics IF blocks
 * @param[in] width      Horizontal width of graphics plane
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneAlphaBlendingHorizontalWidthSet (uint32_t frame, uint32_t width)
{
    R_GLCDC->LAYER[frame].HORIZONTAL_b.SIZE = width & GRn_AB3_GRCHW_MASK;
}  /* End of function HW_GLCD_GRplaneAlphaBlendingHorizontalWidthSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets vertical start position of rectangular area in the graphics plane
 * @param[in] frame      Number of graphics IF blocks
 * @param[in] position   Number of start line for the rectangular area in the specified graphics plane against the
 *                        background screen
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneAlphaBlendingRectangleVerticalStartPositionSet (uint32_t frame, uint32_t position)
{
    R_GLCDC->LAYER[frame].ALPHA_VERTICAL_b.START = position & GRn_AB4_ARCVS_MASK;
}  /* End of function HW_GLCD_GRplaneAlphaBlendingRectangleVerticalStartPositionSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets vertical width of rectangular area in the graphics plane
 * @param[in] frame      Number of graphics IF blocks
 * @param[in] width      Vertical width (height) of the rectangular area in the specified graphics plane
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneAlphaBlendingRectangleVerticalWidthSet (uint32_t frame, uint32_t width)
{
    R_GLCDC->LAYER[frame].ALPHA_VERTICAL_b.SIZE = width & GRn_AB4_ARCVW_MASK;
}  /* End of function HW_GLCD_GRplaneAlphaBlendingRectangleVerticalWidthSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets horizontal start position of rectangular area in the graphics plane
 * @param[in] frame      Number of graphics IF blocks
 * @param[in] position   Number of start cycle in a line for the rectangular area in the specified graphics plane
 *                        against the background screen
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneAlphaBlendingRectangleHorizontalStartPositionSet (uint32_t frame, uint32_t position)
{
    R_GLCDC->LAYER[frame].ALPHA_HORIZONTAL_b.START = position & GRn_AB5_ARCHS_MASK;
}  /* End of function HW_GLCD_GRplaneAlphaBlendingRectangleHorizontalStartPositionSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets horizontal width of rectangular area in the graphics plane
 * @param[in] frame      Number of graphics IF blocks
 * @param[in] width      Horizontal width of the rectangular area in the specified graphics plane
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneAlphaBlendingRectangleHorizontalWidthSet (uint32_t frame, uint32_t width)
{
    R_GLCDC->LAYER[frame].ALPHA_HORIZONTAL_b.SIZE = width & GRn_AB5_ARCHW_MASK;
}  /* End of function HW_GLCD_GRplaneAlphaBlendingRectangleHorizontalWidthSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets the variation of alpha value which is added/subtracted when fading control is
 *  performed.
 * @param[in] frame      Number of graphics IF blocks
 * @param[in] variation  Variation of alpha value which is added/subtracted when fading control is performed. When
 *                        value is to be decreased, bit8 of this value is set to '1'.
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneFadeInOutVariationValueSet (uint32_t frame, uint32_t variation)
{
    R_GLCDC->LAYER[frame].ALPHA_BLEND1_b.ADDER = variation & GRn_AB6_ARCCOEF_MASK;
}  /* End of function HW_GLCD_GRplaneFadeInOutVariationValueSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets the frame rate of alpha value which is added/subtracted when fading control is
 *  performed. adding/subtracting is held once with specified frame rate.
 * @param[in] frame      Number of graphics IF blocks
 * @param[in] rate       Frame rate of alpha value adding/subtracting when fading control is performed.
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneFadeInOutVariationFrameRateSet (uint32_t frame, uint32_t rate)
{
    R_GLCDC->LAYER[frame].ALPHA_BLEND1_b.RATE = rate & GRn_AB6_ARCRATE_MASK;
}  /* End of function HW_GLCD_GRplaneFadeInOutVariationFrameRateSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Sets the initial value of alpha value which is used for fading control.
 * @param[in] frame      Number of graphics IF blocks
 * @param[in] value      Initial value of alpha value for fading control
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneRectangleAlphaInitialValueSet (uint32_t frame, uint32_t value)
{
    R_GLCDC->LAYER[frame].ALPHA_BLEND2_b.START = value & GRn_AB7_ARCDEF_MASK;
}  /* End of function HW_GLCD_GRplaneRectangleAlphaInitialValueSet() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Checks whether CLUT plane1 is currently selected or not.
 * @param[in] frame      Number of graphics IF blocks
 * @retval    true (CLUT plane1 is selected) / false (CLUT plane0 is selected)
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_IsCLUTplane1Selected (uint32_t frame)
{
    return R_GLCDC->LAYER[frame].CLUT_INT_b.SEL;
}  /* End of function HW_GLCD_IsCLUTplane1Selected() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Selects CLUT plane0 as current plane for the specified graphics layer.
 * @param[in] frame      Number of graphics IF blocks
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_CLUTplane0Select (uint32_t frame)
{
    R_GLCDC->LAYER[frame].CLUT_INT_b.SEL = 0;
}  /* End of function HW_GLCD_CLUTplane0Select() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Selects CLUT plane1 as current plane for the specified graphics layer.
 * @param[in] frame      Number of graphics IF blocks
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_CLUTplane1Select (uint32_t frame)
{
    R_GLCDC->LAYER[frame].CLUT_INT_b.SEL = 1;
}  /* End of function HW_GLCD_CLUTplane1Select() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Enables specified graphics layer register update at the next Vsync timing.
 * @param[in] frame      Number of graphics IF blocks
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GRplaneUpdateEnable (uint32_t frame)
{
    R_GLCDC->LAYER[frame].CONTROL_b.UPDATE = 1;
}  /* End of function HW_GLCD_GRplaneUpdateEnable() */

/*******************************************************************************************************************//**
 * Graphics data IF blocks setting, Checks whether specified graphics layer register update is underway or not.
 * @param[in] frame      Number of graphics IF blocks
 * @retval    true : Register update for the graphics plane block is underway / false : Register update is not underway
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_IsGRplaneUpdating (uint32_t frame)
{
    return R_GLCDC->LAYER[frame].CONTROL_b.UPDATE;
}  /* End of function HW_GLCD_IsGRplaneUpdating() */

/*******************************************************************************************************************//**
 * System control setting, Sets clock division ratio
 * @param[in] divider  Division ratio (1/32(GLCD_PANEL_CLK_DIVISOR_32) - 1/1(GLCD_PANEL_CLK_DIVISOR_1))
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_DOTCLK_DividerSet (uint8_t divider)
{
    R_GLCDC->SYSTEM.PANEL_CLK_b.DIVIDER = divider & SYSCNT_PANEL_CLK_DCDR_MASK;
}  /* End of function HW_GLCD_DOTCLK_DividerSet() */

/*******************************************************************************************************************//**
 * System control setting, Enables dot clock output
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_DOTCLK_OutputEnable (void)
{
    R_GLCDC->SYSTEM.PANEL_CLK_b.CLKEN = 1;
}  /* End of function HW_GLCD_DOTCLK_OutputEnable() */

/*******************************************************************************************************************//**
 * System control setting, Selects internal clock for dot clock source
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_DOTCLK_InternalClockSelect (void)
{
    R_GLCDC->SYSTEM.PANEL_CLK_b.CLKSEL = 1;
}  /* End of function HW_GLCD_DOTCLK_InternalClockSelect() */

/*******************************************************************************************************************//**
 * System control setting, Selects external clock for dot clock source
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_DOTCLK_ExternalClockSelect (void)
{
    R_GLCDC->SYSTEM.PANEL_CLK_b.CLKSEL = 0;
}  /* End of function HW_GLCD_DOTCLK_ExternalClockSelect() */

/*******************************************************************************************************************//**
 * System control setting, Selects parallel RGB
 * @retval    void
 * @param     void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_DOTCLK_ParallelRGBSelect (void)
{
    R_GLCDC->SYSTEM.PANEL_CLK_b.PIXSEL = 0;
}  /* End of function HW_GLCD_DOTCLK_ParallelRGBSelect() */

/*******************************************************************************************************************//**
 * System control setting, Selects serial RGB (dot clock frequency becomes quarter)
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_DOTCLK_SerialRGBSelect (void)
{
    R_GLCDC->SYSTEM.PANEL_CLK_b.PIXSEL = 1;
}  /* End of function HW_GLCD_DOTCLK_SerialRGBSelect() */

/*******************************************************************************************************************//**
 * System control setting, Enables line detect function
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_VposDetectEnable (void)
{
    R_GLCDC->SYSTEM.DETECT_b.VPOS = 1;
}  /* End of function HW_GLCD_VposDetectEnable() */

/*******************************************************************************************************************//**
 * System control setting, Disables line detect function
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_VposDetectDisable (void)
{
    R_GLCDC->SYSTEM.DETECT_b.VPOS = 0;
}  /* End of function HW_GLCD_VposDetectDisable() */

/*******************************************************************************************************************//**
 * System control setting, Enables underflow detect function for graphics layer 1
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GR1UnderflowDetectEnable (void)
{
    R_GLCDC->SYSTEM.DETECT_b.L1_UNDF = 1;
}  /* End of function HW_GLCD_GR1UnderflowDetectEnable() */

/*******************************************************************************************************************//**
 * System control setting, Disables underflow detect function for graphics layer 1
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GR1UnderflowDetectDisable (void)
{
    R_GLCDC->SYSTEM.DETECT_b.L1_UNDF = 0;
}  /* End of function HW_GLCD_GR1UnderflowDetectDisable() */

/*******************************************************************************************************************//**
 * System control setting, Enables underflow detect function for graphics layer 2
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GR2UnderflowDetectEnable (void)
{
    R_GLCDC->SYSTEM.DETECT_b.L2_UNDF = 1;
}  /* End of function HW_GLCD_GR2UnderflowDetectEnable() */

/*******************************************************************************************************************//**
 * System control setting, Disables underflow detect function for graphics layer 2
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GR2UnderflowDetectDisable (void)
{
    R_GLCDC->SYSTEM.DETECT_b.L2_UNDF = 0;
}  /* End of function HW_GLCD_GR2UnderflowDetectDisable() */

/*******************************************************************************************************************//**
 * System control setting, Enables line detect interrupt
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_VposInterruptEnable (void)
{
    R_GLCDC->SYSTEM.EVENT_b.VPOS = 1;
}  /* End of function HW_GLCD_VposInterruptEnable() */

/*******************************************************************************************************************//**
 * System control setting, Enables underflow interrupt for graphics layer 1
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GR1UnderflowInterruptEnable (void)
{
    R_GLCDC->SYSTEM.EVENT_b.L1_UNDF = 1;
}  /* End of function HW_GLCD_GR1UnderflowInterruptEnable() */

/*******************************************************************************************************************//**
 * System control setting, Enables underflow interrupt for graphics layer 2
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GR2UnderflowInterruptEnable (void)
{
    R_GLCDC->SYSTEM.EVENT_b.L2_UNDF = 1;
}  /* End of function HW_GLCD_GR2UnderflowInterruptEnable() */

/*******************************************************************************************************************//**
 * System control setting, Disables line detect interrupt
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_VposInterruptDisable (void)
{
    R_GLCDC->SYSTEM.EVENT_b.VPOS = 0;
}  /* End of function HW_GLCD_VposInterruptDisable() */

/*******************************************************************************************************************//**
 * System control setting, Disables underflow interrupt for graphics layer 1
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GR1UnderflowInterruptDisable (void)
{
    R_GLCDC->SYSTEM.EVENT_b.L1_UNDF = 0;
}  /* End of function HW_GLCD_GR1UnderflowInterruptDisable() */

/*******************************************************************************************************************//**
 * System control setting, Disables underflow interrupt for graphics layer 2
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GR2UnderflowInterruptDisable (void)
{
    R_GLCDC->SYSTEM.EVENT_b.L2_UNDF = 0;
}  /* End of function HW_GLCD_GR2UnderflowInterruptDisable() */

/*******************************************************************************************************************//**
 * System control setting, Clears line detect interrupt flag status
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_VposInterruptStatusClear (void)
{
    R_GLCDC->SYSTEM.CLEAR_b.VPOS = 1;
}  /* End of function HW_GLCD_VposInterruptStatusClear() */

/*******************************************************************************************************************//**
 * System control setting, Clears underflow interrupt flag status for graphics layer 1
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GR1UnderflowInterruptStatusClear (void)
{
    R_GLCDC->SYSTEM.CLEAR_b.L1_UNDF = 1;
}  /* End of function HW_GLCD_GR1UnderflowInterruptStatusClear() */

/*******************************************************************************************************************//**
 * System control setting, Clears underflow interrupt flag status for graphics layer 2
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GR2UnderflowInterruptStatusClear (void)
{
    R_GLCDC->SYSTEM.CLEAR_b.L2_UNDF = 1;
}  /* End of function HW_GLCD_GR2UnderflowInterruptStatusClear() */

/*******************************************************************************************************************//**
 * System control setting, Checks whether line detect interrupt is happen or not.
 * @param     void
 * @retval    true : Line detect interrupt is happening / false : Line detect interrupt is not happening
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_VposInterruptStatusCheck (void)
{
    return R_GLCDC->SYSTEM.CLEAR_b.VPOS;
}  /* End of function HW_GLCD_VposInterruptStatusCheck() */

/*******************************************************************************************************************//**
 * System control setting, Checks whether underflow interrupt flag status for graphics layer 1 or not.
 * @param     void
 * @retval    true : Underflow interrupt is happening / false : Underflow interrupt is not happening
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_GR1UnderflowInterruptStatusCheck (void)
{
    return R_GLCDC->SYSTEM.CLEAR_b.L1_UNDF;
}  /* End of function HW_GLCD_GR1UnderflowInterruptStatusCheck() */

/*******************************************************************************************************************//**
 * System control setting, Checks whether underflow interrupt flag status for graphics layer 2 or not.
 * @param     void
 * @retval    true : Underflow interrupt is happening / false : Underflow interrupt is not happening
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_GR2UnderflowInterruptStatusCheck (void)
{
    return R_GLCDC->SYSTEM.CLEAR_b.L2_UNDF;
}  /* End of function HW_GLCD_GR2UnderflowInterruptStatusCheck() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Has an update request on gamma correction register value.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaUpdateGreen (void)
{
    R_GLCDC->GAMMA_G.CONTROL_b.UPDATE = 1;
}  /* End of function HW_GLCD_GammaUpdateGreen() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Checks gamma correction register update is underway or not.
 * @param     void
 * @retval    true : Register update is underway / false : Register update is not underway
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_IsGammaGreenUpdating (void)
{
    return R_GLCDC->GAMMA_G.CONTROL_b.UPDATE;
}  /* End of function HW_GLCD_IsGammaGreenUpdating() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Has an update request on gamma correction register value.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaUpdateBlue (void)
{
    R_GLCDC->GAMMA_B.CONTROL_b.UPDATE = 1;
}  /* End of function HW_GLCD_GammaUpdateBlue() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Checks gamma correction register update is underway or not.
 * @param     void
 * @retval    true : Register update is underway / false : Register update is not underway
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_IsGammaBlueUpdating (void)
{
    return R_GLCDC->GAMMA_B.CONTROL_b.UPDATE;
}  /* End of function HW_GLCD_IsGammaBlueUpdating() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Has an update request on gamma correction register value.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaUpdateRed (void)
{
    R_GLCDC->GAMMA_R.CONTROL_b.UPDATE = 1;
}  /* End of function HW_GLCD_GammaUpdateRed() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Checks gamma correction register update is underway or not.
 * @retval    true : Register update is underway / false : Register update is not underway
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_GLCD_IsGammaRedUpdating (void)
{
    return R_GLCDC->GAMMA_R.CONTROL_b.UPDATE;
}  /* End of function HW_GLCD_IsGammaRedUpdating() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Enables gamma correction for green color.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionGreenOn (void)
{
    R_GLCDC->GAMMA_G.CORRECTION_b.ENABLE = 1;
}  /* End of function HW_GLCD_GammaCorrectionGreenOn() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Enables gamma correction for blue color.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionBlueOn (void)
{
    R_GLCDC->GAMMA_B.CORRECTION_b.ENABLE = 1;
}  /* End of function HW_GLCD_GammaCorrectionBlueOn() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Enables gamma correction for red color.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionRedOn (void)
{
    R_GLCDC->GAMMA_R.CORRECTION_b.ENABLE = 1;
}  /* End of function HW_GLCD_GammaCorrectionRedOn() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Disables gamma correction for green color.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionGreenOff (void)
{
    R_GLCDC->GAMMA_G.CORRECTION_b.ENABLE = 0;
}  /* End of function HW_GLCD_GammaCorrectionGreenOff() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Disables gamma correction for blue color.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionBlueOff (void)
{
    R_GLCDC->GAMMA_B.CORRECTION_b.ENABLE = 0;
}  /* End of function HW_GLCD_GammaCorrectionBlueOff() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Disables gamma correction for red color.
 * @param     void
 * @retval    void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionRedOff (void)
{
    R_GLCDC->GAMMA_R.CORRECTION_b.ENABLE = 0;
}  /* End of function HW_GLCD_GammaCorrectionRedOff() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets gain value for the specified entry in gamma look up table. This
 *  function sets the specified value to the field in b26:b16 in gamma LUT for green color.
 * @param     lut    Number of LUT register
 * @param     value  Gain value for the specified entry (000h:0.000 - 7FFh:1.999)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionGreenGainUpper16BitsSet (uint8_t lut, uint32_t value)
{
    /* Lower number of GAINnn is mapped to upper 16bits of GAMG_LUTx
     * e.g. GAMG_LUT1.GAIN00
     */
    R_GLCDC->GAMMA_G.GAIN_b[lut].LOW = value & GAMX_LUTX_GAIN_MASK;
}  /* End of function HW_GLCD_GammaCorrectionGreenGainUpper16BitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets gain value for the specified entry in gamma look up table. This
 *  function sets the specified value to the field in b10:b0 in gamma LUT for green color.
 * @param     lut    Number of LUT register
 * @param     value  Gain value for the specified entry (000h:0.000 - 7FFh:1.999)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionGreenGainLower16BitsSet (uint8_t lut, uint32_t value)
{
    /* Higher number of GAINnn is mapped to lower 16bits of GAMG_LUTx
     * e.g. GAMG_LUT1.GAIN01
     */
    R_GLCDC->GAMMA_G.GAIN_b[lut].HIGH = value & GAMX_LUTX_GAIN_MASK;
}  /* End of function HW_GLCD_GammaCorrectionGreenGainLower16BitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets gain value for the specified entry in gamma look up table. This
 *  function sets the specified value to the field in b26:b16 in gamma LUT for blue color.
 * @param     lut    Number of LUT register
 * @param     value  Gain value for the specified entry (000h:0.000 - 7FFh:1.999)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionBlueGainUpper16BitsSet (uint8_t lut, uint32_t value)
{
    /* Lower number of GAINnn is mapped to upper 16bits of GAMB_LUTx
     * e.g. GAMB_LUT1.GAIN00
     */
    R_GLCDC->GAMMA_B.GAIN_b[lut].LOW = value & GAMX_LUTX_GAIN_MASK;
}  /* End of function HW_GLCD_GammaCorrectionBlueGainUpper16BitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets gain value for the specified entry in gamma look up table. This
 *  function sets the specified value to the field in b10:b0 in gamma LUT for blue color.
 * @param     lut    Number of LUT register
 * @param     value  Gain value for the specified entry (000h:0.000 - 7FFh:1.999)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionBlueGainLower16BitsSet (uint8_t lut, uint32_t value)
{
    /* Lower number of GAINnn is mapped to lower 16bits of GAMB_LUTx
     * e.g. GAMB_LUT1.GAIN01
     */
    R_GLCDC->GAMMA_B.GAIN_b[lut].HIGH = value & GAMX_LUTX_GAIN_MASK;
}  /* End of function HW_GLCD_GammaCorrectionBlueGainLower16BitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets gain value for the specified entry in gamma look up table. This
 *  function sets the specified value to the field in b26:b16 in gamma LUT for red color.
 * @param     lut    Number of LUT register
 * @param     value  Gain value for the specified entry (000h:0.000 - 7FFh:1.999)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionRedGainUpper16BitsSet (uint8_t lut, uint32_t value)
{
    /* Lower number of GAINnn is mapped to upper 16bits of GAMR_LUTx
     * e.g. GAMR_LUT1.GAIN00
     */
    R_GLCDC->GAMMA_R.GAIN_b[lut].LOW = value & GAMX_LUTX_GAIN_MASK;
}  /* End of function HW_GLCD_GammaCorrectionRedGainUpper16BitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets gain value for the specified entry in gamma look up table. This
 *  function sets the specified value to the field in b10:b0 in gamma LUT for red color.
 * @param     lut    Number of LUT register
 * @param     value  Gain value for the specified entry (000h:0.000 - 7FFh:1.999)
 * @retval    void
 * @note      Parameter check is not held in this function
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionRedGainLower16BitsSet (uint8_t lut, uint32_t value)
{
    /* Lower number of GAINnn is mapped to lower 16bits of GAMR_LUTx
     * e.g. GAMR_LUT1.GAIN01
     */
    R_GLCDC->GAMMA_R.GAIN_b[lut].HIGH = value & GAMX_LUTX_GAIN_MASK;
}  /* End of function HW_GLCD_GammaCorrectionRedGainLower16BitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets threshold value to each of divided section in gamma correction
 *  curve. This function sets the specified value to the field in b29:b20 in GAMG_AREAn register.
 * @param     area   Number of section
 * @param     value  Threshold value (000h - 3FFh)
 * @retval    void
 * @note      Parameter check is not held in this function. Larger number section have to have larger threshold value
 *            than those in any smaller number sections.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionGreenThresholdHigherBitsSet (uint8_t area, uint32_t value)
{
    R_GLCDC->GAMMA_G.THRESHOLD_b[area].LOW = value & GAMX_AREAX_MASK;
}  /* End of function HW_GLCD_GammaCorrectionGreenThresholdHigherBitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets threshold value to each of divided section in gamma correction
 *  curve. This function sets the specified value to the field in b19:b10 in GAMG_AREAn register.
 * @param     area   Number of section
 * @param     value  Threshold value (000h - 3FFh)
 * @retval    void
 * @note      Parameter check is not held in this function. Larger number section have to have larger threshold value
 *            than those in any smaller number sections.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionGreenThresholdMiddleBitsSet (uint8_t area, uint32_t value)
{
    R_GLCDC->GAMMA_G.THRESHOLD_b[area].MID = value & GAMX_AREAX_MASK;
}  /* End of function HW_GLCD_GammaCorrectionGreenThresholdMiddleBitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets threshold value to each of divided section in gamma correction
 *  curve. This function sets the specified value to the field in b9:b0 in GAMG_AREAn register.
 * @param     area   Number of section
 * @param     value  Threshold value (000h - 3FFh)
 * @retval    void
 * @note      Parameter check is not held in this function. Larger number section have to have larger threshold value
 *            than those in any smaller number sections.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionGreenThresholdLowerBitsSet (uint8_t area, uint32_t value)
{
    R_GLCDC->GAMMA_G.THRESHOLD_b[area].HIGH = value & GAMX_AREAX_MASK;
}  /* End of function HW_GLCD_GammaCorrectionGreenThresholdLowerBitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets threshold value to each of divided section in gamma correction
 *  curve. This function sets the specified value to the field in b29:b20 in GAMB_AREAn register.
 * @param     area   Number of section
 * @param     value  Threshold value (000h - 3FFh)
 * @retval    void
 * @note      Parameter check is not held in this function. Larger number section have to have larger threshold value
 *            than those in any smaller number sections.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionBlueThresholdHigherBitsSet (uint8_t area, uint32_t value)
{
    R_GLCDC->GAMMA_B.THRESHOLD_b[area].LOW = value & GAMX_AREAX_MASK;
}  /* End of function HW_GLCD_GammaCorrectionBlueThresholdHigherBitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets threshold value to each of divided section in gamma correction
 *  curve. This function sets the specified value to the field in b19:b10 in GAMB_AREAn register.
 * @param     area   Number of section
 * @param     value  Threshold value (000h - 3FFh)
 * @retval    void
 * @note      Parameter check is not held in this function. Larger number section have to have larger threshold value
 *            than those in any smaller number sections.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionBlueThresholdMiddleBitsSet (uint8_t area, uint32_t value)
{
    R_GLCDC->GAMMA_B.THRESHOLD_b[area].MID = value & GAMX_AREAX_MASK;
}  /* End of function HW_GLCD_GammaCorrectionBlueThresholdMiddleBitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets threshold value to each of divided section in gamma correction
 *  curve. This function sets the specified value to the field in b9:b0 in GAMB_AREAn register.
 * @param     area   Number of section
 * @param     value  Threshold value (000h - 3FFh)
 * @retval    void
 * @note      Parameter check is not held in this function. Larger number section have to have larger threshold value
 *            than those in any smaller number sections.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionBlueThresholdLowerBitsSet (uint8_t area, uint32_t value)
{
    R_GLCDC->GAMMA_B.THRESHOLD_b[area].HIGH = value & GAMX_AREAX_MASK;
}  /* End of function HW_GLCD_GammaCorrectionBlueThresholdLowerBitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets threshold value to each of divided section in gamma correction
 *  curve. This function sets the specified value to the field in b29:b19 in GAMR_AREAn register.
 * @param     area   Number of section
 * @param     value  Threshold value (000h - 3FFh)
 * @retval    void
 * @note      Parameter check is not held in this function. Larger number section have to have larger threshold value
 *            than those in any smaller number sections.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionRedThresholdHigherBitsSet (uint8_t area, uint32_t value)
{
    R_GLCDC->GAMMA_R.THRESHOLD_b[area].LOW = value & GAMX_AREAX_MASK;
}  /* End of function HW_GLCD_GammaCorrectionRedThresholdHigherBitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets threshold value to each of divided section in gamma correction
 *  curve. This function sets the specified value to the field in b19:b10 in GAMR_AREAn register.
 * @param     area   Number of section
 * @param     value  Threshold value (000h - 3FFh)
 * @retval    void
 * @note      Parameter check is not held in this function. Larger number section have to have larger threshold value
 *            than those in any smaller number sections.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionRedThresholdMiddleBitsSet (uint8_t area, uint32_t value)
{
    R_GLCDC->GAMMA_R.THRESHOLD_b[area].MID = value & GAMX_AREAX_MASK;
}  /* End of function HW_GLCD_GammaCorrectionRedThresholdMiddleBitsSet() */

/*******************************************************************************************************************//**
 * Output control block - Gamma control setting, Sets threshold value to each of divided section in gamma correction
 *  curve. This function sets the specified value to the field in b9:b0 in GAMR_AREAn register.
 * @param     area   Number of section
 * @param     value  Threshold value (000h - 3FFh)
 * @retval    void
 * @note      Parameter check is not held in this function. Larger number section have to have larger threshold value
 *            than those in any smaller number sections.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_GLCD_GammaCorrectionRedThresholdLowerBitsSet (uint8_t area, uint32_t value)
{
    R_GLCDC->GAMMA_R.THRESHOLD_b[area].HIGH = value & GAMX_AREAX_MASK;
}  /* End of function HW_GLCD_GammaCorrectionRedThresholdLowerBitsSet() */
#endif  /* HW_GLCD_COMMON_H */
