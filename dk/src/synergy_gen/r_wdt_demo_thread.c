/* generated thread source file - do not edit */
#include "r_wdt_demo_thread.h"

TX_THREAD r_wdt_demo_thread;
void r_wdt_demo_thread_create(void);
static void r_wdt_demo_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t r_wdt_demo_thread_stack[1024] BSP_PLACE_IN_SECTION(".stack.r_wdt_demo_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
void r_wdt_demo_thread_create(void)
{
    /* Initialize each kernel object. */

    tx_thread_create (&r_wdt_demo_thread, (CHAR *) "WDT HAL demo thread", r_wdt_demo_thread_func, (ULONG) NULL,
                      &r_wdt_demo_thread_stack, 1024, 12, 12, 1, TX_DONT_START);
}

static void r_wdt_demo_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    r_wdt_demo_thread_entry ();
}
