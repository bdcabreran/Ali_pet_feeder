#include "ui_draw.h"

ui_battery_t ui_battery;


void ui_win_show(ui_window_t *win, bool show)
{
    if(show == true)
    {
        BSP_LCD_DrawRect(win->x, win->y, win->w, win->h);
    }
    else 
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_DrawRect(win->x, win->y, win->w, win->h);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    }
}

void ui_battery_init(ui_battery_t *batt)
{
    batt->win.x = 1;
    batt->win.y = 1;

    batt->cont.ptr = &battery_icon;
    batt->cont.x = batt->win.x + 2;
    batt->cont.y = batt->win.y + 2;

    batt->error.ptr = &batt_error_icon;
    batt->error.x = batt->cont.x + 16;
    batt->error.y = batt->cont.y + 16;

    batt->warn.ptr = &batt_warning_icon;
    batt->warn.x = batt->cont.x + 16;
    batt->warn.y = batt->cont.y + 16;

    batt->charge.x = batt->cont.x + 17;
    batt->charge.y = batt->cont.y + 8;
    batt->charge.h = batt->cont.ptr->infoHeader.biHeight - 16;
    batt->charge.w = batt->cont.ptr->infoHeader.biWidth - 24;

    batt->font.f = &Font16;
    batt->font.x = batt->charge.x + 15;
    batt->font.y = batt->charge.y + 10;

    batt->win.h = batt->cont.ptr->infoHeader.biHeight + 4;
    batt->win.w = batt->cont.ptr->infoHeader.biWidth + 4;
}

void ui_battery_show(ui_battery_t *batt, bool show)
{
    if(show == true)
    {
        BSP_LCD_DrawBitmap(batt->cont.x, batt->cont.y, (uint8_t*)batt->cont.ptr);
    }
    else
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_FillRect(batt->win.x, batt->win.y, batt->win.w, batt->win.h);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    }
}

void ui_battery_error(ui_battery_t *batt, bool show)
{
    if (show)
    {
        BSP_LCD_DrawBitmap(batt->error.x, batt->error.y, (uint8_t*)batt->error.ptr);
    }
    else
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_FillRect(batt->error.x, batt->error.y,
                         batt->error.ptr->infoHeader.biWidth,
                         batt->error.ptr->infoHeader.biHeight);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    }
}

void ui_battery_warn(ui_battery_t *batt, bool show)
{
    if (show)
    {
        BSP_LCD_DrawBitmap(batt->warn.x, batt->warn.y, (uint8_t*)batt->warn.ptr);
    }
    else
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_FillRect(batt->warn.x, batt->warn.y,
                         batt->warn.ptr->infoHeader.biWidth,
                         batt->warn.ptr->infoHeader.biHeight);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    }
}

void ui_battery_charge(ui_battery_t *batt, uint8_t batt_level, bool show)
{
    if (show)
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_FillRect(batt->charge.x, batt->charge.y, batt->charge.w, batt->charge.h);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);

        float width = batt->charge.w*(batt_level/100.0);

        BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
        BSP_LCD_FillRect(batt->charge.x, batt->charge.y, (int)width, batt->charge.h);
        BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
        
        uint8_t buff[5] = {0};
        sprintf(buff, "%d%%", batt_level);

        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
        BSP_LCD_SetFont(batt->font.f);
        BSP_LCD_DisplayStringAt(batt->font.x, batt->font.y, buff ,LEFT_MODE);
        BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
        BSP_LCD_SetBackColor(LCD_DEFAULT_BACKCOLOR);
    }
    else
    {
        BSP_LCD_SetTextColor(LCD_DEFAULT_BACKCOLOR);
        BSP_LCD_FillRect(batt->charge.x, batt->charge.y, batt->charge.w, batt->charge.h);
        BSP_LCD_SetTextColor(LCD_DEFAULT_TEXTCOLOR);
    }
}
