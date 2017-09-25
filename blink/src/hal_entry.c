/* HAL-only entry function */
#include "hal_data.h"

void hal_entry(void)
{
    bool isOn = true;
    while(1)
    {
        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, isOn);
        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, isOn);
        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02, isOn);
        isOn = !isOn;

        for (int i = 0; i < 1E6; i++);

    }
}
