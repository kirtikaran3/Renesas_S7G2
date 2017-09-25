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
 * File Name    : hw_jpeg_decode_common.h
 * Description  : JPEG Decode (JPEG_DECODE) Module hardware common header file.
 **********************************************************************************************************************/

#ifndef HW_JPEG_DECODE_COMMON_H
#define HW_JPEG_DECODE_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

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

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* Register bit definition for BUS arbitration control */

#define MSTP_JPEG (1U << 5)

/* define jpeg interrupt enable register 0 bits.  */
#define JPEG_INTE0_INT3 (0x08)                          ///< Enable image size available in decode.
#define JPEG_INTE0_INT5 (0x20)                          ///< Enable the final number error in decode.
#define JPEG_INTE0_INT6 (0x40)                          ///< Enable the total number error in decode.
#define JPEG_INTE0_INT7 (0x80)                          ///< Enable the number error of restart interval in decode.

/* define jpeg interrupt enable register 1 bits.  */
#define JPEG_INTE1_DOUTLEN (0x00000001)                 ///< Enable output lines number available in decode.
#define JPEG_INTE1_JINEN   (0x00000002)                 ///< Enable input amount available in decode.
#define JPEG_INTE1_DBTEN   (0x00000004)                 ///< Enable data transfer completed in decode.
#define JPEG_INTE1_DINLEN  (0x00000020)                 ///< Enable input line number available in encode.
#define JPEG_INTE1_CBTEN   (0x00000040)                 ///< Enable data transfer completed in encode.

/* define jpeg interrupt status register 0 bits.  */
#define JPEG_INTE0_INS3 (0x08)                          ///< image size is available in decode.
#define JPEG_INTE0_INS5 (0x20)                          ///< encounter a encoded data error in decode.
#define JPEG_INTE0_INS6 (0x40)                          ///< complete encoding process or decoding process.

/* define jpeg interrupt status register 1 bits.  */
#define JPEG_INTE1_DOUTLF (0x00000001)                  ///< the number of lines of output image data is available in
                                                        // decode.
#define JPEG_INTE1_JINF   (0x00000002)                  ///< the amount of input coded data is available in decode.
#define JPEG_INTE1_DBTF   (0x00000004)                  ///< the last output image data is written in decode.
#define JPEG_INTE1_DINLF  (0x00000020)                  ///< the number of input data lines is available in encode.
#define JPEG_INTE1_CBTF   (0x00000040)                  ///< the last output coded data is written in encode.

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
#define JPEG_OPERATION_ENCODE 0x00
#define JPEG_OPERATION_DECODE 0x01

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
 * General setting, Provides power to the JPEG module.
 * @param     none
 **********************************************************************************************************************/
__STATIC_INLINE void HW_JPEG_PowerOn (void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
    R_MSTP->MSTPCRC &= (~MSTP_JPEG);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);
}  /* End of function HW_GLCD_PowerOn() */

/*******************************************************************************************************************//**
 * General setting, Removes power from the JPEG module.
 * @param     none
 **********************************************************************************************************************/
__STATIC_INLINE void HW_JPEG_PowerOff (void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
    R_MSTP->MSTPCRC |= MSTP_JPEG;
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);
}  /* End of function HW_GLCD_PowerOff() */

/*******************************************************************************************************************//**
 * Function name : HW_JPEG_SetProcess
 * Description   : Set the JPEG work mode: decode or encode
 * @param     process         JPEG Encode or Decode
 **********************************************************************************************************************/
__STATIC_INLINE void HW_JPEG_SetProcess (uint8_t process)
{
    R_JPEG->JCMOD_b.DSP = process & 0x01U;
}  /* End of function HW_JPEG_SetProcess() */

/*******************************************************************************************************************//**
 * General setting, resst bus.
 * @param     none
 **********************************************************************************************************************/
__STATIC_INLINE void HW_JPEG_BusReset (void)
{
    R_JPEG->JCCMD_b.BRST = 0x01;
}  /* End of function HW_JPEG_BusRest() */

/*******************************************************************************************************************//**
 * Start JPEG core.                  
 * Description   : start the JPEG Core
 * @param     void
 **********************************************************************************************************************/
__STATIC_INLINE void HW_JPEG_CoreStart (void)
{
    R_JPEG->JCCMD_b.JSRT = 0x1;
}

/*******************************************************************************************************************//**
 * General setting, clear CoreStop
 * @param[in] none
 **********************************************************************************************************************/
__STATIC_INLINE void HW_JPEG_CoreStopClear (void)
{
    R_JPEG->JCCMD_b.JRST = 0x1;
}

/*******************************************************************************
 * Set source data size.                           
 * @param          size     Source data size in bytes
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_SourceDataSizeSet (uint32_t size)
{
    R_JPEG->JIFDSDC_b.JDATAS = (size & 0x0000FFF8);
}

/*******************************************************************************
 * Clear the JPEG Core stop command
 * @param        : None
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_ClearRequest (void)
{
    R_JPEG->JCCMD_b.JEND = 0x1;
}

/*******************************************************************************
 * Set Pixel Format
 * Arguments     pixel_format    Input data pixel format to be configured
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_InputPixelFormatSet (uint8_t pixel_format)
{
    R_JPEG->JCMOD_b.REDU = pixel_format & 0x07U;
}

/*******************************************************************************
 * Get Pixel Format
 * @param        None
 * @return       Pixel Format
 *******************************************************************************/
__STATIC_INLINE jpeg_decode_color_space_t HW_JPEG_InputPixelFormatGet (void)
{
    return (jpeg_decode_color_space_t) (R_JPEG->JCMOD_b.REDU);
}

/*******************************************************************************
 * Enable decode Input Count Mode
 * @param        None
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeInCountModeEnable (void)
{
    R_JPEG->JIFDCNT_b.JINC = 0x01;
}

/*******************************************************************************
 * Disable decode Input Count Mode
 * @param        None
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeInCountModeDisable (void)
{
    R_JPEG->JIFDCNT_b.JINC = 0x00;
}

/*******************************************************************************
 * Configure decode Input Count Mode
 * @param        resume_mode     Set the input resume mode
 * @param        num_of_data     Input data size in bytes
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeInCountModeConfig (uint8_t resume_mode, uint16_t num_of_data)
{
    R_JPEG->JIFDCNT_b.JINRINI = resume_mode & 0x01U; //0x0:Not Reset Address, 0x1:Reset Address
    R_JPEG->JIFDSDC_b.JDATAS  = num_of_data; //Number of data is transfer when count-mode is on (in 8-bytes uint)
}

/*******************************************************************************
 * Enable decode Output Count Mode
 * @param        None
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeOutCountModeEnable (void)
{
    R_JPEG->JIFDCNT_b.DOUTLC = 0x01;
}

/*******************************************************************************
 * Disable decode Output Count Mode
 * @param        None
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeOutCountModeDisable (void)
{
    R_JPEG->JIFDCNT_b.DOUTLC = 0x00;
}

/*******************************************************************************
 * Configure decode Output Count Mode
 * @param        resume_mode      Set the output resume mode
 * @param        num_of_lines     Lines to decode
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeOutCountModeConfig (uint8_t resume_mode, uint16_t num_of_lines)
{
    R_JPEG->JIFDCNT_b.DOUTRINI = resume_mode & 0x01U;  //0x0:Not Reset Address, 0x1:Reset Address
    R_JPEG->JIFDDLC_b.LINES    = num_of_lines; //Number of lines to decode 
}

/*******************************************************************************
 * Decode input count mode Resume Command
 * @param        resume_enb          Resume mode
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeInCountModeResume (uint8_t resume_enb)
{
    R_JPEG->JIFDCNT_b.JINRCMD = resume_enb & 0x01U;
}

/*******************************************************************************
 * Decode output count mode Resume Command
 * @param        resume_enb          Resume mode
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeOutCountModeResume (uint8_t resume_enb)
{
    R_JPEG->JIFDCNT_b.DOUTRCMD = resume_enb & 0x01U;
}

/*******************************************************************************
 * Configure decode source buffer address
 * @param        src_jpeg             source jpeg data
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeSourceAddressSet (void * src_jpeg)
{
    R_JPEG->JIFDSA = (uint32_t) src_jpeg;
}

/*******************************************************************************
 * Conifugre decode destination address
 * @address        Output buffer address
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeDestinationAddressSet (uint32_t address)
{
    R_JPEG->JIFDDA = address;
}

/*******************************************************************************
 * Retrieves the address of the decode output buffer
 * @param         None
 * @return        Destination Address
 *******************************************************************************/
__STATIC_INLINE uint32_t HW_JPEG_DecodeSourceAddressGet (void)
{
    return (R_JPEG->JIFDSA);
}

/*******************************************************************************
 * Retrieves decode destination address
 * @param          None
 * @return         Destination Address
 *******************************************************************************/
__STATIC_INLINE uint32_t HW_JPEG_DecodeDestinationAddressGet (void)
{
    return (R_JPEG->JIFDDA);
}

/*******************************************************************************
 * Setting Data Swap
 * @param          input_swap_mode    Input Data Swap Mode 
 * @param          output_swap_mode   Output Data Swap Mode
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeDataSwap (uint8_t input_swap_mode, uint8_t output_swap_mode)
{
    R_JPEG->JIFDCNT_b.JINSWAP  = input_swap_mode & 0x07U;
    R_JPEG->JIFDCNT_b.DOUTSWAP = output_swap_mode & 0x07U;
}

/*******************************************************************************
 * Get the number of lines decoded into the output buffer
 * @param           None
 * @return          The number of lines
 *******************************************************************************/
__STATIC_INLINE uint32_t HW_JPEG_LinesDecodedGet (void)
{
    return R_JPEG->JIFDDLC_b.LINES;
}

/*******************************************************************************
 * Get the Error number
 * @param          None
 * @return         the number of JPEG IRQ
 *******************************************************************************/
__STATIC_INLINE uint8_t HW_JPEG_ErrorGet (void)
{
    /*if(1==R_JPEG->JINTS0_b.INS5)*/ //whether to do this.
    return (R_JPEG->JCDERR_b.ERR);
}

/*******************************************************************************
 * Select Pixel Format
 * @param        format    Output image format
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeOutputImageFormatSet (uint8_t format)
{
    R_JPEG->JIFDCNT_b.OPF = format & 0x03U;
}

/*******************************************************************************
 * Get Pixel Format
 * @param          None 
 * @return         Pixel Format
 *******************************************************************************/
__STATIC_INLINE int HW_JPEG_DecodeOutputImageFormatGet (void)
{
    return (R_JPEG->JIFDCNT_b.OPF);
}

/*******************************************************************************
 * Set the decoding output sub sample
 * @param                V_subsampling  Vertical Subsample value
 * @param                H_subsampling  Horizontal Subsample value
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_OutputSubsampleSet (uint8_t V_subsampling, uint8_t H_subsampling)
{
    R_JPEG->JIFDCNT_b.VINTER = V_subsampling & 0x03U;
    R_JPEG->JIFDCNT_b.HINTER = H_subsampling & 0x03U;
}

/*******************************************************************************
 * Set output horizontal stride
 * @param            horizontal_stride   Horizontal stride value, in bytes
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_HorizontalStrideSet (uint32_t horizontal_stride)
{
    R_JPEG->JIFDDOFST = horizontal_stride;
}

/*******************************************************************************
 * Get output horizontal stride
 * @param          None
 * @return         Horizontal stride value, in bytes
 *******************************************************************************/
__STATIC_INLINE uint32_t HW_JPEG_HorizontalStrideGet (void)
{
    return (R_JPEG->JIFDDOFST);
}

/*******************************************************************************
 * Set the output image alpha
 * @param      Alpha  Alpha value to be applied to ARGB8888 output format
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_DecodeOutputAlphaSet (uint8_t alpha)
{
    R_JPEG->JIFDADT = alpha;
}

/*******************************************************************************
 * Set InterruptEnable0 Value
 * @param      interrupts  Interrupt mask bits to program for InterruptEnable0
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_InterruptEnable0Set (uint8_t interrupts)
{
    R_JPEG->JINTE0 = interrupts;
}

/*******************************************************************************
 * Set InterruptEnable1 Value
 * @param      interrupts  Interrupt mask bits to program for InterruptEnable1
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_InterruptEnable1Set (uint32_t interrupts)
{
    R_JPEG->JINTE1 = interrupts;
}

/*******************************************************************************
 * Retrieves InterruptEnable1 Value
 * @param      None
 * @return     INTE1 value
 *******************************************************************************/
__STATIC_INLINE uint32_t HW_JPEG_InterruptEnable1Get (void)
{
    return (R_JPEG->JINTE1);
}

/*******************************************************************************
 * Retrieves IntStatus0 Value
 * @param      None
 * @return     INT0 status value
 *******************************************************************************/
__STATIC_INLINE uint8_t HW_JPEG_InterruptStatus0Get (void)
{
    return (R_JPEG->JINTS0);
}

/*******************************************************************************
 * Retrieves IntStatus1 Value
 * @param      None
 * @return     INT1 status value
 *******************************************************************************/
__STATIC_INLINE uint32_t HW_JPEG_InterruptStatus1Get (void)
{
    return (R_JPEG->JINTS1);
}

/*******************************************************************************
 * Set InterruptStatus0 Value
 * @param      value  InterruptStatus0 value to be configured
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_InterruptStatus0Set (uint8_t value)
{
    R_JPEG->JINTS0 = value;
}

/*******************************************************************************
 * Set InterruptStatus1 Value
 * @param      value  InterruptStatus1 value to be configured
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_InterruptStatus1Set (uint8_t value)
{
    R_JPEG->JINTS1 = value;
}

/*******************************************************************************
 * Get JPEG Decode Error
 * @param      None
 * @return     JPEG error code
 *******************************************************************************/
__STATIC_INLINE uint8_t HW_JPEG_DecodeErrorGet (void)
{
    return (R_JPEG->JCDERR_b.ERR);
}

/*******************************************************************************
 * Get JPEG image size (horizontal and vertial)
 * @param      p_horizontal  Pointer to the storage space for the horizontal value
 * @param      p_vertical    Pointer to the storage space for the vertical value
 *******************************************************************************/
__STATIC_INLINE void HW_JPEG_ImageSizeGet (uint16_t * p_horizontal, uint16_t * p_vertical)
{
    uint16_t upper;
    uint16_t lower;
    upper = (uint16_t) (R_JPEG->JCHSZU << 8);
    lower = (uint16_t) (R_JPEG->JCHSZD);
    *p_horizontal = (upper | lower);
    upper = (uint16_t) (R_JPEG->JCVSZU << 8);
    lower = (uint16_t) (R_JPEG->JCVSZD);
    *p_vertical   = (upper | lower);
}
#endif /* HW_JPEG_DECODE_COMMON_H */
