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
 * File Name    : sf_thread_monitor_api.h
 * Description  : Thread Monitor interface
 **********************************************************************************************************************/


#ifndef SF_THREAD_MONITOR_API_H
#define SF_THREAD_MONITOR_API_H

/*******************************************************************************************************************//**
 * @ingroup SF_Interface_Library
 * @defgroup SF_THREAD_MONITOR_API Thread Monitor Framework Interface
 * @brief RTOS-integrated Framework Interface for monitoring of threads.
 *
 * Any misbehaving threads cause a reset of the device.
 * Both the WDT and IWDT HAL modules are supported by this framework module.
 *
 * @section SF_THREAD_MONITOR_API_SUMMARY Summary
 * This is a ThreadX aware Watchdog Timer Thread Monitor Framework for monitoring threads in an
 * application in which threads are executing at an expected rate. Threads to be monitored register themselves through
 * SF_THREAD_MONITOR_ThreadRegister() and increment a count by calling SF_THREAD_MONITOR_CountIncrement() each time
 * they execute. Each monitored thread also provides expected maximum and minimum count values for normal execution.
 *
 * The Thread Monitor runs periodically and checks the count value of each monitored thread. If the count value falls
 * outside of the expected range of values, the Watchdog Timer is allowed to reset the device. If all thread counts
 * are within their expected ranges, then the Watchdog Timer is refreshed.
 *
 * The WDT and IWDT modules are supported by the Thread Monitor.
 *
 * The Framework Layer can be used to protect the entire software project. This is achieved through a
 * high priority thread (Framework Layer) which runs periodically within the refresh permitted window of the Watchdog
 * Timer selected (IWDT is safest as has its own clock source and is started automatically after reset). This thread
 * monitors the state of every other thread in the system. If any of these threads are not running as expected, then the
 * Watchdog Timer is not refreshed and is not allowed to reset the system. If the threads are running as expected,
 * then the Watchdog Timer is refreshed.
 *
 * Monitoring the other threads is achieved as follows: Each monitored thread increments a count variable each time
 * it runs. The Thread Monitor thread checks the count variable of each thread to make sure it is within an expected
 * range. If any of the variables are out of range a reset is allowed. Otherwise all variables are cleared to zero and
 * the watchdog is refreshed. A profiling mode is used to establish the expected ranges.
 *
 *
 * This approach is described in the following article:
 *
 * Jack Ganssle, "Great Watchdog Timers for Embedded Systems," http://www.ganssle.com/watchdogs.htm
 *
 * This method requires the instrumenting of each thread to increment its count variable, but this is little overhead
 * for the massive gain in protection.
 *
 * Interface used:
 * @ref WDT_API
 *
  * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * Thread Monitor Interface description: @ref FrameworkThreadMonitorInterface
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_thread_monitor_cfg.h"
/* Include driver API and ThreadX API */
#include "r_wdt_api.h"
#include "tx_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define THREAD_MONITOR_THREAD_STACK_SIZE 400u

/** Version of the API defined in this file */
#define SF_THREAD_MONITOR_API_VERSION_MAJOR (1)
#define SF_THREAD_MONITOR_API_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Configuration for RTOS Thread Monitor driver */
typedef struct st_sf_thread_monitor_watchdog_type
{
    wdt_instance_t const  * p_lower_lvl_wdt;            ///< Pointer to lower level watchdog instance
    bool                    profiling_mode_enabled;     ///< Enables or disables profiling mode
    UINT                    priority;                   ///< Priority of thread monitor thread
} sf_thread_monitor_cfg_t;

/** Counter block for each monitored thread. */
typedef struct st_sf_thread_monitor_thread_counter
{
    uint32_t  current_count;                        ///< Current count value for a thread

    /** Minimum expected count value. If the current count is less than this value the watchdog will reset. */
    uint32_t  minimum_count;

    /** Maximum expected count value. If the current count is more than this value the watchdog will reset */
    uint32_t  maximum_count;

    /** Indicates to the monitoring thread whether this count data is currently active. When a thread is registered this
     * value will be set to false as the count is likely to be a partial count and so should not be monitored. This
     * value will be set to true by the thread monitor thread when it clears all counts to zero. */
    bool       active;

    TX_THREAD  * p_thread;                          ///< Pointer to thread for this counter data.
} sf_thread_monitor_thread_counter_t;

/** Counter block for each monitored thread. */
typedef struct st_sf_thread_monitor_counter_min_max
{
    /** Minimum expected count value. If the current count is less than this value the watchdog will reset. */
    uint32_t  minimum_count;

    /** Maximum expected count value. If the current count is more than this value the watchdog will reset */
    uint32_t  maximum_count;
} sf_thread_monitor_counter_min_max_t;

/** Thread monitor control block. DO NOT INITIALIZE.  Initialization occurs when ::SF_THREAD_MONITOR_Open is called */
typedef struct st_sf_thread_monitor_ctrl
{
    /** Used by driver to check if the control structure is valid */
    uint32_t  open;

    /** Pointer to interface structure for the watchdog peripheral */
    wdt_instance_t const * p_lower_lvl_wdt;

    /** Time in milliseconds of the watchdog timeout period. Used to calculate the period of the monitoring thread. */
    uint32_t  timeout_period_msec;

    /** Maximum count value of the watchdog. Used to synchronise to the count. */
    uint32_t  timeout_period_watchdog_clocks;

    /** Used by the driver to check if profiling mode is enabled.*/
    bool      profiling_mode_enabled;

    TX_MUTEX  mutex;                            ///< Mutex to protect access to the thread counters

    /** Value used to verify profiling mode is enabled when prfiling_mode_enabled == true.*/
    uint32_t  profiling_mode_check;

    /** Data storage for the thread counter information.*/
    sf_thread_monitor_thread_counter_t  thread_counters[THREAD_MONITOR_CFG_MAX_NUMBER_OF_THREADS];

    TX_THREAD                           thread;     ///< Thread monitor thread.

    void const                          * p_extend; ///< Extended configuration data

    /** Stack for thread monitor thread.  */
    uint8_t  stack[THREAD_MONITOR_THREAD_STACK_SIZE];
} sf_thread_monitor_ctrl_t;

/** Thread monitor API structure.  Thread Monitor implementations use the following API. */
typedef struct st_sf_thread_monitor_api
{
    /**************************************************************************************************************//**
     * @brief   Configures the WDT or IWDT module. From the configuration data the
     *          timeout period of the WDT/IWDT is determined and a thread created monitoring registered threads.
     * @par Implemented as
     *  - SF_THREAD_MONITOR_Open()
     * @param[in,out] p_ctrl    Pointer to a structure allocated by user. Elements initialized here.
     * @param[in]     p_cfg     Pointer to configuration structure. All elements of the structure must
     *                          be set by user.
     *****************************************************************************************************************/
    ssp_err_t (* open)(sf_thread_monitor_ctrl_t * const p_ctrl, sf_thread_monitor_cfg_t const * const p_cfg);

    /**************************************************************************************************************//**
     * @brief   Suspends the thread monitoring thread. Watchdog peripheral no longer refreshed.
     * @par Implemented as
     *  - SF_THREAD_MONITOR_Close()
     * @param[in,out] p_ctrl    Control structure set in ::SF_THREAD_MONITOR_Open.
     *****************************************************************************************************************/
    ssp_err_t (* close)(sf_thread_monitor_ctrl_t * const p_ctrl);

    /**************************************************************************************************************//**
     * @brief   Registers a thread for monitoring.
     * @par Implemented as
     *  - SF_THREAD_MONITOR_ThreadRegister()
     * @param[in,out] p_ctrl                Control structure set in ::SF_THREAD_MONITOR_Open.
     * @param[in]     p_counter_min_max     Pointer to structure containing min and max values for thread to be
     *                                      registered
     * values.
     *****************************************************************************************************************/
    ssp_err_t (* threadRegister)(sf_thread_monitor_ctrl_t * const    p_ctrl,
                                 sf_thread_monitor_counter_min_max_t const * p_counter_min_max);

    /**************************************************************************************************************//**
     * @brief   Removes a thread from being monitored.
     * @par Implemented as
     *  - SF_THREAD_MONITOR_ThreadUnregister()
     * @param[in,out] p_ctrl                Control structure set in ::SF_THREAD_MONITOR_Open.
     *****************************************************************************************************************/
    ssp_err_t (* threadUnregister)(sf_thread_monitor_ctrl_t * const p_ctrl);

    /**************************************************************************************************************//**
     * @brief   Safely increments a monitored thread's count value.
     * @par Implemented as
     *  - SF_THREAD_MONITOR_CountIncrement()
     * @param[in,out] p_ctrl                Control structure set in ::SF_THREAD_MONITOR_Open.
     *****************************************************************************************************************/
    ssp_err_t (* countIncrement)(sf_thread_monitor_ctrl_t * const p_ctrl);

    /**************************************************************************************************************//**
     * @brief   Get version and store it in provided pointer p_version.
     * @par Implemented as
     *  - SF_THREAD_MONITOR_VersionGet()
     * @param[in,out] p_version             Pointer to structure storing API and code versions.
     *****************************************************************************************************************/
    ssp_err_t (* versionGet)(ssp_version_t * const p_version);
} sf_thread_monitor_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_sf_thread_monitor_instance
{
    sf_thread_monitor_ctrl_t       * p_ctrl;   ///< Pointer to the control structure for this instance
    sf_thread_monitor_cfg_t  const * p_cfg;    ///< Pointer to the configuration structure for this instance
    sf_thread_monitor_api_t  const * p_api;    ///< Pointer to the API structure for this instance
} sf_thread_monitor_instance_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Public Functions
 **********************************************************************************************************************/
#endif //SF_WATCHDOG_API_H

/*******************************************************************************************************************//**
 * @} (end SF_Library)
 **********************************************************************************************************************/
