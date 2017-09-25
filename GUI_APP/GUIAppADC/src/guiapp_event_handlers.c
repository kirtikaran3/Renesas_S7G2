

#include "main_thread.h"
#include "gx_api.h"
#include "gui/gui_adc_specifications.h"
#include "gui/gui_adc_resources.h"

#include "lcd_setup/lcd.h"

GX_WINDOW_ROOT                   * p_window_root;
extern GX_CONST GX_STUDIO_WIDGET * gui_adc_widget_table[];

adc_data_size_t                  adc_data;
bool                             flag = false; //flag to notify the ADC result is updated

/* New Thread entry function */
void main_thread_entry (void)
{
    ssp_err_t err;

    UINT      status = TX_SUCCESS;

    /* Initializes GUIX. */
    status = gx_system_initialize();
    if (TX_SUCCESS != status)
    {
        while (1)
        {
        }
    }

    /* Initializes GUIX drivers. */
    err = g_sf_el_gx.p_api->open(g_sf_el_gx.p_ctrl, g_sf_el_gx.p_cfg);
    if (SSP_SUCCESS != err)
    {
        while (1)
        {
        }
    }

    gx_studio_display_configure(DISPLAY_1,
                                g_sf_el_gx.p_api->setup,
                                LANGUAGE_ENGLISH,
                                DISPLAY_1_THEME_1,
                                &p_window_root);

    err = g_sf_el_gx.p_api->canvasInit(g_sf_el_gx.p_ctrl, p_window_root);
    if (SSP_SUCCESS != err)
    {
        while (1)
        {
        }
    }

    GX_CONST GX_STUDIO_WIDGET ** pp_studio_widget = &gui_adc_widget_table[0];
    GX_WIDGET                 * p_first_screen    = NULL;

    while (GX_NULL != *pp_studio_widget)
    {
        // We must first create the widgets according the data generated in GUIX Studio.

        // Once we are working on the widget we want to see first, save the pointer for later.
        if (0 == strcmp("window1", (char *) (*pp_studio_widget)->widget_name))
        {
            gx_studio_named_widget_create((*pp_studio_widget)->widget_name, (GX_WIDGET *) p_window_root, GX_NULL);
        }
        else
        {
            gx_studio_named_widget_create((*pp_studio_widget)->widget_name, GX_NULL, GX_NULL);
        }

        // Move to next top-level widget
        pp_studio_widget++;
    }

    // Attach the first screen to the root so we can see it when the root is shown
    gx_widget_attach(p_window_root, p_first_screen);

    if (TX_SUCCESS != status)
    {
        while (1)
        {
        }
    }

    /* Shows the root window to make it and patients screen visible. */
    status = gx_widget_show(p_window_root);
    if (TX_SUCCESS != status)
    {
        while (1)
        {
        }
    }

    /* Lets GUIX run. */
    status = gx_system_start();
    if (TX_SUCCESS != status)
    {
        while (1)
        {
        }
    }

    /** Open the SPI driver to initialize the LCD (SK-S7G2) **/
    err = g_spi_lcdc.p_api->open(g_spi_lcdc.p_ctrl, g_spi_lcdc.p_cfg);
    if (err)
    {
        while (1)
        {
        }
    }

    /** Setup the ILI9341V (SK-S7G2) **/
    ILI9341V_Init();

    flag = false;            //flag to notify the ADC result is updated

    g_adc.p_api->open(g_adc.p_ctrl, g_adc.p_cfg);

    //Configure ADC Channel
    g_adc.p_api->scanCfg(g_adc.p_ctrl, g_adc.p_channel_cfg);

    while (1)
    {
        //Start the ADC scan
        g_adc.p_api->scanStart(g_adc.p_ctrl);

        //Check scan status
        err = g_adc.p_api->scanStatusGet(g_adc.p_ctrl);

        if (SSP_SUCCESS == err)
        {
            // Stop the scan
            g_adc.p_api->scanStop(g_adc.p_ctrl);
        }

        if (flag == true)
        {
            char text[8];
            gx_utility_ltoa((LONG) adc_data, text, 8);
            // Change the prompt text
            status = gx_prompt_text_set(&window1.window1_prompt, text);

            flag   = false;

            // Redraw the prompt
            gx_system_dirty_mark((GX_WIDGET *) &window1.window1_prompt);
            gx_system_canvas_refresh();

            tx_thread_sleep(100);
        }
    }

    /* TODO: add your own code here */
    while (1)
    {
        tx_thread_sleep(1);
    }
}

void g_lcd_spi_callback (spi_callback_args_t * p_args)
{
    if (p_args->event == SPI_EVENT_TRANSFER_COMPLETE)
    {
        tx_semaphore_ceiling_put(&g_main_semaphore_lcdc, 1);
    }
}

void g_adc_callback (adc_callback_args_t * p_args)
{
    //Read the result
    g_adc.p_api->read(g_adc.p_ctrl, ADC_REG_TEMPERATURE, &adc_data);
    flag = true;
}
