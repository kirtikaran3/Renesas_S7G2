/* generated thread source file - do not edit */
#include "sf_i2c_pcal1_thread.h"

TX_THREAD sf_i2c_pcal1_thread;
void sf_i2c_pcal1_thread_create(void);
static void sf_i2c_pcal1_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t sf_i2c_pcal1_thread_stack[1024] BSP_PLACE_IN_SECTION(".stack.sf_i2c_pcal1_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#if I2C_ON_SCI_CALLBACK_USED_g_i2c_mainboard
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(i2c_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
i2c_ctrl_t g_i2c_mainboard_ctrl;
const i2c_cfg_t g_i2c_mainboard_cfg =
{ .channel = 7, .rate = I2C_RATE_STANDARD, .slave = 0x27, .addr_mode = I2C_ADDR_MODE_7BIT, .sda_delay = 300,
  .p_callback = NULL, .p_context = (void *) &g_i2c_mainboard, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_mainboard =
{ .p_ctrl = &g_i2c_mainboard_ctrl, .p_cfg = &g_i2c_mainboard_cfg, .p_api = &g_i2c_master_on_sci };

#if I2C_ON_SCI_CALLBACK_USED_g_i2c_mainboard
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(i2c_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(i2c_callback_args_t * p_args);
void NULL_internal(i2c_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
sf_i2c_ctrl_t g_sf_i2c_device_mainboard_ctrl;
const sf_i2c_cfg_t g_sf_i2c_device_mainboard_cfg =
{ .p_bus = (sf_i2c_bus_t *) &g_sf_i2c_bus, .p_lower_lvl_cfg = &g_i2c_mainboard_cfg, };
/* Instance structure to use this module. */
const sf_i2c_instance_t g_sf_i2c_device_mainboard =
{ .p_ctrl = &g_sf_i2c_device_mainboard_ctrl, .p_cfg = &g_sf_i2c_device_mainboard_cfg, .p_api = &g_sf_i2c_on_sf_i2c };
void sf_i2c_pcal1_thread_create(void)
{
    /* Initialize each kernel object. */

    tx_thread_create (&sf_i2c_pcal1_thread, (CHAR *) "I2C PCAL1 demo thread", sf_i2c_pcal1_thread_func, (ULONG) NULL,
                      &sf_i2c_pcal1_thread_stack, 1024, 12, 12, 1, TX_DONT_START);
}

static void sf_i2c_pcal1_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    sf_i2c_pcal1_thread_entry ();
}
