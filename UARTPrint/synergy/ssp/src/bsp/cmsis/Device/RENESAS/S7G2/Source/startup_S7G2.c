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
* File Name    : startup_S7G2.c
* Description  : Startup for the S7G2
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"

/* Only build this file if this board is chosen. */
#if defined(BSP_MCU_GROUP_S7G2)

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
static uint8_t g_main_stack[BSP_CFG_STACK_MAIN_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_STACK);

/* Process stack */
#if (BSP_CFG_STACK_PROCESS_BYTES > 0)
BSP_DONT_REMOVE static uint8_t g_process_stack[BSP_CFG_STACK_PROCESS_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_STACK);
#endif

/* Heap */
BSP_DONT_REMOVE static uint8_t g_heap[BSP_CFG_HEAP_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_HEAP);

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
#pragma weak port0_irq_isr                            = Default_Handler
#pragma weak port1_irq_isr                            = Default_Handler
#pragma weak port2_irq_isr                            = Default_Handler
#pragma weak port3_irq_isr                            = Default_Handler
#pragma weak port4_irq_isr                            = Default_Handler
#pragma weak port5_irq_isr                            = Default_Handler
#pragma weak port6_irq_isr                            = Default_Handler
#pragma weak port7_irq_isr                            = Default_Handler
#pragma weak port8_irq_isr                            = Default_Handler
#pragma weak port9_irq_isr                            = Default_Handler
#pragma weak port10_irq_isr                           = Default_Handler
#pragma weak port11_irq_isr                           = Default_Handler
#pragma weak port12_irq_isr                           = Default_Handler
#pragma weak port13_irq_isr                           = Default_Handler
#pragma weak port14_irq_isr                           = Default_Handler
#pragma weak port15_irq_isr                           = Default_Handler
#pragma weak dmac0_dmac_isr                           = Default_Handler
#pragma weak dmac1_dmac_isr                           = Default_Handler
#pragma weak dmac2_dmac_isr                           = Default_Handler
#pragma weak dmac3_dmac_isr                           = Default_Handler
#pragma weak dmac4_dmac_isr                           = Default_Handler
#pragma weak dmac5_dmac_isr                           = Default_Handler
#pragma weak dmac6_dmac_isr                           = Default_Handler
#pragma weak dmac7_dmac_isr                           = Default_Handler
#pragma weak dtc_transfer_isr                         = Default_Handler
#pragma weak dtc_complete_isr                         = Default_Handler
#pragma weak dtc_dtc_end_isr                          = Default_Handler
#pragma weak exdmac0_exdmac_isr                       = Default_Handler
#pragma weak exdmac1_exdmac_isr                       = Default_Handler
#pragma weak icu_canceling_snooze_mode_isr            = Default_Handler
#pragma weak fcu_fiferr_isr                           = Default_Handler
#pragma weak fcu_frdyi_isr                            = Default_Handler
#pragma weak fcu_eccerr_isr                           = Default_Handler
#pragma weak lvd1_lvd1_isr                            = Default_Handler
#pragma weak lvd2_lvd2_isr                            = Default_Handler
#pragma weak vbatt_vbat_isr                           = Default_Handler
#pragma weak mosc_osc_stop_isr                        = Default_Handler
#pragma weak cpusys_snooze_mode_entry_flag_isr        = Default_Handler
#pragma weak agt0_agti_isr                            = Default_Handler
#pragma weak agt0_agtcmai_isr                         = Default_Handler
#pragma weak agt0_agtcmbi_isr                         = Default_Handler
#pragma weak agt1_agti_isr                            = Default_Handler
#pragma weak agt1_agtcmai_isr                         = Default_Handler
#pragma weak agt1_agtcmbi_isr                         = Default_Handler
#pragma weak iwdt_nmiundf_n_isr                       = Default_Handler
#pragma weak cwdt_nmiundf_n_isr                       = Default_Handler
#pragma weak rtc_alm_isr                              = Default_Handler
#pragma weak rtc_prd_isr                              = Default_Handler
#pragma weak rtc_cup_isr                              = Default_Handler
#pragma weak s12ad0_adi_isr                           = Default_Handler
#pragma weak s12ad0_gbadi_isr                         = Default_Handler
#pragma weak s12ad0_cmpai_isr                         = Default_Handler
#pragma weak s12ad0_cmpbi_isr                         = Default_Handler
#pragma weak s12ad0_compare_match_isr                 = Default_Handler
#pragma weak s12ad0_compare_mismatch_isr              = Default_Handler
#pragma weak s12ad1_adi_isr                           = Default_Handler
#pragma weak s12ad1_gbadi_isr                         = Default_Handler
#pragma weak s12ad1_cmpai_isr                         = Default_Handler
#pragma weak s12ad1_cmpbi_isr                         = Default_Handler
#pragma weak s12ad1_compare_match_isr                 = Default_Handler
#pragma weak s12ad1_compare_mismatch_isr              = Default_Handler
#pragma weak comp_oc0_comp_irq_isr                    = Default_Handler
#pragma weak comp_rd1_comp_irq_isr                    = Default_Handler
#pragma weak comp_rd2_comp_irq_isr                    = Default_Handler
#pragma weak comp_rd3_comp_irq_isr                    = Default_Handler
#pragma weak comp_rd4_comp_irq_isr                    = Default_Handler
#pragma weak comp_rd5_comp_irq_isr                    = Default_Handler
#pragma weak comp_lp_comp_c0irq_isr                   = Default_Handler
#pragma weak comp_lp_comp_c1irq_isr                   = Default_Handler
#pragma weak usbfs_d0fifo_isr                         = Default_Handler
#pragma weak usbfs_d1fifo_isr                         = Default_Handler
#pragma weak usbfs_usbi_isr                           = Default_Handler
#pragma weak usbfs_usbr_isr                           = Default_Handler
#pragma weak riic0_rxi_isr                            = Default_Handler
#pragma weak riic0_txi_isr                            = Default_Handler
#pragma weak riic0_tei_isr                            = Default_Handler
#pragma weak riic0_eei_isr                            = Default_Handler
#pragma weak riic0_wui_isr                            = Default_Handler
#pragma weak riic1_rxi_isr                            = Default_Handler
#pragma weak riic1_txi_isr                            = Default_Handler
#pragma weak riic1_tei_isr                            = Default_Handler
#pragma weak riic1_eei_isr                            = Default_Handler
#pragma weak riic2_rxi_isr                            = Default_Handler
#pragma weak riic2_txi_isr                            = Default_Handler
#pragma weak riic2_tei_isr                            = Default_Handler
#pragma weak riic2_eei_isr                            = Default_Handler
#pragma weak ssi0_ssitxi_isr                          = Default_Handler
#pragma weak ssi0_ssirxi_isr                          = Default_Handler
#pragma weak ssi0_ssirt_isr                           = Default_Handler
#pragma weak ssi0_ssif_isr                            = Default_Handler
#pragma weak ssi1_ssitxi_isr                          = Default_Handler
#pragma weak ssi1_ssirxi_isr                          = Default_Handler
#pragma weak ssi1_ssirt_isr                           = Default_Handler
#pragma weak ssi1_ssif_isr                            = Default_Handler
#pragma weak src_idei_isr                             = Default_Handler
#pragma weak src_odfi_isr                             = Default_Handler
#pragma weak src_ovf_isr                              = Default_Handler
#pragma weak src_udf_isr                              = Default_Handler
#pragma weak src_cef_isr                              = Default_Handler
#pragma weak pdc_pcdfi_isr                            = Default_Handler
#pragma weak pdc_pcfei_isr                            = Default_Handler
#pragma weak pdc_pceri_isr                            = Default_Handler
#pragma weak ctsu_ctsuwr_isr                          = Default_Handler
#pragma weak ctsu_ctsurd_isr                          = Default_Handler
#pragma weak ctsu_ctsufn_isr                          = Default_Handler
#pragma weak key_intkr_isr                            = Default_Handler
#pragma weak doc_dopcf_isr                            = Default_Handler
#pragma weak cac_ferrf_isr                            = Default_Handler
#pragma weak cac_mendf_isr                            = Default_Handler
#pragma weak cac_ovff_isr                             = Default_Handler
#pragma weak rcan20_ers_isr                           = Default_Handler
#pragma weak rcan20_rxf_isr                           = Default_Handler
#pragma weak rcan20_txf_isr                           = Default_Handler
#pragma weak rcan20_rxm_isr                           = Default_Handler
#pragma weak rcan20_txm_isr                           = Default_Handler
#pragma weak rcan21_ers_isr                           = Default_Handler
#pragma weak rcan21_rxf_isr                           = Default_Handler
#pragma weak rcan21_txf_isr                           = Default_Handler
#pragma weak rcan21_rxm_isr                           = Default_Handler
#pragma weak rcan21_txm_isr                           = Default_Handler
#pragma weak gpio_port_group_a_isr                    = Default_Handler
#pragma weak gpio_port_group_b_isr                    = Default_Handler
#pragma weak gpio_port_group_c_isr                    = Default_Handler
#pragma weak gpio_port_group_d_isr                    = Default_Handler
#pragma weak elc0_software_event_isr                  = Default_Handler
#pragma weak elc1_software_event_isr                  = Default_Handler
#pragma weak poeg_group_event0_isr                    = Default_Handler
#pragma weak poeg_group_event1_isr                    = Default_Handler
#pragma weak poeg_group_event2_isr                    = Default_Handler
#pragma weak poeg_group_event3_isr                    = Default_Handler
#pragma weak gpt0_capture_compare_int_a_isr           = Default_Handler
#pragma weak gpt0_capture_compare_int_b_isr           = Default_Handler
#pragma weak gpt0_compare_int_c_isr                   = Default_Handler
#pragma weak gpt0_compare_int_d_isr                   = Default_Handler
#pragma weak gpt0_compare_int_e_isr                   = Default_Handler
#pragma weak gpt0_compare_int_f_isr                   = Default_Handler
#pragma weak gpt0_counter_overflow_isr                = Default_Handler
#pragma weak gpt0_counter_underflow_isr               = Default_Handler
#pragma weak gpt0_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt0_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt1_capture_compare_int_a_isr           = Default_Handler
#pragma weak gpt1_capture_compare_int_b_isr           = Default_Handler
#pragma weak gpt1_compare_int_c_isr                   = Default_Handler
#pragma weak gpt1_compare_int_d_isr                   = Default_Handler
#pragma weak gpt1_compare_int_e_isr                   = Default_Handler
#pragma weak gpt1_compare_int_f_isr                   = Default_Handler
#pragma weak gpt1_counter_overflow_isr                = Default_Handler
#pragma weak gpt1_counter_underflow_isr               = Default_Handler
#pragma weak gpt1_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt1_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt2_capture_compare_int_a_isr           = Default_Handler
#pragma weak gpt2_capture_compare_int_b_isr           = Default_Handler
#pragma weak gpt2_compare_int_c_isr                   = Default_Handler
#pragma weak gpt2_compare_int_d_isr                   = Default_Handler
#pragma weak gpt2_compare_int_e_isr                   = Default_Handler
#pragma weak gpt2_compare_int_f_isr                   = Default_Handler
#pragma weak gpt2_counter_overflow_isr                = Default_Handler
#pragma weak gpt2_counter_underflow_isr               = Default_Handler
#pragma weak gpt2_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt2_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt3_capture_compare_int_a_isr           = Default_Handler
#pragma weak gpt3_capture_compare_int_b_isr           = Default_Handler
#pragma weak gpt3_compare_int_c_isr                   = Default_Handler
#pragma weak gpt3_compare_int_d_isr                   = Default_Handler
#pragma weak gpt3_compare_int_e_isr                   = Default_Handler
#pragma weak gpt3_compare_int_f_isr                   = Default_Handler
#pragma weak gpt3_counter_overflow_isr                = Default_Handler
#pragma weak gpt3_counter_underflow_isr               = Default_Handler
#pragma weak gpt3_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt3_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt4_capture_compare_int_a_isr           = Default_Handler
#pragma weak gpt4_capture_compare_int_b_isr           = Default_Handler
#pragma weak gpt4_compare_int_c_isr                   = Default_Handler
#pragma weak gpt4_compare_int_d_isr                   = Default_Handler
#pragma weak gpt4_compare_int_e_isr                   = Default_Handler
#pragma weak gpt4_compare_int_f_isr                   = Default_Handler
#pragma weak gpt4_counter_overflow_isr                = Default_Handler
#pragma weak gpt4_counter_underflow_isr               = Default_Handler
#pragma weak gpt4_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt4_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt5_capture_compare_int_a_isr           = Default_Handler
#pragma weak gpt5_capture_compare_int_b_isr           = Default_Handler
#pragma weak gpt5_compare_int_c_isr                   = Default_Handler
#pragma weak gpt5_compare_int_d_isr                   = Default_Handler
#pragma weak gpt5_compare_int_e_isr                   = Default_Handler
#pragma weak gpt5_compare_int_f_isr                   = Default_Handler
#pragma weak gpt5_counter_overflow_isr                = Default_Handler
#pragma weak gpt5_counter_underflow_isr               = Default_Handler
#pragma weak gpt5_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt5_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt6_capture_compare_int_a_isr           = Default_Handler
#pragma weak gpt6_capture_compare_int_b_isr           = Default_Handler
#pragma weak gpt6_compare_int_c_isr                   = Default_Handler
#pragma weak gpt6_compare_int_d_isr                   = Default_Handler
#pragma weak gpt6_compare_int_e_isr                   = Default_Handler
#pragma weak gpt6_compare_int_f_isr                   = Default_Handler
#pragma weak gpt6_counter_overflow_isr                = Default_Handler
#pragma weak gpt6_counter_underflow_isr               = Default_Handler
#pragma weak gpt6_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt6_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt7_capture_compare_int_a_isr           = Default_Handler
#pragma weak gpt7_capture_compare_int_b_isr           = Default_Handler
#pragma weak gpt7_compare_int_c_isr                   = Default_Handler
#pragma weak gpt7_compare_int_d_isr                   = Default_Handler
#pragma weak gpt7_compare_int_e_isr                   = Default_Handler
#pragma weak gpt7_compare_int_f_isr                   = Default_Handler
#pragma weak gpt7_counter_overflow_isr                = Default_Handler
#pragma weak gpt7_counter_underflow_isr               = Default_Handler
#pragma weak gpt7_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt7_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt8_capture_compare_int_a_isr           = Default_Handler
#pragma weak gpt8_capture_compare_int_b_isr           = Default_Handler
#pragma weak gpt8_compare_int_c_isr                   = Default_Handler
#pragma weak gpt8_compare_int_d_isr                   = Default_Handler
#pragma weak gpt8_compare_int_e_isr                   = Default_Handler
#pragma weak gpt8_compare_int_f_isr                   = Default_Handler
#pragma weak gpt8_counter_overflow_isr                = Default_Handler
#pragma weak gpt8_counter_underflow_isr               = Default_Handler
#pragma weak gpt8_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt8_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt9_capture_compare_int_a_isr           = Default_Handler
#pragma weak gpt9_capture_compare_int_b_isr           = Default_Handler
#pragma weak gpt9_compare_int_c_isr                   = Default_Handler
#pragma weak gpt9_compare_int_d_isr                   = Default_Handler
#pragma weak gpt9_compare_int_e_isr                   = Default_Handler
#pragma weak gpt9_compare_int_f_isr                   = Default_Handler
#pragma weak gpt9_counter_overflow_isr                = Default_Handler
#pragma weak gpt9_counter_underflow_isr               = Default_Handler
#pragma weak gpt9_ad_trig_a_isr                       = Default_Handler
#pragma weak gpt9_ad_trig_b_isr                       = Default_Handler
#pragma weak gpt10_capture_compare_int_a_isr          = Default_Handler
#pragma weak gpt10_capture_compare_int_b_isr          = Default_Handler
#pragma weak gpt10_compare_int_c_isr                  = Default_Handler
#pragma weak gpt10_compare_int_d_isr                  = Default_Handler
#pragma weak gpt10_compare_int_e_isr                  = Default_Handler
#pragma weak gpt10_compare_int_f_isr                  = Default_Handler
#pragma weak gpt10_counter_overflow_isr               = Default_Handler
#pragma weak gpt10_counter_underflow_isr              = Default_Handler
#pragma weak gpt10_ad_trig_a_isr                      = Default_Handler
#pragma weak gpt10_ad_trig_b_isr                      = Default_Handler
#pragma weak gpt11_capture_compare_int_a_isr          = Default_Handler
#pragma weak gpt11_capture_compare_int_b_isr          = Default_Handler
#pragma weak gpt11_compare_int_c_isr                  = Default_Handler
#pragma weak gpt11_compare_int_d_isr                  = Default_Handler
#pragma weak gpt11_compare_int_e_isr                  = Default_Handler
#pragma weak gpt11_compare_int_f_isr                  = Default_Handler
#pragma weak gpt11_counter_overflow_isr               = Default_Handler
#pragma weak gpt11_counter_underflow_isr              = Default_Handler
#pragma weak gpt11_ad_trig_a_isr                      = Default_Handler
#pragma weak gpt11_ad_trig_b_isr                      = Default_Handler
#pragma weak gpt12_capture_compare_int_a_isr          = Default_Handler
#pragma weak gpt12_capture_compare_int_b_isr          = Default_Handler
#pragma weak gpt12_compare_int_c_isr                  = Default_Handler
#pragma weak gpt12_compare_int_d_isr                  = Default_Handler
#pragma weak gpt12_compare_int_e_isr                  = Default_Handler
#pragma weak gpt12_compare_int_f_isr                  = Default_Handler
#pragma weak gpt12_counter_overflow_isr               = Default_Handler
#pragma weak gpt12_counter_underflow_isr              = Default_Handler
#pragma weak gpt12_ad_trig_a_isr                      = Default_Handler
#pragma weak gpt12_ad_trig_b_isr                      = Default_Handler
#pragma weak gpt13_capture_compare_int_a_isr          = Default_Handler
#pragma weak gpt13_capture_compare_int_b_isr          = Default_Handler
#pragma weak gpt13_compare_int_c_isr                  = Default_Handler
#pragma weak gpt13_compare_int_d_isr                  = Default_Handler
#pragma weak gpt13_compare_int_e_isr                  = Default_Handler
#pragma weak gpt13_compare_int_f_isr                  = Default_Handler
#pragma weak gpt13_counter_overflow_isr               = Default_Handler
#pragma weak gpt13_counter_underflow_isr              = Default_Handler
#pragma weak gpt13_ad_trig_a_isr                      = Default_Handler
#pragma weak gpt13_ad_trig_b_isr                      = Default_Handler
#pragma weak gpt_uvw_edge_isr                         = Default_Handler
#pragma weak ether_ipls_isr                           = Default_Handler
#pragma weak ether_mint_isr                           = Default_Handler
#pragma weak ether_pint_isr                           = Default_Handler
#pragma weak ether_eint0_isr                          = Default_Handler
#pragma weak ether_eint1_isr                          = Default_Handler
#pragma weak ether_ether0_rise_isr                    = Default_Handler
#pragma weak ether_ether1_rise_isr                    = Default_Handler
#pragma weak ether_ether2_rise_isr                    = Default_Handler
#pragma weak ether_ether3_rise_isr                    = Default_Handler
#pragma weak ether_ether4_rise_isr                    = Default_Handler
#pragma weak ether_ether5_rise_isr                    = Default_Handler
#pragma weak ether_ether0_fall_isr                    = Default_Handler
#pragma weak ether_ether1_fall_isr                    = Default_Handler
#pragma weak ether_ether2_fall_isr                    = Default_Handler
#pragma weak ether_ether3_fall_isr                    = Default_Handler
#pragma weak ether_ether4_fall_isr                    = Default_Handler
#pragma weak ether_ether5_fall_isr                    = Default_Handler
#pragma weak usbhs_d0fifo_isr                         = Default_Handler
#pragma weak usbhs_d1fifo_isr                         = Default_Handler
#pragma weak usbhs_usbir_isr                          = Default_Handler
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
#pragma weak sci2_rxi_isr                             = Default_Handler
#pragma weak sci2_txi_isr                             = Default_Handler
#pragma weak sci2_tei_isr                             = Default_Handler
#pragma weak sci2_eri_isr                             = Default_Handler
#pragma weak sci2_am_isr                              = Default_Handler
#pragma weak sci3_rxi_isr                             = Default_Handler
#pragma weak sci3_txi_isr                             = Default_Handler
#pragma weak sci3_tei_isr                             = Default_Handler
#pragma weak sci3_eri_isr                             = Default_Handler
#pragma weak sci3_am_isr                              = Default_Handler
#pragma weak sci4_rxi_isr                             = Default_Handler
#pragma weak sci4_txi_isr                             = Default_Handler
#pragma weak sci4_tei_isr                             = Default_Handler
#pragma weak sci4_eri_isr                             = Default_Handler
#pragma weak sci4_am_isr                              = Default_Handler
#pragma weak sci5_rxi_isr                             = Default_Handler
#pragma weak sci5_txi_isr                             = Default_Handler
#pragma weak sci5_tei_isr                             = Default_Handler
#pragma weak sci5_eri_isr                             = Default_Handler
#pragma weak sci5_am_isr                              = Default_Handler
#pragma weak sci6_rxi_isr                             = Default_Handler
#pragma weak sci6_txi_isr                             = Default_Handler
#pragma weak sci6_tei_isr                             = Default_Handler
#pragma weak sci6_eri_isr                             = Default_Handler
#pragma weak sci6_am_isr                              = Default_Handler
#pragma weak sci7_rxi_isr                             = Default_Handler
#pragma weak sci7_txi_isr                             = Default_Handler
#pragma weak sci7_tei_isr                             = Default_Handler
#pragma weak sci7_eri_isr                             = Default_Handler
#pragma weak sci7_am_isr                              = Default_Handler
#pragma weak sci8_rxi_isr                             = Default_Handler
#pragma weak sci8_txi_isr                             = Default_Handler
#pragma weak sci8_tei_isr                             = Default_Handler
#pragma weak sci8_eri_isr                             = Default_Handler
#pragma weak sci8_am_isr                              = Default_Handler
#pragma weak sci9_rxi_isr                             = Default_Handler
#pragma weak sci9_txi_isr                             = Default_Handler
#pragma weak sci9_tei_isr                             = Default_Handler
#pragma weak sci9_eri_isr                             = Default_Handler
#pragma weak sci9_am_isr                              = Default_Handler
#pragma weak rspi0_spri_isr                           = Default_Handler
#pragma weak rspi0_spti_isr                           = Default_Handler
#pragma weak rspi0_spii_isr                           = Default_Handler
#pragma weak rspi0_spei_isr                           = Default_Handler
#pragma weak rspi0_sp_elctend_isr                     = Default_Handler
#pragma weak rspi1_spri_isr                           = Default_Handler
#pragma weak rspi1_spti_isr                           = Default_Handler
#pragma weak rspi1_spii_isr                           = Default_Handler
#pragma weak rspi1_spei_isr                           = Default_Handler
#pragma weak rspi1_sp_elctend_isr                     = Default_Handler
#pragma weak qspi_intr_isr                            = Default_Handler
#pragma weak sdhi_mmc0_accs_isr                       = Default_Handler
#pragma weak sdhi_mmc0_sdio_isr                       = Default_Handler
#pragma weak sdhi_mmc0_card_isr                       = Default_Handler
#pragma weak sdhi_mmc0_odmsdbreq_isr                  = Default_Handler
#pragma weak sdhi_mmc1_accs_isr                       = Default_Handler
#pragma weak sdhi_mmc1_sdio_isr                       = Default_Handler
#pragma weak sdhi_mmc1_card_isr                       = Default_Handler
#pragma weak sdhi_mmc1_odmsdbreq_isr                  = Default_Handler
#pragma weak ext_divider_intmd_isr                    = Default_Handler
#pragma weak tsip_proc_busy_n_isr                     = Default_Handler
#pragma weak tsip_romok_n_isr                         = Default_Handler
#pragma weak tsip_long_plg_n_isr                      = Default_Handler
#pragma weak tsip_test_busy_n_isr                     = Default_Handler
#pragma weak tsip_wrrdy_0_n_isr                       = Default_Handler
#pragma weak tsip_wrrdy_1_n_isr                       = Default_Handler
#pragma weak tsip_wrrdy_4_n_isr                       = Default_Handler
#pragma weak tsip_rdrdy_0_n_isr                       = Default_Handler
#pragma weak tsip_rdrdy_1_n_isr                       = Default_Handler
#pragma weak tsip_integrate_wrrdy_n_isr               = Default_Handler
#pragma weak tsip_integrate_rdrdy_n_isr               = Default_Handler
#pragma weak lcdc_lcdc_level_0_isr                    = Default_Handler
#pragma weak lcdc_lcdc_level_1_isr                    = Default_Handler
#pragma weak lcdc_lcdc_level_2_isr                    = Default_Handler
#pragma weak twod_engine_irq_isr                      = Default_Handler
#pragma weak jpeg_jedi_isr                            = Default_Handler
#pragma weak jpeg_jdti_isr                            = Default_Handler
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
void port0_irq_isr                           (void) WEAK_REF_ATTRIBUTE;
void port1_irq_isr                           (void) WEAK_REF_ATTRIBUTE;
void port2_irq_isr                           (void) WEAK_REF_ATTRIBUTE;
void port3_irq_isr                           (void) WEAK_REF_ATTRIBUTE;
void port4_irq_isr                           (void) WEAK_REF_ATTRIBUTE;
void port5_irq_isr                           (void) WEAK_REF_ATTRIBUTE;
void port6_irq_isr                           (void) WEAK_REF_ATTRIBUTE;
void port7_irq_isr                           (void) WEAK_REF_ATTRIBUTE;
void port8_irq_isr                           (void) WEAK_REF_ATTRIBUTE;
void port9_irq_isr                           (void) WEAK_REF_ATTRIBUTE;
void port10_irq_isr                          (void) WEAK_REF_ATTRIBUTE;
void port11_irq_isr                          (void) WEAK_REF_ATTRIBUTE;
void port12_irq_isr                          (void) WEAK_REF_ATTRIBUTE;
void port13_irq_isr                          (void) WEAK_REF_ATTRIBUTE;
void port14_irq_isr                          (void) WEAK_REF_ATTRIBUTE;
void port15_irq_isr                          (void) WEAK_REF_ATTRIBUTE;
void dmac0_dmac_isr                          (void) WEAK_REF_ATTRIBUTE;
void dmac1_dmac_isr                          (void) WEAK_REF_ATTRIBUTE;
void dmac2_dmac_isr                          (void) WEAK_REF_ATTRIBUTE;
void dmac3_dmac_isr                          (void) WEAK_REF_ATTRIBUTE;
void dmac4_dmac_isr                          (void) WEAK_REF_ATTRIBUTE;
void dmac5_dmac_isr                          (void) WEAK_REF_ATTRIBUTE;
void dmac6_dmac_isr                          (void) WEAK_REF_ATTRIBUTE;
void dmac7_dmac_isr                          (void) WEAK_REF_ATTRIBUTE;
void dtc_transfer_isr                        (void) WEAK_REF_ATTRIBUTE;
void dtc_complete_isr                        (void) WEAK_REF_ATTRIBUTE;
void dtc_dtc_end_isr                         (void) WEAK_REF_ATTRIBUTE;
void exdmac0_exdmac_isr                      (void) WEAK_REF_ATTRIBUTE;
void exdmac1_exdmac_isr                      (void) WEAK_REF_ATTRIBUTE;
void icu_canceling_snooze_mode_isr           (void) WEAK_REF_ATTRIBUTE;
void fcu_fiferr_isr                          (void) WEAK_REF_ATTRIBUTE;
void fcu_frdyi_isr                           (void) WEAK_REF_ATTRIBUTE;
void fcu_eccerr_isr                          (void) WEAK_REF_ATTRIBUTE;
void lvd1_lvd1_isr                           (void) WEAK_REF_ATTRIBUTE;
void lvd2_lvd2_isr                           (void) WEAK_REF_ATTRIBUTE;
void mosc_osc_stop_isr                       (void) WEAK_REF_ATTRIBUTE;
void cpusys_snooze_mode_entry_flag_isr       (void) WEAK_REF_ATTRIBUTE;
void agt0_agti_isr                           (void) WEAK_REF_ATTRIBUTE;
void agt0_agtcmai_isr                        (void) WEAK_REF_ATTRIBUTE;
void agt0_agtcmbi_isr                        (void) WEAK_REF_ATTRIBUTE;
void agt1_agti_isr                           (void) WEAK_REF_ATTRIBUTE;
void agt1_agtcmai_isr                        (void) WEAK_REF_ATTRIBUTE;
void agt1_agtcmbi_isr                        (void) WEAK_REF_ATTRIBUTE;
void iwdt_nmiundf_n_isr                      (void) WEAK_REF_ATTRIBUTE;
void cwdt_nmiundf_n_isr                      (void) WEAK_REF_ATTRIBUTE;
void rtc_alm_isr                             (void) WEAK_REF_ATTRIBUTE;
void rtc_prd_isr                             (void) WEAK_REF_ATTRIBUTE;
void rtc_cup_isr                             (void) WEAK_REF_ATTRIBUTE;
void s12ad0_adi_isr                          (void) WEAK_REF_ATTRIBUTE;
void s12ad0_gbadi_isr                        (void) WEAK_REF_ATTRIBUTE;
void s12ad0_cmpai_isr                        (void) WEAK_REF_ATTRIBUTE;
void s12ad0_cmpbi_isr                        (void) WEAK_REF_ATTRIBUTE;
void s12ad0_compare_match_isr                (void) WEAK_REF_ATTRIBUTE;
void s12ad0_compare_mismatch_isr             (void) WEAK_REF_ATTRIBUTE;
void s12ad1_adi_isr                          (void) WEAK_REF_ATTRIBUTE;
void s12ad1_gbadi_isr                        (void) WEAK_REF_ATTRIBUTE;
void s12ad1_cmpai_isr                        (void) WEAK_REF_ATTRIBUTE;
void s12ad1_cmpbi_isr                        (void) WEAK_REF_ATTRIBUTE;
void s12ad1_compare_match_isr                (void) WEAK_REF_ATTRIBUTE;
void s12ad1_compare_mismatch_isr             (void) WEAK_REF_ATTRIBUTE;
void comp_oc0_comp_irq_isr                   (void) WEAK_REF_ATTRIBUTE;
void comp_rd1_comp_irq_isr                   (void) WEAK_REF_ATTRIBUTE;
void comp_rd2_comp_irq_isr                   (void) WEAK_REF_ATTRIBUTE;
void comp_rd3_comp_irq_isr                   (void) WEAK_REF_ATTRIBUTE;
void comp_rd4_comp_irq_isr                   (void) WEAK_REF_ATTRIBUTE;
void comp_rd5_comp_irq_isr                   (void) WEAK_REF_ATTRIBUTE;
void usbfs_d0fifo_isr                        (void) WEAK_REF_ATTRIBUTE;
void usbfs_d1fifo_isr                        (void) WEAK_REF_ATTRIBUTE;
void usbfs_usbi_isr                          (void) WEAK_REF_ATTRIBUTE;
void usbfs_usbr_isr                          (void) WEAK_REF_ATTRIBUTE;
void riic0_rxi_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic0_txi_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic0_tei_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic0_eei_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic0_wui_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic1_rxi_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic1_txi_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic1_tei_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic1_eei_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic2_rxi_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic2_txi_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic2_tei_isr                           (void) WEAK_REF_ATTRIBUTE;
void riic2_eei_isr                           (void) WEAK_REF_ATTRIBUTE;
void ssi0_ssitxi_isr                         (void) WEAK_REF_ATTRIBUTE;
void ssi0_ssirxi_isr                         (void) WEAK_REF_ATTRIBUTE;
void ssi0_ssirt_isr                          (void) WEAK_REF_ATTRIBUTE;
void ssi0_ssif_isr                           (void) WEAK_REF_ATTRIBUTE;
void ssi1_ssitxi_isr                         (void) WEAK_REF_ATTRIBUTE;
void ssi1_ssirxi_isr                         (void) WEAK_REF_ATTRIBUTE;
void ssi1_ssirt_isr                          (void) WEAK_REF_ATTRIBUTE;
void ssi1_ssif_isr                           (void) WEAK_REF_ATTRIBUTE;
void src_idei_isr                            (void) WEAK_REF_ATTRIBUTE;
void src_odfi_isr                            (void) WEAK_REF_ATTRIBUTE;
void src_ovf_isr                             (void) WEAK_REF_ATTRIBUTE;
void src_udf_isr                             (void) WEAK_REF_ATTRIBUTE;
void src_cef_isr                             (void) WEAK_REF_ATTRIBUTE;
void pdc_pcdfi_isr                           (void) WEAK_REF_ATTRIBUTE;
void pdc_pcfei_isr                           (void) WEAK_REF_ATTRIBUTE;
void pdc_pceri_isr                           (void) WEAK_REF_ATTRIBUTE;
void ctsu_ctsuwr_isr                         (void) WEAK_REF_ATTRIBUTE;
void ctsu_ctsurd_isr                         (void) WEAK_REF_ATTRIBUTE;
void ctsu_ctsufn_isr                         (void) WEAK_REF_ATTRIBUTE;
void key_intkr_isr                           (void) WEAK_REF_ATTRIBUTE;
void doc_dopcf_isr                           (void) WEAK_REF_ATTRIBUTE;
void cac_ferrf_isr                           (void) WEAK_REF_ATTRIBUTE;
void cac_mendf_isr                           (void) WEAK_REF_ATTRIBUTE;
void cac_ovff_isr                            (void) WEAK_REF_ATTRIBUTE;
void rcan20_ers_isr                          (void) WEAK_REF_ATTRIBUTE;
void rcan20_rxf_isr                          (void) WEAK_REF_ATTRIBUTE;
void rcan20_txf_isr                          (void) WEAK_REF_ATTRIBUTE;
void rcan20_rxm_isr                          (void) WEAK_REF_ATTRIBUTE;
void rcan20_txm_isr                          (void) WEAK_REF_ATTRIBUTE;
void rcan21_ers_isr                          (void) WEAK_REF_ATTRIBUTE;
void rcan21_rxf_isr                          (void) WEAK_REF_ATTRIBUTE;
void rcan21_txf_isr                          (void) WEAK_REF_ATTRIBUTE;
void rcan21_rxm_isr                          (void) WEAK_REF_ATTRIBUTE;
void rcan21_txm_isr                          (void) WEAK_REF_ATTRIBUTE;
void gpio_port_group_a_isr                   (void) WEAK_REF_ATTRIBUTE;
void gpio_port_group_b_isr                   (void) WEAK_REF_ATTRIBUTE;
void gpio_port_group_c_isr                   (void) WEAK_REF_ATTRIBUTE;
void gpio_port_group_d_isr                   (void) WEAK_REF_ATTRIBUTE;
void elc0_software_event_isr                 (void) WEAK_REF_ATTRIBUTE;
void elc1_software_event_isr                 (void) WEAK_REF_ATTRIBUTE;
void poeg_group_event0_isr                   (void) WEAK_REF_ATTRIBUTE;
void poeg_group_event1_isr                   (void) WEAK_REF_ATTRIBUTE;
void poeg_group_event2_isr                   (void) WEAK_REF_ATTRIBUTE;
void poeg_group_event3_isr                   (void) WEAK_REF_ATTRIBUTE;
void gpt0_capture_compare_int_a_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt0_capture_compare_int_b_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt0_compare_int_c_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt0_compare_int_d_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt0_compare_int_e_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt0_compare_int_f_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt0_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt0_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt0_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt0_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt1_capture_compare_int_a_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt1_capture_compare_int_b_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt1_compare_int_c_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt1_compare_int_d_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt1_compare_int_e_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt1_compare_int_f_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt1_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt1_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt1_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt1_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt2_capture_compare_int_a_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt2_capture_compare_int_b_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt2_compare_int_c_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt2_compare_int_d_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt2_compare_int_e_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt2_compare_int_f_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt2_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt2_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt2_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt2_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt3_capture_compare_int_a_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt3_capture_compare_int_b_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt3_compare_int_c_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt3_compare_int_d_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt3_compare_int_e_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt3_compare_int_f_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt3_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt3_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt3_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt3_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt4_capture_compare_int_a_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt4_capture_compare_int_b_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt4_compare_int_c_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt4_compare_int_d_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt4_compare_int_e_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt4_compare_int_f_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt4_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt4_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt4_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt4_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt5_capture_compare_int_a_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt5_capture_compare_int_b_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt5_compare_int_c_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt5_compare_int_d_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt5_compare_int_e_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt5_compare_int_f_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt5_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt5_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt5_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt5_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt6_capture_compare_int_a_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt6_capture_compare_int_b_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt6_compare_int_c_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt6_compare_int_d_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt6_compare_int_e_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt6_compare_int_f_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt6_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt6_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt6_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt6_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt7_capture_compare_int_a_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt7_capture_compare_int_b_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt7_compare_int_c_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt7_compare_int_d_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt7_compare_int_e_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt7_compare_int_f_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt7_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt7_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt7_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt7_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt8_capture_compare_int_a_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt8_capture_compare_int_b_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt8_compare_int_c_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt8_compare_int_d_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt8_compare_int_e_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt8_compare_int_f_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt8_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt8_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt8_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt8_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt9_capture_compare_int_a_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt9_capture_compare_int_b_isr          (void) WEAK_REF_ATTRIBUTE;
void gpt9_compare_int_c_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt9_compare_int_d_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt9_compare_int_e_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt9_compare_int_f_isr                  (void) WEAK_REF_ATTRIBUTE;
void gpt9_counter_overflow_isr               (void) WEAK_REF_ATTRIBUTE;
void gpt9_counter_underflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt9_ad_trig_a_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt9_ad_trig_b_isr                      (void) WEAK_REF_ATTRIBUTE;
void gpt10_capture_compare_int_a_isr         (void) WEAK_REF_ATTRIBUTE;
void gpt10_capture_compare_int_b_isr         (void) WEAK_REF_ATTRIBUTE;
void gpt10_compare_int_c_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt10_compare_int_d_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt10_compare_int_e_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt10_compare_int_f_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt10_counter_overflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt10_counter_underflow_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt10_ad_trig_a_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt10_ad_trig_b_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt11_capture_compare_int_a_isr         (void) WEAK_REF_ATTRIBUTE;
void gpt11_capture_compare_int_b_isr         (void) WEAK_REF_ATTRIBUTE;
void gpt11_compare_int_c_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt11_compare_int_d_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt11_compare_int_e_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt11_compare_int_f_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt11_counter_overflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt11_counter_underflow_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt11_ad_trig_a_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt11_ad_trig_b_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt12_capture_compare_int_a_isr         (void) WEAK_REF_ATTRIBUTE;
void gpt12_capture_compare_int_b_isr         (void) WEAK_REF_ATTRIBUTE;
void gpt12_compare_int_c_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt12_compare_int_d_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt12_compare_int_e_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt12_compare_int_f_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt12_counter_overflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt12_counter_underflow_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt12_ad_trig_a_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt12_ad_trig_b_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt13_capture_compare_int_a_isr         (void) WEAK_REF_ATTRIBUTE;
void gpt13_capture_compare_int_b_isr         (void) WEAK_REF_ATTRIBUTE;
void gpt13_compare_int_c_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt13_compare_int_d_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt13_compare_int_e_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt13_compare_int_f_isr                 (void) WEAK_REF_ATTRIBUTE;
void gpt13_counter_overflow_isr              (void) WEAK_REF_ATTRIBUTE;
void gpt13_counter_underflow_isr             (void) WEAK_REF_ATTRIBUTE;
void gpt13_ad_trig_a_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt13_ad_trig_b_isr                     (void) WEAK_REF_ATTRIBUTE;
void gpt_uvw_edge_isr                        (void) WEAK_REF_ATTRIBUTE;
void ether_ipls_isr                          (void) WEAK_REF_ATTRIBUTE;
void ether_mint_isr                          (void) WEAK_REF_ATTRIBUTE;
void ether_pint_isr                          (void) WEAK_REF_ATTRIBUTE;
void ether_eint0_isr                         (void) WEAK_REF_ATTRIBUTE;
void ether_eint1_isr                         (void) WEAK_REF_ATTRIBUTE;
void ether_ether0_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void ether_ether1_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void ether_ether2_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void ether_ether3_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void ether_ether4_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void ether_ether5_rise_isr                   (void) WEAK_REF_ATTRIBUTE;
void ether_ether0_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void ether_ether1_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void ether_ether2_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void ether_ether3_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void ether_ether4_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void ether_ether5_fall_isr                   (void) WEAK_REF_ATTRIBUTE;
void usbhs_d0fifo_isr                        (void) WEAK_REF_ATTRIBUTE;
void usbhs_d1fifo_isr                        (void) WEAK_REF_ATTRIBUTE;
void usbhs_usbir_isr                         (void) WEAK_REF_ATTRIBUTE;
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
void sci2_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci2_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci2_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci2_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci2_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci3_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci3_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci3_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci3_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci3_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci4_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci4_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci4_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci4_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci4_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci5_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci5_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci5_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci5_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci5_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci6_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci6_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci6_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci6_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci6_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci7_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci7_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci7_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci7_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci7_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci8_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci8_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci8_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci8_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci8_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void sci9_rxi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_txi_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_tei_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_eri_isr                            (void) WEAK_REF_ATTRIBUTE;
void sci9_am_isr                             (void) WEAK_REF_ATTRIBUTE;
void rspi0_spri_isr                          (void) WEAK_REF_ATTRIBUTE;
void rspi0_spti_isr                          (void) WEAK_REF_ATTRIBUTE;
void rspi0_spii_isr                          (void) WEAK_REF_ATTRIBUTE;
void rspi0_spei_isr                          (void) WEAK_REF_ATTRIBUTE;
void rspi0_sp_elctend_isr                    (void) WEAK_REF_ATTRIBUTE;
void rspi1_spri_isr                          (void) WEAK_REF_ATTRIBUTE;
void rspi1_spti_isr                          (void) WEAK_REF_ATTRIBUTE;
void rspi1_spii_isr                          (void) WEAK_REF_ATTRIBUTE;
void rspi1_spei_isr                          (void) WEAK_REF_ATTRIBUTE;
void rspi1_sp_elctend_isr                    (void) WEAK_REF_ATTRIBUTE;
void qspi_intr_isr                           (void) WEAK_REF_ATTRIBUTE;
void sdhi_mmc0_accs_isr                      (void) WEAK_REF_ATTRIBUTE;
void sdhi_mmc0_sdio_isr                      (void) WEAK_REF_ATTRIBUTE;
void sdhi_mmc0_card_isr                      (void) WEAK_REF_ATTRIBUTE;
void sdhi_mmc0_odmsdbreq_isr                 (void) WEAK_REF_ATTRIBUTE;
void sdhi_mmc1_accs_isr                      (void) WEAK_REF_ATTRIBUTE;
void sdhi_mmc1_sdio_isr                      (void) WEAK_REF_ATTRIBUTE;
void sdhi_mmc1_card_isr                      (void) WEAK_REF_ATTRIBUTE;
void sdhi_mmc1_odmsdbreq_isr                 (void) WEAK_REF_ATTRIBUTE;
void ext_divider_intmd_isr                   (void) WEAK_REF_ATTRIBUTE;
void tsip_proc_busy_n_isr                    (void) WEAK_REF_ATTRIBUTE;
void tsip_romok_n_isr                        (void) WEAK_REF_ATTRIBUTE;
void tsip_long_plg_n_isr                     (void) WEAK_REF_ATTRIBUTE;
void tsip_test_busy_n_isr                    (void) WEAK_REF_ATTRIBUTE;
void tsip_wrrdy_0_n_isr                      (void) WEAK_REF_ATTRIBUTE;
void tsip_wrrdy_1_n_isr                      (void) WEAK_REF_ATTRIBUTE;
void tsip_wrrdy_4_n_isr                      (void) WEAK_REF_ATTRIBUTE;
void tsip_rdrdy_0_n_isr                      (void) WEAK_REF_ATTRIBUTE;
void tsip_rdrdy_1_n_isr                      (void) WEAK_REF_ATTRIBUTE;
void tsip_integrate_wrrdy_n_isr              (void) WEAK_REF_ATTRIBUTE;
void tsip_integrate_rdrdy_n_isr              (void) WEAK_REF_ATTRIBUTE;
void lcdc_lcdc_level_0_isr                   (void) WEAK_REF_ATTRIBUTE;
void lcdc_lcdc_level_1_isr                   (void) WEAK_REF_ATTRIBUTE;
void lcdc_lcdc_level_2_isr                   (void) WEAK_REF_ATTRIBUTE;
void twod_engine_irq_isr                     (void) WEAK_REF_ATTRIBUTE;
void jpeg_jedi_isr                           (void) WEAK_REF_ATTRIBUTE;
void jpeg_jdti_isr                           (void) WEAK_REF_ATTRIBUTE;

/* Vector table. */
BSP_DONT_REMOVE const exc_ptr_t __Vectors[112] BSP_PLACE_IN_SECTION(BSP_SECTION_VECTOR) =
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
    port0_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ1 != BSP_IRQ_DISABLED)
    port1_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ2 != BSP_IRQ_DISABLED)
    port2_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ3 != BSP_IRQ_DISABLED)
    port3_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ4 != BSP_IRQ_DISABLED)
    port4_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ5 != BSP_IRQ_DISABLED)
    port5_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ6 != BSP_IRQ_DISABLED)
    port6_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ7 != BSP_IRQ_DISABLED)
    port7_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ8 != BSP_IRQ_DISABLED)
    port8_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ9 != BSP_IRQ_DISABLED)
    port9_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ10 != BSP_IRQ_DISABLED)
    port10_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ11 != BSP_IRQ_DISABLED)
    port11_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ12 != BSP_IRQ_DISABLED)
    port12_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ13 != BSP_IRQ_DISABLED)
    port13_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ14 != BSP_IRQ_DISABLED)
    port14_irq_isr,
#endif
#if (BSP_IRQ_CFG_ICU_IRQ15 != BSP_IRQ_DISABLED)
    port15_irq_isr,
#endif
#if (BSP_IRQ_CFG_DMAC0_INT != BSP_IRQ_DISABLED)
    dmac0_dmac_isr,
#endif
#if (BSP_IRQ_CFG_DMAC1_INT != BSP_IRQ_DISABLED)
    dmac1_dmac_isr,
#endif
#if (BSP_IRQ_CFG_DMAC2_INT != BSP_IRQ_DISABLED)
    dmac2_dmac_isr,
#endif
#if (BSP_IRQ_CFG_DMAC3_INT != BSP_IRQ_DISABLED)
    dmac3_dmac_isr,
#endif
#if (BSP_IRQ_CFG_DMAC4_INT != BSP_IRQ_DISABLED)
    dmac4_dmac_isr,
#endif
#if (BSP_IRQ_CFG_DMAC5_INT != BSP_IRQ_DISABLED)
    dmac5_dmac_isr,
#endif
#if (BSP_IRQ_CFG_DMAC6_INT != BSP_IRQ_DISABLED)
    dmac6_dmac_isr,
#endif
#if (BSP_IRQ_CFG_DMAC7_INT != BSP_IRQ_DISABLED)
    dmac7_dmac_isr,
#endif
#if (BSP_IRQ_CFG_DTC_COMPLETE != BSP_IRQ_DISABLED)
    dtc_complete_isr,
#endif
#if (BSP_IRQ_CFG_DTC_END != BSP_IRQ_DISABLED)
    dtc_dtc_end_isr,
#endif
#if (BSP_IRQ_CFG_ICU_SNOOZE_CANCEL != BSP_IRQ_DISABLED)
    icu_canceling_snooze_mode_isr,
#endif
#if (BSP_IRQ_CFG_FCU_FIFERR != BSP_IRQ_DISABLED)
    fcu_fiferr_isr,
#endif
#if (BSP_IRQ_CFG_FCU_FRDYI != BSP_IRQ_DISABLED)
    fcu_frdyi_isr,
#endif
#if (BSP_IRQ_CFG_LVD_LVD1 != BSP_IRQ_DISABLED)
    lvd1_lvd1_isr,
#endif
#if (BSP_IRQ_CFG_LVD_LVD2 != BSP_IRQ_DISABLED)
    lvd2_lvd2_isr,
#endif
#if (BSP_IRQ_CFG_CGC_MOSC_STOP != BSP_IRQ_DISABLED)
    mosc_osc_stop_isr,
#endif
#if (BSP_IRQ_CFG_LPM_SNOOZE_REQUEST != BSP_IRQ_DISABLED)
    cpusys_snooze_mode_entry_flag_isr,
#endif
#if (BSP_IRQ_CFG_AGT0_INT != BSP_IRQ_DISABLED)
    agt0_agti_isr,
#endif
#if (BSP_IRQ_CFG_AGT0_COMPARE_A != BSP_IRQ_DISABLED)
    agt0_agtcmai_isr,
#endif
#if (BSP_IRQ_CFG_AGT0_COMPARE_B != BSP_IRQ_DISABLED)
    agt0_agtcmbi_isr,
#endif
#if (BSP_IRQ_CFG_AGT1_INT != BSP_IRQ_DISABLED)
    agt1_agti_isr,
#endif
#if (BSP_IRQ_CFG_AGT1_COMPARE_A != BSP_IRQ_DISABLED)
    agt1_agtcmai_isr,
#endif
#if (BSP_IRQ_CFG_AGT1_COMPARE_B != BSP_IRQ_DISABLED)
    agt1_agtcmbi_isr,
#endif
#if (BSP_IRQ_CFG_IWDT_UNDERFLOW != BSP_IRQ_DISABLED)
    iwdt_nmiundf_n_isr,
#endif
#if (BSP_IRQ_CFG_WDT_UNDERFLOW != BSP_IRQ_DISABLED)
    cwdt_nmiundf_n_isr,
#endif
#if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED)
    rtc_alm_isr,
#endif
#if (BSP_IRQ_CFG_RTC_PERIOD != BSP_IRQ_DISABLED)
    rtc_prd_isr,
#endif
#if (BSP_IRQ_CFG_RTC_CARRY != BSP_IRQ_DISABLED)
    rtc_cup_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_SCAN_END != BSP_IRQ_DISABLED)
    s12ad0_adi_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_SCAN_END_B != BSP_IRQ_DISABLED)
    s12ad0_gbadi_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_WINDOW_A != BSP_IRQ_DISABLED)
    s12ad0_cmpai_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_WINDOW_B != BSP_IRQ_DISABLED)
    s12ad0_cmpbi_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_COMPARE_MATCH != BSP_IRQ_DISABLED)
    s12ad0_compare_match_isr,
#endif
#if (BSP_IRQ_CFG_ADC0_COMPARE_MISMATCH != BSP_IRQ_DISABLED)
    s12ad0_compare_mismatch_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_SCAN_END != BSP_IRQ_DISABLED)
    s12ad1_adi_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_SCAN_END_B != BSP_IRQ_DISABLED)
    s12ad1_gbadi_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_WINDOW_A != BSP_IRQ_DISABLED)
    s12ad1_cmpai_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_WINDOW_B != BSP_IRQ_DISABLED)
    s12ad1_cmpbi_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_COMPARE_MATCH != BSP_IRQ_DISABLED)
    s12ad1_compare_match_isr,
#endif
#if (BSP_IRQ_CFG_ADC1_COMPARE_MISMATCH != BSP_IRQ_DISABLED)
    s12ad1_compare_mismatch_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_0 != BSP_IRQ_DISABLED)
    comp_oc0_comp_irq_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_1 != BSP_IRQ_DISABLED)
    comp_rd1_comp_irq_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_2 != BSP_IRQ_DISABLED)
    comp_rd2_comp_irq_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_3 != BSP_IRQ_DISABLED)
    comp_rd3_comp_irq_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_4 != BSP_IRQ_DISABLED)
    comp_rd4_comp_irq_isr,
#endif
#if (BSP_IRQ_CFG_COMP_HS_5 != BSP_IRQ_DISABLED)
    comp_rd5_comp_irq_isr,
#endif
#if (BSP_IRQ_CFG_USBFS_FIFO_0 != BSP_IRQ_DISABLED)
    usbfs_d0fifo_isr,
#endif
#if (BSP_IRQ_CFG_USBFS_FIFO_1 != BSP_IRQ_DISABLED)
    usbfs_d1fifo_isr,
#endif
#if (BSP_IRQ_CFG_USBFS_INT != BSP_IRQ_DISABLED)
    usbfs_usbi_isr,
#endif
#if (BSP_IRQ_CFG_USBFS_RESUME != BSP_IRQ_DISABLED)
    usbfs_usbr_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_RXI != BSP_IRQ_DISABLED)
    riic0_rxi_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_TXI != BSP_IRQ_DISABLED)
    riic0_txi_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_TEI != BSP_IRQ_DISABLED)
    riic0_tei_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_ERI != BSP_IRQ_DISABLED)
    riic0_eei_isr,
#endif
#if (BSP_IRQ_CFG_IIC0_WUI != BSP_IRQ_DISABLED)
    riic0_wui_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_RXI != BSP_IRQ_DISABLED)
    riic1_rxi_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_TXI != BSP_IRQ_DISABLED)
    riic1_txi_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_TEI != BSP_IRQ_DISABLED)
    riic1_tei_isr,
#endif
#if (BSP_IRQ_CFG_IIC1_ERI != BSP_IRQ_DISABLED)
    riic1_eei_isr,
#endif
#if (BSP_IRQ_CFG_IIC2_RXI != BSP_IRQ_DISABLED)
    riic2_rxi_isr,
#endif
#if (BSP_IRQ_CFG_IIC2_TXI != BSP_IRQ_DISABLED)
    riic2_txi_isr,
#endif
#if (BSP_IRQ_CFG_IIC2_TEI != BSP_IRQ_DISABLED)
    riic2_tei_isr,
#endif
#if (BSP_IRQ_CFG_IIC2_ERI != BSP_IRQ_DISABLED)
    riic2_eei_isr,
#endif
#if (BSP_IRQ_CFG_SSI0_TXI != BSP_IRQ_DISABLED)
    ssi0_ssitxi_isr,
#endif
#if (BSP_IRQ_CFG_SSI0_RXI != BSP_IRQ_DISABLED)
    ssi0_ssirxi_isr,
#endif
#if (BSP_IRQ_CFG_SSI0_INT != BSP_IRQ_DISABLED)
    ssi0_ssif_isr,
#endif
#if (BSP_IRQ_CFG_SSI1_TXI_RXI != BSP_IRQ_DISABLED)
    ssi1_ssirt_isr,
#endif
#if (BSP_IRQ_CFG_SSI1_INT != BSP_IRQ_DISABLED)
    ssi1_ssif_isr,
#endif
#if (BSP_IRQ_CFG_SRC_INPUT_FIFO_EMPTY != BSP_IRQ_DISABLED)
    src_idei_isr,
#endif
#if (BSP_IRQ_CFG_SRC_OUTPUT_FIFO_FULL != BSP_IRQ_DISABLED)
    src_odfi_isr,
#endif
#if (BSP_IRQ_CFG_SRC_OUTPUT_FIFO_OVERFLOW != BSP_IRQ_DISABLED)
    src_ovf_isr,
#endif
#if (BSP_IRQ_CFG_SRC_OUTPUT_FIFO_UNDERFLOW != BSP_IRQ_DISABLED)
    src_udf_isr,
#endif
#if (BSP_IRQ_CFG_SRC_CONVERSION_END != BSP_IRQ_DISABLED)
    src_cef_isr,
#endif
#if (BSP_IRQ_CFG_PDC_RECEIVE_DATA_READY != BSP_IRQ_DISABLED)
    pdc_pcdfi_isr,
#endif
#if (BSP_IRQ_CFG_PDC_FRAME_END != BSP_IRQ_DISABLED)
    pdc_pcfei_isr,
#endif
#if (BSP_IRQ_CFG_PDC_INT != BSP_IRQ_DISABLED)
    pdc_pceri_isr,
#endif
#if (BSP_IRQ_CFG_CTSU_WRITE != BSP_IRQ_DISABLED)
    ctsu_ctsuwr_isr,
#endif
#if (BSP_IRQ_CFG_CTSU_READ != BSP_IRQ_DISABLED)
    ctsu_ctsurd_isr,
#endif
#if (BSP_IRQ_CFG_CTSU_END != BSP_IRQ_DISABLED)
    ctsu_ctsufn_isr,
#endif
#if (BSP_IRQ_CFG_KEY_INT != BSP_IRQ_DISABLED)
    key_intkr_isr,
#endif
#if (BSP_IRQ_CFG_DOC_INT != BSP_IRQ_DISABLED)
    doc_dopcf_isr,
#endif
#if (BSP_IRQ_CFG_CAC_FREQUENCY_ERROR != BSP_IRQ_DISABLED)
    cac_ferrf_isr,
#endif
#if (BSP_IRQ_CFG_CAC_MEASUREMENT_END != BSP_IRQ_DISABLED)
    cac_mendf_isr,
#endif
#if (BSP_IRQ_CFG_CAC_OVERFLOW != BSP_IRQ_DISABLED)
    cac_ovff_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_ERROR != BSP_IRQ_DISABLED)
    rcan20_ers_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_FIFO_RX != BSP_IRQ_DISABLED)
    rcan20_rxf_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_FIFO_TX != BSP_IRQ_DISABLED)
    rcan20_txf_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_MAILBOX_RX != BSP_IRQ_DISABLED)
    rcan20_rxm_isr,
#endif
#if (BSP_IRQ_CFG_CAN0_MAILBOX_TX != BSP_IRQ_DISABLED)
    rcan20_txm_isr,
#endif
#if (BSP_IRQ_CFG_CAN1_ERROR != BSP_IRQ_DISABLED)
    rcan21_ers_isr,
#endif
#if (BSP_IRQ_CFG_CAN1_FIFO_RX != BSP_IRQ_DISABLED)
    rcan21_rxf_isr,
#endif
#if (BSP_IRQ_CFG_CAN1_FIFO_TX != BSP_IRQ_DISABLED)
    rcan21_txf_isr,
#endif
#if (BSP_IRQ_CFG_CAN1_MAILBOX_RX != BSP_IRQ_DISABLED)
    rcan21_rxm_isr,
#endif
#if (BSP_IRQ_CFG_CAN1_MAILBOX_TX != BSP_IRQ_DISABLED)
    rcan21_txm_isr,
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_1 != BSP_IRQ_DISABLED)
    gpio_port_group_a_isr,
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_2 != BSP_IRQ_DISABLED)
    gpio_port_group_b_isr,
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_3 != BSP_IRQ_DISABLED)
    gpio_port_group_c_isr,
#endif
#if (BSP_IRQ_CFG_IOPORT_EVENT_4 != BSP_IRQ_DISABLED)
    gpio_port_group_d_isr,
#endif
#if (BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_0 != BSP_IRQ_DISABLED)
    elc0_software_event_isr,
#endif
#if (BSP_IRQ_CFG_ELC_SOFTWARE_EVENT_1 != BSP_IRQ_DISABLED)
    elc1_software_event_isr,
#endif
#if (BSP_IRQ_CFG_POEG0_EVENT != BSP_IRQ_DISABLED)
    poeg_group_event0_isr,
#endif
#if (BSP_IRQ_CFG_POEG1_EVENT != BSP_IRQ_DISABLED)
    poeg_group_event1_isr,
#endif
#if (BSP_IRQ_CFG_POEG2_EVENT != BSP_IRQ_DISABLED)
    poeg_group_event2_isr,
#endif
#if (BSP_IRQ_CFG_POEG3_EVENT != BSP_IRQ_DISABLED)
    poeg_group_event3_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt0_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt0_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_C != BSP_IRQ_DISABLED)
    gpt0_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_D != BSP_IRQ_DISABLED)
    gpt0_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_E != BSP_IRQ_DISABLED)
    gpt0_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COMPARE_F != BSP_IRQ_DISABLED)
    gpt0_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt0_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt0_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt0_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT0_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt0_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt1_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt1_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_C != BSP_IRQ_DISABLED)
    gpt1_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_D != BSP_IRQ_DISABLED)
    gpt1_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_E != BSP_IRQ_DISABLED)
    gpt1_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COMPARE_F != BSP_IRQ_DISABLED)
    gpt1_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt1_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt1_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt1_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT1_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt1_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt2_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt2_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_C != BSP_IRQ_DISABLED)
    gpt2_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_D != BSP_IRQ_DISABLED)
    gpt2_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_E != BSP_IRQ_DISABLED)
    gpt2_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COMPARE_F != BSP_IRQ_DISABLED)
    gpt2_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt2_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt2_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt2_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT2_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt2_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt3_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt3_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_C != BSP_IRQ_DISABLED)
    gpt3_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_D != BSP_IRQ_DISABLED)
    gpt3_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_E != BSP_IRQ_DISABLED)
    gpt3_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COMPARE_F != BSP_IRQ_DISABLED)
    gpt3_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt3_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt3_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt3_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT3_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt3_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt4_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt4_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_C != BSP_IRQ_DISABLED)
    gpt4_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_D != BSP_IRQ_DISABLED)
    gpt4_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_E != BSP_IRQ_DISABLED)
    gpt4_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COMPARE_F != BSP_IRQ_DISABLED)
    gpt4_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt4_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt4_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt4_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT4_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt4_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt5_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt5_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_C != BSP_IRQ_DISABLED)
    gpt5_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_D != BSP_IRQ_DISABLED)
    gpt5_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_E != BSP_IRQ_DISABLED)
    gpt5_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COMPARE_F != BSP_IRQ_DISABLED)
    gpt5_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt5_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt5_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt5_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT5_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt5_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt6_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt6_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_C != BSP_IRQ_DISABLED)
    gpt6_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_D != BSP_IRQ_DISABLED)
    gpt6_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_E != BSP_IRQ_DISABLED)
    gpt6_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COMPARE_F != BSP_IRQ_DISABLED)
    gpt6_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt6_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt6_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt6_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT6_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt6_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt7_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt7_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_C != BSP_IRQ_DISABLED)
    gpt7_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_D != BSP_IRQ_DISABLED)
    gpt7_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_E != BSP_IRQ_DISABLED)
    gpt7_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COMPARE_F != BSP_IRQ_DISABLED)
    gpt7_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt7_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt7_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt7_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT7_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt7_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt8_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt8_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_C != BSP_IRQ_DISABLED)
    gpt8_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_D != BSP_IRQ_DISABLED)
    gpt8_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_E != BSP_IRQ_DISABLED)
    gpt8_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COMPARE_F != BSP_IRQ_DISABLED)
    gpt8_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt8_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt8_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt8_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT8_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt8_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt9_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt9_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_C != BSP_IRQ_DISABLED)
    gpt9_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_D != BSP_IRQ_DISABLED)
    gpt9_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_E != BSP_IRQ_DISABLED)
    gpt9_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COMPARE_F != BSP_IRQ_DISABLED)
    gpt9_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt9_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt9_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt9_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT9_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt9_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt10_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt10_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_C != BSP_IRQ_DISABLED)
    gpt10_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_D != BSP_IRQ_DISABLED)
    gpt10_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_E != BSP_IRQ_DISABLED)
    gpt10_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COMPARE_F != BSP_IRQ_DISABLED)
    gpt10_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt10_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt10_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt10_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT10_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt10_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt11_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt11_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_C != BSP_IRQ_DISABLED)
    gpt11_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_D != BSP_IRQ_DISABLED)
    gpt11_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_E != BSP_IRQ_DISABLED)
    gpt11_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COMPARE_F != BSP_IRQ_DISABLED)
    gpt11_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt11_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt11_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt11_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT11_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt11_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt12_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt12_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_C != BSP_IRQ_DISABLED)
    gpt12_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_D != BSP_IRQ_DISABLED)
    gpt12_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_E != BSP_IRQ_DISABLED)
    gpt12_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COMPARE_F != BSP_IRQ_DISABLED)
    gpt12_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt12_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt12_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt12_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT12_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt12_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_CAPTURE_COMPARE_A != BSP_IRQ_DISABLED)
    gpt13_capture_compare_int_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_CAPTURE_COMPARE_B != BSP_IRQ_DISABLED)
    gpt13_capture_compare_int_b_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_C != BSP_IRQ_DISABLED)
    gpt13_compare_int_c_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_D != BSP_IRQ_DISABLED)
    gpt13_compare_int_d_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_E != BSP_IRQ_DISABLED)
    gpt13_compare_int_e_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COMPARE_F != BSP_IRQ_DISABLED)
    gpt13_compare_int_f_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)
    gpt13_counter_overflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_COUNTER_UNDERFLOW != BSP_IRQ_DISABLED)
    gpt13_counter_underflow_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_AD_TRIG_A != BSP_IRQ_DISABLED)
    gpt13_ad_trig_a_isr,
#endif
#if (BSP_IRQ_CFG_GPT13_AD_TRIG_B != BSP_IRQ_DISABLED)
    gpt13_ad_trig_b_isr,
#endif
#if (BSP_IRQ_CFG_OPS_UVW_EDGE != BSP_IRQ_DISABLED)
    gpt_uvw_edge_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_IPLS != BSP_IRQ_DISABLED)
    ether_ipls_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_MINT != BSP_IRQ_DISABLED)
    ether_mint_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_PINT != BSP_IRQ_DISABLED)
    ether_pint_isr,
#endif
#if (BSP_IRQ_CFG_EDMAC0_EINT != BSP_IRQ_DISABLED)
    ether_eint0_isr,
#endif
#if (BSP_IRQ_CFG_EDMAC1_EINT != BSP_IRQ_DISABLED)
    ether_eint1_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER0_RISE != BSP_IRQ_DISABLED)
    ether_ether0_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER1_RISE != BSP_IRQ_DISABLED)
    ether_ether1_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER2_RISE != BSP_IRQ_DISABLED)
    ether_ether2_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER3_RISE != BSP_IRQ_DISABLED)
    ether_ether3_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER4_RISE != BSP_IRQ_DISABLED)
    ether_ether4_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER5_RISE != BSP_IRQ_DISABLED)
    ether_ether5_rise_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER0_FALL != BSP_IRQ_DISABLED)
    ether_ether0_fall_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER1_FALL != BSP_IRQ_DISABLED)
    ether_ether1_fall_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER2_FALL != BSP_IRQ_DISABLED)
    ether_ether2_fall_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER3_FALL != BSP_IRQ_DISABLED)
    ether_ether3_fall_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER4_FALL != BSP_IRQ_DISABLED)
    ether_ether4_fall_isr,
#endif
#if (BSP_IRQ_CFG_EPTPC_TIMER5_FALL != BSP_IRQ_DISABLED)
    ether_ether5_fall_isr,
#endif
#if (BSP_IRQ_CFG_USBHS_FIFO_0 != BSP_IRQ_DISABLED)
    usbhs_d0fifo_isr,
#endif
#if (BSP_IRQ_CFG_USBHS_FIFO_1 != BSP_IRQ_DISABLED)
    usbhs_d1fifo_isr,
#endif
#if (BSP_IRQ_CFG_USBHS_USB_INT_RESUME != BSP_IRQ_DISABLED)
    usbhs_usbir_isr,
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
#if (BSP_IRQ_CFG_SCI2_RXI != BSP_IRQ_DISABLED)
    sci2_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI2_TXI != BSP_IRQ_DISABLED)
    sci2_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI2_TEI != BSP_IRQ_DISABLED)
    sci2_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI2_ERI != BSP_IRQ_DISABLED)
    sci2_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI2_AM != BSP_IRQ_DISABLED)
    sci2_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI3_RXI != BSP_IRQ_DISABLED)
    sci3_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI3_TXI != BSP_IRQ_DISABLED)
    sci3_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI3_TEI != BSP_IRQ_DISABLED)
    sci3_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI3_ERI != BSP_IRQ_DISABLED)
    sci3_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI3_AM != BSP_IRQ_DISABLED)
    sci3_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI4_RXI != BSP_IRQ_DISABLED)
    sci4_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI4_TXI != BSP_IRQ_DISABLED)
    sci4_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI4_TEI != BSP_IRQ_DISABLED)
    sci4_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI4_ERI != BSP_IRQ_DISABLED)
    sci4_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI4_AM != BSP_IRQ_DISABLED)
    sci4_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI5_RXI != BSP_IRQ_DISABLED)
    sci5_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI5_TXI != BSP_IRQ_DISABLED)
    sci5_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI5_TEI != BSP_IRQ_DISABLED)
    sci5_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI5_ERI != BSP_IRQ_DISABLED)
    sci5_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI5_AM != BSP_IRQ_DISABLED)
    sci5_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI6_RXI != BSP_IRQ_DISABLED)
    sci6_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI6_TXI != BSP_IRQ_DISABLED)
    sci6_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI6_TEI != BSP_IRQ_DISABLED)
    sci6_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI6_ERI != BSP_IRQ_DISABLED)
    sci6_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI6_AM != BSP_IRQ_DISABLED)
    sci6_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI7_RXI != BSP_IRQ_DISABLED)
    sci7_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI7_TXI != BSP_IRQ_DISABLED)
    sci7_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI7_TEI != BSP_IRQ_DISABLED)
    sci7_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI7_ERI != BSP_IRQ_DISABLED)
    sci7_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI7_AM != BSP_IRQ_DISABLED)
    sci7_am_isr,
#endif
#if (BSP_IRQ_CFG_SCI8_RXI != BSP_IRQ_DISABLED)
    sci8_rxi_isr,
#endif
#if (BSP_IRQ_CFG_SCI8_TXI != BSP_IRQ_DISABLED)
    sci8_txi_isr,
#endif
#if (BSP_IRQ_CFG_SCI8_TEI != BSP_IRQ_DISABLED)
    sci8_tei_isr,
#endif
#if (BSP_IRQ_CFG_SCI8_ERI != BSP_IRQ_DISABLED)
    sci8_eri_isr,
#endif
#if (BSP_IRQ_CFG_SCI8_AM != BSP_IRQ_DISABLED)
    sci8_am_isr,
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
    rspi0_spri_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_TXI != BSP_IRQ_DISABLED)
    rspi0_spti_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_IDLE != BSP_IRQ_DISABLED)
    rspi0_spii_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_ERI != BSP_IRQ_DISABLED)
    rspi0_spei_isr,
#endif
#if (BSP_IRQ_CFG_SPI0_TEI != BSP_IRQ_DISABLED)
    rspi0_sp_elctend_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_RXI != BSP_IRQ_DISABLED)
    rspi1_spri_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_TXI != BSP_IRQ_DISABLED)
    rspi1_spti_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_IDLE != BSP_IRQ_DISABLED)
    rspi1_spii_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_ERI != BSP_IRQ_DISABLED)
    rspi1_spei_isr,
#endif
#if (BSP_IRQ_CFG_SPI1_TEI != BSP_IRQ_DISABLED)
    rspi1_sp_elctend_isr,
#endif
#if (BSP_IRQ_CFG_QSPI_INT != BSP_IRQ_DISABLED)
    qspi_intr_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_ACCS != BSP_IRQ_DISABLED)
    sdhi_mmc0_accs_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_SDIO != BSP_IRQ_DISABLED)
    sdhi_mmc0_sdio_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_CARD != BSP_IRQ_DISABLED)
    sdhi_mmc0_card_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC0_DMA_REQ != BSP_IRQ_DISABLED)
    sdhi_mmc0_odmsdbreq_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_ACCS != BSP_IRQ_DISABLED)
    sdhi_mmc1_accs_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_SDIO != BSP_IRQ_DISABLED)
    sdhi_mmc1_sdio_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_CARD != BSP_IRQ_DISABLED)
    sdhi_mmc1_card_isr,
#endif
#if (BSP_IRQ_CFG_SDHIMMC1_DMA_REQ != BSP_IRQ_DISABLED)
    sdhi_mmc1_odmsdbreq_isr,
#endif
#if (BSP_IRQ_CFG_DIVIDER_INT != BSP_IRQ_DISABLED)
    ext_divider_intmd_isr,
#endif
#if (BSP_IRQ_CFG_SCE_PROC_BUSY != BSP_IRQ_DISABLED)
    tsip_proc_busy_n_isr,
#endif
#if (BSP_IRQ_CFG_SCE_ROMOK != BSP_IRQ_DISABLED)
    tsip_romok_n_isr,
#endif
#if (BSP_IRQ_CFG_SCE_LONG_PLG != BSP_IRQ_DISABLED)
    tsip_long_plg_n_isr,
#endif
#if (BSP_IRQ_CFG_SCE_TEST_BUSY != BSP_IRQ_DISABLED)
    tsip_test_busy_n_isr,
#endif
#if (BSP_IRQ_CFG_SCE_WRRDY_0 != BSP_IRQ_DISABLED)
    tsip_wrrdy_0_n_isr,
#endif
#if (BSP_IRQ_CFG_SCE_WRRDY_1 != BSP_IRQ_DISABLED)
    tsip_wrrdy_1_n_isr,
#endif
#if (BSP_IRQ_CFG_SCE_WRRDY_4 != BSP_IRQ_DISABLED)
    tsip_wrrdy_4_n_isr,
#endif
#if (BSP_IRQ_CFG_SCE_RDRDY_0 != BSP_IRQ_DISABLED)
    tsip_rdrdy_0_n_isr,
#endif
#if (BSP_IRQ_CFG_SCE_RDRDY_1 != BSP_IRQ_DISABLED)
    tsip_rdrdy_1_n_isr,
#endif
#if (BSP_IRQ_CFG_SCE_INTEGRATE_WRRDY != BSP_IRQ_DISABLED)
    tsip_integrate_wrrdy_n_isr,
#endif
#if (BSP_IRQ_CFG_SCE_INTEGRATE_RDRDY != BSP_IRQ_DISABLED)
    tsip_integrate_rdrdy_n_isr,
#endif
#if (BSP_IRQ_CFG_GLCDC_LINE_DETECT != BSP_IRQ_DISABLED)
    lcdc_lcdc_level_0_isr,
#endif
#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_1 != BSP_IRQ_DISABLED)
    lcdc_lcdc_level_1_isr,
#endif
#if (BSP_IRQ_CFG_GLCDC_UNDERFLOW_2 != BSP_IRQ_DISABLED)
    lcdc_lcdc_level_2_isr,
#endif
#if (BSP_IRQ_CFG_DRW_INT != BSP_IRQ_DISABLED)
    twod_engine_irq_isr,
#endif
#if (BSP_IRQ_CFG_JPEG_JEDI != BSP_IRQ_DISABLED)
    jpeg_jedi_isr,
#endif
#if (BSP_IRQ_CFG_JPEG_JDTI != BSP_IRQ_DISABLED)
    jpeg_jdti_isr,
#endif
};

#endif
