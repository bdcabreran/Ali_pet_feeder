#include "ui_fsm.h"
#include "printf_dbg.h"
#include "buttons.h"
#include "time_event.h"
#include "ui_draw.h"

/**@brief Enable/Disable debug messages */
#define UI_FSM_DEBUG 0
#define UI_FSM_TAG "ui : "

/**@brief uart debug function for server comm operations  */
#if UI_FSM_DEBUG
#define ui_fsm_dbg(format, ...)         \
    do                                           \
    {											\
    	printf_dbg_color(E_YEL, UI_FSM_TAG); \
        printf_dbg(format,##__VA_ARGS__ );       \
    } while (0)
#else
#define ui_fsm_dbg(format, ...) \
	do                                       \
	{ /* Do nothing */                       \
	} while (0)
#endif


#define UPDATE_GUI_MS       (1000)

/**
 * @brief State Machine States
 */
typedef enum
{
    ST_UI_INVALID = 0,
    ST_UI_MAIN_MENU,
    ST_UI_DATE_TIME_CONFIG,
    ST_UI_PET_CALL_CONFIG,
    ST_UI_FEEDER_CONFIG,
    ST_UI_OPEN_DRAWER,
    ST_UI_LAST,
}ui_state_t;

typedef enum
{
    EVT_INT_UI_INVALID = 0,
    EVT_INT_UI_LAST
}ui_ev_int_t;

typedef struct
{
    time_event_t update_gui;
}ui_ev_time_t;

typedef struct 
{
    btn_ev_ext_t btn;
    ui_ev_int_t  internal;
    ui_ev_time_t time;
}ui_event_t;

typedef struct
{
    event_queue_t queue;
}ui_iface_t;

struct ui_fsm_t
{
    ui_state_t state;
    ui_event_t event;
    ui_iface_t iface;
};

static struct ui_fsm_t ui_fsm;

/////////////////////// Static state function declaration //////////////////////////////////////
static void main_menu_enter_seq(ui_handle_t handle);
static void entry_action_main_menu(ui_handle_t handle);
static void main_menu_on_react(ui_handle_t handle);


////////////////////////////// Public function declaration //////////////////////////////////////

ui_handle_t ui_fsm_get(void)
{
    return &ui_fsm;
}

void ui_fsm_init(ui_handle_t handle)
{
    // ui_battery_init(&ui_battery);
    // ui_drawers_init(&ui_drawers);
    ui_feeder_config_init(&ui_feeder_conf);

	main_menu_enter_seq(handle);
}

void ui_fsm_run(ui_handle_t handle)
{
    switch (handle->state)
    {
        case ST_UI_MAIN_MENU:  main_menu_on_react(handle); break;
        case ST_UI_DATE_TIME_CONFIG: break;
        case ST_UI_PET_CALL_CONFIG: break;
        case ST_UI_FEEDER_CONFIG: break;
        case ST_UI_OPEN_DRAWER: break;
    default:
        break;
    }

}

static void fsm_set_next_state(ui_handle_t handle, ui_state_t next_st)
{
    handle->state = next_st;
    handle->event.btn = EVT_EXT_BTN_INVALID;
    handle->event.internal = EVT_INT_UI_INVALID;
}

void ui_fsm_time_update(ui_handle_t handle)
{
	time_event_t *time_event = (time_event_t *)&handle->event;
	for (int tev_idx = 0; tev_idx < sizeof(handle->event.time) / sizeof(time_event_t); tev_idx++)
	{
		time_event_update(time_event);
		time_event++;
	}
}

void ui_fsm_set_ext_event(ui_handle_t handle, event_t *event)
{
    if(event->header.fsm_src == BTN_FSM)
    {
        ui_fsm_dbg("btn event detected\r\n");
        handle->event.btn = event->header.name;
    }
}



/////////////////////////////////// Main Menu State  ///////////////////////////////////////////

static void main_menu_enter_seq(ui_handle_t handle)
{
    ui_fsm_dbg("enter seq \t[ main menu ]");
    fsm_set_next_state(handle, ST_UI_MAIN_MENU);
    entry_action_main_menu(handle);
}

static void entry_action_main_menu(ui_handle_t handle)
{
    /*1. draw battery */
    // ui_battery_show(&ui_battery, true);
    /*1. put icons in the main screen with the updated values */
    /*2. put cursor in the first navigation item */
    /*3. start timer to update gui */
    ui_feeder_config_init(&ui_feeder_conf);

    for (size_t i = 0; i < FEEDER_MEALn; i++)
    {
        ui_feeder_config_set_open_time_hour(&ui_feeder_conf, i, i);
        HAL_Delay(500);
        ui_feeder_config_confirm_open_time_hour(&ui_feeder_conf, i, i);
        HAL_Delay(500);
        ui_feeder_config_set_open_time_min(&ui_feeder_conf, i, i + 30);
        HAL_Delay(500);
        ui_feeder_config_confirm_open_time_min(&ui_feeder_conf, i, i + 30);
        HAL_Delay(500);
        ui_feeder_config_set_am_fm(&ui_feeder_conf, i, TIME_AM);
        HAL_Delay(500);
        ui_feeder_config_confirm_am_fm(&ui_feeder_conf, i, TIME_AM);
        HAL_Delay(500);
    }

    time_event_start(&handle->event.time.update_gui, UPDATE_GUI_MS);
}

static void main_menu_on_react(ui_handle_t handle)
{
    /*navigation key update item selection*/
    switch (handle->event.btn)
    {
        case EVT_EXT_BTN_UP_PRESSED:              break;
        case EVT_EXT_BTN_DOWN_PRESSED:            break;
        case EVT_EXT_BTN_LEFT_PRESSED:            break;
        case EVT_EXT_BTN_RIGHT_PRESSED:           break;
        case EVT_EXT_BTN_ENTER_PRESSED:           break;
        case EVT_EXT_BTN_UP_AND_ENTER_PRESSED:    break;
        case EVT_EXT_BTN_DOWN_AND_ENTER_PRESSED:  break;
        case EVT_EXT_BTN_LEFT_AND_ENTER_PRESSED:  break;
        case EVT_EXT_BTN_RIGHT_AND_ENTER_PRESSED: break;  
    default:
        break;
    }

    if(time_event_is_raised(&handle->event.time.update_gui)== true)
    {
        /*Update GUI*/
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////// Drawing Functions  ///////////////////////////////////////////






