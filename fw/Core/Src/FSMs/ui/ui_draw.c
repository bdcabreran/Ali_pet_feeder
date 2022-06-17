#include "ui_draw.h"

ui_battery_t ui_battery;
ui_thermostat_t ui_thermostat;
ui_drawers_t ui_drawers;
ui_date_time_t ui_date_time;
ui_thermostat_config_t ui_therm_conf;
ui_feeder_menu_t ui_feeder_menu;
ui_date_time_menu_t ui_date_time_menu;

typedef struct
{
    char *name;
    uint16_t x;
    uint16_t y;
} label_t;

typedef struct
{
    uint16_t x;
    uint16_t y;
} pos_t;

static const char *am_fm_str[TIMEn] = {"AM", "FM"}; 
static const char *c_f_str[TEMP_UNITSn] = {"°C", "°F"}; 


//////////////////////////////////// Static Common Functions  //////////////////////////////////////////////

static void ui_draw_window(ui_window_t *win, uint16_t color, bool show)
{
    if (show == true)
    {
        BSP_LCD_SetTextColor(color);
        BSP_LCD_DrawRect(win->x, win->y, win->w, win->h);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    }
    else
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_DrawRect(win->x, win->y, win->w, win->h);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    }
}

static void ui_clear_window(ui_window_t *win)
{
    BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
    BSP_LCD_FillRect(win->x, win->y, win->w, win->h);
    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
}

static void ui_draw_icon(ui_icon_t *icon)
{
    BSP_LCD_DrawBitmap(icon->x, icon->y, (uint8_t*)icon->ptr);
}

static void ui_draw_circle(ui_window_t *win, uint8_t radio, uint16_t color)
{
    BSP_LCD_SetTextColor(color);
    BSP_LCD_DrawCircle(win->x, win->y, radio);
    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
}

static void ui_fill_circle(ui_window_t *win, uint8_t radio, uint16_t color)
{
    BSP_LCD_SetTextColor(color);
    BSP_LCD_FillCircle(win->x, win->y, radio);
    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
}

static void ui_fill_window(ui_window_t *win, uint16_t color)
{
    BSP_LCD_SetTextColor(color);
    BSP_LCD_FillRect(win->x, win->y, win->w, win->h);
    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR); 
}

static void ui_display_string(ui_window_t *win, uint8_t *text, sFONT *font, uint16_t color)
{
    BSP_LCD_SetTextColor(color);
    BSP_LCD_SetFont(font);
    BSP_LCD_DisplayStringAt(win->x, win->y, text, LEFT_MODE);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_DEFAULT_BACKCOLOR);
}

//////////////////////////////////// Battery Icon Related Functions   //////////////////////////////////////////////
void ui_battery_icon_init(ui_battery_t *batt)
{
    /* Main window position - all other components will move according to this coordinate */
    batt->win.main.x = 366;
    batt->win.main.y = 10;
    batt->win.main.h = 81;
    batt->win.main.w = 96;

    batt->icon.batt.ptr = &battery_icon;
    batt->icon.batt.x = batt->win.main.x + 4;
    batt->icon.batt.y = batt->win.main.y + 16;

    batt->icon.error.ptr = &batt_error_icon;
    batt->icon.error.x = batt->icon.batt.x + 16;
    batt->icon.error.y = batt->icon.batt.y + 16;

    batt->icon.warn.ptr = &batt_warning_icon;
    batt->icon.warn.x = batt->icon.batt.x + 16;
    batt->icon.warn.y = batt->icon.batt.y + 16;

    batt->shape.charge.x = batt->icon.batt.x + 17;
    batt->shape.charge.y = batt->icon.batt.y + 8;
    batt->shape.charge.h = batt->icon.batt.ptr->infoHeader.biHeight - 16;
    batt->shape.charge.w = batt->icon.batt.ptr->infoHeader.biWidth - 24;

    batt->text.x = batt->shape.charge.x + 15;
    batt->text.y = batt->shape.charge.y + 10;
}

void ui_battery_icon_show(ui_battery_t *batt, bool show)
{
    if(show)
    {
        /*Display Battery Icon*/
        ui_draw_icon(&batt->icon.batt);
    }
    else
    {
        /*Clear battery screen section */
        ui_clear_window(&batt->win.main);
    }
}

static void ui_battery_draw_charge(ui_battery_t *batt, uint8_t batt_lvl)
{
    float width = batt->shape.charge.w * (batt_lvl / 100.0);
    uint8_t str[5] = {0};
    sprintf(str, "%d%%", batt_lvl);

    uint8_t temp_w = batt->shape.charge.w;
    batt->shape.charge.w = (int)width;
        
    /*Critical battery level */
    if(width > 0 && width < 7)
    {
        ui_fill_window(&batt->shape.charge, LCD_COLOR_RED);
        ui_display_string(&batt->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR);   
    }

    /*Low battery level */
    if(width >= 7  && width < 30)
    {
        ui_fill_window(&batt->shape.charge, LCD_COLOR_YELLOW);
        ui_display_string(&batt->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR); 
    }

    /*Medium battery level */
    if(width >= 30  && width < 70)
    {
        ui_fill_window(&batt->shape.charge, LCD_COLOR_YELLOW);
        ui_display_string(&batt->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR); 
    }

    /*High battery level */
    if(width >= 70  && width <= 100)
    {
        ui_fill_window(&batt->shape.charge, LCD_COLOR_GREEN);
        ui_display_string(&batt->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR); 
    }

    batt->shape.charge.w = temp_w;
}

void ui_battery_icon_set_config(ui_battery_t *batt, ui_battery_config *config)
{
    /*Paint select battery item property*/
    if (config->select == UI_ITEM_SELECT)
        ui_draw_window(&batt->win.main, UI_SELECTION_COLOR, true);
    else
        ui_draw_window(&batt->win.main, LCD_DEFAULT_BACKCOLOR, false);
    
    /*Clear charging info */
    ui_clear_window(&batt->shape.charge); 

    switch (config->set)
    {
        case BATT_ST_NO_DETECTED: { ui_draw_icon(&batt->icon.error); } break;
        case BATT_ST_LOW_BATTERY: { ui_draw_icon(&batt->icon.warn);  } break;
        case BATT_ST_CHARGING:    { ui_battery_draw_charge(batt, config->charge); } break;
    }
}

//////////////////////////////////// Drawer Box Related Functions    //////////////////////////////////////////////

void ui_drawers_init(ui_drawers_t *drawers)
{
    /*position in the screen, all the remaining components will move with it*/
    drawers->win.main.x = 15;
    drawers->win.main.y = 10;
    drawers->win.main.h = 81;
    drawers->win.main.w = 132;

    drawers->shape.main.x = drawers->win.main.x + 4;
    drawers->shape.main.y = drawers->win.main.y + 4;
    drawers->shape.main.h = drawers->win.main.h - 8;
    drawers->shape.main.w = drawers->win.main.w - 8;

    drawers->shape.single.w = 50;
    drawers->shape.single.h = 25;
}

void ui_drawers_show(ui_drawers_t *drawers, bool show)
{
    static const pos_t drawer_pos[DRAWERn] = { {7, 7}, {66, 7}, {7, 41}, {66, 41} };

    uint8_t str_buff[5] = {};

    if(show)
    {
        ui_draw_window(&drawers->shape.main, LCD_DEFAULT_TEXTCOLOR, true);

        for (uint8_t i = 0; i < DRAWERn; i++)
        {
            drawers->shape.single.x = drawers->shape.main.x + drawer_pos[i].x;
            drawers->shape.single.y = drawers->shape.main.y + drawer_pos[i].y;
            ui_draw_window(&drawers->shape.single, LCD_DEFAULT_TEXTCOLOR, true);

            ui_window_t text_pos;
            text_pos.x = drawers->shape.single.x + 15;
            text_pos.y = drawers->shape.single.y + 4;
            sprintf(str_buff,"%d", DRAWERn + 1);
            ui_display_string(&text_pos, str_buff, &Font16, LCD_DEFAULT_TEXTCOLOR);
        }
    }
    else
    {
        ui_clear_window(&drawers->win.main);
    }
}

void ui_drawers_set_config(ui_drawers_t *drawers, ui_drawer_config *config)
{   
    ui_drawers_show(drawers, true);
    sFONT    *font = &Font20;

    /*Paint select battery item property*/
    if (config->select.main == UI_ITEM_SELECT)
    {
        font = &Font24;
        ui_draw_window(&drawers->win.main, UI_SELECTION_COLOR, true);
    }
    else
    {
        ui_draw_window(&drawers->win.main, UI_SELECTION_COLOR, true);
    }

    /*Paint select battery item property*/
    if (config->select.single == UI_ITEM_SELECT)
    {
        static const pos_t drawer_pos[DRAWERn] = { {7, 7}, {66, 7}, {7, 41}, {66, 41} };
        drawers->shape.single.x = drawers->shape.main.x + drawer_pos[config->drawer.no].x;
        drawers->shape.single.y = drawers->shape.main.y + drawer_pos[config->drawer.no].y;
        ui_draw_window(&drawers->shape.single, UI_SELECTION_COLOR, true);

        ui_window_t text_pos;
        text_pos.x = drawers->shape.single.x + 15;
        text_pos.y = drawers->shape.single.y + 4;

        uint8_t str_buff[5];
        sprintf(str_buff, "%d", config->drawer.no + 1);
        ui_display_string(&text_pos, str_buff, &font, UI_SELECTION_COLOR);
    }

    switch (config->drawer.st)
    {
        case DRAWER_ST_OPEN:    { /* place drawing operations for this state here */ } break;
        case DRAWER_ST_OPENING: { /* place drawing operations for this state here */ } break;
        case DRAWER_ST_CLOSE:   { /* place drawing operations for this state here */ } break;
        case DRAWER_ST_CLOSING: { /* place drawing operations for this state here */ } break;
    default:
        break;
    }
}

//////////////////////////////////// Feeder Config Menu Related Functions //////////////////////////////////////////
void ui_feeder_menu_init(ui_feeder_menu_t *menu)
{
    /*position in the screen */
    menu->win.main.x = 19;
    menu->win.main.y = 98;
    menu->win.main.w = 442;
    menu->win.main.h = 216;
    menu->win.daily.x = menu->win.main.x + 393;
    menu->win.daily.y = menu->win.main.y + 39;
    menu->win.daily.w = 36;
    menu->win.daily.h = 18;

    static const label_t header[4] = { {"OPEN_TIME",102, 2}, {"CLOSE_TIME",208, 2}, {"DATE",325, 2}, {"DAILY",380, 2}};

    static const char *meals[FEEDER_MEALn] = {{"Breakfast"}, {"Snack 1"}, {"Lunch"},{"Snack 2"},{"Dinner"}, {"Snack 3"}};

    for (int i = 0; i < FEEDER_MEALn; i++)
    {       
        ui_window_t text_pos;
        text_pos.x = header->x;
        text_pos.y = header->y;
        ui_display_string(&text_pos, header->name, &Font16, LCD_DEFAULT_TEXTCOLOR);
    }

    for (int i = 0; i < FEEDER_MEALn; i++)
    {
        // meal selection 
        ui_window_t text_pos;
        text_pos.x = 2;
        text_pos.y = 33*(i + 1);
        ui_display_string(&text_pos, meals[i], &Font16, LCD_DEFAULT_TEXTCOLOR);

        // open hour 
        menu->meal_td[i].time.open.hour.x    = menu->win.main.x + (10 + 102);
        menu->meal_td[i].time.open.hour.y    = menu->win.main.y + (i+1)*(33);
        menu->meal_td[i].time.open.min.x     = menu->win.main.x + (10 + 102 + 30);
        menu->meal_td[i].time.open.min.y     = menu->win.main.y + (i+1)*(33);
        menu->meal_td[i].time.open.am_fm.x   = menu->win.main.x + (10 + 102 + 50);
        menu->meal_td[i].time.open.am_fm.y   = menu->win.main.y + (i+1)*(33);

        // close hour
        menu->meal_td[i].time.close.hour.x   = menu->win.main.x + (10 + 209);
        menu->meal_td[i].time.close.hour.y   = menu->win.main.y + (i+1)*(33);
        menu->meal_td[i].time.close.min.x    = menu->win.main.x + (10 + 209 + 30);
        menu->meal_td[i].time.close.min.y    = menu->win.main.y + (i+1)*(33);
        menu->meal_td[i].time.close.am_fm.x  = menu->win.main.x + (10 + 209 + 50);
        menu->meal_td[i].time.close.am_fm.y  = menu->win.main.y + (i+1)*(33);

        // date 
        menu->meal_td[i].date.day.x        = menu->win.main.x + (10 + 316);
        menu->meal_td[i].date.month.x      = menu->win.main.x + (10 + 316 + 30);
        menu->meal_td[i].date.day.y        = menu->win.main.y + (i+1)*(33);
        menu->meal_td[i].date.month.y      = menu->win.main.y + (i+1)*(33);
    }


}
void ui_feeder_menu_show(ui_feeder_menu_t *menu, bool show)
{
    ui_window_t text_pos;

    if(show)
    {
        for (int i = 0; i < FEEDER_MEALn; i++)
        {
            // Open time
            ui_display_string(&menu->meal_td[i].time.open.hour, "--:", &Font16, LCD_DEFAULT_TEXTCOLOR);
            ui_display_string(&menu->meal_td[i].time.open.min , "--", &Font16, LCD_DEFAULT_TEXTCOLOR);
            ui_display_string(&menu->meal_td[i].time.open.am_fm, am_fm_str[TIME_FM], &Font16, LCD_DEFAULT_TEXTCOLOR);

            // Close time 
            ui_display_string(&menu->meal_td[i].time.close.hour, "--:", &Font16, LCD_DEFAULT_TEXTCOLOR);
            ui_display_string(&menu->meal_td[i].time.close.min , "--", &Font16, LCD_DEFAULT_TEXTCOLOR);
            ui_display_string(&menu->meal_td[i].time.close.am_fm, am_fm_str[TIME_FM], &Font16, LCD_DEFAULT_TEXTCOLOR);

            // Day Month
            ui_display_string(&menu->meal_td[i].date.day, "--/", &Font16, LCD_DEFAULT_TEXTCOLOR);
            ui_display_string(&menu->meal_td[i].date.month, "--", &Font16, LCD_DEFAULT_TEXTCOLOR);
        }
    }
    else
    {
        ui_clear_window(&menu->win.main);
    }
}

void ui_feeder_menu_set_config(ui_feeder_menu_t *menu, ui_feeder_config_t *config)
{
    uint8_t str[5];
    uint16_t color = LCD_DEFAULT_TEXTCOLOR;
    sFONT    *font = &Font16;
    ui_window_t text_pos;

    if (config->select == UI_ITEM_SELECT)
    {
        color = UI_SELECTION_COLOR;
        font = &Font20;
    }
    
    switch (config->set)
    {
        case FEEDER_CNF_OPEN_TIME_HOUR: {
            sprintf(str, "%.2d", config->time.hour);
            ui_display_string(&menu->meal_td[config->meal].time.open.hour, str, font, color);
        } break;

        case FEEDER_CNF_OPEN_TIME_MIN: {
            sprintf(str, "%.2d", config->time.minute);
            ui_display_string(&menu->meal_td[config->meal].time.open.min, str, font, color);
        } break;

        case FEEDER_CNF_OPEN_TIME_AM_FM: {
            ui_display_string(&menu->meal_td[config->meal].time.open.am_fm,
                              am_fm_str[config->time.am_fm], font, color);
        } break;

        case FEEDER_CNF_CLOSE_TIME_HOUR: {
            sprintf(str, "%.2d", config->time.hour);
            ui_display_string(&menu->meal_td[config->meal].time.close.hour, str, font, color);
        } break;

        case FEEDER_CNF_CLOSE_TIME_MIN: { 
            sprintf(str, "%.2d", config->time.minute);
            ui_display_string(&menu->meal_td[config->meal].time.close.min, str, font, color);
        } break;

        case FEEDER_CNF_CLOSE_TIME_AM_FM:
        {
            ui_display_string(&menu->meal_td[config->meal].time.close.am_fm,
                              am_fm_str[config->time.am_fm], font, color);
        } break;

        case FEEDER_CNF_DATE_DAY: {
            sprintf(str, "%.2d", config->date.day);
            ui_display_string(&menu->meal_td[config->meal].date.day, str, font, color);
        }
        break;

        case FEEDER_CNF_DATE_MONTH: {
            sprintf(str, "%.2d", config->date.day);
            ui_display_string(&menu->meal_td[config->meal].date.month, str, font, color);
        }
        break;

        case FEEDER_CNF_DATE_DAILY: {

            ui_draw_window(&menu->win.daily, color, true);
            ui_window_t win = {.x = menu->win.daily.x + 1, .y = menu->win.daily.y + 1, \
                                .h = menu->win.daily.h - 2, .w = menu->win.daily.w - 2};

            if(config->date.daily_st == FEEDER_DAILY_MEAL_ENABLE)
            {
                ui_fill_window(&win, LCD_COLOR_GREEN);
            }
            else
            {
                ui_fill_window(&win, LCD_DEFAULT_BACKCOLOR);
            }

         } break;

        default:
        printf("no valid option for feeder schedule config");
        break;
    }

}



//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////
void ui_date_time_init(ui_date_time_menu_t *menu)
{
    menu->win.main.x = 17;
    menu->win.main.y = 116;
    menu->win.main.w = 442;
    menu->win.main.h = 161;

    menu->time.hour.x  = menu->win.main.x + 31;
    menu->time.hour.y  = menu->win.main.y + 29;
    menu->time.min.x   = menu->win.main.x + 191;
    menu->time.min.y   = menu->win.main.y + 29;
    menu->date.day.x   = menu->win.main.x + 347;
    menu->date.day.y   = menu->win.main.y + 61;
    menu->date.month.x = menu->win.main.x + 389;
    menu->date.month.y = menu->win.main.y + 61;
}

void ui_date_time_show(ui_date_time_menu_t *menu, bool show)
{
    if(show)
    {
        ui_display_string(&menu->time.hour, "--:", &Font24, LCD_DEFAULT_TEXTCOLOR);
        ui_display_string(&menu->time.min, "--", &Font24, LCD_DEFAULT_TEXTCOLOR);

        ui_display_string(&menu->date.day, "--/", &Font16, LCD_DEFAULT_TEXTCOLOR);
        ui_display_string(&menu->date.month, "--", &Font16, LCD_DEFAULT_TEXTCOLOR);

        ui_draw_window(&menu->win.main, LCD_DEFAULT_TEXTCOLOR, true);
    }
    else
    {
        ui_clear_window(&menu->win.main);
    }
}

void ui_date_time_set_config(ui_date_time_menu_t *menu, ui_date_time_config_t *config)
{
    uint8_t str[5];
    uint16_t color = LCD_DEFAULT_TEXTCOLOR;

    if (config->select == UI_ITEM_SELECT)
        color = UI_SELECTION_COLOR;

    switch (config->set)
    {
    case DATE_TIME_CNF_HOUR: {
            sprintf(str, "%.2d", config->time.hour);
            ui_display_string(&menu->time.hour, str, &Font24, color);
    } break;

    case DATE_TIME_CNF_MIN: {
            sprintf(str, "%.2d", config->time.min);
            ui_display_string(&menu->time.min, str, &Font24, color);
    } break;

    case DATE_TIME_CNF_DAY: {
            sprintf(str, "%.2d", config->date.day);
            ui_display_string(&menu->date.day, str, &Font16, color);
    } break;

    case DATE_TIME_CNF_MONTH: {
            sprintf(str, "%.2d", config->date.day);
            ui_display_string(&menu->date.day, str, &Font16, color);
    } break;

    default:
        break;
    }
}

//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////
/* Thermostat icon Functions */
void ui_thermostat_init(ui_thermostat_t *menu)
{
    menu->win.main.x = 157
    menu->win.main.y = 11;
    menu->win.main.w = 104;
    menu->win.main.h = 81;

    menu->icon.therm.ptr = &thermostat;
    menu->icon.therm.x = menu->win.main.x + 1;
    menu->icon.therm.y = menu->win.main.y + 4;
    
    menu->shape.temp.x = menu->win.main.x + 13;
    menu->shape.temp.y = menu->win.main.y + 11;
    menu->shape.temp.h = 49;
    menu->shape.temp.w = 4;

    menu->shape.circle.x = menu->win.main.x + 7;
    menu->shape.circle.y = menu->win.main.y + 52;

    menu->text.temp.x = menu->win.main.x + 38; 
    menu->text.temp.y = menu->win.main.y + 21;
}

void ui_thermostat_show(ui_thermostat_t *menu, bool show)
{
    if(show)
    {
        ui_draw_icon(&menu->icon);
        ui_draw_window(&menu->shape.temp);
        ui_draw_circle(&menu->shape.circle, 8, LCD_DEFAULT_TEXTCOLOR)
        ui_display_string(&menu->text, "--C", &Font16, LCD_DEFAULT_TEXTCOLOR);
    }
    else 
    {
        ui_clear_window(&menu->win.main);
    }

}

void ui_thermostat_set_config(ui_thermostat_t *menu, ui_thermostat_config_t *config)
{
    uint8_t str[7];
    uint16_t color = LCD_DEFAULT_TEXTCOLOR;

    if (config->select == UI_ITEM_SELECT)
        color = UI_SELECTION_COLOR;

    /*!< TODO: make the hight change with the temperature*/
    ui_draw_icon(&menu->icon);
    ui_draw_window(&menu->shape.temp);
    ui_fill_window(&menu->shape.temp);
    ui_draw_circle(&menu->shape.circle, 8, LCD_DEFAULT_TEXTCOLOR);
    ui_fill_circle(&menu->shape.circle, 7, LCD_DEFAULT_TEXTCOLOR);
    sprintf(str, "%d%s", config->temp.val, c_f_str[config->temp.unit]) 
    ui_display_string(&menu->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR);
}


//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////

/* Thermostat Config Menu Functions */
void ui_thermostat_menu_init(ui_date_time_menu_t *menu);
void ui_thermostat_menu_show(ui_date_time_menu_t *menu);
void ui_thermostat_menu_set_config(ui_date_time_menu_t *menu, date_time_config_t *config);

//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////

/* Pet Call Icon Function */
void ui_pet_call_icon_init(pet_call_icon_menu_t *menu);
void ui_pet_call_icon_show(pet_call_icon_menu_t *menu);
void ui_pet_call_icon_set_config(pet_call_icon_menu_t *menu, pet_call_icon_config_t *config);

//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////

/* Pet Call Config Menu Function */
void ui_pet_call_menu_init(pet_call_menu_t *menu);
void ui_pet_call_menu_show(pet_call_menu_t *menu);
void ui_pet_call_menu_set_config(pet_call_menu_t *menu, pet_call_menu_config_t *config);




//////////////////////////////////////// END ///////////////////////////////////////////////////////////////////////

/////////////////////////////////// Date Hour Related Functions /////////////////////////////////////////////



/////////////////////////////////// Drawers Related Functions /////////////////////////////////////////////



/////////////////////////////////// Thermostat Related Functions /////////////////////////////////////////////
void ui_thermostat_init(ui_thermostat_t *therm, bool show)
{
    if(show)
    {

    }
    else 
    {

    }
}

void ui_thermostat_set_temp(ui_thermostat_t *therm, uint8_t temp_units, int temp,bool show)
{
    if(show)
    {


    }
    else
    {

    }
}



/////////////////////////////////// Battery Related Functions /////////////////////////////////////////////
