/* generated thread source file - do not edit */
#include "blinky_thread.h"

TX_THREAD blinky_thread;
void blinky_thread_create(void);
static void blinky_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t blinky_thread_stack[1024] BSP_PLACE_IN_SECTION(".stack.blinky_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
void blinky_thread_create(void)
{
    /* Initialize each kernel object. */

    tx_thread_create (&blinky_thread, (CHAR *) "Blinky Thread", blinky_thread_func, (ULONG) NULL, &blinky_thread_stack,
                      1024, 1, 1, 1, TX_AUTO_START);
}

static void blinky_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    blinky_thread_entry ();
}
