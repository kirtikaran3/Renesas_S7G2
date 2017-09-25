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

#ifndef R_LPM_PRIVATE_API_H
#define R_LPM_PRIVATE_API_H

/***********************************************************************************************************************
 * Private Instance API Functions. DO NOT USE! Use functions through Interface API structure instead.
 **********************************************************************************************************************/
ssp_err_t R_LPM_Init (lpm_cfg_t const * const p_cfg);
ssp_err_t R_LPM_MSTPCRSet (uint32_t mstpcra_value,
                           uint32_t mstpcrb_value,
                           uint32_t mstpcrc_value,
                           uint32_t mstpcrd_value);
ssp_err_t R_LPM_MSTPCRGet (uint32_t * mstpcra_value,
                           uint32_t * mstpcrb_value,
                           uint32_t * mstpcrc_value,
                           uint32_t * mstpcrd_value);
ssp_err_t R_LPM_ModuleStop (lpm_mstp_t module);
ssp_err_t R_LPM_ModuleStart (lpm_mstp_t module);
ssp_err_t R_LPM_OperatingPowerModeSet (lpm_operating_power_t power_mode, lpm_subosc_t subosc);
ssp_err_t R_LPM_SnoozeEnable (lpm_snooze_rxd0_t    rdx0_mode,
                              lpm_snooze_dtc_t     dtc_mode,
                              lpm_snooze_request_t requests,
                              lpm_snooze_end_t     triggers);
ssp_err_t R_LPM_SnoozeDisable (void);
ssp_err_t R_LPM_LowPowerConfigure (lpm_low_power_mode_t     power_mode,
                                   lpm_output_port_enable_t output_port_enable,
                                   lpm_power_supply_t       power_supply,
                                   lpm_io_port_t            io_port_state);
ssp_err_t R_LPM_WUPENSet (uint32_t wupen_value);
ssp_err_t R_LPM_WUPENGet (uint32_t * wupen_value);
ssp_err_t R_LPM_DeepStandbyCancelRequestEnable (lpm_deep_standby_t        pin_signal,
                                                lpm_cancel_request_edge_t rising_falling);
ssp_err_t R_LPM_DeepStandbyCancelRequestDisable (lpm_deep_standby_t pin_signal);
ssp_err_t R_LPM_LowPowerModeEnter (void);
ssp_err_t R_LPM_VersionGet (ssp_version_t * const p_version);

#endif /* R_LPM_PRIVATE_API_H */