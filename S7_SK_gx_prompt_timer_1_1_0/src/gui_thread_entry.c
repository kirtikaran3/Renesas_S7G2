#include "gui_thread.h"
#include "lcd_setup/lcd_setup.h"

#include "guix_gen/gxs_specifications.h"
#include "guix_gen/gxs_resources.h"

#define APP_ERR_TRAP(a) if(a) while(1);

void gui_thread_entry(void);

GX_WINDOW_ROOT * p_window_root = NULL;

static UINT    val;
static GX_CHAR txt_buffer[4];

/* GUI Thread entry function */
void gui_thread_entry(void)
{
    UINT        status;
    GX_PROMPT * p_prompt;


    // Variable to hold ADC Data
       float adcCounts;
      float adcVoltage;

      // Tick Rate
      #define COUNTS_PER_MILLISECOND  (120E6 / 1000)

      #define POT_CHANNEL 0
      #define TEMP_CHANNEL 28
      #define INTERNAL_VREF_CHANNEL 29



      status = gx_system_initialize();
    APP_ERR_TRAP(status)

    status = g_sf_el_gx.p_api->open(g_sf_el_gx.p_ctrl, g_sf_el_gx.p_cfg);
    APP_ERR_TRAP(status)

    status = gx_studio_display_configure(0, g_sf_el_gx.p_api->setup, 0, 0, &p_window_root);
    APP_ERR_TRAP(status)

    status = g_sf_el_gx.p_api->canvasInit(g_sf_el_gx.p_ctrl, p_window_root);
    APP_ERR_TRAP(status)

    status = gx_studio_named_widget_create("w_main", (GX_WIDGET *) p_window_root, GX_NULL);
    APP_ERR_TRAP(status)

    status = gx_widget_show(p_window_root);
    APP_ERR_TRAP(status)

    status = gx_system_start();
    APP_ERR_TRAP(status)

#ifdef BSP_BOARD_S7G2_SK
    /** Setup the ILI9341V **/
    status = ILI9341V_Init(&g_spi_lcd);
    APP_ERR_TRAP(status)
#endif

    /* Set the prompt pointer to txt_counter widget */
    status = gx_system_widget_find(ID_TXT_COUNTER, GX_SEARCH_DEPTH_INFINITE, (GX_WIDGET **) &p_prompt);
    if (status)
        while (1);


         while(1)
    {

             g_adc0.p_api->open (g_adc0.p_ctrl, g_adc0.p_cfg);

                  // Configure Scan
                  g_adc0.p_api->scanCfg (g_adc0.p_ctrl, g_adc0.p_channel_cfg);

                  // Work around for temperature reading in Scan Mode. Thanks Josh! (http://en-eu.knowledgebase.renesas.com/English_Content/Renesas_Synergy%E2%84%A2_Platform/Renesas_Synergy_Knowledge_Base/ADC_Temperature_Sensor_Issue-_S7G2)
                  adc_sample_state_t tempsensor_sample_state = {ADC_SAMPLE_STATE_TEMPERATURE, 0x80};
                  g_adc0.p_api->sampleStateCountSet(g_adc0.p_ctrl, &tempsensor_sample_state);

                  // Start ADC Scan
                  g_adc0.p_api->scanStart (g_adc0.p_ctrl);


                 g_adc0.p_api->read (g_adc0.p_ctrl, POT_CHANNEL, &adcCounts);
                //g_adc0.p_api->read (g_adc0.p_ctrl, TEMP_CHANNEL, &temperatureCounts);
                       // g_adc0.p_api->read (g_adc0.p_ctrl, INTERNAL_VREF_CHANNEL, &internalVRefCounts);

                        // Convert Counts to Voltage
                        adcVoltage = ((adcCounts * 3.3f) / 4095.0f) + 10 ;


        /* Convert current counter value to string */
        gx_utility_ltoa((LONG) val, txt_buffer, 4);

        /* Let GUIX know that txt_counter should be re-drawn */
        status = gx_system_dirty_mark((GX_WIDGET *) p_prompt);
        if (status)
            while (1);

        /* Force re-draw all "dirty" widgets */
        gx_system_canvas_refresh();


        /* Attach txt_buffer to txt_counter widget */
               val = adcCounts;

               tx_thread_sleep(100);

               val = adcVoltage;

               gx_utility_ltoa((LONG) val, txt_buffer, 4);
        status = gx_prompt_text_set(p_prompt, txt_buffer);
        if (status)
            while (1);

        tx_thread_sleep(100);

    }
}
