#include "application_define.h"

void input_thread_entry(void);

/* Input Thread entry function */
void input_thread_entry(void)
{
    UINT status;

    status = g_sw4_irq.p_api->open(g_sw4_irq.p_ctrl, g_sw4_irq.p_cfg);
    APP_ERROR_TRAP(status)

    status = g_sw5_irq.p_api->open(g_sw5_irq.p_ctrl, g_sw5_irq.p_cfg);
    APP_ERROR_TRAP(status)

    while (1)
    {
        tx_thread_sleep (1);
    }
}

void g_sw4_irq_callback(external_irq_callback_args_t * p_args)
{
    GX_EVENT gxe = {0, 0, 0, 0, 0};

    gxe.gx_event_type = APP_EVENT_SW4_PRESS;
    gx_system_event_send(&gxe);
}

void g_sw5_irq_callback(external_irq_callback_args_t * p_args)
{
    GX_EVENT gxe = {0, 0, 0, 0, 0};

    gxe.gx_event_type = APP_EVENT_SW5_PRESS;
    gx_system_event_send(&gxe);
}
