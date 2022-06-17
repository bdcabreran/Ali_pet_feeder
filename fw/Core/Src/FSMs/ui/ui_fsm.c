#include "ui_fsm.h"
#include "printf_dbg.h"
#include "buttons.h"
#include "time_event.h"
#include "ui_draw.h"
#include "drawer_fsm.h"

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


#define UPDATE_GUI_MS       (500)

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
    ST_UI_DRAWER_ST_REQUEST,
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

    struct
    {
        ui_drawers_config_t drawers;
        ui_battery_config_t battery;
    }ui;

}ui_iface_t;

struct ui_fsm_t
{
    ui_state_t state;
    ui_event_t event;
    ui_iface_t iface;
};

static struct ui_fsm_t ui_fsm;

/////////////////////// Static state function declaration //////////////////////////////////////
static void enter_seq_main_menu(ui_handle_t handle);
static void entry_action_main_menu(ui_handle_t handle);
static void main_menu_on_react(ui_handle_t handle);

static void drawer_request_enter_seq(ui_handle_t handle);
static void entry_action_drawer_request(ui_handle_t handle);
static void drawer_request_on_react(ui_handle_t handle);


//////////////////////// Static Miscellaneous functions ////////////////////////////////////////
static void gui_update_battery(ui_handle_t handle);



////////////////////////////// Public function declaration //////////////////////////////////////

ui_handle_t ui_fsm_get(void)
{
    return &ui_fsm;
}

void ui_fsm_init(ui_handle_t handle)
{
	enter_seq_main_menu(handle);
}

void ui_fsm_run(ui_handle_t handle)
{
    switch (handle->state)
    {
        case ST_UI_MAIN_MENU:          main_menu_on_react(handle); break;
        case ST_UI_DATE_TIME_CONFIG: break;
        case ST_UI_PET_CALL_CONFIG: break;
        case ST_UI_FEEDER_CONFIG: break;
        case ST_UI_DRAWER_ST_REQUEST: drawer_request_on_react(handle); break;
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
	time_event_t *time_event = (time_event_t *)&handle->event.time;
	for (int tev_idx = 0; tev_idx < sizeof(handle->event.time) / sizeof(time_event_t); tev_idx++)
	{
		time_event_update(time_event);
		time_event++;
	}
}

void ui_fsm_write_event(ui_handle_t handle, event_t *event)
{
    if(event->fsm.src == BTN_FSM)
    {
        ui_fsm_dbg("btn event detected\r\n");
        handle->event.btn = event->name;
    }
}



/////////////////////////////////// Main Menu State  ///////////////////////////////////////////

static void enter_seq_main_menu(ui_handle_t handle)
{
    ui_fsm_dbg("enter seq \t[ main menu ]");
    fsm_set_next_state(handle, ST_UI_MAIN_MENU);
    entry_action_main_menu(handle);
}

static void entry_action_main_menu(ui_handle_t handle)
{
    /*Initialize UI elements */
    ui_battery_init(&ui_battery);
    ui_drawers_init(&ui_drawers);

//    ui_battery_init(&ui_thermostat);
//    ui_feeder_menu_init(&ui_feeder_menu);

    /*Show main menu elements */
    ui_battery_show(&ui_battery, true);
    ui_drawers_show(&ui_drawers, true);

    time_event_start(&handle->event.time.update_gui, UPDATE_GUI_MS);
}


static void main_menu_on_react(ui_handle_t handle)
{
    /*navigation key update item selection*/
	ui_drawers_config_t *drawer_cfg = &handle->iface.ui.drawers;

    switch (handle->event.btn)
    {

    case EVT_EXT_BTN_UP_PRESSED:
        break;
    case EVT_EXT_BTN_DOWN_PRESSED:
        break;
    case EVT_EXT_BTN_LEFT_PRESSED:
        break;
    case EVT_EXT_BTN_RIGHT_PRESSED:
        break;
    case EVT_EXT_BTN_ENTER_PRESSED:
        break;

    case EVT_EXT_BTN_UP_AND_ENTER_PRESSED:   {drawer_cfg->drawer.no = DRAWER_NO_1; drawer_request_enter_seq(handle); } break;
    case EVT_EXT_BTN_DOWN_AND_ENTER_PRESSED: {drawer_cfg->drawer.no = DRAWER_NO_2; drawer_request_enter_seq(handle); } break;
    case EVT_EXT_BTN_LEFT_AND_ENTER_PRESSED: {drawer_cfg->drawer.no = DRAWER_NO_3; drawer_request_enter_seq(handle); } break;
    case EVT_EXT_BTN_RIGHT_AND_ENTER_PRESSED:{drawer_cfg->drawer.no = DRAWER_NO_4; drawer_request_enter_seq(handle); } break;

    default:
        break;
    };

    if(time_event_is_raised(&handle->event.time.update_gui) == true)
    {
        time_event_start(&handle->event.time.update_gui, UPDATE_GUI_MS);
        gui_update_battery(handle);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// Drawer Request State  ///////////////////////////////////////////
void drawer_request_enter_seq(ui_handle_t handle)
{
    ui_fsm_dbg("enter seq \t[ drawer request ]");
    fsm_set_next_state(handle, ST_UI_DRAWER_ST_REQUEST);
    entry_action_drawer_request(handle);
}

static void notify_manual_drawer_operation(ui_handle_t handle)
{
    ui_drawers_config_t *drawer_cfg = &handle->iface.ui.drawers;
    drawer_ctrl_info *info = drawer_fsm_get_info(drawer_cfg->drawer.no);
    event_t event;

    event.name = EVT_EXT_DRW_INVALID;
    event.fsm.src = UI_FSM;
    event.fsm.dst = DRAWER_FSM;
    event.data.len = sizeof(drawer_ev_ext_data_t);
    ((drawer_ev_ext_data_t*)event.data.buff)->no = drawer_cfg->drawer.no;

    if (info->status.curr == DRAWER_ST_CLOSE || info->status.curr == DRAWER_ST_CLOSING)
    {
        ui_fsm_dbg("manual opening to drawer no [%d]\r\n", handle->iface.drawer + 1);
        event.name = EVT_EXT_DRW_OPEN;
    }

    else if (info->status.curr == DRAWER_ST_OPEN || info->status.curr == DRAWER_ST_OPENING)
    {
        ui_fsm_dbg("manual closing to drawer no [%d]\r\n", handle->iface.drawer + 1);
        event.name = EVT_EXT_DRW_CLOSE;
    }

    if(event.name != EVT_EXT_DRW_INVALID)
    {
        // event_manager_write(event_manager_fsm_get(), &event);
    }
}


void entry_action_drawer_request(ui_handle_t handle)
{
    /* Notify Drawer FSM */
    notify_manual_drawer_operation(handle);

    /* Update UI Elements */
    ui_drawers_config_t *ui_config = &handle->iface.ui.drawers;
    drawer_ctrl_info *info = drawer_fsm_get_info(ui_config->drawer.no);
    ui_config->drawer.st = info->status.curr;
    ui_config->select.single = UI_ITEM_SELECT;

    ui_drawers_set_config(&ui_drawers, ui_config);
}

void drawer_request_on_react(ui_handle_t handle)
{
    // no required actions 
    enter_seq_main_menu(handle);
}



/////////////////////////////////// Drawing Functions  ///////////////////////////////////////////

static void gui_update_battery(ui_handle_t handle)
{
    ui_fsm_dbg("update battery status \r\n");
    ui_battery_config_t *ui_config = &handle->iface.ui.battery;
    static uint8_t batt_dummy_val = 0;
    ui_config->select = UI_ITEM_DESELECT;
    ui_config->set    = BATT_ST_CHARGING;
    ui_config->charge = (batt_dummy_val++ % 100);
    ui_battery_set_config(&ui_battery, ui_config);
}
