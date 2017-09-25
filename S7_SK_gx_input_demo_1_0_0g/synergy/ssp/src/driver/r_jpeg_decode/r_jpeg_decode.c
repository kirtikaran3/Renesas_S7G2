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
 * File Name    : r_jpeg_decode.c
 * Description  : JPEG device low level functions used to implement JPEG_DECODE interface driver.
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_jpeg_decode.h"
#include "hw/hw_jpeg_decode_private.h"
#include "r_jpeg_decode_private.h"
#include "r_jpeg_decode_private_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#ifndef JPEG_ERROR_RETURN
#define JPEG_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), "jpeg", &g_module_version)
#endif

#define JPEG_ALIGNMENT_8   (0x07)
#define JPEG_ALIGNMENT_16  (0x0F)
#define JPEG_ALIGNMENT_32  (0x1F)

#define JPEG_INBUFFER_SET  (0x02)
#define JPEG_OUTBUFFER_SET (0x04)

#define JPEG_MODE_DECODE   (0x80)

#define BUFFER_MAX_SIZE    (0xfff8)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
void jpeg_jdti_isr (void);
void jpeg_jedi_isr (void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/** Implementation of General JPEG Codec Driver  */

const jpeg_decode_api_t g_jpeg_decode_on_jpeg_decode =
{
    .open                = R_JPEG_Decode_Open,
    .outputBufferSet     = R_JPEG_Decode_OutputBufferSet,
    .inputBufferSet      = R_JPEG_Decode_InputBufferSet,
    .linesDecodedGet     = R_JPEG_Decode_LinesDecodedGet,
    .horizontalStrideSet = R_JPEG_Decode_HorizontalStrideSet,
    .imageSubsampleSet   = R_JPEG_Decode_ImageSubsampleSet,
    .imageSizeGet        = R_JPEG_Decode_ImageSizeGet,
    .statusGet           = R_JPEG_Decode_StatusGet,
    .close               = R_JPEG_Decode_Close,
    .versionGet          = R_JPEG_Decode_VersionGet,
    .pixelFormatGet      = R_JPEG_Decode_PixelFormatGet,
};

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/** Version data structure used by error logger macro. */
static const ssp_version_t g_module_version =
{
    .api_version_minor  = JPEG_DECODE_API_VERSION_MINOR,
    .api_version_major  = JPEG_DECODE_API_VERSION_MAJOR,
    .code_version_major = JPEG_DECODE_CODE_VERSION_MAJOR,
    .code_version_minor = JPEG_DECODE_CODE_VERSION_MINOR
};

/* global pointer to the jpeg ctrl.  */
static jpeg_decode_ctrl_t * gp_ctrl = NULL;

/*******************************************************************************************************************//**
 * @addtogroup JPEG_DECODE
 * @{
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @brief  Initialize the JPEG Codec module.  This function configures the JPEG Codec for decoding
 *         operation, sets up the registers for data format and pixel format based on user-supplied
 *         configuration parameters.  Interrupts are enabled to support image size read operation and callback
 *         functions.
 *
 * @retval  SSP_SUCCESS         JPEG Codec module is properly configured and is ready to take input data.
 * @retval  SSP_ERR_IN_USE      JPEG Codec is already in use.
 * @retval  SSP_ERR_ASSERTION   Pointer to the control block or the configuration structure is NULL.
 * @retval  SSP_ERR_HW_LOCKED   JPEG Codec resource is locked.
 **********************************************************************************************************************/
ssp_err_t R_JPEG_Decode_Open (jpeg_decode_ctrl_t * const p_ctrl, jpeg_decode_cfg_t const * const p_cfg)
{
    uint8_t inten0 = 0x00;
    uint32_t inten1 = 0x00000000;
#if JPEG_DECODE_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_ctrl);
    JPEG_ERROR_RETURN((JPEG_DECODE_STATUS_FREE == p_ctrl->status), SSP_ERR_IN_USE);
#endif

    /** Verify JPEG Codec is not already used.  */
    bsp_hw_lock_t lock    = BSP_HW_LOCK_JPEG;
    ssp_err_t     bsp_err = R_BSP_HardwareLock(lock);
    JPEG_ERROR_RETURN((SSP_SUCCESS == bsp_err), SSP_ERR_HW_LOCKED);

    /** Record the configuration settings. */
    p_ctrl->pixel_format = p_cfg->pixel_format;

    /** Initialize horizontal stride value. */
    p_ctrl->horizontal_stride = 0;

    /** Initialize output buffer size. */
    p_ctrl->outbuffer_size = 0;

    /** Initialize total_lines_decoded */
    p_ctrl->total_lines_decoded = 0;

    /** Provide power to the JPEG module.  */
    HW_JPEG_PowerOn();

    /** Perform bus reset */
    HW_JPEG_BusReset();

    /** Reset the destination buffer address.   */
    HW_JPEG_DecodeDestinationAddressSet(0);

    /** Reset the source buffer address.   */
    HW_JPEG_DecodeSourceAddressSet(0);

    /** Reset the horizontal stride.  */
    HW_JPEG_HorizontalStrideSet(0);

    /** Configure the JPEG module for decode operation. */
    HW_JPEG_SetProcess(JPEG_OPERATION_DECODE);

    /** Set image format for the decoded image. */
    HW_JPEG_DecodeOutputImageFormatSet(p_cfg->pixel_format);

    /** If the output pixel format is ARGB8888, also configure the alpha vaule. */
    if (JPEG_DECODE_PIXEL_FORMAT_ARGB8888 == p_cfg->pixel_format)
    {
        /** Set the alpha value for the decoded image. */
        HW_JPEG_DecodeOutputAlphaSet(p_cfg->alpha_value);
    }

    /** Set the output data format. */
    HW_JPEG_DecodeDataSwap(p_cfg->output_data_format, p_cfg->input_data_format);

    /** The following interrupts are enabled:
     *  Interrupt on all errors
     *  Interrupt on Image Size
     */
    inten0 |= JPEG_INTE0_INT5 | JPEG_INTE0_INT6 | JPEG_INTE0_INT7;
    /* Enable image size interrupt.  */
    inten0 |= JPEG_INTE0_INT3;

    NVIC_EnableIRQ(JPEG_JEDI_IRQn);

    HW_JPEG_InterruptEnable0Set(inten0);
    inten1 |= JPEG_INTE1_DBTEN;
    NVIC_EnableIRQ(JPEG_JDTI_IRQn);
    HW_JPEG_InterruptEnable1Set(inten1);

    /** Record user supplies callback routine. */
    p_ctrl->p_callback = p_cfg->p_callback;

    /** Set the ctrl status.  */
    p_ctrl->status = JPEG_DECODE_STATUS_IDLE;

    /** Record the ctrl handle.  ISR routines need to access the control block. */
    gp_ctrl = p_ctrl;

    p_ctrl->p_context = p_cfg->p_context;

    /** All done.  Return success. */
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Assign output buffer to the JPEG Codec for storing output data.
 * @note   The number of image lines to be decoded depends on the size of the buffer and the horizontal stride
 *         settings. Once the output buffer size is known, the horizontal stride value is known, and the input 
 *         pixel format is known (the input pixel format is obtained by the JPEG decoder from the JPEG headers), 
 *         the driver automatically computes the number of lines that can be decoded into the output buffer.  
 *         After these lines are decoded, the JPEG engine pauses and a callback function is triggered, so the application
 *         is able to provide the next buffer for the JPEG module to resume the operation.
 *
 *         The JPEG decoding operation automatically starts after both the input buffer and the output buffer are set,
 *         and the output buffer is big enough to hold at least eight lines of decoded image data.
 *
 * @retval        SSP_SUCCESS                The output buffer is properly assigned to JPEG codec device.
 * @retval        SSP_ERR_ASSERTION          Pointer to the control block is NULL, or the pointer to the output_buffer.
 *                                           is NULL, or the output_buffer_size is 0.
 * @retval        SSP_ERR_INVALID_ALIGNMENT  Buffer starting address is not 8-byte aligned.
 * @retval        SSP_ERR_NOT_OPEN           JPEG not opened.
 **********************************************************************************************************************/
ssp_err_t R_JPEG_Decode_OutputBufferSet (jpeg_decode_ctrl_t * p_ctrl, void * output_buffer, uint32_t output_buffer_size)
{
    uint16_t lines_to_decode = 0;
    uint16_t horizontal, vertical;
#if JPEG_DECODE_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != output_buffer);
    SSP_ASSERT(NULL != p_ctrl);
    JPEG_ERROR_RETURN((JPEG_DECODE_STATUS_FREE != p_ctrl->status), SSP_ERR_NOT_OPEN);
    JPEG_ERROR_RETURN(!(output_buffer_size & JPEG_ALIGNMENT_8), SSP_ERR_INVALID_ALIGNMENT);
    JPEG_ERROR_RETURN(!((uint32_t) output_buffer & JPEG_ALIGNMENT_8), SSP_ERR_INVALID_ALIGNMENT);
#endif

    /* Return error code if any errors were detected. */
    if (JPEG_DECODE_STATUS_ERROR & p_ctrl->status)
    {
        return(p_ctrl->error_code);
    }

    /** If the image size is not ready yet, the driver does not know the input pixel format.
     *  Without that information, the driver is unable to compute the number of lines of image
     *  to decode.  In this case, the driver would record the output buffer size.  Once all the
     *  information is ready, the driver would attempt to start the decoding process. */
    if (0 == (JPEG_DECODE_STATUS_IMAGE_SIZE_READY & p_ctrl->status))
    {
        /** Set the decoding destination address.  */
        HW_JPEG_DecodeDestinationAddressSet((uint32_t) output_buffer);

        /** Record the size of the output buffer. */
        p_ctrl->outbuffer_size = output_buffer_size;
    }
    else
    {
        /** Set the decoding destination address.  */
        HW_JPEG_DecodeDestinationAddressSet((uint32_t) output_buffer);

        /** For a given buffer size, compute number of lines to decode if the image size acquisition is known. */
        if(p_ctrl->horizontal_stride)
        {
            lines_to_decode = (uint16_t) (output_buffer_size / p_ctrl->horizontal_stride);

            HW_JPEG_ImageSizeGet(&horizontal, &vertical);
            if(lines_to_decode > (vertical - p_ctrl->total_lines_decoded))
            {
                lines_to_decode = (uint16_t) (vertical - (uint16_t) p_ctrl->total_lines_decoded);
            }

            if (JPEG_DECODE_COLOR_SPACE_YCBCR420 == HW_JPEG_InputPixelFormatGet())
            {
                lines_to_decode &= (uint16_t) (~15);
                lines_to_decode = lines_to_decode/2;
            }
            else
            {
                lines_to_decode &= (uint16_t) (~7);
            }

            if (!lines_to_decode)
            {
                return SSP_ERR_JPEG_BUFFERSIZE_NOT_ENOUGH;
            }


            /** If the driver status is IMAGE_SIZE_READY with no other flags,
             *  that means the driver just received IMAGE_SIZE.  It has not started
             *  the decoding process yet. */
            if (JPEG_DECODE_STATUS_IMAGE_SIZE_READY == p_ctrl->status)
            {
                uint16_t horizontal_bytes;

                HW_JPEG_ImageSizeGet(&horizontal, &vertical);

                if(JPEG_DECODE_PIXEL_FORMAT_ARGB8888 == p_ctrl->pixel_format)
                {
                   horizontal_bytes = (uint16_t) (horizontal * 4);
                }
                else
                {
                   horizontal_bytes = (uint16_t) (horizontal * 2);
                }
                if(horizontal_bytes > p_ctrl->horizontal_stride)
                {
                   p_ctrl->status |= JPEG_DECODE_STATUS_ERROR;
                   p_ctrl->error_code = SSP_ERR_JPEG_UNSUPPORTED_IMAGE_SIZE;

                   return SSP_ERR_JPEG_UNSUPPORTED_IMAGE_SIZE;
                }


                /** in default, the output count is open.  */
                uint32_t inten1;
                inten1  = HW_JPEG_InterruptEnable1Get();
                inten1 |= JPEG_INTE1_DOUTLEN;
                HW_JPEG_InterruptEnable1Set(inten1);

                HW_JPEG_DecodeOutCountModeEnable();

                HW_JPEG_DecodeOutCountModeConfig(JPEG_DECODE_COUNT_MODE_ADDRESS_REINITIALIZE, lines_to_decode);

                /** If Input buffer is set, output buffer is set, and horizontal stride is set, the driver is able
                 *  to determine the number of lines to decode, and start the decoding operation. */
                if(HW_JPEG_DecodeSourceAddressGet())
                {
                    /** Set internal status information. */
                    p_ctrl->status |= JPEG_DECODE_STATUS_RUNNING;

                    /** Start the core.  */
                    HW_JPEG_CoreStopClear();
                }
            }
            /** If the current status is OUTPUT_PAUSE, the driver
             *  needs to resume the operation. */
            else if (JPEG_DECODE_STATUS_OUTPUT_PAUSE & p_ctrl->status)
            {

                /** Clear internal status information. */
                p_ctrl->status &= (~JPEG_DECODE_STATUS_OUTPUT_PAUSE);

                /** Set internal status information. */
                p_ctrl->status |= JPEG_DECODE_STATUS_RUNNING;

                HW_JPEG_DecodeOutCountModeConfig(JPEG_DECODE_COUNT_MODE_ADDRESS_REINITIALIZE, lines_to_decode);

                /** Resume the jpeg core.  */
                HW_JPEG_DecodeOutCountModeResume(0x01);
            }
        }
        else
        {
            /** Record the size of the output buffer. */
            p_ctrl->outbuffer_size = output_buffer_size;
        }
    }
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Returns the number of lines decoded into the output buffer.
 * @note   Use this function to retrieve number of image lines written to the output buffer after JPEG decoded
 *         a partial image.  Combined with the horizontal stride settings and the output pixel format, the application
 *         can compute the amount of data to read from the output buffer.
 *
 * @retval        SSP_SUCCESS                The output buffer is properly assigned to JPEG codec device.
 * @retval        SSP_ERR_ASSERTION          Pointer to the control block is NULL, or the pointer to the output_buffer.
 *                                           is NULL, or the output_buffer_size is 0.
 * @retval        SSP_ERR_NOT_OPEN           JPEG not opened.
 **********************************************************************************************************************/
ssp_err_t R_JPEG_Decode_LinesDecodedGet (jpeg_decode_ctrl_t * p_ctrl, uint32_t * p_lines)
{
#if JPEG_DECODE_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_lines);
    SSP_ASSERT(NULL != p_ctrl);
    JPEG_ERROR_RETURN((JPEG_DECODE_STATUS_FREE != p_ctrl->status), SSP_ERR_NOT_OPEN);
#endif

    /* Return error code if any errors were detected. */
    if (JPEG_DECODE_STATUS_ERROR & p_ctrl->status)
    {
        return(p_ctrl->error_code);
    }

    /* Get the pixel format */
    if (JPEG_DECODE_COLOR_SPACE_YCBCR420 == HW_JPEG_InputPixelFormatGet())
    {
        /* Get the line number of decoded.  */
        *p_lines = HW_JPEG_LinesDecodedGet() * 2;
    }
    else
    {
        /* Get the line number of decoded.  */
        *p_lines = HW_JPEG_LinesDecodedGet() * 1;
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Assign input data buffer to JPEG codec for processing.
 * @note   After the amount of data is processed, the JPEG driver triggers a callback function with the flag
 *         JPEG_OPERATION_INPUT_PAUSE set.
 *         The application supplies the next chunk of data to the driver so JPEG decoding can resume.
 *
 *         The JPEG decoding operation automatically starts after both the input buffer and the output buffer are set,
 *         and the output buffer is big enough to hold at least one line of decoded image data.
 *
 * @retval        SSP_SUCCESS                The input data buffer is properly assigned to JPEG Codec device.
 * @retval        SSP_ERR_ASSERTION          Pointer to the control block is NULL, or the pointer to the input_buffer is
 *                                           NULL, or the input_buffer_size is 0.
 * @retval        SSP_ERR_INVALID_ALIGNMENT  Buffer starting address is not 8-byte aligned.
 * @retval        SSP_ERR_NOT_OPEN           JPEG not opened.
 **********************************************************************************************************************/
ssp_err_t R_JPEG_Decode_InputBufferSet (jpeg_decode_ctrl_t * const p_ctrl,
                                        void                       * p_data_buffer,
                                        uint32_t                   data_buffer_size)
{
#if JPEG_DECODE_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_data_buffer);
    SSP_ASSERT(NULL != p_ctrl);
    JPEG_ERROR_RETURN((JPEG_DECODE_STATUS_FREE != p_ctrl->status), SSP_ERR_NOT_OPEN);
    JPEG_ERROR_RETURN(!(data_buffer_size & JPEG_ALIGNMENT_8), SSP_ERR_INVALID_ALIGNMENT);
    JPEG_ERROR_RETURN(!((uint32_t) p_data_buffer & JPEG_ALIGNMENT_8), SSP_ERR_INVALID_ALIGNMENT);
#endif

    /* Return error code if any errors were detected. */
    if (JPEG_DECODE_STATUS_ERROR & p_ctrl->status)
    {
        return(p_ctrl->error_code);
    }

    /** Configure the input buffer address. */
    HW_JPEG_DecodeSourceAddressSet(p_data_buffer);

    /** If the system is idle, start the JPEG engine.  This allows the
     *  system to obtain image information (image size and input pixel format).  This
     *  information is needed to drive the decode process later on. */
    if (JPEG_DECODE_STATUS_IDLE == p_ctrl->status)
    {
        /** Based on buffer size, detect the in count mode setting.
         *  The driver is able to read input data in chunks.  However the size of each chunk
         *  is limited to BUFFER_MAX_SIZE.   Therefore, if the input data size is larger than
         *  BUFFER_MAX_SIZE, the driver assumes the entire input data is present, and can be decoded
         *  without additional input data.   Otherwise, the driver enables input stream feature.
         *  This works even if the entire input size is smaller than BUFFER_MAX_SIZE. */
        if (data_buffer_size <= BUFFER_MAX_SIZE)
        {
            uint32_t inten1;
            inten1  = HW_JPEG_InterruptEnable1Get();
            inten1 |= JPEG_INTE1_JINEN;
            HW_JPEG_InterruptEnable1Set(inten1);

            HW_JPEG_DecodeInCountModeEnable();

            HW_JPEG_DecodeInCountModeConfig(JPEG_DECODE_COUNT_MODE_ADDRESS_REINITIALIZE, (uint16_t) data_buffer_size);
        }
        else
        {
            HW_JPEG_DecodeInCountModeDisable();
        }

        /** Set the internal status.  */
        p_ctrl->status = JPEG_DECODE_STATUS_HEADER_PROCESSING;

        /* Start the core. */
        HW_JPEG_CoreStart();
    }

    /* If the JPEG driver is paused for input data, the driver needs to resume the
     * operation. */
    else if (JPEG_DECODE_STATUS_INPUT_PAUSE & p_ctrl->status)
    {
        /** Clear internal status information. */
        p_ctrl->status &= (~JPEG_DECODE_STATUS_INPUT_PAUSE);

        /** Set internal status information. */
        p_ctrl->status |= JPEG_DECODE_STATUS_RUNNING;

        HW_JPEG_DecodeInCountModeConfig(JPEG_DECODE_COUNT_MODE_ADDRESS_REINITIALIZE, (uint16_t) data_buffer_size);
        /** Resume the jpeg core.  */
        HW_JPEG_DecodeInCountModeResume(0x01);
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Configure horizontal and vertical subsample.
 * @note   Use for scaling the decoded
 *         image.
 *
 * @retval        SSP_SUCCESS                Horizontal Stride value is properly configured.
 * @retval        SSP_ERR_ASSERTION          Pointer to the control block is NULL.
 * @retval        SSP_ERR_NOT_OPEN           JPEG not opened.
 **********************************************************************************************************************/
ssp_err_t R_JPEG_Decode_ImageSubsampleSet (jpeg_decode_ctrl_t * const p_ctrl,
                                           jpeg_decode_subsample_t    horizontal_subsample,
                                           jpeg_decode_subsample_t    vertical_subsample)
{
#if JPEG_DECODE_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    JPEG_ERROR_RETURN((JPEG_DECODE_STATUS_FREE != p_ctrl->status), SSP_ERR_NOT_OPEN);
#endif

    /* Return error code if any errors were detected. */
    if (JPEG_DECODE_STATUS_ERROR & p_ctrl->status)
    {
        return(p_ctrl->error_code);
    }

    HW_JPEG_OutputSubsampleSet(horizontal_subsample, vertical_subsample);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Configure horizontal stride setting.
 * @note   Use when the horizontal stride needs to match the image width and the image size is
 *         unknown when opening the JPEG driver. (If the image size is known prior to the open call,
 *         pass the horizontal stride value in the jpef_cfg_t  structure.) After the image size becomes available,
 *         use this function to update the horizontal stride value. If the driver must decode one
 *         line at a time, the horizontal stride can be set to zero.
 *
 * @retval        SSP_SUCCESS                Horizontal Stride value is properly configured.
 * @retval        SSP_ERR_ASSERTION          Pointer to the control block is NULL.
 * @retval        SSP_ERR_INVALID_ALIGNMENT  Horizontal stride is not 8-byte aligned.
 * @retval        SSP_ERR_NOT_OPEN           JPEG not opened.
 **********************************************************************************************************************/
ssp_err_t R_JPEG_Decode_HorizontalStrideSet (jpeg_decode_ctrl_t * p_ctrl, uint32_t horizontal_stride)
{
#if JPEG_DECODE_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    JPEG_ERROR_RETURN((JPEG_DECODE_STATUS_FREE != p_ctrl->status), SSP_ERR_NOT_OPEN);
#endif

    if (JPEG_DECODE_PIXEL_FORMAT_ARGB8888 == HW_JPEG_DecodeOutputImageFormatGet())
    {
        horizontal_stride *= 4;
    }
    else
    {
        horizontal_stride *= 2;
    }

#if JPEG_DECODE_CFG_PARAM_CHECKING_ENABLE
    JPEG_ERROR_RETURN(!(horizontal_stride & JPEG_ALIGNMENT_8), SSP_ERR_INVALID_ALIGNMENT);
#endif

    /** Record the horizontal stride value in the control block */
    p_ctrl->horizontal_stride = horizontal_stride;

    /** Set the horizontal stride.  */
    HW_JPEG_HorizontalStrideSet(horizontal_stride);

    /** If the parameters all are set, resume the core to decode.   */
    if ((JPEG_DECODE_STATUS_IMAGE_SIZE_READY == p_ctrl->status) && HW_JPEG_DecodeDestinationAddressGet() && HW_JPEG_DecodeSourceAddressGet())
    {
        uint16_t lines_to_decode = 0;
        uint16_t horizontal, vertical;
        uint16_t horizontal_bytes;

        /** For the given buffer size, compute number of lines to decode. */
        if (p_ctrl->horizontal_stride && p_ctrl->outbuffer_size)
        {
            lines_to_decode = (uint16_t) (p_ctrl->outbuffer_size / p_ctrl->horizontal_stride);

            if (JPEG_DECODE_COLOR_SPACE_YCBCR420 == HW_JPEG_InputPixelFormatGet())
            {
                lines_to_decode &= (uint16_t) (~15);
                lines_to_decode = lines_to_decode/2;

            }
            else
            {
                lines_to_decode &= (uint16_t) (~7);
            }
            if (!lines_to_decode)
            {
                return SSP_ERR_JPEG_BUFFERSIZE_NOT_ENOUGH;
            }
        }
        HW_JPEG_ImageSizeGet(&horizontal, &vertical);

        if(JPEG_DECODE_PIXEL_FORMAT_ARGB8888 == p_ctrl->pixel_format)
        {
           horizontal_bytes = (uint16_t) (horizontal * 4);
        }
        else
        {
           horizontal_bytes = (uint16_t) (horizontal * 2);
        }
        if(horizontal_bytes > p_ctrl->horizontal_stride)
        {
           p_ctrl->status |= JPEG_DECODE_STATUS_ERROR;
           p_ctrl->error_code = SSP_ERR_JPEG_UNSUPPORTED_IMAGE_SIZE;

           return SSP_ERR_JPEG_UNSUPPORTED_IMAGE_SIZE;
        }

        /** Configure the out count mode. */
        uint32_t inten1;
        inten1  = HW_JPEG_InterruptEnable1Get();
        inten1 |= JPEG_INTE1_DOUTLEN;
        HW_JPEG_InterruptEnable1Set(inten1);

        HW_JPEG_DecodeOutCountModeEnable();

        HW_JPEG_DecodeOutCountModeConfig(JPEG_DECODE_COUNT_MODE_ADDRESS_REINITIALIZE, lines_to_decode);

        /* Set the ctrl status.  */
        p_ctrl->status |= JPEG_DECODE_STATUS_RUNNING;

        /* Clear JPEG stop.  */
        HW_JPEG_CoreStopClear();
    }
    return SSP_SUCCESS;
}


/*******************************************************************************************************************//**
 * @brief  Cancel an outstanding JPEG codec operation and close the device.
 *
 * @retval        SSP_SUCCESS                The input data buffer is properly assigned to JPEG Codec device.
 * @retval        SSP_ERR_ASSERTION          Pointer to the control block is NULL.
 * @retval        SSP_ERR_NOT_OPEN           JPEG not opened.
 **********************************************************************************************************************/
ssp_err_t R_JPEG_Decode_Close (jpeg_decode_ctrl_t * p_ctrl)
{
#if JPEG_DECODE_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    JPEG_ERROR_RETURN((JPEG_DECODE_STATUS_FREE != p_ctrl->status), SSP_ERR_NOT_OPEN);
#endif

    /** Clear JPEG JINTE0 interrupt and JINTE1 interrupt. */
    HW_JPEG_InterruptEnable0Set(0);
    HW_JPEG_InterruptEnable1Set(0);

    HW_JPEG_InterruptStatus0Set(0);
    HW_JPEG_InterruptStatus1Set(0);

    /** Disable
     *  JEDI and JDTI at NVIC */
    NVIC_DisableIRQ(JPEG_JEDI_IRQn);
    NVIC_DisableIRQ(JPEG_JDTI_IRQn);

    HW_JPEG_DecodeInCountModeResume(0x01);
    HW_JPEG_DecodeOutCountModeResume(0x01);


    /** Power off the JPEG codec.  */
    HW_JPEG_PowerOff();

    /** Reset the jpeg status flag in the driver.  */
    gp_ctrl        = NULL;
    p_ctrl->status = JPEG_DECODE_STATUS_FREE;

    /** Unlock module at BSP level. */
    bsp_hw_lock_t lock = BSP_HW_LOCK_JPEG;
    R_BSP_HardwareUnlock(lock);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Obtain the size of the image.  This operation is valid during
 *         JPEG decoding operation.
 *
 * @retval        SSP_SUCCESS                 The image size is available and the horizontal and vertical values are
 *                                            stored in the memory pointed to by p_horizontal_size and p_vertical_size.
 * @retval        SSP_ERR_ASSERTION           Pointer to the control block is NULL.
 * @retval        SSP_ERR_IMAGE_SIZE_UNKNOWN  The image size is unknown.  More input data may be needed.
 * @retval        SSP_ERR_INVALID_MODE        JPEG Codec module is not decoding.
 * @retval        SSP_ERR_NOT_OPEN            JPEG is not opened.
 **********************************************************************************************************************/
ssp_err_t R_JPEG_Decode_ImageSizeGet (jpeg_decode_ctrl_t * p_ctrl,
                                      uint16_t           * p_horizontal_size,
                                      uint16_t           * p_vertical_size)
{
    SSP_PARAMETER_NOT_USED(p_ctrl);

#if JPEG_DECODE_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_horizontal_size);
    SSP_ASSERT(NULL != p_vertical_size);
    JPEG_ERROR_RETURN((JPEG_DECODE_STATUS_FREE != p_ctrl->status), SSP_ERR_NOT_OPEN);
#endif

    /* Get the image horizontal and vertical size.*/
    HW_JPEG_ImageSizeGet(p_horizontal_size, p_vertical_size);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Get the status of the JPEG codec.  This function can also be used to poll the device.
 *
 * @retval         SSP_SUCCESS                 The status information is successfully retrieved.
 * @retval         SSP_ERR_ASSERTION           Pointer to the control block is NULL.
 * @retval         SSP_ERR_NOT_OPEN            JPEG is not opened.
 **********************************************************************************************************************/
ssp_err_t R_JPEG_Decode_StatusGet (jpeg_decode_ctrl_t * p_ctrl, jpeg_decode_status_t * p_status)
{
#if JPEG_DECODE_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_status);
    JPEG_ERROR_RETURN((JPEG_DECODE_STATUS_FREE != p_ctrl->status), SSP_ERR_NOT_OPEN);
#endif


    /** HW does not report error.  Return internal status information. */
    *p_status = p_ctrl->status;

    if (JPEG_DECODE_STATUS_ERROR & p_ctrl->status)
    {
        return(p_ctrl->error_code);
    }

    return SSP_SUCCESS;
}


/*******************************************************************************************************************//**
 * @brief  Get the input pixel format.
 *
 * @retval         SSP_SUCCESS                 The status information is successfully retrieved.
 * @retval         SSP_ERR_ASSERTION           Pointer to the control block is NULL.
 * @retval         SSP_ERR_NOT_OPEN            JPEG is not opened.
 **********************************************************************************************************************/
ssp_err_t R_JPEG_Decode_PixelFormatGet (jpeg_decode_ctrl_t * p_ctrl, jpeg_decode_color_space_t * p_color_space)
{
    SSP_PARAMETER_NOT_USED(p_ctrl);

#if JPEG_DECODE_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_color_space);
    JPEG_ERROR_RETURN((JPEG_DECODE_STATUS_FREE != p_ctrl->status), SSP_ERR_NOT_OPEN);
#endif

    /** HW does not report error.  Return internal status information. */
    *p_color_space = HW_JPEG_InputPixelFormatGet();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief   Get version of the display interface and GLCD HAL code.
 *
 * @retval      SSP_SUCCESS   Version number
 * @note  This function is reentrant.
 **********************************************************************************************************************/
ssp_err_t R_JPEG_Decode_VersionGet (ssp_version_t * p_version)
{
    *p_version = g_module_version;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup JPEG_DECODE)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief   JPEG internal function:  JEDI Interrupt Service Routine (ISR). If
 *          present, this ISR replaces the weak one in BSP start code.
 * @param   None
 * @retval  None
 **********************************************************************************************************************/
void jpeg_jedi_isr (void)
{
    /** Save context if RTOS is used */
    SF_CONTEXT_SAVE

    jpeg_decode_ctrl_t * p_ctrl;
    uint8_t            intertype;
    uint16_t horizontal, vertical;
    jpeg_decode_color_space_t color_format;
    uint16_t horizontal_bytes;
    jpeg_decode_callback_args_t args;

    /** Obtain the control block. */
    p_ctrl = gp_ctrl;

    /** Get the interrupt type.  */
    intertype = HW_JPEG_InterruptStatus0Get();

    /** Clear any pending interrupts. */
    R_BSP_IrqStatusClear(JPEG_JEDI_IRQn);

    /** Clear the interrupt status.  */
    HW_JPEG_InterruptStatus0Set(0);

    /** Do nothing if the control block is NULL */
    if ((NULL == p_ctrl) || (JPEG_DECODE_STATUS_IDLE & p_ctrl->status) || (0 == intertype))
    {
        /* Restore context if RTOS is used */
        SF_CONTEXT_RESTORE

        return;
    }

    if (intertype & JPEG_INTE0_INS5)
    {

        /** Clear the request.  */
        HW_JPEG_ClearRequest();

        /* Set the internal status.  */
        p_ctrl->status = JPEG_DECODE_STATUS_ERROR;

        p_ctrl->error_code = (ssp_err_t) (HW_JPEG_ErrorGet() + (uint32_t) SSP_ERR_JPEG_ERR);
        /** Invoke user-supplied callback function if it is set. */
        if (NULL != p_ctrl->p_callback)
        {
            args.status = p_ctrl->status;
            p_ctrl->p_callback(&args);
        }
    }

    if (intertype & JPEG_INTE0_INS3)
    {

        /** Clear the request.  */
        HW_JPEG_ClearRequest();

        /** Clear internal status information. */
        p_ctrl->status &= (~JPEG_DECODE_STATUS_HEADER_PROCESSING);

        /* Set the ctrl status.  */
        p_ctrl->status |= JPEG_DECODE_STATUS_IMAGE_SIZE_READY;

        /** JPEG header is decoded.  Obtain image size, and input pixel format.  Verify that image size (width and height)
            is aligned to the Minimum Coded Unit. */

        HW_JPEG_ImageSizeGet(&horizontal, &vertical);
        color_format = HW_JPEG_InputPixelFormatGet();

        switch(color_format)
        {
        case JPEG_DECODE_COLOR_SPACE_YCBCR444:
            /* 8 lines by 8 pixels. */
            if((horizontal & JPEG_ALIGNMENT_8) || (vertical & JPEG_ALIGNMENT_8))
            {
                p_ctrl->status |= JPEG_DECODE_STATUS_ERROR;
                p_ctrl->error_code = SSP_ERR_JPEG_UNSUPPORTED_IMAGE_SIZE;
            }
            break;
            
        case JPEG_DECODE_COLOR_SPACE_YCBCR422:
            /* 8 lines by 16 pixels. */
            if((horizontal & JPEG_ALIGNMENT_16) || (vertical & JPEG_ALIGNMENT_8))
            {
                p_ctrl->status |= JPEG_DECODE_STATUS_ERROR;
                p_ctrl->error_code = SSP_ERR_JPEG_UNSUPPORTED_IMAGE_SIZE;
            }
            break;

        case JPEG_DECODE_COLOR_SPACE_YCBCR411:
            /* 8 lines by 32 pixels. */
            if((horizontal & JPEG_ALIGNMENT_32) || (vertical & JPEG_ALIGNMENT_8))
            {
                p_ctrl->status |= JPEG_DECODE_STATUS_ERROR;
                p_ctrl->error_code = SSP_ERR_JPEG_UNSUPPORTED_IMAGE_SIZE;
            }
            break;

        case JPEG_DECODE_COLOR_SPACE_YCBCR420:        
            /* 16 lines by 16 pixels. */
            if((horizontal & JPEG_ALIGNMENT_16) || (vertical & JPEG_ALIGNMENT_16))
            {
                p_ctrl->status |= JPEG_DECODE_STATUS_ERROR;
                p_ctrl->error_code = SSP_ERR_JPEG_UNSUPPORTED_IMAGE_SIZE;
            }
            break;
            
        default:
            p_ctrl->status |= JPEG_DECODE_STATUS_ERROR;
            p_ctrl->error_code = SSP_ERR_JPEG_UNSUPPORTED_PIXEL_FORMAT;
            break;
        }

        /** Invoke user-supplied callback function if it is set. */
        if (NULL != p_ctrl->p_callback)
        {
            args.status = p_ctrl->status;
            p_ctrl->p_callback(&args);
        }
    }

    if (intertype & JPEG_INTE0_INS6)
    {
        /** Clear the request.  */
        HW_JPEG_ClearRequest();

        /** Clear internal status information. */
        p_ctrl->status &= (~JPEG_DECODE_STATUS_RUNNING);
    }

    if (0 == (intertype & (JPEG_INTE0_INS6 | JPEG_INTE0_INS3 | JPEG_INTE0_INS5)))
    {
        /* Restore context if RTOS is used */
        SF_CONTEXT_RESTORE;

        return;
    }


    if(p_ctrl->status & JPEG_DECODE_STATUS_ERROR)
    {
        /* If error is detected, no need to further process this interrupt.  Simply return. */
        SF_CONTEXT_RESTORE;
            
        return;
    }


    if (intertype & JPEG_INTE0_INS3)
    {

        /** If both Input buffer and  output buffer are set, and horizontal stride is set, the driver is available
         *  to determine the number of lines to decode, and start the decoding operation. */
        if (HW_JPEG_DecodeSourceAddressGet() && HW_JPEG_DecodeDestinationAddressGet() && HW_JPEG_HorizontalStrideGet())
        {
            HW_JPEG_ImageSizeGet(&horizontal, &vertical);

            if(JPEG_DECODE_PIXEL_FORMAT_ARGB8888 == p_ctrl->pixel_format)
            {
               horizontal_bytes = (uint16_t) (horizontal * 4);
            }
            else
            {
               horizontal_bytes = (uint16_t) (horizontal * 2);
            }
            if(horizontal_bytes > p_ctrl->horizontal_stride)
            {
               p_ctrl->status |= JPEG_DECODE_STATUS_ERROR;
               p_ctrl->error_code = SSP_ERR_JPEG_UNSUPPORTED_IMAGE_SIZE;

               /* Restore context if RTOS is used */
               SF_CONTEXT_RESTORE;

               return;
            }

            uint16_t lines_to_decode = 0;
            /** For the given buffer size, compute number of lines to decode. */
            if (p_ctrl->horizontal_stride && p_ctrl->outbuffer_size)
            {
                lines_to_decode = (uint16_t) (p_ctrl->outbuffer_size / p_ctrl->horizontal_stride);

                HW_JPEG_ImageSizeGet(&horizontal, &vertical);
                if(lines_to_decode > (vertical - p_ctrl->total_lines_decoded))
                {
                    lines_to_decode = (uint16_t) (vertical - (uint16_t) p_ctrl->total_lines_decoded);
                }

                if (JPEG_DECODE_COLOR_SPACE_YCBCR420 == HW_JPEG_InputPixelFormatGet())
                {
                    lines_to_decode &= (uint16_t) (~15);
                    lines_to_decode = lines_to_decode/2;
                }
                else
                {
                    lines_to_decode &= (uint16_t) (~7);
                }
            }

            /** according the buffer size, detect the out count mode setting.  */
            uint32_t inten1;
            inten1  = HW_JPEG_InterruptEnable1Get();
            inten1 |= JPEG_INTE1_DOUTLEN;
            HW_JPEG_InterruptEnable1Set(inten1);

            HW_JPEG_DecodeOutCountModeEnable();

            HW_JPEG_DecodeOutCountModeConfig(JPEG_DECODE_COUNT_MODE_ADDRESS_REINITIALIZE, lines_to_decode);

            /* Set the ctrl status.  */
            p_ctrl->status |= JPEG_DECODE_STATUS_RUNNING;

            /* Clear JPEG stop.  */
            HW_JPEG_CoreStopClear();
        }
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE;
}



/*******************************************************************************************************************//**
 * @brief   JPEG internal function:  JDTI Interrupt Service Routine (ISR). If
 *          present this ISR replaces the weak one in BSP start code.
 * @param   None
 * @retval  None
 **********************************************************************************************************************/
void jpeg_jdti_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    jpeg_decode_ctrl_t * p_ctrl;
    uint32_t            intertype;
    jpeg_decode_callback_args_t args;
    p_ctrl    = gp_ctrl;
    /** Get the interrupt type.  */
    intertype = HW_JPEG_InterruptStatus1Get();

    /** Clear the interrupt flag. */
    R_BSP_IrqStatusClear(JPEG_JDTI_IRQn);

    /** Clear the interrupt flag.  */
    HW_JPEG_InterruptStatus1Set(0x0);

    /** Do nothing if the control block is NULL */
    if ((NULL == p_ctrl) || (JPEG_DECODE_STATUS_IDLE & p_ctrl->status) || (0 == intertype) )
    {
        /* Restore context if RTOS is used */
        SF_CONTEXT_RESTORE;

        return;
    }

    /* Return if there are errors. */
    if (JPEG_DECODE_STATUS_ERROR & p_ctrl->status)
    {
        /* Restore context if RTOS is used */
        SF_CONTEXT_RESTORE;

        return;
    }


    if (intertype & JPEG_INTE1_JINF)
    {
        /** Clear internal status information. */
        p_ctrl->status &= (~JPEG_DECODE_STATUS_RUNNING);

        /* Set the ctrl status.  */
        p_ctrl->status |= JPEG_DECODE_STATUS_INPUT_PAUSE;

        /** Clear the source address.  */
        HW_JPEG_DecodeSourceAddressSet(0);

        /** Invoke user-supplied callback function if it is set. */
        if ((NULL != p_ctrl->p_callback))
        {
            args.status = p_ctrl->status;
            p_ctrl->p_callback(&args);
        }
    }

    if (intertype  & JPEG_INTE1_DOUTLF)
    {
        uint32_t lines_decoded = 0;
        uint16_t horizontal, vertical;      

        /** Clear internal status information. */
        p_ctrl->status &= (~JPEG_DECODE_STATUS_RUNNING);

        /* Set the ctrl status.  */
        p_ctrl->status |= JPEG_DECODE_STATUS_OUTPUT_PAUSE;

        /** Clear the destination address.  */
        HW_JPEG_DecodeDestinationAddressSet(0);

        /** Obtain the number of lines decoded in this operation. */
        if(SSP_SUCCESS != R_JPEG_Decode_LinesDecodedGet(p_ctrl, &lines_decoded))
        {

            /* Restore context if RTOS is used */
            SF_CONTEXT_RESTORE
                
            return;
        }

        /** Increment the number of lines decoded. */
        p_ctrl->total_lines_decoded += lines_decoded;

        HW_JPEG_ImageSizeGet(&horizontal, &vertical);
        if(p_ctrl->total_lines_decoded > vertical)
        {
            p_ctrl->total_lines_decoded = vertical;
        }

        /** Invoke user-supplied callback function if it is set. */
        if ((NULL != p_ctrl->p_callback))
        {
            args.status = p_ctrl->status;
            p_ctrl->p_callback(&args);
        }
    }

    if (intertype & JPEG_INTE1_DBTF)
    {
        /** Clear internal status information. */
        p_ctrl->status &= (~JPEG_DECODE_STATUS_RUNNING);

        /* Set the ctrl status.  */
        p_ctrl->status |= JPEG_DECODE_STATUS_DONE;

        /** Invoke user-supplied callback function if it is set. */
        if ((NULL != p_ctrl->p_callback))
        {
            args.status = p_ctrl->status;
            p_ctrl->p_callback(&args);
        }
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}


