#include "hal_data.h"
#include "Timer.h"

ssp_err_t TimerInitialize(void)
{
    ssp_err_t error;

    // Open the timer using the configured options from the configurator
    error = g_timer.p_api->open (g_timer.p_ctrl, g_timer.p_cfg);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    return SSP_SUCCESS;
}

ssp_err_t TimerSleepMs(uint32_t milliseconds)
{
    timer_size_t timerValue;
    uint32_t clockFrequency;
    uint32_t ticksPerMillisecond;
    ssp_err_t error;

    error = g_cgc_on_cgc.systemClockFreqGet (CGC_SYSTEM_CLOCKS_PCLKA, &clockFrequency);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    ticksPerMillisecond = clockFrequency / 1000;

    error = g_timer.p_api->reset (g_timer.p_ctrl);
    if (error != SSP_SUCCESS)
    {
        return error;
    }

    while (true)
    {
        error = g_timer.p_api->counterGet (g_timer.p_ctrl, &timerValue);
        if (error != SSP_SUCCESS)
        {
            return error;
        }

        if (timerValue >= (ticksPerMillisecond * milliseconds))
        {
            break;
        }
    }

    return SSP_SUCCESS;
}
