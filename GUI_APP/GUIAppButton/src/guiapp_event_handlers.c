

#include "gui/guiapp_resources.h"
#include "gui/guiapp_specifications.h"

#include "main_thread.h"
#include "hal_data.h"

static bool button_enabled = false;

extern GX_WINDOW_ROOT * p_window_root;

static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old);
static void update_text_id(GX_WIDGET * p_widget, GX_RESOURCE_ID id, UINT string_id);

bool ledon = true;
bool ledoff = false;



UINT window1_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process(widget, event_ptr);

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_BUTTONENABLER, GX_EVENT_TOGGLE_ON):
        button_enabled = true;
        update_text_id(widget->gx_widget_parent, ID_WINDOWCHANGER, GX_STRING_ID_BUTTON_ENABLED);
        update_text_id(widget->gx_widget_parent, ID_INSTRUCTIONS, GX_STRING_ID_INSTRUCT_BUTTON);
        break;
    case GX_SIGNAL(ID_BUTTONENABLER, GX_EVENT_TOGGLE_OFF):
        button_enabled = false;
        update_text_id(widget->gx_widget_parent, ID_WINDOWCHANGER, GX_STRING_ID_BUTTON_DISABLED);
        update_text_id(widget->gx_widget_parent, ID_INSTRUCTIONS, GX_STRING_ID_INSTRUCT_CHECKBOX);
        break;
    case GX_SIGNAL(ID_WINDOWCHANGER, GX_EVENT_CLICKED):
        if(button_enabled){
            show_window((GX_WINDOW*)&window2, (GX_WIDGET*)widget, true);
        }
        break;
    default:
        gx_window_event_process(widget, event_ptr);
        break;
    }

    return result;
}

UINT window2_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process(widget, event_ptr);

    switch (event_ptr->gx_event_type){
        case GX_EVENT_PEN_UP:
            show_window((GX_WINDOW*)&window3, (GX_WIDGET*)widget, true);
            break;
        default:
            result = gx_window_event_process(widget, event_ptr);
            break;
    }
    return result;
}


UINT window3_handler(GX_WINDOW *widget,GX_EVENT *event_ptr){

    UINT result =  gx_window_event_process(widget,event_ptr);



    switch(event_ptr->gx_event_type){

        case GX_SIGNAL(ID_RED, GX_EVENT_TOGGLE_ON):

               g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00,0);
               g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01,1);
               g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02,0);
               break;

        case GX_SIGNAL(ID_RED,GX_EVENT_TOGGLE_OFF):

               g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00,0);
               g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01,0);
               g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02,0);
               break;

        case GX_SIGNAL(ID_GREEN,GX_EVENT_TOGGLE_ON):

                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01,1);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00,0);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02,0);
                break;

        case GX_SIGNAL(ID_GREEN,GX_EVENT_TOGGLE_OFF):

                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00,0);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01,0);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02,0);
               break;

        case GX_SIGNAL(ID_YELLOW,GX_EVENT_TOGGLE_ON):

                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02,0);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00,0);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01,1);
                break;

        case GX_SIGNAL(ID_YELLOW,GX_EVENT_TOGGLE_OFF):

                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00,0);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01,0);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02,0);
                break;

        case GX_SIGNAL(ID_CONTROLPANEL,GX_EVENT_CLICKED):

                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00,0);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01,0);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02,0);

                break;
        case GX_SIGNAL(ID_COLORS,GX_EVENT_CLICKED):
                        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00,0);
                        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01,0);
                        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02,0);
                        for(int i=0;i<1E6;i++);
                        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00,1);
                        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01,1);
                        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02,1);
                        for(int i=0;i<1E6;i++);

                break;
        default:

                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00,ledoff);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01,ledoff);
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02,ledoff);
                result = gx_window_event_process(widget,event_ptr);

            break;

    }



    return result;

}

static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old)
{
    UINT err = GX_SUCCESS;

    if (!p_new->gx_widget_parent)
    {
        err = gx_widget_attach(p_window_root, p_new);
    }
    else
    {
        err = gx_widget_show(p_new);
    }

    gx_system_focus_claim(p_new);

    GX_WIDGET * p_old = p_widget;
    if (p_old && detach_old)
    {
        if (p_old != (GX_WIDGET*)p_new)
        {
            gx_widget_detach(p_old);
        }
    }

    return err;
}

static void update_text_id(GX_WIDGET * p_widget, GX_RESOURCE_ID id, UINT string_id)
{
   // string_id = "Do it";
    GX_PROMPT * p_prompt = NULL;

    ssp_err_t err = gx_widget_find(p_widget, id, GX_SEARCH_DEPTH_INFINITE, (GX_WIDGET**)&p_prompt);
    if (TX_SUCCESS == err)
    {
        gx_prompt_text_id_set(p_prompt, string_id);
    }
}

