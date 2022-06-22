#include "ui_fsm.h"
#include "printf_dbg.h"
#include "buttons.h"
#include "time_event.h"
#include "ui_draw.h"
#include "drawer_fsm.h"
#include "string.h"

/**@brief Enable/Disable debug messages */
#define UI_FSM_DEBUG 1
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


#define UPDATE_GUI_MS           (1000)
#define CURSOR_INACTIVITY_MS    (7000)
#define NOTIFICATION_MSG_MS     (2000)

/**
 * @brief State Machine States
 */
typedef enum
{
    ST_UI_INVALID = 0,
    ST_UI_MAIN_MENU,
    ST_UI_DRAWER_SELECT,
    ST_UI_FEEDER_CONFIG,
    ST_UI_DRAWER_ST_REQUEST,
    ST_UI_DATE_TIME_CONFIG,
    ST_UI_PETCALL_CONFIG,
    ST_UI_THERM_CONFIG,
    ST_UI_BATT_CONFIG,
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
        ui_feeder_config_t           feeder_menu[DRAWERn];
        ui_date_time_config_t        dt_menu;
        ui_petcall_menu_config_t     petcall_menu;
        ui_thermostat_menu_config_t  therm_menu;
    }ui;

    struct
    {
        uint8_t item;
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

static void enter_seq_drawer_request(ui_handle_t handle);
static void entry_action_drawer_request(ui_handle_t handle);
static void drawer_request_on_react(ui_handle_t handle);

static void enter_seq_drawer_select(ui_handle_t handle);
static void entry_action_drawer_select(ui_handle_t handle);
static void drawer_select_on_react(ui_handle_t handle);

static void enter_seq_feeder_config(ui_handle_t handle);
static void entry_action_feeder_config(ui_handle_t handle);
static void exit_action_feeder_config(ui_handle_t handle);
static void feeder_config_on_react(ui_handle_t handle);

static void enter_seq_therm_config(ui_handle_t handle);
static void entry_action_therm_config(ui_handle_t handle);
static void therm_config_on_react(ui_handle_t handle);

static void enter_seq_petcall_config(ui_handle_t handle);
static void entry_action_petcall_config(ui_handle_t handle);
static void petcall_config_on_react(ui_handle_t handle);

static void enter_seq_battery_config(ui_handle_t handle);
static void entry_action_battery_config(ui_handle_t handle);
static void battery_config_on_react(ui_handle_t handle);

static void enter_seq_date_time_config(ui_handle_t handle);
static void entry_action_date_time_config(ui_handle_t handle);
static void date_time_config_on_react(ui_handle_t handle);


//////////////////////// Static Miscellaneous functions ////////////////////////////////////////
static void ui_update_battery(ui_handle_t handle);
// static void ui_update_date_time(ui_handle_t handle);
static void ui_update_thermostat(ui_handle_t handle);
static void notify_manual_drawer_operation(ui_handle_t handle);
static void ui_update_item_selection(ui_handle_t handle, ui_select_t select);

static void main_menu_enter_pressed(ui_handle_t handle);
static void feeder_config_enter_pressed(ui_handle_t handle);
static void drawer_select_enter_pressed(ui_handle_t handle);
static void drawer_select_key_pressed(ui_handle_t handle);


static void time_config_increase_hour( uint8_t *hour);
static void time_config_decrease_hour(uint8_t *hour);
static void time_config_increase_min(uint8_t *min);
static void time_config_decrease_min(uint8_t *min);
static void date_config_increase_day(uint8_t *day);
static void date_config_decrease_day(uint8_t *day);
static void date_config_increase_month(uint8_t *month);
static void date_config_decrease_month(uint8_t *month);

////////////////////////////// Public function declaration //////////////////////////////////////

ui_handle_t ui_fsm_get(void)
{
    return &ui_fsm;
}

void ui_fsm_init(ui_handle_t handle)
{
    /*Initialize UI elements */
    ui_battery_init(&ui_battery);
    ui_drawers_init(&ui_drawers);
    ui_thermostat_init(&ui_thermostat);
    ui_petcall_init(&ui_petcall);
    ui_feeder_menu_init(&ui_feeder_menu);
    ui_date_time_init(&ui_date_time);
    ui_notification_msg_init(&ui_notification);

    /*display fixed elements in the Screen */
    ui_battery_show(&ui_battery, true);
    ui_drawers_show(&ui_drawers, true);
    ui_thermostat_show(&ui_thermostat, true);
    ui_petcall_show(&ui_petcall, true);

    // thermostat demo
    ui_update_thermostat(handle);

	enter_seq_main_menu(handle);
}

void ui_fsm_run(ui_handle_t handle)
{
    switch (handle->state)
    {
        case ST_UI_MAIN_MENU:          main_menu_on_react(handle);         break;
        case ST_UI_DATE_TIME_CONFIG:   date_time_config_on_react(handle);  break;
        case ST_UI_PETCALL_CONFIG:     petcall_config_on_react(handle);    break;
        case ST_UI_FEEDER_CONFIG:      feeder_config_on_react(handle);     break;
        case ST_UI_DRAWER_SELECT:      drawer_select_on_react(handle);     break;
        case ST_UI_DRAWER_ST_REQUEST:  drawer_request_on_react(handle);    break;
        case ST_UI_THERM_CONFIG:       therm_config_on_react(handle);      break;
        case ST_UI_BATT_CONFIG:        battery_config_on_react(handle);    break;

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
        // ui_fsm_dbg("btn event detected\r\n");
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
    // show date time 
    ui_date_time_show(&ui_date_time, true);
    ui_update_battery(&ui_battery);

    /* Set cursor to first item */
    time_event_start(&handle->event.time.update_gui, UPDATE_GUI_MS);
    time_event_stop(&handle->event.time.cursor_inact);
}

static void main_menu_button_pressed(ui_handle_t handle)
{
    /*navigation key update item selection*/
	ui_drawers_config_t *drawer_cfg = &handle->iface.ui.drawers;

    switch (handle->event.btn)
    {

    case EVT_EXT_BTN_UP_PRESSED:    {/*Up action in main menu */  } break;
    case EVT_EXT_BTN_DOWN_PRESSED:  {/*Down action in main menu */} break;

    case EVT_EXT_BTN_LEFT_PRESSED:
    {
        /*deselect previous item */
        ui_update_item_selection(handle, UI_ITEM_DESELECT);

        /*update item pointer */
        if(handle->iface.cursor.item > 0)
        	handle->iface.cursor.item -= 1;
		else
			handle->iface.cursor.item = UI_MAIN_MENU_ITEM_DATE_TIME;

        /*select new item */
        ui_update_item_selection(handle, UI_ITEM_SELECT);
        time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
    } break;
        
    case EVT_EXT_BTN_RIGHT_PRESSED: {
        ui_update_item_selection(handle, UI_ITEM_DESELECT);

        handle->iface.cursor.item += 1;
        handle->iface.cursor.item %= UI_MAIN_MENU_ITEMn;

        ui_update_item_selection(handle, UI_ITEM_SELECT);
        time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
    } break;

    /*item menu enter */
    case EVT_EXT_BTN_ENTER_PRESSED: { main_menu_enter_pressed(handle); } break;

    /*open/close drawer request */
    case EVT_EXT_BTN_UP_AND_ENTER_PRESSED:   {drawer_cfg->drawer.no = DRAWER_NO_1; enter_seq_drawer_request(handle); } break;
    case EVT_EXT_BTN_DOWN_AND_ENTER_PRESSED: {drawer_cfg->drawer.no = DRAWER_NO_2; enter_seq_drawer_request(handle); } break;
    case EVT_EXT_BTN_LEFT_AND_ENTER_PRESSED: {drawer_cfg->drawer.no = DRAWER_NO_3; enter_seq_drawer_request(handle); } break;
    case EVT_EXT_BTN_RIGHT_AND_ENTER_PRESSED:{drawer_cfg->drawer.no = DRAWER_NO_4; enter_seq_drawer_request(handle); } break;

    default:
        break;
    };
}


static void main_menu_on_react(ui_handle_t handle)
{
    /*process button events */
    if (handle->event.btn != EVT_EXT_BTN_INVALID)
    {
        main_menu_button_pressed(handle);
        handle->event.btn = EVT_EXT_BTN_INVALID;
    }

    /* update gui timer event */
    if(time_event_is_raised(&handle->event.time.update_gui) == true)
    {
        time_event_start(&handle->event.time.update_gui, UPDATE_GUI_MS);
        ui_update_battery(handle);
    }

    /* item cursor timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        ui_update_item_selection(handle, UI_ITEM_DESELECT);
        time_event_stop(&handle->event.time.cursor_inact);
        handle->iface.cursor.item = UI_MAIN_MENU_ITEM_DATE_TIME;
    }
    
}


/////////////////////////////////// Drawer Request State  ///////////////////////////////////////////
void enter_seq_drawer_request(ui_handle_t handle)
{
    ui_fsm_dbg("enter seq \t[ drawer request ]");
    fsm_set_next_state(handle, ST_UI_DRAWER_ST_REQUEST);
    entry_action_drawer_request(handle);
}

void entry_action_drawer_request(ui_handle_t handle)
{
    /* Show message in the screen */
    ui_date_time_show(&ui_date_time, false);
    ui_notification_msg_show(&ui_notification, true);

    /* Notify Drawer FSM */
    notify_manual_drawer_operation(handle);
    
    /* Update UI Elements */
    ui_drawers_config_t *ui_config = &handle->iface.ui.drawers;
    drawer_ctrl_info_t *info = drawer_fsm_get_info(ui_config->drawer.no);
    ui_config->drawer.st = info->status.curr;
    ui_config->select.single = UI_ITEM_SELECT;
    ui_drawers_set_config(&ui_drawers, ui_config);

    time_event_start(&handle->event.time.dummy, 3000);
}

void drawer_request_on_react(ui_handle_t handle)
{
    // no required actions 
    if(time_event_is_raised(&handle->event.time.dummy) == true)
    {
        ui_notification_msg_show(&ui_notification, false);
        ui_drawers_config_t *ui_config = &handle->iface.ui.drawers;;
        ui_config->select.single = UI_ITEM_DESELECT;
        ui_drawers_set_config(&ui_drawers, ui_config);
        enter_seq_main_menu(handle);
    }
}

/////////////////////////////////// Feeder Select State  ///////////////////////////////////////////
void enter_seq_drawer_select(ui_handle_t handle)
{
    ui_fsm_dbg("\t enter seq [ drawer select ] \r\n");
    fsm_set_next_state(handle, ST_UI_DRAWER_SELECT);
    entry_action_drawer_select(handle);
}
static void entry_action_drawer_select(ui_handle_t handle)
{
    /*deselect main window */
    ui_update_item_selection(handle, UI_ITEM_DESELECT);

    /*select first element */
    ui_drawers_config_t *config = &handle->iface.ui.drawers;
    handle->iface.cursor.item = DRAWER_NO_1;
    config->drawer.no = handle->iface.cursor.item;
    config->select.single = UI_ITEM_SELECT;
    ui_drawers_set_config(&ui_drawers, config);

    /*wait up to 5s for user input*/
    time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS );
}
static void drawer_select_on_react(ui_handle_t handle)
{
//    uint8_t item = handle->iface.cursor.item;

    switch (handle->event.btn)
    {

    case EVT_EXT_BTN_LEFT_PRESSED : 
    case EVT_EXT_BTN_RIGHT_PRESSED: {
        drawer_select_key_pressed(handle);
        time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
    } break;

    case EVT_EXT_BTN_UP_PRESSED:    break;
    case EVT_EXT_BTN_DOWN_PRESSED:  break;
    case EVT_EXT_BTN_ENTER_PRESSED: { drawer_select_enter_pressed(handle); } break;
    
    default:
        break;
    };

    if (handle->event.btn != EVT_EXT_BTN_INVALID)
        handle->event.btn = EVT_EXT_BTN_INVALID;

    /* item cursor inactivity timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        /*clean last selection */
        ui_drawers_config_t *config = &handle->iface.ui.drawers;
        config->select.single = UI_ITEM_DESELECT;
        ui_drawers_set_config(&ui_drawers, config);
        time_event_stop(&handle->event.time.cursor_inact);

        /*go to main menu */
        enter_seq_main_menu(handle);
    }
}

/////////////////////////////////// Feeder Config State  ///////////////////////////////////////////

static void enter_seq_feeder_config(ui_handle_t handle)
{
    ui_fsm_dbg("\t enter seq [ feeder config ] \r\n");
    fsm_set_next_state(handle, ST_UI_FEEDER_CONFIG);
    entry_action_feeder_config(handle);
}

static void entry_action_feeder_config(ui_handle_t handle)
{
    ui_date_time_show(&ui_date_time, false);
    ui_feeder_menu_show(&ui_feeder_menu, true);

    ui_feeder_config_t *config = &handle->iface.ui.feeder_menu[handle->iface.ui.drawers.drawer.no];

    /*load previous configuration */

    for (int i = FEEDER_CNF_OPEN_TIME_HOUR; i < FEEDER_CNFn; i++)
    {
        for (size_t j = FEEDER_MEAL_BREAKFAST; j < FEEDER_MEALn; j++)
        {
            config->set = i;
            config->meal = j;
            config->select.single = UI_ITEM_DESELECT;
            ui_feeder_menu_set_config(&ui_feeder_menu, config);        
        }
    }
    
    handle->iface.cursor.item = FEEDER_CNF_OPEN_TIME_HOUR;
    config->meal = FEEDER_MEAL_BREAKFAST;
    config->set = handle->iface.cursor.item;
    config->select.single = UI_ITEM_SELECT;
    config->date.daily_st = FEEDER_DAILY_MEAL_DISABLE;

    ui_feeder_menu_set_config(&ui_feeder_menu, config);

    /*wait up to 5s for user input*/
    time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS );
}

static void exit_action_feeder_config(ui_handle_t handle)
{
    ui_feeder_menu_show(&ui_feeder_menu, false);
}

static void feeder_config_on_react(ui_handle_t handle)
{
	drawer_no_t drawer_no = handle->iface.ui.drawers.drawer.no;

    switch (handle->event.btn)
    {

    case EVT_EXT_BTN_LEFT_PRESSED:
    {

        ui_feeder_config_t *config = &handle->iface.ui.feeder_menu[drawer_no];
        config->select.single = UI_ITEM_DESELECT;
        ui_feeder_menu_set_config(&ui_feeder_menu, config);

        if (handle->iface.cursor.item > FEEDER_CNF_OPEN_TIME_HOUR)
            handle->iface.cursor.item--;
        else
            handle->iface.cursor.item =  FEEDER_CNF_DATE_DAILY;

        config->set = handle->iface.cursor.item;
        config->select.single = UI_ITEM_SELECT;
        ui_feeder_menu_set_config(&ui_feeder_menu, config);

        if (handle->iface.cursor.item == FEEDER_CNF_DATE_DAILY)
        {
            if (config->meal > FEEDER_MEAL_BREAKFAST)
                config->meal--;
            else
                config->meal = FEEDER_MEAL_SNACK_3;
        };


        time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
    }
    break;

    case EVT_EXT_BTN_RIGHT_PRESSED:
    {

        ui_feeder_config_t *config = &handle->iface.ui.feeder_menu[drawer_no];
        config->select.single = UI_ITEM_DESELECT;
        ui_feeder_menu_set_config(&ui_feeder_menu, config);

        handle->iface.cursor.item++;
        handle->iface.cursor.item %= FEEDER_CNFn;
        config->set = handle->iface.cursor.item;
        config->select.single = UI_ITEM_SELECT;
        ui_feeder_menu_set_config(&ui_feeder_menu, config);

        if (handle->iface.cursor.item == FEEDER_CNF_DATE_DAILY)
        {
            config->meal++;
            config->meal %= FEEDER_MEALn;
        };

        time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
    }
    break;

    case EVT_EXT_BTN_UP_PRESSED:
    {
        /*increase item value */
        uint8_t drawer_no = handle->iface.ui.drawers.drawer.no;
       ui_feeder_config_t *config = &handle->iface.ui.feeder_menu[drawer_no];

        switch (handle->iface.cursor.item)
        {
        case FEEDER_CNF_OPEN_TIME_HOUR:
        {
            time_config_increase_hour(&config->time.open.hour);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_OPEN_TIME_MIN:
        {
            time_config_increase_min(&config->time.open.minute);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_OPEN_TIME_AM_FM:
        {
            config->time.open.am_fm = !config->time.open.am_fm;
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_CLOSE_TIME_HOUR:
        {
            time_config_increase_hour(&config->time.close.hour);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_CLOSE_TIME_MIN:
        {
            time_config_increase_min(&config->time.close.minute);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_CLOSE_TIME_AM_FM:
        {
            config->time.close.am_fm = !config->time.close.am_fm;
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_DATE_DAY:
        {
            date_config_increase_day(&config->date.day);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_DATE_MONTH:
        {
            date_config_increase_month(&config->date.month);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_DATE_DAILY:
        { 
            config->date.daily_st = !config->date.daily_st;
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
           } break;
        default:
            break;
        }

        time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);

    }   
    break;

    case EVT_EXT_BTN_DOWN_PRESSED:  
    {
        /*increase item value */
        uint8_t drawer_no = handle->iface.ui.drawers.drawer.no;
       ui_feeder_config_t *config = &handle->iface.ui.feeder_menu[drawer_no];

        switch (handle->iface.cursor.item)
        {
        case FEEDER_CNF_OPEN_TIME_HOUR:
        {
            time_config_decrease_hour(&config->time.open.hour);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_OPEN_TIME_MIN:
        {
            time_config_decrease_min(&config->time.open.minute);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_OPEN_TIME_AM_FM:
        {
            config->time.open.am_fm = !config->time.open.am_fm;
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_CLOSE_TIME_HOUR:
        {
            time_config_decrease_hour(&config->time.close.hour);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_CLOSE_TIME_MIN:
        {
            time_config_decrease_min(&config->time.open.minute);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_CLOSE_TIME_AM_FM:
        {
            config->time.open.am_fm = !config->time.open.am_fm;
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_DATE_DAY:
        {
            date_config_decrease_day(&config->date.day);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_DATE_MONTH:
        {
            date_config_decrease_month(&config->date.month);
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
        }
        break;

        case FEEDER_CNF_DATE_DAILY:
        { 
            config->date.daily_st = !config->date.daily_st;
            ui_feeder_menu_set_config(&ui_feeder_menu, config);
           } break;
        default:
            break;
        }

        time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
    }
    break;
    case EVT_EXT_BTN_ENTER_PRESSED: { feeder_config_enter_pressed(handle); } break;
    
    default:
        break;
    };
    
    if (handle->event.btn != EVT_EXT_BTN_INVALID)
        handle->event.btn = EVT_EXT_BTN_INVALID;

    /* item cursor inactivity timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        /*clean last selection */
        ui_drawers_config_t *config = &handle->iface.ui.drawers;
        config->select.single = UI_ITEM_DESELECT;
        ui_drawers_set_config(&ui_drawers, config);
        time_event_stop(&handle->event.time.cursor_inact);

        exit_action_feeder_config(handle);
        enter_seq_main_menu(handle);
    }
    
}


/////////////////////////////////// Thermostat Config State  ///////////////////////////////////////////
static void enter_seq_therm_config(ui_handle_t handle)
{
    ui_fsm_dbg("\t enter seq [ therm config ] \r\n");
    fsm_set_next_state(handle, ST_UI_THERM_CONFIG);
    entry_action_therm_config(handle);
}

static void entry_action_therm_config(ui_handle_t handle)
{
    ui_feeder_menu_show(&ui_feeder_menu, false);
    ui_notification_msg_show(&ui_notification, true);
    ui_notification_msg_set(&ui_notification, "Thermostat Menu.");
    time_event_start(&handle->event.time.cursor_inact, NOTIFICATION_MSG_MS);
}

static void therm_config_on_react(ui_handle_t handle)
{
    /* item cursor inactivity timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        exit_action_feeder_config(handle);
        enter_seq_main_menu(handle);
    }
}


/////////////////////////////////// Petcall Config State  ///////////////////////////////////////////
static void enter_seq_petcall_config(ui_handle_t handle)
{
    ui_fsm_dbg("\t enter seq [ petcall config ] \r\n");
    fsm_set_next_state(handle, ST_UI_PETCALL_CONFIG);
    entry_action_petcall_config(handle);
}

static void entry_action_petcall_config(ui_handle_t handle)
{
    ui_feeder_menu_show(&ui_feeder_menu, false);
    ui_notification_msg_show(&ui_notification, true);
    ui_notification_msg_set(&ui_notification, "Petcall Menu.");
    time_event_start(&handle->event.time.cursor_inact, NOTIFICATION_MSG_MS);
}

static void petcall_config_on_react(ui_handle_t handle)
{
    /* item cursor inactivity timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        exit_action_feeder_config(handle);
        enter_seq_main_menu(handle);
    }
}


/////////////////////////////////// Battery Config State  ///////////////////////////////////////////
static void enter_seq_battery_config(ui_handle_t handle)
{
    ui_fsm_dbg("\t enter seq [ battery config ] \r\n");
    fsm_set_next_state(handle, ST_UI_BATT_CONFIG);
    entry_action_battery_config(handle);
}

static void entry_action_battery_config(ui_handle_t handle)
{
    ui_feeder_menu_show(&ui_feeder_menu, false);
    ui_notification_msg_show(&ui_notification, true);
    ui_notification_msg_set(&ui_notification, "Battery Menu.");
    time_event_start(&handle->event.time.cursor_inact, NOTIFICATION_MSG_MS);

}

static void battery_config_on_react(ui_handle_t handle)
{
    /* item cursor inactivity timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        exit_action_feeder_config(handle);
        enter_seq_main_menu(handle);
    }
}



/////////////////////////////////// Date Time Config State  ///////////////////////////////////////////
static void enter_seq_date_time_config(ui_handle_t handle)
{
    ui_fsm_dbg("\t enter seq [ date time config ] \r\n");
    fsm_set_next_state(handle, ST_UI_DATE_TIME_CONFIG);
    entry_action_date_time_config(handle);
}

static void entry_action_date_time_config(ui_handle_t handle)
{
    ui_feeder_menu_show(&ui_feeder_menu, false);
    ui_notification_msg_show(&ui_notification, true);
    ui_notification_msg_set(&ui_notification, "Date Time Menu.");
    time_event_start(&handle->event.time.cursor_inact, NOTIFICATION_MSG_MS);
}

static void date_time_config_on_react(ui_handle_t handle)
{
    /* item cursor inactivity timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        exit_action_feeder_config(handle);
        enter_seq_main_menu(handle);
    }
}



/////////////////////////////////// Drawing Functions  ///////////////////////////////////////////

static void notify_manual_drawer_operation(ui_handle_t handle)
{
    char str[30];
    ui_drawers_config_t *drawer_cfg = &handle->iface.ui.drawers;
    drawer_ctrl_info_t *info = drawer_fsm_get_info(drawer_cfg->drawer.no);
    event_t event;

    event.info.name = EVT_EXT_DRW_INVALID;
    event.info.fsm.src = UI_FSM;
    event.info.fsm.dst = DRAWER_FSM;
    event.info.data_len = sizeof(drawer_ev_ext_data_t);
    ((drawer_ev_ext_data_t*)event.data.buff)->no = drawer_cfg->drawer.no;

    if (info->status.curr == DRAWER_ST_CLOSE || info->status.curr == DRAWER_ST_CLOSING)
    {
        sprintf(str, "Opening Drawer no %d", drawer_cfg->drawer.no + 1);
        ui_fsm_dbg("%s\r\n", str);
        event.info.name = EVT_EXT_DRW_OPEN;
    }

    else if (info->status.curr == DRAWER_ST_OPEN || info->status.curr == DRAWER_ST_OPENING)
    {
        sprintf(str, "Closing Drawer no %d", drawer_cfg->drawer.no + 1);
        ui_fsm_dbg("%s\r\n", str);
        event.info.name = EVT_EXT_DRW_CLOSE;
    }

    if(event.info.name != EVT_EXT_DRW_INVALID)
    {
        event_manager_write(event_manager_fsm_get(), &event);
    }

    ui_notification_msg_set(&ui_notification, str);
}

static void ui_update_battery(ui_handle_t handle)
{
    // ui_fsm_dbg("update battery status \r\n");
    ui_battery_config_t *ui_config = &handle->iface.ui.battery;
    static uint8_t batt_dummy_val = 0;
    ui_config->set    = BATT_ST_CHARGING;
    ui_config->charge = (batt_dummy_val++ % 100);
    ui_battery_set_config(&ui_battery, ui_config);
}

static void ui_update_date_time(ui_handle_t handle)
{
    handle->iface.ui.dt_menu.set = DATE_TIME_CNF_HOUR;
    handle->iface.ui.dt_menu.date.day = 18;
    handle->iface.ui.dt_menu.date.month = 06;
    handle->iface.ui.dt_menu.time.min = 30;
    time_config_increase_min(&handle->iface.ui.dt_menu.time.hour);
    ui_date_time_set_config(&ui_date_time, &handle->iface.ui.dt_menu);
}

static void ui_update_thermostat(ui_handle_t handle)
{
    /*get information from temperature fsm */
    thermostat_info_t *info = temp_ctrl_get_info();
    ui_thermostat_config_t *ui_config = &handle->iface.ui.therm;
    ui_config->select = UI_ITEM_DESELECT;
    ui_config->temp.unit = info->unit;
    ui_config->temp.val =  info->value.sensed;
    ui_thermostat_set_config(&ui_thermostat, ui_config);
}

static void drawer_select_enter_pressed(ui_handle_t handle)
{
    ui_fsm_dbg("drawer enter pressed\r\n");
    if(handle->iface.ui.drawers.select.single == UI_ITEM_SELECT)
    {
        enter_seq_feeder_config(handle);
    }
}

static void drawer_select_key_pressed(ui_handle_t handle)
{
    ui_fsm_dbg("drawer key pressed \r\n");
    ui_drawers_config_t *config = &handle->iface.ui.drawers;

    /*clean previous drawer selection */
    config->drawer.no = handle->iface.cursor.item;
    config->select.single = UI_ITEM_DESELECT;
    ui_drawers_set_config(&ui_drawers, config);

    /*set new drawer number */
    if(handle->event.btn == EVT_EXT_BTN_LEFT_PRESSED)
    {
        handle->iface.cursor.item--;
        handle->iface.cursor.item %= DRAWERn;
    }
    else if (handle->event.btn == EVT_EXT_BTN_RIGHT_PRESSED)
    {
        handle->iface.cursor.item++;
        handle->iface.cursor.item %= DRAWERn;
    }

    /*update drawer selection  */
    config->drawer.no = handle->iface.cursor.item;
    config->select.single = UI_ITEM_SELECT;
    ui_drawers_set_config(&ui_drawers, config);
}

static void main_menu_enter_pressed(ui_handle_t handle)
{
    /* define enter sequence according item selected */
    switch (handle->iface.cursor.item)
    {
    case UI_MAIN_MENU_ITEM_DRAWERS:
    {
        if (handle->iface.ui.drawers.select.main == UI_ITEM_SELECT)
            enter_seq_drawer_select(handle);
    }
    break;

    case UI_MAIN_MENU_ITEM_THERMOSTAT:
    {
        if (handle->iface.ui.therm.select == UI_ITEM_SELECT)
            enter_seq_therm_config(handle);
    }
    break;

    case UI_MAIN_MENU_ITEM_PETCALL:
    {
        if (handle->iface.ui.petcall.select == UI_ITEM_SELECT)
            enter_seq_petcall_config(handle);
    }
    break;

    case UI_MAIN_MENU_ITEM_BATTERY:
    {
        if (handle->iface.ui.battery.select == UI_ITEM_SELECT)
            enter_seq_battery_config(handle);
    }
    break;

    case UI_MAIN_MENU_ITEM_DATE_TIME:
    {
        if (handle->iface.ui.dt_menu.select.main == UI_ITEM_SELECT)
            enter_seq_date_time_config(handle);
    }
    break;

    default:
        break;
    }
}

static void feeder_config_enter_pressed(ui_handle_t handle)
{
    /* Notify Saving Data */
    ui_feeder_menu_show(&ui_feeder_menu, false);
    ui_notification_msg_show(&ui_notification, true);
    ui_notification_msg_set(&ui_notification, "Saving Feeder Config.");

    /* config timer to go main menu */    
    time_event_start(&handle->event.time.cursor_inact, 2000);
}


static void ui_update_item_selection(ui_handle_t handle, ui_select_t select)
{
    switch (handle->iface.cursor.item)
    {
    case UI_MAIN_MENU_ITEM_DRAWERS:
    {
        ui_drawers_config_t *config = &handle->iface.ui.drawers;
        config->select.main = select;
        ui_drawers_set_config(&ui_drawers, config);
    }
    break;

    case UI_MAIN_MENU_ITEM_THERMOSTAT:
    {
        ui_thermostat_config_t *config = &handle->iface.ui.therm;
        config->select = select;
        ui_thermostat_set_config(&ui_thermostat, config);
    }
    break;

    case UI_MAIN_MENU_ITEM_PETCALL:
    {
        ui_petcall_config_t *config = &handle->iface.ui.petcall;
        config->select = select;
        ui_petcall_set_config(&ui_petcall, config);
    }
    break;

    case UI_MAIN_MENU_ITEM_BATTERY:
    {
        ui_battery_config_t *config = &handle->iface.ui.battery;
        config->select = select;
        ui_battery_set_config(&ui_battery, config);
    }
    break;

    case UI_MAIN_MENU_ITEM_DATE_TIME:
    {
        ui_date_time_config_t *config = &handle->iface.ui.dt_menu;
        config->select.main = select;
        ui_date_time_set_config(&ui_date_time, config);
    }
    break;

    default:
        break;
    }
}

static void time_config_increase_hour( uint8_t *hour)
{
    if( (*hour) < 12 )
        (*hour)++;
    else
        (*hour) = 1;
}

static void time_config_decrease_hour(uint8_t *hour)
{
    if ((*hour) > 1)
        (*hour)--;
    else
        (*hour) = 12;
}

static void time_config_increase_min(uint8_t *min)
{
    if( (*min) < 59 )
        (*min)++;
    else
        (*min) = 0;
}

static void time_config_decrease_min(uint8_t *min)
{
    if ((*min) > 0)
        (*min)--;
    else
        (*min) = 59;
}

static void date_config_increase_day(uint8_t *day)
{
    if ((*day) < 30)
        (*day)++;
    else
        (*day) = 1;
}

static void date_config_decrease_day(uint8_t *day)
{
    if ((*day) > 1)
        (*day)--;
    else
        (*day) = 30;
}

static void date_config_decrease_month(uint8_t *moth)
{
    if ((*moth) < 12)
        (*moth)++;
    else
        (*moth) = 1;
}
static void date_config_increase_month(uint8_t *moth)
{
    if ((*moth) > 1)
        (*moth)--;
    else
        (*moth) = 12;
}
