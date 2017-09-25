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
/***********************************************************************************************************************
* File Name    : startup_S128.c
* Description  : Startup for the S128
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"

/* Only build this file if this board is chosen. */
#if defined(BSP_MCU_GROUP_S128)

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Defines function pointers to be used with vector table. */
typedef void (* exc_ptr_t)(void);

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
void Reset_Handler(void);
void Default_Handler(void);
int  main(void);

/***********************************************************************************************************************
* Function Name: Reset_Handler
* Description  : MCU starts executing here out of reset. Main stack pointer is setup already.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void Reset_Handler (void)
{
    /* Initialize system using BSP. */
    SystemInit();

    /* Call user application. */
    main();

    while (1)
    {
        /* Infinite Loop. */
    }
}

/***********************************************************************************************************************
* Function Name: Default_Handler
* Description  : Default exception handler.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void Default_Handler (void)
{
    while(1);
}

/* Stacks. */
/* Main stack */
/* IAR requires pragma for setting alignment. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t g_main_stack[BSP_CFG_STACK_MAIN_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_STACK) \
                                                      BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);

/* Process stack */
#if (BSP_CFG_STACK_PROCESS_BYTES > 0)
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
BSP_DONT_REMOVE static uint8_t g_process_stack[BSP_CFG_STACK_PROCESS_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_STACK) \
                                                                            BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif

/* Heap */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
BSP_DONT_REMOVE static uint8_t g_heap[BSP_CFG_HEAP_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_HEAP) \
                                                          BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);

/* All system exceptions in the vector table are weak references to Default_Handler. If the user wishes to handle
 * these exceptions in their code they should define their own function with the same name.
 */
#if defined(__ICCARM__)
#define WEAK_REF_ATTRIBUTE

#pragma weak HardFault_Handler                        = Default_Handler
#pragma weak MemManage_Handler                        = Default_Handler
#pragma weak BusFault_Handler                         = Default_Handler
#pragma weak UsageFault_Handler                       = Default_Handler
#pragma weak SVC_Handler                              = Default_Handler
#pragma weak DebugMon_Handler                         = Default_Handler
#pragma weak PendSV_Handler                           = Default_Handler
#pragma weak SysTick_Handler                          = Default_Handler
#pragma weak icu_irq0_isr                             = Default_Handler
#pragma weak icu_irq1_isr                             = Default_Handler
#pragma weak icu_irq2_isr                             = Default_Handler
#pragma weak icu_irq3_isr                             = Default_Handler
#pragma weak icu_irq4_isr                             = Default_Handler
#pragma weak icu_irq5_isr                             = Default_Handler
#pragma weak icu_irq6_isr                             = Default_Handler
#pragma weak icu_irq7_isr                             = Default_Handler
#pragma weak dtc_complete_isr                         = Default_Handler
#pragma weak dtc_end_isr                              = Default_Handler
#pragma weak icu_snooze_cancel_isr                    = Default_Handler
#pragma weak fcu_frdyi_isr                            = Default_Handler
#pragma weak lvd_lvd1_isr                             = Default_Handler
#pragma weak lvd_lvd2_isr                             = Default_Handler
#pragma weak cgc_mosc_stop_isr                        = Default_Handler
#pragma weak lpm_snooze_request_isr                   = Default_Handler
#pragma weak agt0_int_isr                             = Default_Handler
#pragma weak agt0_compare_a_isr                       = Default_Handler
#pragma weak agt0_compare_b_isr                       = Default_Handler
#pragma weak agt1_int_isr                             = Default_Handler
#pragma weak agt1_compare_a_isr                       = Default_Handler
#pragma weak agt1_compare_b_isr                       = Default_Handler
#pragma weak iwdt_underflow_isr                       = Default_Handler
#pragma weak wdt_underflow_isr                        = Default_Handler
#pragma weak rtc_alarm_isr                            = Default_Handler
#pragma weak rtc_period_isr                           = Default_Handler
#pragma weak rtc_carry_isr                            = Default_Handler
#pragma weak adc0_scan_end_isr                        = Default_Handler
#pragma weak adc0_scan_end_b_isr                      = Default_Handler
#pragma weak adc0_window_a_isr                        = Default_Handler
#pragma weak adc0_window_b_isr                        = Default_Handler
#pragma weak adc0_compare_match_isr                   = Default_Handler
#pragma weak adc0_compare_mismatch_isr                = Default_Handler
#pragma weak comp_hs_0_isr                            = Default_Handler
#pragma weak comp_hs_1_isr                            = Default_Handler
#pragma weak comp_hs_2_isr                            = Default_Handler
#pragma weak comp_lp_0_isr                            = Default_Handler
#pragma weak comp_lp_1_isr                            = Default_Handler
#pragma weak usbfs_int_isr                            = Default_Handler
#pragma weak usbfs_resume_isr                         = Default_Handler
#pragma weak iic0_rxi_isr                             = Default_Handler
#pragma weak iic0_txi_isr                             = Default_Handler
#pragma weak iic0_tei_isr                             = Default_Handler
#pragma weak iic0_eri_isr                             = Default_Handler
#pragma weak iic0_wui_isr                             = Default_Handler
#pragma weak iic1_rxi_isr                             = Default_Handler
#pragma weak iic1_txi_isr                             = Default_Handler
#pragma weak iic1_tei_isr                             = Default_Handler
#pragma weak iic1_eri_isr                             = Default_Handler
#pragma weak ctsu_write_isr                           = Default_Handler
#pragma weak ctsu_read_isr                            = Default_Handler
#pragma weak ctsu_end_isr                             = Default_Handler
#pragma weak key_int_isr                              = Default_Handler
#pragma weak doc_int_isr                              = Default_Handler
#pragma weak cac_frequency_error_isr                  = Default_Handler
#pragma weak cac_measurement_end_isr                  = Default_Handler
#pragma weak cac_overflow_isr                         = Default_Handler
#pragma weak can0_error_isr                           = Default_Handler
#pragma weak can0_fifo_rx_isr                         = Default_Handler
#pragma weak can0_fifo_tx_isr                         = Default_Handler
#pragma weak can0_mailbox_rx_isr                      = Default_Handler
#pragma weak can0_mailbox_tx_isr                      = Default_Handler
#pragma weak ioport_event_1_isr                       = Default_Handler
#pragma weak ioport_event_2_isr                       = Default_Handler
#pragma weak elc_software_event_0_isr                 = Default_Handler
#pragma weak elc_software_event_1_isr                 = Default_Handler
#pragma weak poeg0_event_isr                          = Default_Handler
#pragma weak poeg1_event_isr                          = Default_Handler
#pragma weak dali_dei_isr                             = Default_Handler
#pragma weak dali_cli_isr                             = Default_Handler
#pragma weak dali_sdi_isr                             = Default_Handler
#pragma weak dali_bpi_isr                             = Default_Handler
#pragma weak dali_fei_isr                             = Default_Handler
#pragma weak dali_sdi_or_bpi_isr                      = Default_Handler
#pragma weak gpt0_capture_compare_a_isr               = Default_Handler
#pragma weak gpt0_capture_compare_b_isr               = Default_Handler
#pragma weak gpt0_compare_c_isr                       = Default_Handler
#pragma weak gpt0_compare_d_isr                       = Default_Handler
#pragma weak gpt0_counter_overflow_isr                = Default_Handler
#pragma weak gpt0_counter_underflow_isr               = Default_Handler
#pragma weak gpt1_capture_compare_a_isr               = Default_Handler
#pragma weak gpt1_capture_compare_b_isr               = Default_Handler
#pragma weak gpt1_compare_c_isr                       = Default_Handler
#pragma weak gpt1_compare_d_isr                       = Default_Handler
#pragma weak gpt1_counter_overflow_isr                = Default_Handler
#pragma weak gpt1_counter_underflow_isr               = Default_Handler
#pragma weak gpt2_capture_compare_a_isr               = Default_Handler
#pragma weak gpt2_capture_compare_b_isr               = Default_Handler
#pragma weak gpt2_compare_c_isr                       = Default_Handler
#pragma weak gpt2_compare_d_isr                       = Default_Handler
#pragma weak gpt2_counter_overflow_isr                = Default_Handler
#pragma weak gpt2_counter_underflow_isr               = Default_Handler
#pragma weak gpt3_capture_compare_a_isr               = Default_Handler
#pragma weak gpt3_capture_compare_b_isr               = Default_Handler
#pragma weak gpt3_compare_c_isr                       = Default_Handler
#pragma weak gpt3_compare_d_isr                       = Default_Handler
#pragma weak gpt3_counter_overflow_isr                = Default_Handler
#pragma weak gpt3_counter_underflow_isr               = Default_Handler
#pragma weak gpt4_capture_compare_a_isr               = Default_Handler
#pragma weak gpt4_capture_compare_b_isr               = Default_Handler
#pragma weak gpt4_compare_c_isr                       = Default_Handler
#pragma weak gpt4_compare_d_isr                       = Default_Handler
#pragma weak gpt4_counter_overflow_isr                = Default_Handler
#pragma weak gpt4_counter_underflow_isr               = Default_Handler
#pragma weak gpt5_capture_compare_a_isr               = Default_Handler
#pragma weak gpt5_capture_compare_b_isr               = Default_Handler
#pragma weak gpt5_compare_c_isr                       = Default_Handler
#pragma weak gpt5_compare_d_isr                       = Default_Handler
#pragma weak gpt5_counter_overflow_isr                = Default_Handler
#pragma weak gpt5_counter_underflow_isr               = Default_Handler
#pragma weak gpt6_capture_compare_a_isr               = Default_Handler
#pragma weak gpt6_capture_compare_b_isr               = Default_Handler
#pragma weak gpt6_compare_c_isr                       = Default_Handler
#pragma weak gpt6_compare_d_isr                       = Default_Handler
#pragma weak gpt6_counter_overflow_isr                = Default_Handler
#pragma weak gpt6_counter_underflow_isr               = Default_Handler
#pragma weak ops_uvw_edge_isr                         = Default_Handler
#pragma weak sci0_rxi_isr                             = Default_Handler
#pragma weak sci0_txi_isr                             = Default_Handler
#pragma weak sci0_tei_isr                             = Default_Handler
#pragma weak sci0_eri_isr                             = Default_Handler
#pragma weak sci0_am_isr                              = Default_Handler
#pragma weak sci0_rxi_or_eri_isr                      = Default_Handler
#pragma weak sci1_rxi_isr                             = Default_Handler
#pragma weak sci1_txi_isr                             = Default_Handler
#pragma weak sci1_tei_isr                             = Default_Handler
#pragma weak sci1_eri_isr                             = Default_Handler
#pragma weak sci1_am_isr                              = Default_Handler
#pragma weak sci9_rxi_isr                             = Default_Handler
#pragma weak sci9_txi_isr                             = Default_Handler
#pragma weak sci9_tei_isr                             = Default_Handler
#pragma weak sci9_eri_isr                             = Default_Handler
#pragma weak sci9_am_isr                              = Default_Handler
#pragma weak spi0_rxi_isr                             = Default_Handler
#pragma weak spi0_txi_isr                             = Default_Handler
#pragma weak spi0_idle_isr                            = Default_Handler
#pragma weak spi0_eri_isr                             = Default_Handler
#pragma weak spi0_tei_isr                             = Default_Handler
#pragma weak spi1_rxi_isr                             = Default_Handler
#pragma weak spi1_txi_isr                             = Default_Handler
#pragma weak spi1_idle_isr                            = Default_Handler
#pragma weak spi1_eri_isr                             = Default_Handler
#pragma weak spi1_tei_isr                             = Default_Handler
#pragma weak aes_wrreq_isr                            = Default_Handler
#pragma weak aes_rdreq_isr                            = Default_Handler
#pragma weak trng_rdreq_isr                           = Default_Handler
#elif defined(__GNUC__)
#define WEAK_REF_ATTRIBUTE      __attribute__ ((weak, alias("Default_Handler")))
#endif

void NMI_Handler(void);                      //NMI has many sources and is handled by BSP
void HardFault_Handler                       (void) WEAK_REF_ATTRIBUTE;
void MemManage_Handler                       (void) WEAK_REF_ATTRIBUTE;
void BusFault_Handler                        (void) WEAK_REF_ATTRIBUTE;
void UsageFault_Handler                      (void) WEAK_REF_ATTRIBUTE;
void SVC_Handler                             (void) WEAK_REF_ATTRIBUTE;
void DebugMon_Handler                        (void) WEAK_REF_ATTRIBUTE;
void PendSV_Handler                          (void) WEAK_REF_ATTRIBUTE;
void SysTick_Handler                         (void) WEAK_REF_ATTRIBUTE;
void icu_irq0_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq1_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq2_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq3_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq4_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq5_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq6_isr                            (void) WEAK_REF_ATTRIBUTE;
void icu_irq7_isr                            (void) WEAK_REF_ATTRIBUTE;
void dtc_complete_isr                        (void) WEAK_REF_ATTRIBUTE;
void dtc_end_isr                             (void) WEAK_REF_ATTRIBUTE;
void icu_snooze_cancel_isr                   (void) WEAK_REF_ATTRIBUTE;
void fcu_frdyi_isr                           (void) WEAK_REF_ATTRIBUTE;
void lvd_lvd1_isr                            (void) WEAK_REF_ATTRIBUTE;
void lvd_lvd2_isr                            (void) WEAK_REF_ATTRIBUTE;
void cgc_mosc_stop_isr                       (void) WEAK_REF_ATTRIBUTE;
void lpm_snooze_request_isr                  (void) WEAK_REF_ATTRIBUTE;
void agt0_int_isr                            (void) WEAK_REF_ATTRIBUTE;
void agt0_compare_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void agt0_compare_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void agt1_int_isr                            (void) WEAK_REF_ATTRIBUTE;
void agt1_compare_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void agt1_compare_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void iwdt_underflow_isr                      (void) WEAK_REF_ATTRIBUTE;
void wdt_underflow_isr                       (void) WEAK_REF_ATTRIBUTE;
void rtc_alarm_isr                           (void) WEAK_REF_ATTRIBUTE;
void rtc_period_isr                          (void) WEAK_REF_ATTRIBUTE;
void rtc_carry_isr                           (void) WEAK_REF_ATTRIBUTE;
void adc0_scan_end_isr                       (void) WEAK_REF_ATTRIBUTE;
void adc0_scan_end_b_isr                     (void) WEAK_REF_ATTRIBUTE;
void adc0_window_a_isr                       (void) WEAK_REF_ATTRIBUTE;
void adc0_window_b_isr                       (void) WEAK_REF_ATTRIBUTE;
void adc0_compare_match_isr                  (void) WEAK_REF_ATTRIBUTE;
void adc0_compare_mismatch_isr               (void) WEAK_REF_ATTRIBUTE;
void comp_hs_0_isr                           (void) WEAK_REF_ATTRIBUTE;
void comp_hs_1_isr                           (void) WEAK_REF_ATTRIBUTE;
void comp_hs_2_isr                           (void) WEAK_REF_ATTRIBUTE;
void comp_lp_0_isr                           (void) WEAK_REF_ATTRIBUTE;
void comp_lp_1_isr                           (void) WEAK_REF_ATTRIBUTE;
void usbfs_int_isr                           (void) WEAK_REF_ATTRIBUTE;
void usbfs_resume_isr                        (void) WEAK_REF_ATTRIBUTE;
void iic0_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic0_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic0_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic0_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic0_wui_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic1_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic1_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic1_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void iic1_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void ctsu_write_isr                          (void) WEAK_REF_ATTRIBUTE;
void ctsu_read_isr                           (void) WEAK_REF_ATTRIBUTE;
void ctsu_end_isr                            (void) WEAK_REF_ATTRIBUTE;
void key_int_isr                             (void) WEAK_REF_ATTRIBUTE;
void doc_int_isr                             (void) WEAK_REF_ATTRIBUTE;
void cac_frequency_error_isr                 (void) WEAK_REF_ATTRIBUTE;
void cac_measurement_end_isr                 (void) WEAK_REF_ATTRIBUTE;
void cac_overflow_isr                        (void) WEAK_REF_ATTRIBUTE;
void can0_error_isr                          (void) WEAK_REF_ATTRIBUTE;
void can0_fifo_rx_isr                        (void) WEAK_REF_ATTRIBUTE;
void can0_fifo_tx_isr                        (void) WEAK_REF_ATTRIBUTE;
void can0_mailbox_rx_isr                     (void) WEAK_REF_ATTRIBUTE;
void can0_mailbox_tx_isr                     (void) WEAK_REF_ATTRIBUTE;
void ioport_event_1_isr                      (void) WEAK_REF_ATTRIBUTE;
void ioport_event_2_isr                      (void) WEAK_REF_ATTRIBUTE;
void elc_software_event_0_isr                (void) WEAK_REF_ATTRIBUTE;
void elc_software_event_1_isr                (void) WEAK_REF_ATTRIBUTE;
void poeg0_event_isr                         (void) WEAK_REF_ATTRIBUTE;
void poeg1_event_isr                         (void) WEAK_REF_ATTRIBUTE;
void dali_dei_isr                            (void) WEAK_REF_ATTRIBUTE;
void dali_cli_isr                            (void) WEAK_REF_ATTRIBUTE;
void dali_sdi_isr                            (void) WEAK_REF_ATTRIBUTE;
void dali_bpi_isr                            (void) WEAK_REF_ATTRIBUTE;
void dali_fei_isr                            (void) WEAK_REF_ATTRIBUTE;
void dali_sdi_or_bpi_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt0_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt0_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt0_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt0_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt0_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt0_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt1_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt1_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt1_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt1_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt1_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt1_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt2_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt2_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt2_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt2_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt2_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt2_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt3_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt3_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt3_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt3_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt3_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt3_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt4_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt4_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt4_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt4_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt4_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt4_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt5_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt5_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt5_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt5_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt5_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt5_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt6_capture_compare_a_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt6_capture_compare_b_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt6_compare_c_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt6_compare_d_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt6_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt6_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void ops_uvw_edge_isr                        (void) WEAK_REF_ATTRIBUTE;
void sci0_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci0_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci0_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci0_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci0_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci0_rxi_or_eri_isr                     (void) WEAK_REF_ATTRIBUTE;
void sci1_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci1_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci1_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci1_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci1_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci9_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void spi0_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi0_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi0_idle_isr                           (void) WEAK_REF_ATTRIBUTE;
void spi0_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi0_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi1_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi1_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi1_idle_isr                           (void) WEAK_REF_ATTRIBUTE;
void spi1_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void spi1_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void aes_wrreq_isr                           (void) WEAK_REF_ATTRIBUTE;
void aes_rdreq_isr                           (void) WEAK_REF_ATTRIBUTE;
void trng_rdreq_isr                          (void) WEAK_REF_ATTRIBUTE;

/* Vector table. */
BSP_DONT_REMOVE const exc_ptr_t __Vectors[BSP_VECTOR_TABLE_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_VECTOR) =
{
    (exc_ptr_t)(&g_main_stack[0] + BSP_CFG_STACK_MAIN_BYTES),           /*      Initial Stack Pointer     */
    Reset_Handler,                                                      /*      Reset Handler             */
    NMI_Handler,                                                        /*      NMI Handler               */
    HardFault_Handler,                                                  /*      Hard Fault Handler        */
    MemManage_Handler,                                                  /*      MPU Fault Handler         */
    BusFault_Handler,                                                   /*      Bus Fault Handler         */
    UsageFault_Handler,                                                 /*      Usage Fault Handler       */
    0,                                                                  /*      Reserved                  */
    0,                                                                  /*      Reserved                  */
    0,                                                                  /*      Reserved                  */
    0,                                                                  /*      Reserved                  */
    SVC_Handler,                                                        /*      SVCall Handler            */
    DebugMon_Handler,                                                   /*      Debug Monitor Handler     */
    0,                                                                  /*      Reserved                  */
    PendSV_Handler,                                                     /*      PendSV Handler            */
    SysTick_Handler,                                                    /*      SysTick Handler           */
#if (BSP_IRQ_CFG_ICU_IRQ0 != BSP_IRQ_DISABLED)
    icu_irq0_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ1 != BSP_IRQ_DISABLED)
    icu_irq1_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ2 != BSP_IRQ_DISABLED)
    icu_irq2_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ3 != BSP_IRQ_DISABLED)
    icu_irq3_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ4 != BSP_IRQ_DISABLED)
    icu_irq4_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ5 != BSP_IRQ_DISABLED)
    icu_irq5_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ6 != BSP_IRQ_DISABLED)
    icu_irq6_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ7 != BSP_IRQ_DISABLED)
    icu_irq7_isr,
#endif
#if (BSP_IRQ_CFG_DTC_COMPLETE != BSP_IRQ_DISABLED)
    dtc_complete_isr,
#endif
#if (BSP_IRQ_CFG_DTC_END != BSP_IRQ_DISABLED)
    dtc_end_isr,
#endif
#if (BSP_IRQ_CFG_ICU_SNOOZE_CANCEL != BSP_IRQ_DISABLED)
    icu_snooze_cancel_isr,
#endif
#if (BSP_IRQ_CFG_FCU_FRDYI != BSP_IRQ_DISABLED)
    fcu_frdyi_isr,
#endif
#if (BSP_IRQ_CFG_LVD_LVD1 != BSP_IRQ_DISABLED)
    lvd_lvd1_isr,
#endif
#if (BSP_IRQ_CFG_LVD_LVD2 != BSP_IRQ_DISABLED)
    lvd_lvd2_isr,
#endif
#if (BSP_IRQ_CFG_CGC_MOSC_STOP != BSP_IRQ_DISABLED)
    cgc_mosc_stop_isr,
#endif
#if (BSP_IRQ_CFG_LPM_SNOOZE_REQUEST != BSP_IRQ_DISABLED)
    lpm_snooze_request_isr,
#endif
#if (BSP_IRQ_CFG_AGT0_INT != BSP_IRQ_DISABLED)
    agt0_int_isr,
#endif
#if (BSP_IRQ_CFG_AGT0_COMPARE_A != BSP_IRQ_DISABLED)
    agt0_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_AGT0_COMPARE_B != BSP_IRQ_DISABLED)
    agt0_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_AGT1_INT != BSP_IRQ_DISABLED)
    agt1_int_isr,
#endif
#if (BSP_IRQ_CFG_AGT1_COMPARE_A != BSP_IRQ_DISABLED)
    agt1_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_AGT1_COMPARE_B != BSP_IRQ_DISABLED)
    agt1_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_IWDT_UNDERFLOW != BSP_IRQ_DISABLED)
    iwdt_underflow_isr,
#endif
#if (BSP_IRQ_CFG_WDT_UNDERFLOW != BSP_IRQ_DISABLED)
    wdt_underflow_isr,
#endif
#if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED)
    rtc_alarm_isr,
#endif
#if (BSP_IRQ_CFG_RTC_PERIOD != BSP_IRQ_DISABLED)
    rtc_period_isr,
#endif
#if (BSP_IRQ_CFG_RTC_CARRY != BSP_IRQ_DISABLED)
    rtc_carry_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_SCAN_END != BSP_IRQ_DISABLED)
    adc0_scan_end_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_SCAN_END_B != BSP_IRQ_DISABLED)
    adc0_scan_end_b_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_WINDOW_A != BSP_IRQ_DISABLED)
    adc0_window_a_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_WINDOW_B != BSP_IRQ_DISABLED)
    adc0_window_b_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_COMPARE_MATCH != BSP_IRQ_DISABLED)
    adc0_compare_match_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_COMPARE_MISMATCH != BSP_IRQ_DISABLED)
    adc0_compare_mismatch_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_0 != BSP_IRQ_DISABLED)
    comp_hs_0_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_1 != BSP_IRQ_DISABLED)
    comp_hs_1_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_2 != BSP_IRQ_DISABLED)
    comp_hs_2_isr,
#endif
#if (BSP_IRQ_CFG_COMP_LP_0 != BSP_IRQ_DISABLED)
    comp_lp_0_isr,
#endif
#if (BSP_IRQ_CFG_COMP_LP_1 != BSP_IRQ_DISABLED)
    comp_lp_1_isr,
#endif
#if (BSP_IRQ_CFG_USBFS_INT != BSP_IRQ_DISABLED)
    usbfs_int_isr,
#endif
#if (BSP_IRQ_CFG_USBFS_RESUME != BSP_IRQ_DISABLED)
    usbfs_resume_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_RXI != BSP_IRQ_DISABLED)
    iic0_rxi_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_TXI != BSP_IRQ_DISABLED)
    iic0_txi_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_TEI != BSP_IRQ_DISABLED)
    iic0_tei_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_ERI != BSP_IRQ_DISABLED)
    iic0_eri_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_WUI != BSP_IRQ_DISABLED)
    iic0_wui_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_RXI != BSP_IRQ_DISABLED)
    iic1_rxi_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_TXI != BSP_IRQ_DISABLED)
    iic1_txi_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_TEI != BSP_IRQ_DISABLED)
    iic1_tei_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_ERI != BSP_IRQ_DISABLED)
    iic1_eri_isr,
#endif
#if (BSP_IRQ_CFG_CTSU_WRITE != BSP_IRQ_DISABLED)
    ctsu_write_isr,
#endif
#if (BSP_IRQ_CFG_CTSU_READ != BSP_IRQ_DISABLED)
    ctsu_read_isr,
#endif
#if (BSP_IRQ_CFG_CTSU_END != BSP_IRQ_DISABLED)
    ctsu_end_isr,
#endif
#if (BSP_IRQ_CFG_KEY_INT != BSP_IRQ_DISABLED)
    key_int_isr,
#endif
#if (BSP_IRQ_CFG_DOC_INT != BSP_IRQ_DISABLED)
    doc_int_isr,
#endif
#if (BSP_IRQ_CFG_CAC_FREQUENCY_ERROR != BSP_IRQ_DISABLED)
    cac_frequency_error_isr,
#endif
#if (BSP_IRQ_CFG_CAC_MEASUREMENT_END != BSP_IRQ_DISABLED)
    cac_measurement_end_isr,
#endif
#if (BSP_IRQ_CFG_CAC_OVERFLOW != BSP_IRQ_DISABLED)
    cac_overflow_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_ERROR != BSP_IRQ_DISABLED)
    can0_error_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_FIFO_RX != BSP_IRQ_DISABLED)
    can0_fifo_rx_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_FIFO_TX != BSP_IRQ_DISABLED)
    can0_fifo_tx_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_MAILBOX_RX != BSP_IRQ_DISABLED)
    can0_mailbox_rx_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_MAILBOX_TX != BSP_IRQ_DISABLED)
    can0_mailbox_tx_isr,
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_1 != BSP_IRQ_DISABLED)
    ioport_event_1_isr,
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_2 != BSP_IRQ_DISABLED)
    ioport_event_2_isr,
#endif
#if (BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_0 != BSP_IRQ_DISABLED)
    elc_software_event_0_isr,
#endif
#if (BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_1 != BSP_IRQ_DISABLED)
    elc_software_event_1_isr,
#endif
#if (BSP_IRQ_CFG_POEG0_EVENT != BSP_IRQ_DISABLED)
    poeg0_event_isr,
#endif
#if (BSP_IRQ_CFG_POEG1_EVENT != BSP_IRQ_DISABLED)
    poeg1_event_isr,
#endif
#if (BSP_IRQ_CFG_DALI_DEI != BSP_IRQ_DISABLED)
    dali_dei_isr,
#endif
#if (BSP_IRQ_CFG_DALI_CLI != BSP_IRQ_DISABLED)
    dali_cli_isr,
#endif
#if (BSP_IRQ_CFG_DALI_SDI != BSP_IRQ_DISABLED)
    dali_sdi_isr,
#endif
#if (BSP_IRQ_CFG_DALI_BPI != BSP_IRQ_DISABLED)
    dali_bpi_isr,
#endif
#if (BSP_IRQ_CFG_DALI_FEI != BSP_IRQ_DISABLED)
    dali_fei_isr,
#endif
#if (BSP_IRQ_CFG_DALI_SDI_OR_BPI != BSP_IRQ_DISABLED)
    dali_sdi_or_bpi_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt0_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt0_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_C != BSP_IRQ_DISABLED)
    gpt0_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_D != BSP_IRQ_DISABLED)
    gpt0_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt0_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt0_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt1_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt1_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_C != BSP_IRQ_DISABLED)
    gpt1_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_D != BSP_IRQ_DISABLED)
    gpt1_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt1_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt1_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt2_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt2_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_C != BSP_IRQ_DISABLED)
    gpt2_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_D != BSP_IRQ_DISABLED)
    gpt2_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt2_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt2_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt3_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt3_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_C != BSP_IRQ_DISABLED)
    gpt3_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_D != BSP_IRQ_DISABLED)
    gpt3_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt3_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt3_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt4_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt4_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_C != BSP_IRQ_DISABLED)
    gpt4_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_D != BSP_IRQ_DISABLED)
    gpt4_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt4_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt4_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt5_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt5_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_C != BSP_IRQ_DISABLED)
    gpt5_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_D != BSP_IRQ_DISABLED)
    gpt5_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt5_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt5_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt6_capture_compare_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt6_capture_compare_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_C != BSP_IRQ_DISABLED)
    gpt6_compare_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_D != BSP_IRQ_DISABLED)
    gpt6_compare_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt6_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt6_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_OPS_UVW_EDGE != BSP_IRQ_DISABLED)
    ops_uvw_edge_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_RXI != BSP_IRQ_DISABLED)
    sci0_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_TXI != BSP_IRQ_DISABLED)
    sci0_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_TEI != BSP_IRQ_DISABLED)
    sci0_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_ERI != BSP_IRQ_DISABLED)
    sci0_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_AM != BSP_IRQ_DISABLED)
    sci0_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI0_RXI_OR_ERI != BSP_IRQ_DISABLED)
    sci0_rxi_or_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI1_RXI != BSP_IRQ_DISABLED)
    sci1_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI1_TXI != BSP_IRQ_DISABLED)
    sci1_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI1_TEI != BSP_IRQ_DISABLED)
    sci1_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI1_ERI != BSP_IRQ_DISABLED)
    sci1_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI1_AM != BSP_IRQ_DISABLED)
    sci1_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI9_RXI != BSP_IRQ_DISABLED)
    sci9_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI9_TXI != BSP_IRQ_DISABLED)
    sci9_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI9_TEI != BSP_IRQ_DISABLED)
    sci9_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI9_ERI != BSP_IRQ_DISABLED)
    sci9_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI9_AM != BSP_IRQ_DISABLED)
    sci9_am_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_RXI != BSP_IRQ_DISABLED)
    spi0_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_TXI != BSP_IRQ_DISABLED)
    spi0_txi_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_IDLE != BSP_IRQ_DISABLED)
    spi0_idle_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_ERI != BSP_IRQ_DISABLED)
    spi0_eri_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_TEI != BSP_IRQ_DISABLED)
    spi0_tei_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_RXI != BSP_IRQ_DISABLED)
    spi1_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_TXI != BSP_IRQ_DISABLED)
    spi1_txi_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_IDLE != BSP_IRQ_DISABLED)
    spi1_idle_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_ERI != BSP_IRQ_DISABLED)
    spi1_eri_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_TEI != BSP_IRQ_DISABLED)
    spi1_tei_isr,
#endif
#if (BSP_IRQ_CFG_AES_WRREQ != BSP_IRQ_DISABLED)
    aes_wrreq_isr,
#endif
#if (BSP_IRQ_CFG_AES_RDREQ != BSP_IRQ_DISABLED)
    aes_rdreq_isr,
#endif
#if (BSP_IRQ_CFG_TRNG_RDREQ != BSP_IRQ_DISABLED)
    trng_rdreq_isr,
#endif
};

#endif
