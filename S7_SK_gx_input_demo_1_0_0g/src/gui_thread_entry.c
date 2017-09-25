#include "lcd_setup/lcd_setup.h"

#include "application_define.h"

#include "guix_gen/gx_studio_project_resources.h"
#include "guix_gen/gx_studio_project_specifications.h"

void gui_thread_entry(void);

GX_WINDOW_ROOT *p_window_root = NULL;

void g_lcd_spi_callback(spi_callback_args_t * p_args)
{
    SSP_PARAMETER_NOT_USED(p_args);
    tx_semaphore_ceiling_put(&g_lcd_semaphore, 1);
}

/* GUI Thread entry function */
void gui_thread_entry(void)
{
    UINT status;

    status = gx_system_initialize();
    APP_ERROR_TRAP(status)

    status = g_sf_el_gx.p_api->open(g_sf_el_gx.p_ctrl, g_sf_el_gx.p_cfg);
    APP_ERROR_TRAP(status)

    status = gx_studio_display_configure(0, g_sf_el_gx.p_api->setup, 0, 0, &p_window_root);
    APP_ERROR_TRAP(status)

    status = g_sf_el_gx.p_api->canvasInit(g_sf_el_gx.p_ctrl, p_window_root);
    APP_ERROR_TRAP(status)

    status = gx_studio_named_widget_create("w_main", (GX_WIDGET *)p_window_root, GX_NULL);
    APP_ERROR_TRAP(status)

    status = gx_widget_show(p_window_root);
    APP_ERROR_TRAP(status)

    status = gx_system_start();
    APP_ERROR_TRAP(status)

    /** Open the SPI driver to initialize the LCD **/
    status = g_lcd_spi.p_api->open(g_lcd_spi.p_ctrl, (spi_cfg_t *)g_lcd_spi.p_cfg);
    APP_ERROR_TRAP(status)

    /** Setup the ILI9341V **/
    ILI9341V_Init();

    tx_thread_sleep(5);

    status = tx_thread_resume(&input_thread);
    APP_ERROR_TRAP(status)

    while (1)
    {
        tx_thread_sleep(TX_WAIT_FOREVER);
    }
}
