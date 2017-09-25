/*******************************************************************************/
/*  This file is auto-generated by GUIX Studio. Do not edit this file by hand. */
/*  Modifications to this file should only be made by running the GUIX Studio  */
/*  application and re-generating the application. For more information please */
/*  refer to the GUIX Studio user's guide, or visit our web site at            */
/*  www.expresslogic.com.                                                      */
/*                                                                             */
/*  GUIX Studio Revision 5.0                                                   */
/*  Date (dd.mm.yyyy): 18. 3.2016   Time (hh:mm): 16:48                        */
/*******************************************************************************/


#define GUIX_STUDIO_GENERATED_FILE
#include "gx_studio_project_resources.h"
#include "gx_studio_project_specifications.h"

W_MAIN_CONTROL_BLOCK w_main;

UINT gx_studio_icon_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent)
{
    UINT status;
    GX_ICON *icon = (GX_ICON *) control_block;
    GX_ICON_PROPERTIES *props = (GX_ICON_PROPERTIES *) info->properties;
    status = gx_icon_create(icon, info->widget_name, parent, props->normal_pixelmap_id, info->style, info->widget_id, info->size.gx_rectangle_left, info->size.gx_rectangle_top);
    if (props->selected_pixelmap_id)
    {
        gx_icon_pixelmap_set(icon, props->normal_pixelmap_id, props->selected_pixelmap_id);
    }
    return status;
}

UINT gx_studio_progress_bar_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent)
{
    UINT status;
    GX_PROGRESS_BAR *bar = (GX_PROGRESS_BAR *) control_block;
    GX_PROGRESS_BAR_INFO *bar_info = (GX_PROGRESS_BAR_INFO *) info->properties;
    status = gx_progress_bar_create(bar,
                    info->widget_name,
                    parent,
                    bar_info,
                    info->style,
                    info->widget_id,
                    &info->size);
    return status;
}

UINT gx_studio_prompt_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent)
{
    UINT status;
    GX_PROMPT *prompt = (GX_PROMPT *) control_block;
    GX_PROMPT_PROPERTIES *props = (GX_PROMPT_PROPERTIES *) info->properties;
    status = gx_prompt_create(prompt, info->widget_name, parent, props->string_id, info->style, info->widget_id, &info->size);
    if (status == GX_SUCCESS)
    {
        gx_prompt_font_set(prompt, props->font_id);
        gx_prompt_text_color_set(prompt, props->normal_text_color_id, props->selected_text_color_id);
    }
    return status;
}

UINT gx_studio_window_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent)
{
    UINT status;
    GX_WINDOW *window = (GX_WINDOW *) control_block;
    GX_WINDOW_PROPERTIES *props = (GX_WINDOW_PROPERTIES *) info->properties;
    status = gx_window_create(window, info->widget_name, parent, info->style, info->widget_id, &info->size);
    if (status == GX_SUCCESS)
    {
        if (props->wallpaper_id)
        {
            gx_window_wallpaper_set(window, props->wallpaper_id, info->style & GX_STYLE_TILE_WALLPAPER);
        }
    }
    return status;
}
GX_WINDOW_PROPERTIES w_main_properties =
{
    GX_PIXELMAP_ID_BG                        /* wallpaper pixelmap id          */
};
GX_ICON_PROPERTIES w_main_ico_logo_properties =
{
    GX_PIXELMAP_ID_ICO_LOGO,                 /* normal pixelmap id             */
    0                                        /* selected pixelmap id           */
};
GX_PROGRESS_BAR_INFO w_main_bar_temp_properties =
{
    150,                                     /* mimimun value                  */
    350,                                     /* maximum value                  */
    250,                                     /* current value                  */
    0,                                       /* font_id                        */
    5,                                       /* normal text color              */
    6,                                       /* selected text color            */
    7                                        /* fill pixelmap                  */
};
GX_PROGRESS_BAR_INFO w_main_bar_buttons_properties =
{
    0,                                       /* mimimun value                  */
    20,                                      /* maximum value                  */
    10,                                      /* current value                  */
    0,                                       /* font_id                        */
    5,                                       /* normal text color              */
    6,                                       /* selected text color            */
    8                                        /* fill pixelmap                  */
};
GX_PROMPT_PROPERTIES w_main_label_1_properties =
{
    GX_STRING_ID_STRING_TEMP1,               /* string id                      */
    GX_FONT_ID_SMALL,                        /* font id                        */
    GX_COLOR_ID_SELECTED_TEXT,               /* normal text color              */
    GX_COLOR_ID_SELECTED_TEXT                /* selected text color            */
};
GX_PROMPT_PROPERTIES w_main_label_2_properties =
{
    GX_STRING_ID_STRING_TEMP2,               /* string id                      */
    GX_FONT_ID_SMALL,                        /* font id                        */
    GX_COLOR_ID_SELECTED_TEXT,               /* normal text color              */
    GX_COLOR_ID_SELECTED_TEXT                /* selected text color            */
};
GX_PROMPT_PROPERTIES w_main_label_3_properties =
{
    GX_STRING_ID_STRING_BTN1,                /* string id                      */
    GX_FONT_ID_SMALL,                        /* font id                        */
    GX_COLOR_ID_SELECTED_TEXT,               /* normal text color              */
    GX_COLOR_ID_SELECTED_TEXT                /* selected text color            */
};
GX_PROMPT_PROPERTIES w_main_label_4_properties =
{
    GX_STRING_ID_STRING_BTN2,                /* string id                      */
    GX_FONT_ID_SMALL,                        /* font id                        */
    GX_COLOR_ID_SELECTED_TEXT,               /* normal text color              */
    GX_COLOR_ID_SELECTED_TEXT                /* selected text color            */
};

GX_CONST GX_STUDIO_WIDGET w_main_label_4_define =
{
    "label_4",
    GX_TYPE_PROMPT,                          /* widget type                    */
    GX_ID_NONE,                              /* widget id                      */
    #if defined(GX_WIDGET_USER_DATA)
    0,                                       /* user data                      */
    #endif
    GX_STYLE_BORDER_NONE|GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_CENTER,   /* style flags */
    0,                                       /* status flags                   */
    116,                                     /* control block size             */
    GX_COLOR_ID_WIDGET_FILL,                 /* normal color id                */
    GX_COLOR_ID_SELECTED_FILL,               /* selected color id              */
    gx_studio_prompt_create,                 /* create function                */
    GX_NULL,                                 /* drawing function override      */
    GX_NULL,                                 /* event function override        */
    {130, 295, 209, 309},                    /* widget size                    */
    GX_NULL,                                 /* no next widget                 */
    GX_NULL,                                 /* no child widgets               */ 
    (GX_WIDGET *) &w_main.w_main_label_4,    /* control block                  */
    (void *) &w_main_label_4_properties      /* extended properties            */
};

GX_CONST GX_STUDIO_WIDGET w_main_label_3_define =
{
    "label_3",
    GX_TYPE_PROMPT,                          /* widget type                    */
    GX_ID_NONE,                              /* widget id                      */
    #if defined(GX_WIDGET_USER_DATA)
    0,                                       /* user data                      */
    #endif
    GX_STYLE_BORDER_NONE|GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_CENTER,   /* style flags */
    0,                                       /* status flags                   */
    116,                                     /* control block size             */
    GX_COLOR_ID_WIDGET_FILL,                 /* normal color id                */
    GX_COLOR_ID_SELECTED_FILL,               /* selected color id              */
    gx_studio_prompt_create,                 /* create function                */
    GX_NULL,                                 /* drawing function override      */
    GX_NULL,                                 /* event function override        */
    {130, 280, 209, 294},                    /* widget size                    */
    &w_main_label_4_define,                  /* next widget definition         */
    GX_NULL,                                 /* no child widgets               */ 
    (GX_WIDGET *) &w_main.w_main_label_3,    /* control block                  */
    (void *) &w_main_label_3_properties      /* extended properties            */
};

GX_CONST GX_STUDIO_WIDGET w_main_label_2_define =
{
    "label_2",
    GX_TYPE_PROMPT,                          /* widget type                    */
    GX_ID_NONE,                              /* widget id                      */
    #if defined(GX_WIDGET_USER_DATA)
    0,                                       /* user data                      */
    #endif
    GX_STYLE_BORDER_NONE|GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_CENTER,   /* style flags */
    0,                                       /* status flags                   */
    116,                                     /* control block size             */
    GX_COLOR_ID_WIDGET_FILL,                 /* normal color id                */
    GX_COLOR_ID_SELECTED_FILL,               /* selected color id              */
    gx_studio_prompt_create,                 /* create function                */
    GX_NULL,                                 /* drawing function override      */
    GX_NULL,                                 /* event function override        */
    {30, 295, 109, 309},                     /* widget size                    */
    &w_main_label_3_define,                  /* next widget definition         */
    GX_NULL,                                 /* no child widgets               */ 
    (GX_WIDGET *) &w_main.w_main_label_2,    /* control block                  */
    (void *) &w_main_label_2_properties      /* extended properties            */
};

GX_CONST GX_STUDIO_WIDGET w_main_label_1_define =
{
    "label_1",
    GX_TYPE_PROMPT,                          /* widget type                    */
    GX_ID_NONE,                              /* widget id                      */
    #if defined(GX_WIDGET_USER_DATA)
    0,                                       /* user data                      */
    #endif
    GX_STYLE_BORDER_NONE|GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_CENTER,   /* style flags */
    0,                                       /* status flags                   */
    116,                                     /* control block size             */
    GX_COLOR_ID_WIDGET_FILL,                 /* normal color id                */
    GX_COLOR_ID_SELECTED_FILL,               /* selected color id              */
    gx_studio_prompt_create,                 /* create function                */
    GX_NULL,                                 /* drawing function override      */
    GX_NULL,                                 /* event function override        */
    {30, 280, 109, 294},                     /* widget size                    */
    &w_main_label_2_define,                  /* next widget definition         */
    GX_NULL,                                 /* no child widgets               */ 
    (GX_WIDGET *) &w_main.w_main_label_1,    /* control block                  */
    (void *) &w_main_label_1_properties      /* extended properties            */
};

GX_CONST GX_STUDIO_WIDGET w_main_bar_buttons_define =
{
    "bar_buttons",
    GX_TYPE_PROGRESS_BAR,                    /* widget type                    */
    GX_ID_NONE,                              /* widget id                      */
    #if defined(GX_WIDGET_USER_DATA)
    0,                                       /* user data                      */
    #endif
    GX_STYLE_BORDER_THIN|GX_STYLE_TRANSPARENT|GX_STYLE_PROGRESS_VERTICAL,   /* style flags */
    0,                                       /* status flags                   */
    120,                                     /* control block size             */
    GX_COLOR_ID_WIDGET_FILL,                 /* normal color id                */
    GX_COLOR_ID_SELECTED_FILL,               /* selected color id              */
    gx_studio_progress_bar_create,           /* create function                */
    GX_NULL,                                 /* drawing function override      */
    GX_NULL,                                 /* event function override        */
    {150, 55, 189, 274},                     /* widget size                    */
    &w_main_label_1_define,                  /* next widget definition         */
    GX_NULL,                                 /* no child widgets               */ 
    (GX_WIDGET *) &w_main.w_main_bar_buttons, /* control block                 */
    (void *) &w_main_bar_buttons_properties  /* extended properties            */
};

GX_CONST GX_STUDIO_WIDGET w_main_bar_temp_define =
{
    "bar_temp",
    GX_TYPE_PROGRESS_BAR,                    /* widget type                    */
    GX_ID_NONE,                              /* widget id                      */
    #if defined(GX_WIDGET_USER_DATA)
    0,                                       /* user data                      */
    #endif
    GX_STYLE_BORDER_THIN|GX_STYLE_TRANSPARENT|GX_STYLE_PROGRESS_VERTICAL,   /* style flags */
    0,                                       /* status flags                   */
    120,                                     /* control block size             */
    GX_COLOR_ID_WIDGET_FILL,                 /* normal color id                */
    GX_COLOR_ID_SELECTED_FILL,               /* selected color id              */
    gx_studio_progress_bar_create,           /* create function                */
    GX_NULL,                                 /* drawing function override      */
    GX_NULL,                                 /* event function override        */
    {50, 55, 89, 274},                       /* widget size                    */
    &w_main_bar_buttons_define,              /* next widget definition         */
    GX_NULL,                                 /* no child widgets               */ 
    (GX_WIDGET *) &w_main.w_main_bar_temp,   /* control block                  */
    (void *) &w_main_bar_temp_properties     /* extended properties            */
};

GX_CONST GX_STUDIO_WIDGET w_main_ico_logo_define =
{
    "ico_logo",
    GX_TYPE_ICON,                            /* widget type                    */
    GX_ID_NONE,                              /* widget id                      */
    #if defined(GX_WIDGET_USER_DATA)
    0,                                       /* user data                      */
    #endif
    GX_STYLE_BORDER_NONE|GX_STYLE_TRANSPARENT,   /* style flags                */
    0,                                       /* status flags                   */
    100,                                     /* control block size             */
    GX_COLOR_ID_WIDGET_FILL,                 /* normal color id                */
    GX_COLOR_ID_SELECTED_FILL,               /* selected color id              */
    gx_studio_icon_create,                   /* create function                */
    GX_NULL,                                 /* drawing function override      */
    GX_NULL,                                 /* event function override        */
    {0, 5, 239, 44},                         /* widget size                    */
    &w_main_bar_temp_define,                 /* next widget definition         */
    GX_NULL,                                 /* no child widgets               */ 
    (GX_WIDGET *) &w_main.w_main_ico_logo,   /* control block                  */
    (void *) &w_main_ico_logo_properties     /* extended properties            */
};

GX_CONST GX_STUDIO_WIDGET w_main_define =
{
    "w_main",
    GX_TYPE_WINDOW,                          /* widget type                    */
    GX_ID_NONE,                              /* widget id                      */
    #if defined(GX_WIDGET_USER_DATA)
    0,                                       /* user data                      */
    #endif
    GX_STYLE_BORDER_NONE|GX_STYLE_TRANSPARENT|GX_STYLE_ENABLED,   /* style flags */
    GX_STATUS_ACCEPTS_FOCUS,                 /* status flags                   */
    132,                                     /* control block size             */
    GX_COLOR_ID_SELECTED_FILL,               /* normal color id                */
    GX_COLOR_ID_SELECTED_FILL,               /* selected color id              */
    gx_studio_window_create,                 /* create function                */
    GX_NULL,                                 /* drawing function override      */
    (UINT (*)(GX_WIDGET *, GX_EVENT *)) w_main_event, /* event function override */
    {0, 0, 239, 319},                        /* widget size                    */
    GX_NULL,                                 /* next widget                    */
    &w_main_ico_logo_define,                 /* child widget                   */
    (GX_WIDGET *) &w_main,                   /* control block                  */
    (void *) &w_main_properties              /* extended properties            */
};
GX_CONST GX_STUDIO_WIDGET *gx_studio_project_widget_table[] =
{
    &w_main_define,
    GX_NULL
};

GX_WIDGET *gx_studio_widget_create(GX_CONST GX_STUDIO_WIDGET *definition, GX_WIDGET *parent)
{
    UINT status = GX_SUCCESS;
    GX_WIDGET *widget = GX_NULL;

    while(definition && status == GX_SUCCESS)
    {
        if (definition->create_function)
        {
            if (definition->style & GX_STYLE_DYNAMICALLY_ALLOCATED)
            {
                status = gx_widget_allocate(&widget, definition->control_block_size);
                if (status != GX_SUCCESS)
                {
                    return GX_NULL;
                }
            }
            else
            {
                widget = definition->control_block;
            }
            status = definition->create_function(definition, widget, parent);

            if (status == GX_SUCCESS)
            {
                gx_widget_fill_color_set(widget, definition->normal_fill_color_id, definition->selected_fill_color_id);

                if (!(definition->status & GX_STATUS_ACCEPTS_FOCUS))
                {
                    gx_widget_status_remove(widget, GX_STATUS_ACCEPTS_FOCUS);
                }

                if (definition->draw_function)
                {
                    gx_widget_draw_set(widget, definition->draw_function);
                }
                if (definition->event_function)
                {
                    gx_widget_event_process_set(widget, definition->event_function);
                }

                #if defined(GX_WIDGET_USER_DATA)
                widget->gx_widget_user_data = definition->user_data;
                #endif

                if (definition->child_widget)
                {
                    gx_studio_widget_create(definition->child_widget, widget);
                }
            }
        }
        definition = definition->next_widget;
    }
    return widget;
}

UINT gx_studio_named_widget_create(char *name, GX_WIDGET *parent, GX_WIDGET **new_widget)
{
    UINT status = GX_FAILURE;
    UINT index = 0;
    GX_CONST GX_STUDIO_WIDGET *entry;
    GX_WIDGET *control_block = GX_NULL;

    while(1)
    {
        entry =  gx_studio_project_widget_table[index];
        if (!entry)
        {
            break;
        }
        if (!strcmp(name, entry->widget_name))
        {
            control_block = gx_studio_widget_create(entry, parent);
            break;
        }
        index++;
    }

    if (new_widget)
    {
        *new_widget = control_block;
    }
    if (control_block != GX_NULL)
    {
        status = GX_SUCCESS;
    }

    return status;
}


GX_DISPLAY display_1_control_block;
GX_CANVAS  display_1_canvas_control_block;
GX_WINDOW_ROOT display_1_root_window;

GX_DISPLAY display_1_control_block;
GX_CANVAS  display_1_canvas_control_block;
GX_WINDOW_ROOT display_1_root_window;
extern GX_CONST GX_THEME *display_1_theme_table[];
extern GX_CONST CHAR **display_1_language_table[];

GX_STUDIO_DISPLAY_INFO gx_studio_project_display_table[1] =
{
    {
    "display_1",
    "display_1_canvas",
    display_1_theme_table,
    display_1_language_table,
    DISPLAY_1_LANGUAGE_TABLE_SIZE,
    DISPLAY_1_STRING_TABLE_SIZE,
    256,                                     /* x resolution                   */
    320,                                     /* y resolution                   */
    &display_1_control_block,
    &display_1_canvas_control_block,
    &display_1_root_window,
    GX_NULL,                                 /* canvas memory area             */
    163840                                   /* canvas memory size in bytes    */
    }
};

UINT gx_studio_display_configure(USHORT display, UINT (*driver)(GX_DISPLAY *),
    USHORT language, USHORT theme, GX_WINDOW_ROOT **return_root)
{
    GX_CONST GX_THEME *theme_ptr;
    GX_RECTANGLE size;

    GX_STUDIO_DISPLAY_INFO *display_info = &gx_studio_project_display_table[display];


/* create the requested display                                                */

    gx_display_create(display_info->display,
                      display_info->name,
                      driver,
                      display_info->x_resolution,
                      display_info->y_resolution);


/* install the request theme                                                   */

    theme_ptr = gx_studio_project_display_table[display].theme_table[theme];
    gx_display_color_table_set(display_info->display, theme_ptr->theme_color_table, theme_ptr->theme_color_table_size);

/* install the color palette if required                                       */
    if (display_info->display->gx_display_driver_palette_set &&
        theme_ptr->theme_palette != NULL)
    {
        display_info->display->gx_display_driver_palette_set(display_info->display, theme_ptr->theme_palette, theme_ptr->theme_palette_size);
    }

    gx_display_font_table_set(display_info->display, theme_ptr->theme_font_table, theme_ptr->theme_font_table_size);
    gx_display_pixelmap_table_set(display_info->display, theme_ptr->theme_pixelmap_table, theme_ptr->theme_pixelmap_table_size);
    gx_system_language_table_set((GX_CHAR ***) display_info->language_table, display_info->language_table_size, display_info->string_table_size);
    gx_system_active_language_set(language);
    gx_system_scroll_appearance_set(theme_ptr->theme_vertical_scroll_style, (GX_SCROLLBAR_APPEARANCE *) &theme_ptr->theme_vertical_scrollbar_appearance);
    gx_system_scroll_appearance_set(theme_ptr->theme_horizontal_scroll_style, (GX_SCROLLBAR_APPEARANCE *) &theme_ptr->theme_horizontal_scrollbar_appearance);

/* create the canvas for this display                                          */

    gx_canvas_create(display_info->canvas,
                     display_info->canvas_name,
                     display_info->display,
                     GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE,
                     display_info->x_resolution,
                     display_info->y_resolution,
                     display_info->canvas_memory,
                     display_info->canvas_memory_size);

/* Create the root window for this canvas                                      */

    gx_utility_rectangle_define(&size,
                                0, 0,
                                display_info->x_resolution - 1,
                                display_info->y_resolution - 1);

    gx_window_root_create(display_info->root_window,
                          display_info->name,
                          display_info->canvas, GX_STYLE_NONE, 0, &size);
    if (return_root)
    {
        *return_root = display_info->root_window;
    }
    return GX_SUCCESS;
}
#undef GUIX_STUDIO_GENERATED_FILE
