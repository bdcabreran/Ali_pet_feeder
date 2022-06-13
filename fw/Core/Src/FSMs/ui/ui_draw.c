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
    {"1", 7, 7},
    {"2", 66, 7},
    {"3", 7, 41},
    {"4", 66, 41}};

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
        {"Snack 3"  , 2, 33*6}
    };


void ui_win_show(ui_window_t *win, uint16_t color, bool show)
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

/////////////////////////////////// Feeder Scheduler Related Functions /////////////////////////////////////////////

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
    ui_win_show(&menu->win.main, LCD_DEFAULT_TEXTCOLOR, true);
    ui_win_show(&menu->win.daily_en, LCD_DEFAULT_TEXTCOLOR, true);

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
        color = FEEDER_CONFIG_SELECTION_COLOR;
    
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

            ui_win_show(&menu->win.daily_en, color, true);

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

    ui_win_show(&menu->win, LCD_DEFAULT_TEXTCOLOR, true);
}

void ui_date_time_set_config(date_time_menu_t *menu, date_time_config_t *conf)
{
    uint8_t buff[5];
    uint16_t color = LCD_DEFAULT_TEXTCOLOR;

    if (conf->select == UI_ITEM_SELECT)
        color = FEEDER_CONFIG_SELECTION_COLOR;

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
    ui_win_show(&drawers->form.main_frame, LCD_DEFAULT_TEXTCOLOR, true);

    for (uint8_t i = 0; i < DRAWERn; i++)
    {
        drawers->form.single_frame.x = drawers->form.main_frame.x + drawer_label[i].x;
        drawers->form.single_frame.y = drawers->form.main_frame.y + drawer_label[i].y;
        ui_win_show(&drawers->form.single_frame, LCD_DEFAULT_TEXTCOLOR, true);
        BSP_LCD_SetFont(&DRAWER_SELECTION_FONT);
        BSP_LCD_DisplayStringAt(drawers->form.single_frame.x + 15, drawers->form.single_frame.y + 4, drawer_label[i].name, LEFT_MODE);
    }
}

void ui_drawers_init(ui_drawers_t *drawers)
{
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

    ui_drawers_clean(drawers);
    // ui_win_show(&drawers->win.main, DRAWER_SELECTION_COLOR, true);
}

void ui_drawers_select(ui_drawers_t *drawers, uint8_t drawer_no)
{
    /*Clean All Drawers */
    ui_drawers_clean(drawers);

    drawers->form.single_frame.x = drawers->form.main_frame.x + drawer_label[drawer_no].x;
    drawers->form.single_frame.y = drawers->form.main_frame.y + drawer_label[drawer_no].y;
    ui_win_show(&drawers->form.single_frame, DRAWER_SELECTION_COLOR, true);
    BSP_LCD_SetTextColor(DRAWER_SELECTION_COLOR);
    BSP_LCD_SetFont(&DRAWER_SELECTION_FONT);

    BSP_LCD_DisplayStringAt(drawers->form.single_frame.x + 15, drawers->form.single_frame.y + 4, drawer_label[drawer_no].name, LEFT_MODE);
    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    BSP_LCD_SetFont(LCD_DEFAULT_TEXTCOLOR);
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
void ui_battery_init(ui_battery_t *batt)
{
    batt->win.main.x = 366;
    batt->win.main.y = 10;
    batt->win.main.h = 81;
    batt->win.main.w = 96;

    batt->icon.icon_batt.ptr = &battery_icon;
    batt->icon.icon_batt.x = batt->win.main.x + 4;
    batt->icon.icon_batt.y = batt->win.main.y + 16;

    batt->icon.icon_error.ptr = &batt_error_icon;
    batt->icon.icon_error.x = batt->icon.icon_batt.x + 16;
    batt->icon.icon_error.y = batt->icon.icon_batt.y + 16;

    batt->icon.icon_warn.ptr = &batt_warning_icon;
    batt->icon.icon_warn.x = batt->icon.icon_batt.x + 16;
    batt->icon.icon_warn.y = batt->icon.icon_batt.y + 16;

    batt->form.charge.x = batt->icon.icon_batt.x + 17;
    batt->form.charge.y = batt->icon.icon_batt.y + 8;
    batt->form.charge.h = batt->icon.icon_batt.ptr->infoHeader.biHeight - 16;
    batt->form.charge.w = batt->icon.icon_batt.ptr->infoHeader.biWidth - 24;

    batt->text.charge.font = &Font20;
    batt->text.charge.x = batt->form.charge.x + 15;
    batt->text.charge.y = batt->form.charge.y + 10;

    ui_win_show(&batt->win.main, LCD_DEFAULT_TEXTCOLOR, true);

}

void ui_battery_show(ui_battery_t *batt, bool show)
{
    if(show == true)
    {
        BSP_LCD_DrawBitmap(batt->icon.icon_batt.x, batt->icon.icon_batt.y, (uint8_t*)batt->icon.icon_batt.ptr);
    }
    else
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_FillRect(batt->win.main.x, batt->win.main.y, batt->win.main.w, batt->win.main.h);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    }
}

void ui_battery_icon_error(ui_battery_t *batt, bool show)
{
    if (show)
    {
        BSP_LCD_DrawBitmap(batt->icon.icon_error.x, batt->icon.icon_error.y, (uint8_t*)batt->icon.icon_error.ptr);
    }
    else
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_FillRect(batt->icon.icon_error.x, batt->icon.icon_error.y,
                         batt->icon.icon_error.ptr->infoHeader.biWidth,
                         batt->icon.icon_error.ptr->infoHeader.biHeight);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    }
}

void ui_battery_warn(ui_battery_t *batt, bool show)
{
    if (show)
    {
        BSP_LCD_DrawBitmap(batt->icon.icon_warn.x, batt->icon.icon_warn.y, (uint8_t*)batt->icon.icon_warn.ptr);
    }
    else
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_FillRect(batt->icon.icon_warn.x, batt->icon.icon_warn.y,
                         batt->icon.icon_warn.ptr->infoHeader.biWidth,
                         batt->icon.icon_warn.ptr->infoHeader.biHeight);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    }
}

void ui_battery_charge(ui_battery_t *batt, uint8_t batt_level, bool show)
{
    if (show)
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_FillRect(batt->form.charge.x, batt->form.charge.y, batt->form.charge.w, batt->form.charge.h);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);

        float width = batt->form.charge.w*(batt_level/100.0);

        BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
        BSP_LCD_FillRect(batt->form.charge.x, batt->form.charge.y, (int)width, batt->form.charge.h);
        BSP_LCD_SetBackColor(LCD_COLOR_GREEN);

        uint8_t buff[5] = {0};
        sprintf(buff, "%d%%", batt_level);

        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
        BSP_LCD_SetFont(batt->text.charge.font);
        BSP_LCD_DisplayStringAt(batt->text.charge.x, batt->text.charge.y, buff ,LEFT_MODE);
        BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
        BSP_LCD_SetBackColor(LCD_DEFAULT_BACKCOLOR);
    }
    else
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_FillRect(batt->form.charge.x, batt->form.charge.y, batt->form.charge.w, batt->form.charge.h);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    }
}
