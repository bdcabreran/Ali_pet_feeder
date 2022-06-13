#include "ui_draw.h"

ui_battery_t ui_battery;
ui_thermostat_t ui_thermostat;
ui_drawers_t ui_drawers;
ui_date_time_t ui_date_time;
ui_thermostat_config_t ui_therm_conf;
feeder_config_t ui_feeder_conf;

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
    {"OPEN TIME",  102, 2},
    {"CLOSE TIME", 208, 2},
    {"DATE",       315, 2},
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
static void ui_feeder_config_clean(feeder_config_t *feeder_conf)
{
    BSP_LCD_SetFont(&FEEDER_HEADER_FONT);
    for (int i = 0; i < 4; i++)
    {
        BSP_LCD_DisplayStringAt(feeder_header[i].x, feeder_header[i].y, feeder_header[i].name, LEFT_MODE);
    }

    for (int i = 0; i < FEEDER_MEALn; i++)
    {
        BSP_LCD_DisplayStringAt(feeder_meals[i].x, feeder_meals[i].y, feeder_meals[i].name, LEFT_MODE);
    }
}

void ui_feeder_config_init(feeder_config_t *feeder_conf)
{
    feeder_conf->win.main.x = 1;
    feeder_conf->win.main.y = 1;
    feeder_conf->win.main.w = 442;
    feeder_conf->win.main.h = 216;

    for (int i = 0; i < FEEDER_MEALn; i++)
    {
        feeder_conf->config[i].open_time.hour.x    = feeder_conf->win.main.x + (10 + 102);
        feeder_conf->config[i].open_time.min.x     = feeder_conf->win.main.x + (10 + 102 + 30);
        feeder_conf->config[i].open_time.am_fm.x   = feeder_conf->win.main.x + (10 + 102 + 50);

        feeder_conf->config[i].close_time.hour.x   = feeder_conf->win.main.x + (10 + 209);
        feeder_conf->config[i].close_time.min.x    = feeder_conf->win.main.x + (10 + 209 + 30);
        feeder_conf->config[i].close_time.am_fm.x  = feeder_conf->win.main.x + (10 + 209 + 50);

        feeder_conf->config[i].date.day.x        = feeder_conf->win.main.x +   (10 + 316);
        feeder_conf->config[i].date.month.x      = feeder_conf->win.main.x +   (10 + 316 + 20);
    }

    for (int i = 0; i < FEEDER_MEALn; i++)
    {
        feeder_conf->config[i].open_time.hour.y  = feeder_conf->win.main.y + (i+1)*(33);
        feeder_conf->config[i].open_time.min.y   = feeder_conf->win.main.y + (i+1)*(33);
        feeder_conf->config[i].open_time.am_fm.y   = feeder_conf->win.main.y + (i+1)*(33);

        feeder_conf->config[i].close_time.hour.y = feeder_conf->win.main.y + (i+1)*(33);
        feeder_conf->config[i].close_time.min.y  = feeder_conf->win.main.y + (i+1)*(33);
        feeder_conf->config[i].close_time.am_fm.y  = feeder_conf->win.main.y + (i+1)*(33);

        feeder_conf->config[i].date.day.y        = feeder_conf->win.main.y + (i+1)*(33);
        feeder_conf->config[i].date.month.y      = feeder_conf->win.main.y + (i+1)*(33);
    }


    for (int i = 0; i < FEEDER_MEALn; i++)
    {
        BSP_LCD_SetFont(&Font16);
        BSP_LCD_DisplayStringAt(feeder_conf->config[i].open_time.hour.x,
                                feeder_conf->config[i].open_time.hour.y, "--:", LEFT_MODE);
        BSP_LCD_DisplayStringAt(feeder_conf->config[i].open_time.min.x,
                                feeder_conf->config[i].open_time.min.y, "--", LEFT_MODE);
        BSP_LCD_DisplayStringAt(feeder_conf->config[i].open_time.am_fm.x,
                                feeder_conf->config[i].open_time.am_fm.y, set_am_fm[TIME_FM], LEFT_MODE);

        BSP_LCD_DisplayStringAt(feeder_conf->config[i].close_time.hour.x,
                                feeder_conf->config[i].close_time.hour.y, "--:", LEFT_MODE);
        BSP_LCD_DisplayStringAt(feeder_conf->config[i].close_time.min.x,
                                feeder_conf->config[i].close_time.min.y, "--", LEFT_MODE);
        BSP_LCD_DisplayStringAt(feeder_conf->config[i].close_time.am_fm.x,
                                feeder_conf->config[i].close_time.am_fm.y, set_am_fm[TIME_AM], LEFT_MODE);

        BSP_LCD_DisplayStringAt(feeder_conf->config[i].date.day.x,
                                feeder_conf->config[i].date.day.y, "--/", LEFT_MODE);
        BSP_LCD_DisplayStringAt(feeder_conf->config[i].date.month.x,
                                feeder_conf->config[i].date.month.y, "--", LEFT_MODE);

    }

    /*Drawer 1 Settings */
    ui_win_show(&feeder_conf->win.main, LCD_DEFAULT_TEXTCOLOR, true);
    ui_feeder_config_clean(feeder_conf);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
}

void ui_feeder_config_set_open_time_hour(feeder_config_t *feeder_conf, uint8_t meal, uint8_t open_hour)
{
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(FEEDER_CONFIG_SELECTION_COLOR);

    uint8_t tmp_buff[5];
    sprintf(tmp_buff,"%.2d",open_hour);

    BSP_LCD_DisplayStringAt(feeder_conf->config[meal].open_time.hour.x,
                            feeder_conf->config[meal].open_time.hour.y, tmp_buff, LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    BSP_LCD_SetTextColor(&LCD_DEFAULT_FONT);
}

void ui_feeder_config_confirm_open_time_hour(feeder_config_t *feeder_conf, uint8_t meal, uint8_t open_hour)
{
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);

    uint8_t tmp_buff[5];
    sprintf(tmp_buff,"%.2d",open_hour);

    BSP_LCD_DisplayStringAt(feeder_conf->config[meal].open_time.hour.x,
                            feeder_conf->config[meal].open_time.hour.y, tmp_buff, LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    BSP_LCD_SetTextColor(&LCD_DEFAULT_FONT);
}

void ui_feeder_config_set_open_time_min(feeder_config_t *feeder_conf, uint8_t meal, uint8_t open_min)
{
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(FEEDER_CONFIG_SELECTION_COLOR);

    uint8_t tmp_buff[5];
    sprintf(tmp_buff,"%.2d",open_min);

    BSP_LCD_DisplayStringAt(feeder_conf->config[meal].open_time.min.x,
                            feeder_conf->config[meal].open_time.min.y, tmp_buff, LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    BSP_LCD_SetTextColor(&LCD_DEFAULT_FONT);
}

void ui_feeder_config_confirm_open_time_min(feeder_config_t *feeder_conf, uint8_t meal, uint8_t open_min)
{
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);

    uint8_t tmp_buff[5];
    sprintf(tmp_buff,"%.2d",open_min);

    BSP_LCD_DisplayStringAt(feeder_conf->config[meal].open_time.min.x,
                            feeder_conf->config[meal].open_time.min.y, tmp_buff, LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    BSP_LCD_SetTextColor(&LCD_DEFAULT_FONT);
}


void ui_feeder_config_set_am_fm(feeder_config_t *feeder_conf, uint8_t meal, uint8_t am_fm)
{
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(FEEDER_CONFIG_SELECTION_COLOR);

    BSP_LCD_DisplayStringAt(feeder_conf->config[meal].open_time.am_fm.x,
                            feeder_conf->config[meal].open_time.am_fm.y, set_am_fm[am_fm], LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    BSP_LCD_SetTextColor(&LCD_DEFAULT_FONT);
}

void ui_feeder_config_confirm_am_fm(feeder_config_t *feeder_conf, uint8_t meal, uint8_t am_fm)
{
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);

    BSP_LCD_DisplayStringAt(feeder_conf->config[meal].open_time.am_fm.x,
                            feeder_conf->config[meal].open_time.am_fm.y, set_am_fm[am_fm], LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    BSP_LCD_SetTextColor(&LCD_DEFAULT_FONT);
}

void ui_feeder_config_set_close_time(feeder_config_t *feeder_conf,  uint8_t meal, uint8_t open_time)
{

}

void ui_feeder_config_set_date_month(feeder_config_t *feeder_conf,  uint8_t meal, uint8_t month)
{

}

void ui_feeder_config_set_date_day(feeder_config_t *feeder_conf,  uint8_t meal, uint8_t day)
{

}


/////////////////////////////////// Date Hour Related Functions /////////////////////////////////////////////

void ui_date_time_init(ui_date_time_t *date_time)
{

}

void ui_date_time_set_hour(ui_date_time_t *date_time, bool show)
{
    if(show)
    {

    }
    else
    {
        
    }
}

void ui_date_time_set_minute(ui_date_time_t *date_time, bool show)
{
    if(show)
    {

    }
    else
    {
        
    }
}

void ui_date_time_set_month(ui_date_time_t *date_time, bool show)
{
    if(show)
    {

    }
    else
    {
        
    }
}

void ui_date_time_set_day(ui_date_time_t *date_time, bool show)
{
    if(show)
    {

    }
    else
    {
        
    }
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
    drawers->win.main.x = 5;
    drawers->win.main.y = 5;
    drawers->win.main.h = 81;
    drawers->win.main.w = 132;

    drawers->form.main_frame.x = drawers->win.main.x + 4;
    drawers->form.main_frame.y = drawers->win.main.y + 4;
    drawers->form.main_frame.h = drawers->win.main.h - 8;
    drawers->form.main_frame.w = drawers->win.main.w - 8;

    drawers->form.single_frame.w = 50;
    drawers->form.single_frame.h = 25;

    ui_drawers_clean(drawers);
    ui_win_show(&drawers->win.main, DRAWER_SELECTION_COLOR, true);
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
    batt->win.main.x = 1;
    batt->win.main.y = 1;

    batt->icon.icon_batt.ptr = &battery_icon;
    batt->icon.icon_batt.x = batt->win.main.x + 2;
    batt->icon.icon_batt.y = batt->win.main.y + 2;

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

    batt->text.charge.font = &Font16;
    batt->text.charge.x = batt->form.charge.x + 15;
    batt->text.charge.y = batt->form.charge.y + 10;

    batt->win.main.h = batt->icon.icon_batt.ptr->infoHeader.biHeight + 4;
    batt->win.main.w = batt->icon.icon_batt.ptr->infoHeader.biWidth + 4;
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
