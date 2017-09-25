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
 * File Name    : sf_el_nx_comms.h
 * Description  : NetX Telnet server communication framework definitions
 **********************************************************************************************************************/

#ifndef SF_EL_NX_COMMS_H
#define SF_EL_NX_COMMS_H

/*******************************************************************************************************************//**
 * @ingroup SF_Library
 * @defgroup SF_EL_NX_COMMS  Telnet Communication Framework
 * @brief RTOS-integrated Communications Framework NetX telnet server implementation
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_comms_api.h"
#include "sf_el_nx_comms_cfg.h"
#include "nx_api.h"
#include "nx_telnet_server.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SF_EL_NX_COMMS_CODE_VERSION_MAJOR (1)
#define SF_EL_NX_COMMS_CODE_VERSION_MINOR (2)

#define SF_EL_NX_COMMS_PACKET_POOL_MEMORY_SIZE_BYTES   ((1536 + 32 + sizeof(NX_PACKET)) * 50)
#define SF_EL_NX_COMMS_IP_MEMORY_SIZE_BYTES            (2048)
#define SF_EL_NX_COMMS_ARP_MEMORY_SIZE_BYTES           (1024)
#define SF_EL_NX_COMMS_TELNET_SERVER_MEMORY_SIZE_BYTES (2048)
#define SF_EL_NX_COMMS_QUEUE_MEMORY_SIZE_BYTES         (20)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** NetX Telnet server communications driver configuration */
typedef struct st_sf_el_nx_comms_on_comms_ctrl
{
    uint32_t  open;
    NX_PACKET * p_current_packet;
    uint32_t  packet_index;
    TX_MUTEX  mutex[2];
    NX_PACKET_POOL pool;
    uint8_t   pool_memory[SF_EL_NX_COMMS_PACKET_POOL_MEMORY_SIZE_BYTES];
    NX_IP     ip;
    uint8_t   ip_memory[SF_EL_NX_COMMS_IP_MEMORY_SIZE_BYTES];
    uint8_t   arp_memory[SF_EL_NX_COMMS_ARP_MEMORY_SIZE_BYTES];
    NX_TELNET_SERVER telnet_server;
    uint8_t   telnet_server_memory[SF_EL_NX_COMMS_TELNET_SERVER_MEMORY_SIZE_BYTES];
    UINT      logical_connection;
    TX_EVENT_FLAGS_GROUP available;  ///< Flag to tell if this connection is available or not
    TX_QUEUE  queue;                 ///< Queue of received bytes
    uint8_t   queue_memory[SF_EL_NX_COMMS_QUEUE_MEMORY_SIZE_BYTES];
} sf_el_nx_comms_on_comms_ctrl_t;

/** NetX Telnet server device communications driver configuration */
typedef struct st_sf_el_nx_comms_on_comms_cfg
{
    sf_el_nx_comms_on_comms_ctrl_t * p_ctrl; ///< Memory allocated for NetX Telnet server device control block
    uint32_t  ip_address;
    uint32_t  subnet_mask;
    VOID      (*driver) (NX_IP_DRIVER *driver_req_ptr);
} sf_el_nx_comms_on_comms_cfg_t;

/**********************************************************************************************************************
 *
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const sf_comms_api_t g_sf_el_nx_comms_on_sf_comms;
/** @endcond */

/*******************************************************************************************************************//**
 * @} (end defgroup SF_EL_NX_COMMS)
 **********************************************************************************************************************/

#endif // SF_EL_NX_COMMS_H
