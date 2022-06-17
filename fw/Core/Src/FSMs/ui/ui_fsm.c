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


#define UPDATE_GUI_MS           (500)
#define CURSOR_INACTIVITY_MS    (2000)

/**
 * @brief State Machine States
 */
typedef enum
{
    ST_UI_INVALID = 0,
    ST_UI_MAIN_MENU,
    ST_UI_DATE_TIME_CONFIG,
    ST_UI_PETCALL_CONFIG,
    ST_UI_FEEDER_SELECT,
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
    time_event_t cursor_inact;
    time_event_t dummy;
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
        ui_drawers_config_t          drawers;
        ui_battery_config_t          battery;
        ui_petcall_config_t          petcall;
        ui_thermostat_config_t       therm;
        ui_feeder_config_t           feeder_menu;
        ui_date_time_config_t        dt_menu;
        ui_petcall_menu_config_t     petcall_menu;
        ui_thermostat_menu_config_t  therm_menu;
    }ui;

    struct
    {
        ui_main_menu_sel_item_t item;
    }cursor;

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
static void notify_manual_drawer_operation(ui_handle_t handle);
static void ui_update_item_selection(ui_handle_t handle, ui_select_t select);
static void main_menu_enter_pressed(ui_handle_t handle);

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
        case ST_UI_DATE_TIME_CONFIG:   break;
        case ST_UI_PETCALL_CONFIG:     break;
        case ST_UI_FEEDER_CONFIG:      break;
        case ST_UI_DRAWER_ST_REQUEST:  drawer_request_on_react(handle); break;
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
    if(event->info.fsm.src == BTN_FSM)
    {
        ui_fsm_dbg("btn event detected\r\n");
        handle->event.btn = event->info.name;
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
    ui_thermostat_init(&ui_thermostat);
   ui_petcall_init(&ui_petcall);
    ui_feeder_menu_init(&ui_feeder_menu);
    ui_date_time_init(&ui_date_time);

    /*Show main menu elements */
    ui_battery_show(&ui_battery, true);
    ui_drawers_show(&ui_drawers, true);
    ui_thermostat_show(&ui_thermostat, true);
    ui_petcall_show(&ui_petcall, true);
    ui_feeder_menu_show(&ui_feeder_menu, true);

    /* Set cursor to first item */
    time_event_start(&handle->event.time.update_gui, UPDATE_GUI_MS);
    time_event_stop(&handle->event.time.cursor_inact);
}


static void main_menu_on_react(ui_handle_t handle)
{
    /*navigation key update item selection*/
	ui_drawers_config_t *drawer_cfg = &handle->iface.ui.drawers;
    ui_main_menu_sel_item_t item = handle->iface.cursor.item;

    switch (handle->event.btn)
    {

    case EVT_EXT_BTN_UP_PRESSED:    break;
    case EVT_EXT_BTN_DOWN_PRESSED:  break;

    case EVT_EXT_BTN_LEFT_PRESSED:
    {
        ui_update_item_selection(handle, UI_ITEM_DESELECT);
        handle->iface.cursor.item = (item++ % UI_MAIN_MENU_ITEMn);
        ui_update_item_selection(handle, UI_ITEM_SELECT);
        time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
    } break;
        
    case EVT_EXT_BTN_RIGHT_PRESSED: {

        ui_update_item_selection(handle, UI_ITEM_DESELECT);
        handle->iface.cursor.item = (item-- % UI_MAIN_MENU_ITEMn);
        ui_update_item_selection(handle, UI_ITEM_SELECT);
        time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
    } break;

    case EVT_EXT_BTN_ENTER_PRESSED: { main_menu_enter_pressed(handle); } break;
    case EVT_EXT_BTN_UP_AND_ENTER_PRESSED:   {drawer_cfg->drawer.no = DRAWER_NO_1; drawer_request_enter_seq(handle); } break;
    case EVT_EXT_BTN_DOWN_AND_ENTER_PRESSED: {drawer_cfg->drawer.no = DRAWER_NO_2; drawer_request_enter_seq(handle); } break;
    case EVT_EXT_BTN_LEFT_AND_ENTER_PRESSED: {drawer_cfg->drawer.no = DRAWER_NO_3; drawer_request_enter_seq(handle); } break;
    case EVT_EXT_BTN_RIGHT_AND_ENTER_PRESSED:{drawer_cfg->drawer.no = DRAWER_NO_4; drawer_request_enter_seq(handle); } break;

    default:
        break;
    };

    /* update gui timer event */
    if(time_event_is_raised(&handle->event.time.update_gui) == true)
    {
        time_event_start(&handle->event.time.update_gui, UPDATE_GUI_MS);
        gui_update_battery(handle);
    }

    /* item cursor timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        ui_update_item_selection(handle, UI_ITEM_DESELECT);
        time_event_stop(&handle->event.time.cursor_inact);
        handle->iface.cursor.item = UI_MAIN_MENU_ITEM_BATTERY;
    }
    
}


/////////////////////////////////// Drawer Request State  ///////////////////////////////////////////
void drawer_request_enter_seq(ui_handle_t handle)
{
    ui_fsm_dbg("enter seq \t[ drawer request ]");
    fsm_set_next_state(handle, ST_UI_DRAWER_ST_REQUEST);
    entry_action_drawer_request(handle);
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

    time_event_start(&handle->event.time.dummy, 200);
}

void drawer_request_on_react(ui_handle_t handle)
{
    // no required actions 
    if(time_event_is_raised(&handle->event.time.dummy) == true)
    {
        ui_drawers_config_t *ui_config = &handle->iface.ui.drawers;;
        ui_config->select.single = UI_ITEM_DESELECT;
        ui_drawers_set_config(&ui_drawers, ui_config);
        enter_seq_main_menu(handle);
    }
}

/////////////////////////////////// Main Menu State  ///////////////////////////////////////////



/////////////////////////////////// Drawing Functions  ///////////////////////////////////////////

static void notify_manual_drawer_operation(ui_handle_t handle)
{
    ui_drawers_config_t *drawer_cfg = &handle->iface.ui.drawers;
    drawer_ctrl_info *info = drawer_fsm_get_info(drawer_cfg->drawer.no);
    event_t event;

    event.info.name = EVT_EXT_DRW_INVALID;
    event.info.fsm.src = UI_FSM;
    event.info.fsm.dst = DRAWER_FSM;
    event.info.data_len = sizeof(drawer_ev_ext_data_t);
    ((drawer_ev_ext_data_t*)event.data.buff)->no = drawer_cfg->drawer.no;

    if (info->status.curr == DRAWER_ST_CLOSE || info->status.curr == DRAWER_ST_CLOSING)
    {
        ui_fsm_dbg("manual opening to drawer no [%d]\r\n", handle->iface.drawer + 1);
        event.info.name = EVT_EXT_DRW_OPEN;
    }

    else if (info->status.curr == DRAWER_ST_OPEN || info->status.curr == DRAWER_ST_OPENING)
    {
        ui_fsm_dbg("manual closing to drawer no [%d]\r\n", handle->iface.drawer + 1);
        event.info.name = EVT_EXT_DRW_CLOSE;
    }

    if(event.info.name != EVT_EXT_DRW_INVALID)
    {
        event_manager_write(event_manager_fsm_get(), &event);
    }
}

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

static void main_menu_enter_pressed(ui_handle_t handle)
{
    /* define enter sequence according item selected */
    switch (handle->iface.cursor.item)
    {
    case UI_MAIN_MENU_ITEM_DRAWERS:    {  /*enter_sequence_drawer_selection() */} break;
    case UI_MAIN_MENU_ITEM_THERMOSTAT: {  /*enter_sequence_thermostat_menu() */ } break;
    case UI_MAIN_MENU_ITEM_PETCALL:    {  /*enter_sequence_petcall_menu() */    } break;
    case UI_MAIN_MENU_ITEM_BATTERY:    {  /*enter_sequence_battery_menu() */    } break;
    case UI_MAIN_MENU_ITEM_TIME_DATE:  {  /*enter_sequence_time_date_menu() */  } break;
    default:
        break;
    }
}

static void ui_update_item_selection(ui_handle_t handle, ui_select_t select)
{
    switch (handle->iface.cursor.item)
    {
    case UI_MAIN_MENU_ITEM_DRAWERS:
    {
        ui_drawers_config_t *config;
        config = &handle->iface.ui.drawers;
        config->select.main = select;
        ui_drawers_set_config(&ui_drawers, config);
    }
    break;

    case UI_MAIN_MENU_ITEM_THERMOSTAT:
    {
        ui_thermostat_config_t *config;
        config = &handle->iface.ui.therm;
        config->select = select;
        ui_thermostat_set_config(&ui_thermostat, config);
    }
    break;

    case UI_MAIN_MENU_ITEM_PETCALL:
    {
        ui_petcall_config_t *config;
        config = &handle->iface.ui.petcall;
        config->select = select;
        ui_petcall_set_config(&ui_petcall, config);
    }
    break;

    case UI_MAIN_MENU_ITEM_BATTERY:
    {
        ui_battery_config_t *config;
        config = &handle->iface.ui.battery;
        config->select = select;
        ui_battery_set_config(&ui_battery, config);
    }
    break;

    case UI_MAIN_MENU_ITEM_TIME_DATE:
    {
        ui_date_time_config_t *config;
        config = &handle->iface.ui.dt_menu;
        config->select = select;
        ui_date_time_set_config(&ui_date_time, config);
    }
    break;

    default:
        break;
    }
}
