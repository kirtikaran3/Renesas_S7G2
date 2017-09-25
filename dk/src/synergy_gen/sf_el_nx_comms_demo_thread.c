/* generated thread source file - do not edit */
#include "sf_el_nx_comms_demo_thread.h"

TX_THREAD sf_el_nx_comms_demo_thread;
void sf_el_nx_comms_demo_thread_create(void);
static void sf_el_nx_comms_demo_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t sf_el_nx_comms_demo_thread_stack[1024] BSP_PLACE_IN_SECTION(".stack.sf_el_nx_comms_demo_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
void sf_el_nx_comms_demo_thread_create(void)
{
    /* Initialize each kernel object. */

    tx_thread_create (&sf_el_nx_comms_demo_thread, (CHAR *) "NetX comms demo thread", sf_el_nx_comms_demo_thread_func,
                      (ULONG) NULL, &sf_el_nx_comms_demo_thread_stack, 1024, 12, 12, 1, TX_DONT_START);
}

static void sf_el_nx_comms_demo_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    sf_el_nx_comms_demo_thread_entry ();
}
