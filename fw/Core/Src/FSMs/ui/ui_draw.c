#include "ui_draw.h"

/* Icons */
extern BITMAPSTRUCT battery_icon[];
extern BITMAPSTRUCT batt_error_icon[];
extern BITMAPSTRUCT batt_warning_icon[];
extern BITMAPSTRUCT thermostat[];
extern BITMAPSTRUCT enable_icon[];
extern BITMAPSTRUCT recording_icon[];

/* UI elements */
ui_battery_t           ui_battery;
ui_drawers_t           ui_drawers;
ui_thermostat_t        ui_thermostat;
ui_petcall_t           ui_petcall;
ui_date_time_menu_t    ui_date_time;
ui_feeder_menu_t       ui_feeder_menu;
ui_thermostat_config_t ui_therm_conf;
ui_petcall_config_t    ui_petcall_conf;

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
static const char *c_f_str[TEMP_UNITSn] = {"C", "F"}; 


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

static void ui_display_string(ui_window_t *win, char *text, sFONT *font, uint16_t color)
{
    BSP_LCD_SetTextColor(color);
    BSP_LCD_SetFont(font);
    BSP_LCD_DisplayStringAt(win->x, win->y, (uint8_t*)text, LEFT_MODE);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_DEFAULT_BACKCOLOR);
}

//////////////////////////////////// Battery Icon Related Functions   //////////////////////////////////////////////
void ui_battery_init(ui_battery_t *batt)
{
    /* Main window position - all other components will move according to this coordinate */
    batt->win.main.x = 366;
    batt->win.main.y = 10;
    batt->win.main.h = 81;
    batt->win.main.w = 96;

    batt->icon.batt.ptr = battery_icon;
    batt->icon.batt.x = batt->win.main.x + 4;
    batt->icon.batt.y = batt->win.main.y + 16;

    /*commented lines until we extend the MCU memory*/
    // batt->icon.error.ptr = &batt_error_icon;
    batt->icon.error.ptr = battery_icon;
    batt->icon.error.x = batt->icon.batt.x + 16;
    batt->icon.error.y = batt->icon.batt.y + 16;

    // batt->icon.warn.ptr = &batt_warning_icon;
    batt->icon.warn.ptr = battery_icon;
    batt->icon.warn.x = batt->icon.batt.x + 16;
    batt->icon.warn.y = batt->icon.batt.y + 16;

    batt->shape.charge.x = batt->icon.batt.x + 17;
    batt->shape.charge.y = batt->icon.batt.y + 8;
    batt->shape.charge.h = batt->icon.batt.ptr->infoHeader.biHeight - 16;
    batt->shape.charge.w = batt->icon.batt.ptr->infoHeader.biWidth - 24;

    batt->text.x = batt->shape.charge.x + 15;
    batt->text.y = batt->shape.charge.y + 10;
}

void ui_battery_show(ui_battery_t *batt, bool show)
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
    char str[5] = {0};
    sprintf(str, "%d%%", batt_lvl);

    uint8_t temp_w = batt->shape.charge.w;
    batt->shape.charge.w = (int)width;
        
    /*Critical battery level */
    if(batt_lvl > 0 && batt_lvl < 17)
    {
        ui_fill_window(&batt->shape.charge, LCD_COLOR_RED);
        ui_display_string(&batt->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR);   
    }

    /*Medium battery level */
    if(batt_lvl >= 17  && batt_lvl < 70)
    {
        ui_fill_window(&batt->shape.charge, LCD_COLOR_YELLOW);
        ui_display_string(&batt->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR); 
    }

    /*High battery level */
    if(batt_lvl >= 70  && batt_lvl <= 100)
    {
        ui_fill_window(&batt->shape.charge, LCD_COLOR_GREEN);
        ui_display_string(&batt->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR); 
    }

    batt->shape.charge.w = temp_w;
}

void ui_battery_set_config(ui_battery_t *batt, ui_battery_config_t *config)
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

    char str_buff[5] = {};

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
            sprintf(str_buff,"%d", i + 1);
            ui_display_string(&text_pos, str_buff, &Font16, LCD_DEFAULT_TEXTCOLOR);
        }
    }
    else
    {
        ui_clear_window(&drawers->win.main);
    }
}

void ui_drawers_set_config(ui_drawers_t *drawers, ui_drawers_config_t *config)
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
        ui_draw_window(&drawers->win.main, UI_SELECTION_COLOR, false);
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

        char str_buff[5];
        sprintf(str_buff, "%d", config->drawer.no + 1);
        ui_display_string(&text_pos, str_buff, font, UI_SELECTION_COLOR);
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

    for (int i = 0; i < FEEDER_MEALn; i++)
    {
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
    if(show)
    {
        ui_draw_window(&menu->win.main, LCD_DEFAULT_TEXTCOLOR, true);
        ui_draw_window(&menu->win.daily, LCD_DEFAULT_TEXTCOLOR, true);

        static label_t header[4] = { {"OPEN_TIME",102, 2}, {"CLOSE_TIME",208, 2}, {"DATE",325, 2}, {"DAILY",380, 2}};
        static char *meals[FEEDER_MEALn] = {"Breakfast", "Snack 1", "Lunch","Snack 2","Dinner", "Snack 3"};

        for (int i = 0; i < 4; i++)
        {       
            ui_window_t text_pos;
            text_pos.x = menu->win.main.x + header[i].x;
            text_pos.y = menu->win.main.y + header[i].y;
            ui_display_string(&text_pos, header[i].name, &Font16, LCD_DEFAULT_TEXTCOLOR);
        }

        for (int i = 0; i < FEEDER_MEALn; i++)
        {
            // meal selection 
            ui_window_t text_pos;
            text_pos.x = menu->win.main.x + 2;
            text_pos.y = menu->win.main.y + 33*(i + 1);
            ui_display_string(&text_pos, meals[i], &Font16, LCD_DEFAULT_TEXTCOLOR);

            // Open time
            ui_display_string(&menu->meal_td[i].time.open.hour, "--:", &Font16, LCD_DEFAULT_TEXTCOLOR);
            ui_display_string(&menu->meal_td[i].time.open.min , "--", &Font16, LCD_DEFAULT_TEXTCOLOR);
            ui_display_string(&menu->meal_td[i].time.open.am_fm, (char*)am_fm_str[TIME_FM], &Font16, LCD_DEFAULT_TEXTCOLOR);

            // Close time 
            ui_display_string(&menu->meal_td[i].time.close.hour, "--:", &Font16, LCD_DEFAULT_TEXTCOLOR);
            ui_display_string(&menu->meal_td[i].time.close.min , "--", &Font16, LCD_DEFAULT_TEXTCOLOR);
            ui_display_string(&menu->meal_td[i].time.close.am_fm, (char*)am_fm_str[TIME_FM], &Font16, LCD_DEFAULT_TEXTCOLOR);

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
    char str[5];
    uint16_t color = LCD_DEFAULT_TEXTCOLOR;
    sFONT    *font = &Font16;

    if (config->select.single == UI_ITEM_SELECT)
    {
        color = UI_SELECTION_COLOR;
        font = &Font16;
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
                              (char*)am_fm_str[config->time.am_fm], font, color);
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
                              (char*)am_fm_str[config->time.am_fm], font, color);
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
        ui_display_string(&menu->time.hour, "12:", &Font24, LCD_DEFAULT_TEXTCOLOR);
        ui_display_string(&menu->time.min, "00", &Font24, LCD_DEFAULT_TEXTCOLOR);

        ui_display_string(&menu->date.day, "01/", &Font16, LCD_DEFAULT_TEXTCOLOR);
        ui_display_string(&menu->date.month, "12", &Font16, LCD_DEFAULT_TEXTCOLOR);

        ui_draw_window(&menu->win.main, LCD_DEFAULT_TEXTCOLOR, true);
    }
    else
    {
        ui_clear_window(&menu->win.main);
    }
}

void ui_date_time_set_config(ui_date_time_menu_t *menu, ui_date_time_config_t *config)
{
    char str[5];
    uint16_t single_color_sel = LCD_DEFAULT_TEXTCOLOR;
    uint16_t main_color_sel = LCD_DEFAULT_TEXTCOLOR;

    if (config->select.single == UI_ITEM_SELECT)
        single_color_sel = UI_SELECTION_COLOR;

    if (config->select.main == UI_ITEM_SELECT)
        main_color_sel = UI_SELECTION_COLOR;

    ui_draw_window(&menu->win.main, main_color_sel, true);

    switch (config->set)
    {
    case DATE_TIME_CNF_HOUR: {
            sprintf(str, "%.2d", config->time.hour);
            ui_display_string(&menu->time.hour, str, &Font24, single_color_sel);
    } break;

    case DATE_TIME_CNF_MIN: {
            sprintf(str, "%.2d", config->time.min);
            ui_display_string(&menu->time.min, str, &Font24, single_color_sel);
    } break;

    case DATE_TIME_CNF_DAY: {
            sprintf(str, "%.2d", config->date.day);
            ui_display_string(&menu->date.day, str, &Font16, single_color_sel);
    } break;

    case DATE_TIME_CNF_MONTH: {
            sprintf(str, "%.2d", config->date.day);
            ui_display_string(&menu->date.day, str, &Font16, single_color_sel);
    } break;

    default:
        break;
    }
}

//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////
/* Thermostat icon Functions */
void ui_thermostat_init(ui_thermostat_t *menu)
{
    menu->win.main.x = 165;
    menu->win.main.y = 11;
    menu->win.main.w = 90;
    menu->win.main.h = 81;

    menu->icon.therm.ptr = thermostat;
    menu->icon.therm.x = menu->win.main.x + 1;
    menu->icon.therm.y = menu->win.main.y + 4;
    
    menu->shape.temp.x = menu->win.main.x + 19;
    menu->shape.temp.y = menu->win.main.y + 11;
    menu->shape.temp.h = 49;
    menu->shape.temp.w = 6;

    menu->shape.circle.x = menu->win.main.x + 21;
    menu->shape.circle.y = menu->win.main.y + 60;

    menu->text.temp.x = menu->win.main.x + 36; 
    menu->text.temp.y = menu->win.main.y + 25;
}

void ui_thermostat_show(ui_thermostat_t *menu, bool show)
{
    if(show)
    {
        ui_draw_icon(&menu->icon.therm);
        ui_window_t win = {.x = menu->shape.temp.x + 1, .y = menu->shape.temp.y + 1, .w = menu->shape.temp.w - 2, .h = menu->shape.temp.h - 2};
        ui_draw_window(&menu->shape.temp, LCD_DEFAULT_TEXTCOLOR, true);
        ui_draw_circle(&menu->shape.circle, 8, LCD_DEFAULT_TEXTCOLOR);
        ui_fill_circle(&menu->shape.circle, 7, LCD_DEFAULT_BACKCOLOR);
        ui_fill_window(&win, LCD_DEFAULT_BACKCOLOR);
        ui_display_string(&menu->text.temp, "--C", &Font20, LCD_DEFAULT_TEXTCOLOR);
    }
    else 
    {
        ui_clear_window(&menu->win.main);
    }

}

static void ui_thermostat_draw_temp(ui_thermostat_t *therm, uint8_t temp_lvl, temperature_unit_t units)
{
    float hight = therm->shape.temp.h * (temp_lvl / 99.0);
    char str[5] = {0};
    uint16_t color = LCD_COLOR_BLUE;
    sprintf(str, ".%.2d%s", temp_lvl, c_f_str[units]);

    uint8_t temp_h = therm->shape.temp.h;
    therm->shape.temp.h = (int)hight;

    if(units == TEMP_UNITS_CELSIUS)
    {
        /*Low temp value level */
        if(temp_lvl > 0 && temp_lvl < 17)
            color = LCD_COLOR_BLUE;

        /*Medium temp level */
        if(temp_lvl >= 17  && temp_lvl < 70)
            color = LCD_COLOR_YELLOW;

        /*High temp level */
        if(temp_lvl >= 70  && temp_lvl <= 100)
            color = LCD_COLOR_RED;
    }

    /*!>TODO : add fahrenheit option*/
    ui_window_t win = {.x = therm->shape.temp.x + 1, .y = therm->shape.temp.y + 1, .w = therm->shape.temp.w - 2, .h = therm->shape.temp.h - 2};
    ui_fill_circle(&therm->shape.circle, 7, LCD_DEFAULT_BACKCOLOR);
    ui_fill_window(&win, LCD_DEFAULT_BACKCOLOR);
    ui_fill_circle(&therm->shape.circle, 7, color);
    ui_fill_window(&win, color);
    sprintf(str, "%d%s", temp_lvl, c_f_str[units]);
    ui_display_string(&therm->text.temp, str, &Font20, LCD_DEFAULT_TEXTCOLOR);

    therm->shape.temp.h = temp_h;
}


void ui_thermostat_set_config(ui_thermostat_t *menu, ui_thermostat_config_t *config)
{
    char str[7];
    uint16_t color = LCD_DEFAULT_BACKCOLOR;

    if (config->select == UI_ITEM_SELECT)
    {
        color = UI_SELECTION_COLOR;
    }
    ui_draw_window(&menu->win.main, color, true);
    ui_thermostat_draw_temp(menu, config->temp.val, config->temp.unit);
}


//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////

/* Thermostat Config Menu Functions */
void ui_thermostat_menu_init(ui_thermostat_menu_t *menu)
{
    menu->win.main.x = 17;
    menu->win.main.y = 116;
    menu->win.main.h = 161;
    menu->win.main.w = 442;

    menu->win.set_temp.x = menu->win.main.x + 190; 
    menu->win.set_temp.y = menu->win.main.y + 4;
    menu->win.set_temp.h = 40; 
    menu->win.set_temp.w = 74;

    menu->win.set_unit.x = menu->win.main.x + 190;
    menu->win.set_unit.y = menu->win.main.y + 55;
    menu->win.set_unit.w = 40;
    menu->win.set_unit.h = 74;

    menu->win.enable_temp.x = menu->win.main.x + 190;
    menu->win.enable_temp.y = menu->win.main.y + 104;
    menu->win.enable_temp.w = 40;
    menu->win.enable_temp.h = 74;

    menu->icon.therm.ptr = enable_icon;

    /* Adjust text window */
    menu->text.ctrl_temp.x = menu->win.set_temp.x + 5;      
    menu->text.ctrl_temp.y = menu->win.set_temp.y + 5;
}

void ui_thermostat_menu_show(ui_thermostat_menu_t *menu, bool show)
{
    char *str_titles[3] = {"Set Temperature :", "Set Units :", "Thermostat : "};
    char *str_options[2] = {"Fahrenheit/Celsius", "On/Off"};

    if(show)
    {
        // display static text 
        ui_window_t set_temp = {.x = menu->win.main.x + 33, .y =  menu->win.main.y + 14};
        ui_display_string(&set_temp, str_titles[0], &Font16, LCD_DEFAULT_TEXTCOLOR);

        ui_window_t set_units = {.x = menu->win.main.x + 33, .y =  menu->win.main.y + 60};
        ui_display_string(&set_units, str_titles[1], &Font16, LCD_DEFAULT_TEXTCOLOR);

        ui_window_t therm_en = {.x = menu->win.main.x + 33, .y =  menu->win.main.y + 109};
        ui_display_string(&therm_en, str_titles[2], &Font16, LCD_DEFAULT_TEXTCOLOR);

        ui_window_t f_c_option = {.x = menu->win.main.x + 289, .y =  menu->win.main.y + 60};
        ui_display_string(&f_c_option, str_options[0], &Font16, LCD_DEFAULT_TEXTCOLOR);

        ui_window_t therm_en_option = {.x = menu->win.main.x + 289, .y =  menu->win.main.y + 113};
        ui_display_string(&therm_en_option, str_options[1], &Font16, LCD_DEFAULT_TEXTCOLOR);

        // display static icons

        /*adjust offset */
        menu->icon.therm.x = menu->win.set_unit.x; 
        menu->icon.therm.y = menu->win.set_unit.y; 
        ui_draw_icon(&menu->icon.therm);
        menu->icon.therm.x = menu->win.enable_temp.x; 
        menu->icon.therm.y = menu->win.enable_temp.y; 
        ui_draw_icon(&menu->icon.therm);
        
        // display default text val 
        ui_display_string(&menu->text.ctrl_temp, "--C", &Font16, LCD_DEFAULT_TEXTCOLOR);

    }
    else
    {
        ui_clear_window(&menu->win.main);
    }
}

void ui_thermostat_menu_set_config(ui_thermostat_menu_t *menu, ui_thermostat_menu_config_t *config)
{
    char str[5];
    uint16_t color = LCD_DEFAULT_TEXTCOLOR;
    sFONT *font = &LCD_DEFAULT_FONT;

    if (config->select == UI_ITEM_SELECT)
    {
        color = UI_SELECTION_COLOR;
        font = &Font20;
    }

    switch (config->set)
    {
    case THERM_SET_TEMPERATURE: {
        if (config->select == UI_ITEM_SELECT)
            ui_draw_window(&menu->text.ctrl_temp, color, true);
        sprintf(str, "%d%s", config->temp.val, (char*)c_f_str[config->temp.unit]);
        ui_display_string(&menu->text.ctrl_temp, str, font, color);
     } break;

    case THERM_SET_UNIT: { 
        if (config->select == UI_ITEM_SELECT)
            ui_draw_window(&menu->win.set_unit, color, true);

        sprintf(str, "%d%s", config->temp.val, c_f_str[config->temp.unit]);
        ui_display_string(&menu->text.ctrl_temp, str, font, color);

        /*adjust offset */
        ui_window_t win_c = {.x = menu->win.set_unit.x, .y = menu->win.set_unit.y};
        ui_window_t win_f = {.x = menu->win.set_unit.x, .y = menu->win.set_unit.y};
        
        /*clean previous state */
        ui_draw_circle(&win_c,  9, LCD_DEFAULT_BACKCOLOR);
        ui_draw_circle(&win_f, 9, LCD_DEFAULT_BACKCOLOR);

        if(config->temp.unit == TEMP_UNITS_CELSIUS)
            ui_draw_circle(&win_c,  9, color);

        else if(config->temp.unit == TEMP_UNITS_FAHRENHEIT)
            ui_draw_circle(&win_f,  9, color);

    } break;

    case THERM_ENABLE_CTRL: {
        if (config->select == UI_ITEM_SELECT)
            ui_draw_window(&menu->win.set_unit, color, true);

        /*adjust offset */
        ui_window_t therm_en = {.x = menu->win.set_unit.x, .y = menu->win.set_unit.y};
        ui_window_t therm_dis = {.x = menu->win.set_unit.x, .y = menu->win.set_unit.y};
        
        /*clean previous state */
        ui_draw_circle(&therm_en,  9, LCD_DEFAULT_BACKCOLOR);
        ui_draw_circle(&therm_dis, 9, LCD_DEFAULT_BACKCOLOR);

        if (config->temp.unit == TEMP_UNITS_CELSIUS)
            ui_draw_circle(&therm_en, 9, color);

        else if (config->temp.unit == TEMP_UNITS_FAHRENHEIT)
            ui_draw_circle(&therm_dis, 9, color);
     } break;

    default:
        break;
    }
}


//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////

void ui_petcall_init(ui_petcall_t *menu)
{
    menu->win.main.x = 265;
    menu->win.main.y = 11;
    menu->win.main.w = 64;
    menu->win.main.h = 81;

    //commented line to save flash memory
    menu->icon.mic.ptr = thermostat;
    // menu->icon.mic.ptr = &recording_icon;
    menu->icon.mic.x = menu->win.main.x + 1;
    menu->icon.mic.y = menu->win.main.y + 9; 

}

void ui_petcall_show(ui_petcall_t *menu, bool show)
{
    if(show)
    {
        // ui_draw_window(&menu->win.main, LCD_DEFAULT_TEXTCOLOR, true);
        ui_window_t text = {.x = menu->win.main.x + 15 , .y = menu->win.main.y + 20};
        ui_display_string(&text, "MIC", &Font16, LCD_DEFAULT_TEXTCOLOR);
        // ui_draw_icon(&menu->icon.mic);
    }
    else
    {
        ui_clear_window(&menu->win.main);
    }
}

void ui_petcall_set_config(ui_petcall_t *menu, ui_petcall_config_t *config)
{
    uint16_t win_color = LCD_DEFAULT_BACKCOLOR;

    if (config->select == UI_ITEM_SELECT)
    {
        win_color = UI_SELECTION_COLOR;
    }
    ui_draw_window(&menu->win.main, win_color, true);
}


//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////

/* Pet Call Config Menu Function */
void ui_petcall_menu_init(ui_petcall_menu_t *menu)
{
    menu->win.main.x = 17;
    menu->win.main.y = 116; 
    menu->win.main.w = 442;
    menu->win.main.h = 161;
}

void ui_petcall_menu_show(ui_petcall_menu_t *menu, bool show)
{
    if(show)
    {
        ui_draw_window(&menu->win.main, LCD_DEFAULT_TEXTCOLOR, true);
    }
    else
    {
        ui_clear_window(&menu->win.main);
    }

}

void ui_petcall_menu_set_config(ui_petcall_menu_t *menu, ui_petcall_menu_config_t *config)
{
    uint16_t color = LCD_DEFAULT_TEXTCOLOR;

    if (config->select == UI_ITEM_SELECT)
    {
        color = UI_SELECTION_COLOR;
    }

    ui_draw_window(&menu->win.main, color, true);
}




//////////////////////////////////////// END ///////////////////////////////////////////////////////////////////////
