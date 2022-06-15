#include "ui_draw.h"

ui_battery_t ui_battery;
ui_thermostat_t ui_thermostat;
ui_drawers_t ui_drawers;
ui_date_time_t ui_date_time;
ui_thermostat_config_t ui_therm_conf;
feeder_menu_t ui_feeder_menu;
date_time_menu_t ui_date_time_menu;

typedef struct
{
    char *name;
    uint16_t x;
    uint16_t y;
} label_t;

static const char *set_am_fm[TIMEn] = {"AM", "FM"}; 

static const label_t drawer_label[DRAWERn] =
{
    {"1",  7, 7  },
    {"2", 66, 7  },
    {"3",  7, 41 },
    {"4", 66, 41 }};


static void ui_show_win(ui_window_t *win, uint16_t color, bool show)
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

static void ui_clear_win(ui_window_t *win)
{
    BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
    BSP_LCD_FillRect(win->x, win->y, win->w, win->h);
    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
}

static void ui_draw_icon(ui_icon_t *icon)
{
    BSP_LCD_DrawBitmap(icon->x, icon->y, (uint8_t*)icon->ptr);
}

static void ui_fill_win(ui_window_t *win, uint16_t color)
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

    batt->form.charge.x = batt->icon.batt.x + 17;
    batt->form.charge.y = batt->icon.batt.y + 8;
    batt->form.charge.h = batt->icon.batt.ptr->infoHeader.biHeight - 16;
    batt->form.charge.w = batt->icon.batt.ptr->infoHeader.biWidth - 24;

    batt->text.x = batt->form.charge.x + 15;
    batt->text.y = batt->form.charge.y + 10;
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
        ui_clear_win(&batt->win.main);
    }
}

static void ui_battery_draw_charge(ui_battery_t *batt, uint8_t batt_lvl)
{
    float width = batt->form.charge.w * (batt_lvl / 100.0);
    uint8_t str[5] = {0};
    sprintf(str, "%d%%", batt_lvl);

    uint8_t temp_w = batt->form.charge.w;
    batt->form.charge.w = (int)width;
        
    /*Critical battery level */
    if(width > 0 && width < 7)
    {
        ui_fill_win(&batt->form.charge, LCD_COLOR_RED);
        ui_display_string(&batt->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR);   
    }

    /*Low battery level */
    if(width >= 7  && width < 30)
    {
        ui_fill_win(&batt->form.charge, LCD_COLOR_YELLOW);
        ui_display_string(&batt->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR); 
    }

    /*Medium battery level */
    if(width >= 30  && width < 70)
    {
        ui_fill_win(&batt->form.charge, LCD_COLOR_YELLOW);
        ui_display_string(&batt->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR); 
    }

    /*High battery level */
    if(width >= 70  && width <= 100)
    {
        ui_fill_win(&batt->form.charge, LCD_COLOR_GREEN);
        ui_display_string(&batt->text, str, &Font16, LCD_DEFAULT_TEXTCOLOR); 
    }

    batt->form.charge.w = temp_w;
}

void ui_battery_icon_set_config(ui_battery_t *batt, ui_battery_config *config)
{
    /*Paint select battery item property*/
    if (config->select == UI_ITEM_SELECT)
        ui_show_win(&batt->win.main, UI_SELECTION_COLOR, true);
    else
        ui_show_win(&batt->win.main, LCD_DEFAULT_BACKCOLOR, false);
    
    /*Clear charging info */
    ui_clear_win(&batt->form.charge); 

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

    drawers->form.main_frame.x = drawers->win.main.x + 4;
    drawers->form.main_frame.y = drawers->win.main.y + 4;
    drawers->form.main_frame.h = drawers->win.main.h - 8;
    drawers->form.main_frame.w = drawers->win.main.w - 8;

    drawers->form.single_frame.w = 50;
    drawers->form.single_frame.h = 25;
}

void ui_drawers_show(ui_drawers_t *drawers, bool show)
{
    if(show)
    {

    }
    else
    {
        ui_clear_win(&drawers->win.main);
    }
}

void ui_drawers_set_config(ui_drawers_t *drawers, uint8_t drawer_no);


void ui_drawers_select(ui_drawers_t *drawers, uint8_t drawer_no)
{
    /*Clean All Drawers */
    ui_drawers_clean(drawers);

    drawers->form.single_frame.x = drawers->form.main_frame.x + drawer_label[drawer_no].x;
    drawers->form.single_frame.y = drawers->form.main_frame.y + drawer_label[drawer_no].y;
    ui_show_win(&drawers->form.single_frame, DRAWER_SELECTION_COLOR, true);
    BSP_LCD_SetTextColor(DRAWER_SELECTION_COLOR);
    BSP_LCD_SetFont(&DRAWER_NUMBER_FONT);

    BSP_LCD_DisplayStringAt(drawers->form.single_frame.x + 15, drawers->form.single_frame.y + 4, drawer_label[drawer_no].name, LEFT_MODE);
    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    BSP_LCD_SetFont(LCD_DEFAULT_TEXTCOLOR);
}

//////////////////////////////////// Feeder Config Menu Related Functions //////////////////////////////////////////
void ui_feeder_menu_init(feeder_menu_t *menu);
void ui_feeder_menu_show(feeder_menu_t *menu);
void ui_feeder_menu_set_config(feeder_menu_t *menu, feeder_config_t *config);



//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////
void ui_date_time_init(date_time_menu_t *menu);
void ui_date_time_show(date_time_menu_t *menu);
void ui_date_time_set_config(date_time_menu_t *menu, date_time_config_t *config);


//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////
/* Thermostat icon Functions */
void ui_thermostat_icon_init(date_time_menu_t *menu);
void ui_thermostat_icon_show(date_time_menu_t *menu);
void ui_thermostat_icon_set_config(date_time_menu_t *menu, date_time_config_t *config);

//////////////////////////////////// Date Time Config Menu Related Functions ///////////////////////////////////////

/* Thermostat Config Menu Functions */
void ui_thermostat_menu_init(date_time_menu_t *menu);
void ui_thermostat_menu_show(date_time_menu_t *menu);
void ui_thermostat_menu_set_config(date_time_menu_t *menu, date_time_config_t *config);

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


void ui_feeder_menu_init(feeder_menu_t *menu)
{
    menu->win.main.x = 19;
    menu->win.main.y = 98;
    menu->win.main.w = 442;
    menu->win.main.h = 216;

    menu->win.daily_en.x = menu->win.main.x + 393;
    menu->win.daily_en.y = menu->win.main.y + 39;
    menu->win.daily_en.w = 36;
    menu->win.daily_en.h = 18;

    static const label_t feeder_header[4] =
    {
        {"OPEN_TIME",  102, 2},
        {"CLOSE_TIME", 208, 2},
        {"DATE",       325, 2},
        {"DAILY",      380, 2}};

    static const label_t feeder_meals[FEEDER_MEALn] =
    {
        /*Select Meal*/
        {"Breakfast", 2, 33*1},
        {"Snack 1"  , 2, 33*2},
        {"Lunch"    , 2, 33*3},
        {"Snack 2"  , 2, 33*4},
        {"Dinner"   , 2, 33*5},
        {"Snack 3"  , 2, 33*6}};

    for (int i = 0; i < FEEDER_MEALn; i++)
    {
        menu->config[i].open_time.hour.x    = menu->win.main.x + (10 + 102);
        menu->config[i].open_time.min.x     = menu->win.main.x + (10 + 102 + 30);
        menu->config[i].open_time.am_fm.x   = menu->win.main.x + (10 + 102 + 50);

        menu->config[i].close_time.hour.x   = menu->win.main.x + (10 + 209);
        menu->config[i].close_time.min.x    = menu->win.main.x + (10 + 209 + 30);
        menu->config[i].close_time.am_fm.x  = menu->win.main.x + (10 + 209 + 50);

        menu->config[i].date.day.x        = menu->win.main.x +   (10 + 316);
        menu->config[i].date.month.x      = menu->win.main.x +   (10 + 316 + 30);
    }

    for (int i = 0; i < FEEDER_MEALn; i++)
    {
        menu->config[i].open_time.hour.y  = menu->win.main.y + (i+1)*(33);
        menu->config[i].open_time.min.y   = menu->win.main.y + (i+1)*(33);
        menu->config[i].open_time.am_fm.y   = menu->win.main.y + (i+1)*(33);

        menu->config[i].close_time.hour.y = menu->win.main.y + (i+1)*(33);
        menu->config[i].close_time.min.y  = menu->win.main.y + (i+1)*(33);
        menu->config[i].close_time.am_fm.y  = menu->win.main.y + (i+1)*(33);

        menu->config[i].date.day.y        = menu->win.main.y + (i+1)*(33);
        menu->config[i].date.month.y      = menu->win.main.y + (i+1)*(33);
    }


    for (int i = 0; i < FEEDER_MEALn; i++)
    {
        BSP_LCD_SetFont(&Font16);
        BSP_LCD_DisplayStringAt(menu->config[i].open_time.hour.x,
                                menu->config[i].open_time.hour.y, "--:", LEFT_MODE);
        BSP_LCD_DisplayStringAt(menu->config[i].open_time.min.x,
                                menu->config[i].open_time.min.y, "--", LEFT_MODE);
        BSP_LCD_DisplayStringAt(menu->config[i].open_time.am_fm.x,
                                menu->config[i].open_time.am_fm.y, set_am_fm[TIME_FM], LEFT_MODE);

        BSP_LCD_DisplayStringAt(menu->config[i].close_time.hour.x,
                                menu->config[i].close_time.hour.y, "--:", LEFT_MODE);
        BSP_LCD_DisplayStringAt(menu->config[i].close_time.min.x,
                                menu->config[i].close_time.min.y, "--", LEFT_MODE);
        BSP_LCD_DisplayStringAt(menu->config[i].close_time.am_fm.x,
                                menu->config[i].close_time.am_fm.y, set_am_fm[TIME_AM], LEFT_MODE);

        BSP_LCD_DisplayStringAt(menu->config[i].date.day.x,
                                menu->config[i].date.day.y, "--/", LEFT_MODE);
        BSP_LCD_DisplayStringAt(menu->config[i].date.month.x,
                                menu->config[i].date.month.y, "--", LEFT_MODE);
    }

    /*Drawer 1 Settings */
    ui_show_win(&menu->win.main, LCD_DEFAULT_TEXTCOLOR, true);
    ui_show_win(&menu->win.daily_en, LCD_DEFAULT_TEXTCOLOR, true);

    BSP_LCD_SetFont(&FEEDER_HEADER_FONT);
    for (int i = 0; i < 4; i++)
    {
        BSP_LCD_DisplayStringAt(feeder_header[i].x + menu->win.main.x,
                                feeder_header[i].y + menu->win.main.y,
                                feeder_header[i].name, LEFT_MODE);
    }

    for (int i = 0; i < FEEDER_MEALn; i++)
    {
        BSP_LCD_DisplayStringAt(feeder_meals[i].x + menu->win.main.x,
                                feeder_meals[i].y + menu->win.main.y,
                                feeder_meals[i].name, LEFT_MODE);
    }

    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
}


void ui_feeder_menu_set_config(feeder_menu_t *menu, feeder_config_t *fc)
{
    BSP_LCD_SetFont(&Font16);
    uint8_t buff[5];
    uint16_t color = LCD_DEFAULT_TEXTCOLOR;

    if (fc->select == UI_ITEM_SELECT)
        color = UI_SELECTION_COLOR;
    
    BSP_LCD_SetTextColor(color);

    switch (fc->set)
    {
        case FEEDER_CNF_OPEN_TIME_HOUR: {
            sprintf(buff, "%.2d", fc->time.hour);
            BSP_LCD_DisplayStringAt(menu->config[fc->meal].open_time.hour.x,
                                    menu->config[fc->meal].open_time.hour.y, buff, LEFT_MODE);
        } break;
        case FEEDER_CNF_OPEN_TIME_MIN: { 
            sprintf(buff, "%.2d", fc->time.minute);
            BSP_LCD_DisplayStringAt(menu->config[fc->meal].open_time.min.x,
                                    menu->config[fc->meal].open_time.min.y, buff, LEFT_MODE);
        } break;
        case FEEDER_CNF_OPEN_TIME_AM_FM: {
            BSP_LCD_DisplayStringAt(menu->config[fc->meal].open_time.am_fm.x,
                                    menu->config[fc->meal].open_time.am_fm.y, set_am_fm[fc->time.am_fm], LEFT_MODE);
        } break;
        case FEEDER_CNF_CLOSE_TIME_HOUR: {
            sprintf(buff, "%.2d", fc->time.hour);
            BSP_LCD_DisplayStringAt(menu->config[fc->meal].close_time.hour.x,
                                    menu->config[fc->meal].close_time.hour.y, buff, LEFT_MODE);
        } break;
        case FEEDER_CNF_CLOSE_TIME_MIN: { 
            sprintf(buff, "%.2d", fc->time.minute);
            BSP_LCD_DisplayStringAt(menu->config[fc->meal].close_time.min.x,
                                    menu->config[fc->meal].close_time.min.y, buff, LEFT_MODE);
        } break;
        case FEEDER_CNF_CLOSE_TIME_AM_FM: {
            BSP_LCD_DisplayStringAt(menu->config[fc->meal].close_time.am_fm.x,
                                    menu->config[fc->meal].close_time.am_fm.y, set_am_fm[fc->time.am_fm], LEFT_MODE);
        } break;
        case FEEDER_CNF_DATE_DAY: {
            sprintf(buff, "%.2d", fc->date.day);
            BSP_LCD_DisplayStringAt(menu->config[fc->meal].date.day.x,
                                    menu->config[fc->meal].date.day.y, buff, LEFT_MODE);
        }
        break;
        case FEEDER_CNF_DATE_MONTH: {
            sprintf(buff, "%.2d", fc->date.month);
            BSP_LCD_DisplayStringAt(menu->config[fc->meal].date.month.x,
                                    menu->config[fc->meal].date.month.y, buff, LEFT_MODE);
        }
        break;
        case FEEDER_CNF_DATE_DAILY: {

            ui_show_win(&menu->win.daily_en, color, true);

            if(fc->date.daily_en == DAILY_MEAL_ENABLE)
            {
                BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
                BSP_LCD_FillRect(menu->win.daily_en.x + 1, menu->win.daily_en.y + 1,
                                menu->win.daily_en.w - 2, menu->win.daily_en.h -2);
            }
            else
            {
                BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
                BSP_LCD_FillRect(menu->win.daily_en.x +1, menu->win.daily_en.y +1,
                                menu->win.daily_en.w -2, menu->win.daily_en.h -2);
            }

         } break;
    default:
        printf("no valid option for feeder schedule config");
        break;
    }

    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    BSP_LCD_SetTextColor(&LCD_DEFAULT_FONT);
}



/////////////////////////////////// Date Hour Related Functions /////////////////////////////////////////////

void ui_date_time_init(date_time_menu_t *menu)
{
    menu->win.main.x = 17;
    menu->win.main.y = 116;
    menu->win.main.w = 442;
    menu->win.main.h = 161;

    menu->time.hour.x = menu->win.main.x + 31;
    menu->time.hour.y = menu->win.main.y + 29;
    menu->time.min.x = menu->win.main.x + 191;
    menu->time.min.y = menu->win.main.y + 29;

    menu->date.day.x = menu->win.main.x + 347;
    menu->date.day.y = menu->win.main.y + 61;
    menu->date.month.x = menu->win.main.x + 389;
    menu->date.month.y = menu->win.main.y + 61;

    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(menu->time.hour.x, menu->time.hour.y, "--:", LEFT_MODE);
    BSP_LCD_DisplayStringAt(menu->time.min.x, menu->time.min.y, "--", LEFT_MODE);

    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(menu->date.day.x, menu->date.day.y, "--/", LEFT_MODE);
    BSP_LCD_DisplayStringAt(menu->date.month.x, menu->date.month.y, "--", LEFT_MODE);

    ui_show_win(&menu->win, LCD_DEFAULT_TEXTCOLOR, true);
}

void ui_date_time_set_config(date_time_menu_t *menu, date_time_config_t *conf)
{
    uint8_t buff[5];
    uint16_t color = LCD_DEFAULT_TEXTCOLOR;

    if (conf->select == UI_ITEM_SELECT)
        color = UI_SELECTION_COLOR;

    BSP_LCD_SetTextColor(color);

    switch (conf->item)
    {
    case DATE_TIME_CNF_HOUR: {
            BSP_LCD_SetFont(&Font24);
            sprintf(buff, "%.2d", conf->time.hour);
            BSP_LCD_DisplayStringAt(menu->time.hour.x,menu->time.hour.y,buff, LEFT_MODE);
    } break;

    case DATE_TIME_CNF_MIN: {
            BSP_LCD_SetFont(&Font24);
            sprintf(buff, "%.2d", conf->time.min);
            BSP_LCD_DisplayStringAt(menu->time.min.x,menu->time.min.y,buff, LEFT_MODE);
    } break;
    case DATE_TIME_CNF_DAY: {
            BSP_LCD_SetFont(&Font16);
            sprintf(buff, "%.2d", conf->date.day);
            BSP_LCD_DisplayStringAt(menu->date.day.x,menu->date.day.y,buff, LEFT_MODE); 
    } break;
    case DATE_TIME_CNF_MONTH: {
            BSP_LCD_SetFont(&Font16);
            sprintf(buff, "%.2d", conf->date.month);
            BSP_LCD_DisplayStringAt(menu->date.month.x,menu->date.month.y,buff, LEFT_MODE); 
    } break;
    default:
        break;
    }

    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
}


/////////////////////////////////// Drawers Related Functions /////////////////////////////////////////////

static void ui_drawers_clean(ui_drawers_t *drawers)
{
    ui_show_win(&drawers->form.main_frame, LCD_DEFAULT_TEXTCOLOR, true);

    for (uint8_t i = 0; i < DRAWERn; i++)
    {
        drawers->form.single_frame.x = drawers->form.main_frame.x + drawer_label[i].x;
        drawers->form.single_frame.y = drawers->form.main_frame.y + drawer_label[i].y;
        ui_show_win(&drawers->form.single_frame, LCD_DEFAULT_TEXTCOLOR, true);
        BSP_LCD_SetFont(&DRAWER_NUMBER_FONT);
        BSP_LCD_DisplayStringAt(drawers->form.single_frame.x + 15, drawers->form.single_frame.y + 4, drawer_label[i].name, LEFT_MODE);
    }
}

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
