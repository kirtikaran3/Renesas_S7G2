/* generated thread source file - do not edit */
#include "sf_thread_monitor_demo_thread.h"

TX_THREAD sf_thread_monitor_demo_thread;
void sf_thread_monitor_demo_thread_create(void);
static void sf_thread_monitor_demo_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t sf_thread_monitor_demo_thread_stack[1024] BSP_PLACE_IN_SECTION(".stack.sf_thread_monitor_demo_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
void sf_thread_monitor_demo_thread_create(void)
{
    /* Initialize each kernel object. */

    tx_thread_create (&sf_thread_monitor_demo_thread, (CHAR *) "Thread monitor demo thread",
                      sf_thread_monitor_demo_thread_func, (ULONG) NULL, &sf_thread_monitor_demo_thread_stack, 1024, 11,
                      11, 1, TX_DONT_START);
}

static void sf_thread_monitor_demo_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    sf_thread_monitor_demo_thread_entry ();
}
