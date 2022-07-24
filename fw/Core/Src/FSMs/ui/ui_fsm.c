#include "ui_fsm.h"
#include "printf_dbg.h"
#include "time_event.h"
#include "ui_draw.h"
#include "string.h"
#include "rct_api.h"

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


#define UPDATE_GUI_MS           (1000)
#define CURSOR_INACTIVITY_MS    (5000)
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
    ST_UI_THERM_MENU_CONFIG,
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
        ui_feeder_config_info_t      feeder_menu;
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

/////////////////////// Static fsm function declaration //////////////////////////////////////
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
static void exit_action_petcall_config(ui_handle_t handle);

static void enter_seq_battery_config(ui_handle_t handle);
static void entry_action_battery_config(ui_handle_t handle);
static void battery_config_on_react(ui_handle_t handle);

static void enter_seq_date_time_config(ui_handle_t handle);
static void entry_action_date_time_config(ui_handle_t handle);
static void date_time_config_on_react(ui_handle_t handle);


//////////////////////// Static Miscellaneous functions ////////////////////////////////////////
static void ui_update_battery(ui_handle_t handle);
static void ui_update_date_time(ui_handle_t handle);
static void ui_update_thermostat(ui_handle_t handle);
static void notify_manual_drawer_operation(ui_handle_t handle);
static void ui_main_menu_update_item_selection(ui_handle_t handle, ui_select_t select);

/* Main Menu Push button Functions */
static void main_menu_enter_key_pressed(ui_handle_t handle);
static void main_menu_left_right_key_pressed(ui_handle_t handle);

/* Drawer Menu Push Button Functions */
static void drawer_select_enter_key_pressed(ui_handle_t handle);
static void drawer_select_right_left_key_pressed(ui_handle_t handle);

/* Feeder Scheduler Menu Push Button Functions */
static void feeder_config_enter_key_pressed(ui_handle_t handle);
static void feeder_config_right_left_key_pressed(ui_handle_t handle);
static void feeder_config_up_down_pressed(ui_handle_t handle);

/* Date time Menu Push button Functions */
static void date_time_config_enter_key_pressed(ui_handle_t handle);
static void date_time_config_left_right_key_pressed(ui_handle_t handle);
static void date_time_config_up_down_key_pressed(ui_handle_t handle);

/* Thermostat menu Push button Functions */
static void therm_menu_right_left_key_pressed(ui_handle_t handle);
static void therm_menu_enter_key_pressed(ui_handle_t handle);
static void therm_menu_up_down_pressed(ui_handle_t handle);

/* Thermostat menu Push button Functions */
static void petcall_menu_right_left_key_pressed(ui_handle_t handle);
static bool petcall_menu_enter_key_pressed(ui_handle_t handle);
static void petcall_menu_up_down_pressed(ui_handle_t handle);

/* Date-time configuration functions */
static void time_config_increase_hour( uint8_t *hour);
static void time_config_decrease_hour(uint8_t *hour);
static void time_config_increase_min(uint8_t *min);
static void time_config_decrease_min(uint8_t *min);
static void date_config_increase_day(uint8_t *day);
static void date_config_decrease_day(uint8_t *day);
static void date_config_increase_month(uint8_t *month);
static void date_config_decrease_month(uint8_t *month);
static void temperature_increase(uint8_t *temp);
static void temperature_decrease(uint8_t *temp);


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
    ui_date_time_menu_init(&ui_date_time_menu);
    ui_feeder_menu_init(&ui_feeder_menu);
    ui_notification_msg_init(&ui_notification);
    ui_thermostat_menu_init(&ui_therm_menu);
    ui_petcall_menu_init(&ui_petcall_menu);

    /*display fixed elements in the Screen */
    ui_battery_show(&ui_battery, true);
    ui_drawers_show(&ui_drawers, true);
    ui_thermostat_show(&ui_thermostat, true);
    ui_petcall_show(&ui_petcall, true);
    ui_date_time_menu_show(&ui_date_time_menu, true);

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
        case ST_UI_THERM_MENU_CONFIG:  therm_config_on_react(handle);      break;
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
    ui_fsm_dbg("enter seq \t[ main menu ]\r\n");
    fsm_set_next_state(handle, ST_UI_MAIN_MENU);
    entry_action_main_menu(handle);
}

static void entry_action_main_menu(ui_handle_t handle)
{
    // show date time 
    ui_date_time_menu_show(&ui_date_time_menu, true);
    ui_update_battery(handle);
    ui_update_thermostat(handle);
    ui_update_date_time(handle);

    /* Set cursor to first item */
    time_event_start(&handle->event.time.update_gui, UPDATE_GUI_MS);
    time_event_stop(&handle->event.time.cursor_inact);
}

static void main_menu_left_right_key_pressed(ui_handle_t handle)
{
    ui_main_menu_update_item_selection(handle, UI_ITEM_DESELECT);

    if (handle->event.btn == EVT_EXT_BTN_LEFT_PRESSED)
    {
        if (handle->iface.cursor.item > 0)
            handle->iface.cursor.item -= 1;
        else
            handle->iface.cursor.item = UI_MAIN_MENU_ITEM_DATE_TIME;
    }

    else if (handle->event.btn == EVT_EXT_BTN_RIGHT_PRESSED)
    {
        if (handle->iface.cursor.item < UI_MAIN_MENU_ITEM_DATE_TIME)
            handle->iface.cursor.item += 1;
        else
            handle->iface.cursor.item = UI_MAIN_MENU_ITEM_DRAWERS;
    }

    ui_main_menu_update_item_selection(handle, UI_ITEM_SELECT);

    printf("ui item selected [%d]\r\n", handle->iface.cursor.item);
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
    case EVT_EXT_BTN_RIGHT_PRESSED: {
        main_menu_left_right_key_pressed(handle);
        time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
    } break;

    /*item menu enter */
    case EVT_EXT_BTN_ENTER_PRESSED: { main_menu_enter_key_pressed(handle); } break;

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
        ui_update_date_time(handle);
        ui_update_thermostat(handle);
    }

    /* item cursor timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        ui_main_menu_update_item_selection(handle, UI_ITEM_DESELECT);
        time_event_stop(&handle->event.time.cursor_inact);
        handle->iface.cursor.item = UI_MAIN_MENU_ITEM_DATE_TIME;
    }
}


/////////////////////////////////// Drawer Request State  ///////////////////////////////////////////
void enter_seq_drawer_request(ui_handle_t handle)
{
    ui_fsm_dbg("enter seq \t[ drawer request ]\r\n");
    fsm_set_next_state(handle, ST_UI_DRAWER_ST_REQUEST);
    entry_action_drawer_request(handle);
}

void entry_action_drawer_request(ui_handle_t handle)
{
    /* Show message in the screen */
    ui_date_time_menu_show(&ui_date_time_menu, false);
    ui_notification_msg_show(&ui_notification, true);

    /* Notify Drawer FSM */
    notify_manual_drawer_operation(handle);
    
    /* Update UI Elements */
    ui_drawers_config_t *ui_config = &handle->iface.ui.drawers;
    drawer_ctrl_info_t *info = drawer_ctrl_fsm_get_info(ui_config->drawer.no);
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
    ui_main_menu_update_item_selection(handle, UI_ITEM_DESELECT);

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
        drawer_select_right_left_key_pressed(handle);
        time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
    } break;

    case EVT_EXT_BTN_UP_PRESSED:    break;
    case EVT_EXT_BTN_DOWN_PRESSED:  break;
    case EVT_EXT_BTN_ENTER_PRESSED: { drawer_select_enter_key_pressed(handle); } break;
    
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
    ui_date_time_menu_show(&ui_date_time_menu, false);
    ui_feeder_menu_show(&ui_feeder_menu, true);

    /*get drawer information from feeder FSM */
    ui_feeder_config_info_t *config = &handle->iface.ui.feeder_menu;
    feeder_config_info_t  *info = feeder_fsm_get_info();
    feeder_drawer_data_t *drawer_data = &info->drawer_no[handle->iface.ui.drawers.drawer.no];

    /*load configuration for drawer of interest*/
    memcpy((uint8_t*)&config->feeder, (uint8_t *)drawer_data, sizeof(feeder_drawer_data_t));
 
    for(int config_idx = FEEDER_CNF_OPEN_TIME_HOUR; config_idx < FEEDER_CNFn; config_idx++)
    {
        config->set = config_idx;
        config->select.single = UI_ITEM_DESELECT;

        for (int meal_idx = FEEDER_MEAL_BREAKFAST; meal_idx < FEEDER_MEALn; meal_idx++)
        {
            config->meal = meal_idx;
            ui_feeder_menu_set_config(&ui_feeder_menu, config);        
        }
    }  

    /*Select first meal in selected drawer */
    handle->iface.cursor.item = FEEDER_CNF_OPEN_TIME_HOUR;
    config->meal = FEEDER_MEAL_BREAKFAST;
    config->set = handle->iface.cursor.item;
    config->select.single = UI_ITEM_SELECT;
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
    if (handle->event.btn != EVT_EXT_BTN_INVALID)
    {
        switch (handle->event.btn)
        {

        case EVT_EXT_BTN_LEFT_PRESSED:
        case EVT_EXT_BTN_RIGHT_PRESSED:
        {
            feeder_config_right_left_key_pressed(handle);
            time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
        }
        break;

        case EVT_EXT_BTN_UP_PRESSED:
        case EVT_EXT_BTN_DOWN_PRESSED:
        {
            feeder_config_up_down_pressed(handle);
            time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
        }
        break;

        case EVT_EXT_BTN_ENTER_PRESSED:
        {
            feeder_config_enter_key_pressed(handle);
            time_event_start(&handle->event.time.cursor_inact, NOTIFICATION_MSG_MS);
        }
        break;

        default:
            break;
        };

        handle->event.btn = EVT_EXT_BTN_INVALID;
    }

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
    fsm_set_next_state(handle, ST_UI_THERM_MENU_CONFIG);
    entry_action_therm_config(handle);
}

static void entry_action_therm_config(ui_handle_t handle)
{
    ui_thermostat_menu_config_t *ui_config = &handle->iface.ui.therm_menu;
    thermostat_config_info_t *info = temp_ctrl_fsm_get_info();

    /*Select First Item in Date Time Menu */
    ui_date_time_menu_show(&ui_date_time_menu, false);
    ui_thermostat_menu_show(&ui_therm_menu, true);

    /*Load initial configuration*/
    ui_config->temp.unit = info->control.unit;     
    ui_config->temp.status = info->control.status;     
    ui_config->temp.val = info->control.temp;
    ui_config->select.single = UI_ITEM_DESELECT;

    for(uint8_t config_idx = THERM_SET_TEMPERATURE; config_idx <  THERM_LAST; config_idx++)
    {
        ui_config->set = config_idx;
        ui_thermostat_menu_set_config(&ui_therm_menu, ui_config);
    }

    /*Select first element in list */
    handle->iface.cursor.item = THERM_SET_TEMPERATURE;
    ui_config->set = handle->iface.cursor.item;
    ui_config->select.main = UI_ITEM_SELECT;
    ui_config->select.single = UI_ITEM_SELECT;
    ui_thermostat_menu_set_config(&ui_therm_menu, ui_config);

    /*Start timer for cursor inactivity*/
    time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);    
}

static void exit_action_therm_config(ui_handle_t handle)
{
    ui_thermostat_menu_config_t *ui_config = &handle->iface.ui.therm_menu;
    ui_config->set = handle->iface.cursor.item;
    ui_config->select.single = UI_ITEM_DESELECT;
    ui_config->select.main = UI_ITEM_DESELECT;

    ui_thermostat_config_t *config =  &handle->iface.ui.therm;
    config->select = UI_ITEM_DESELECT;
    ui_thermostat_set_config(&ui_thermostat, config);
    
    ui_thermostat_menu_show(&ui_therm_menu, false);
}

static void therm_config_on_react(ui_handle_t handle)
{

    if (handle->event.btn != EVT_EXT_BTN_INVALID)
    {
        switch (handle->event.btn)
        {

        case EVT_EXT_BTN_LEFT_PRESSED:
        case EVT_EXT_BTN_RIGHT_PRESSED:
        {
            therm_menu_right_left_key_pressed(handle);
            time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
        }
        break;

        case EVT_EXT_BTN_UP_PRESSED:
        case EVT_EXT_BTN_DOWN_PRESSED:
        {

            therm_menu_up_down_pressed(handle);
            time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
        }
        break;

        case EVT_EXT_BTN_ENTER_PRESSED:
        {
            therm_menu_enter_key_pressed(handle);
            time_event_start(&handle->event.time.cursor_inact, NOTIFICATION_MSG_MS);
        }
        break;

        default:
            break;
        };

        handle->event.btn = EVT_EXT_BTN_INVALID;
    }

    /* item cursor inactivity timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        exit_action_therm_config(handle);
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
    ui_date_time_menu_show(&ui_feeder_menu, false);
    ui_petcall_menu_show(&ui_petcall_menu, true);
    petcall_config_info_t *info = petcall_fsm_get_info();

    ui_petcall_menu_config_t *ui_config = &handle->iface.ui.petcall_menu;

    /*load initial configuration */
    ui_config->select.main = UI_ITEM_DESELECT;
    ui_config->select.single = UI_ITEM_DESELECT;
    ui_config->en_dis = info->petcall_status;
    ui_config->play_stop = info->score;
    ui_config->rec_start_stop = info->rec_status;
    ui_config->delete_file = info->rec_file;
    ui_config->set = UI_PETCALL_CNF_ENABLE_DISABLE; 
    ui_petcall_menu_set_config(&ui_petcall_menu, ui_config);
    ui_config->set = UI_PETCALL_CNF_REC_START_STOP; 
    ui_petcall_menu_set_config(&ui_petcall_menu, ui_config);
    ui_config->set = UI_PETCALL_CNF_SCORE_START_STOP; 
    ui_petcall_menu_set_config(&ui_petcall_menu, ui_config);
    ui_config->set = UI_PETCALL_CNF_DELETE_RECORDING; 
    ui_petcall_menu_set_config(&ui_petcall_menu, ui_config);
    ui_config->set = UI_PETCALL_CNF_EXIT; 
    ui_petcall_menu_set_config(&ui_petcall_menu, ui_config);

    /*load info from flash */
    handle->iface.cursor.item = UI_PETCALL_CNF_ENABLE_DISABLE;
    ui_config->select.main = UI_ITEM_DESELECT;
    ui_config->select.single = UI_ITEM_SELECT;
    ui_config->en_dis = info->petcall_status;
    ui_config->set = handle->iface.cursor.item; 
    ui_petcall_menu_set_config(&ui_petcall_menu, ui_config);

    time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
}

static void exit_action_petcall_config(ui_handle_t handle)
{
    ui_petcall_menu_config_t *ui_config = &handle->iface.ui.petcall_menu;
    /*load info from flash */
    ui_config->select.main = UI_ITEM_DESELECT;
    ui_config->select.single = UI_ITEM_DESELECT;
    ui_petcall_menu_set_config(&ui_petcall_menu, ui_config);

    ui_petcall_config_t *config = &handle->iface.ui.petcall;
    config->select.main = UI_ITEM_DESELECT;
    config->select.single = UI_ITEM_DESELECT;
    ui_petcall_set_config(&ui_petcall, config);
    
    ui_petcall_menu_show(&ui_petcall_menu, false);
}

static void petcall_config_on_react(ui_handle_t handle)
{
    if (handle->event.btn != EVT_EXT_BTN_INVALID)
    {
        switch (handle->event.btn)
        {

        case EVT_EXT_BTN_LEFT_PRESSED:
        case EVT_EXT_BTN_RIGHT_PRESSED:
        {
            petcall_menu_right_left_key_pressed(handle);
            time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
        }
        break;

        case EVT_EXT_BTN_UP_PRESSED:
        case EVT_EXT_BTN_DOWN_PRESSED:
        {

            petcall_menu_up_down_pressed(handle);
            time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
        }
        break;

        case EVT_EXT_BTN_ENTER_PRESSED:
        {
            if(petcall_menu_enter_key_pressed(handle))
                time_event_start(&handle->event.time.cursor_inact, NOTIFICATION_MSG_MS);
            else
                time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
        }
        break;

        default:
            break;
        };

        handle->event.btn = EVT_EXT_BTN_INVALID;
    }

    /* item cursor inactivity timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        exit_action_petcall_config(handle);
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
    /*Select First Item in Date Time Menu */
    handle->iface.cursor.item = DATE_TIME_CNF_HOUR;
    ui_date_time_config_t *ui_config = &handle->iface.ui.dt_menu;
    ui_config->set = handle->iface.cursor.item;
    ui_config->select.single = UI_ITEM_SELECT;
    ui_date_time_menu_set_config(&ui_date_time_menu, ui_config);

    /*Start timer for cursor inactivity*/
    time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
}

static void exit_action_date_time_config(ui_handle_t handle)
{
    ui_feeder_menu_show(&ui_feeder_menu, false);

    ui_date_time_config_t *ui_config = &handle->iface.ui.dt_menu;
    ui_config->set = handle->iface.cursor.item;
    ui_config->select.single = UI_ITEM_DESELECT;
    ui_config->select.main = UI_ITEM_DESELECT;
}

static void date_time_config_on_react(ui_handle_t handle)
{
    if (handle->event.btn != EVT_EXT_BTN_INVALID)
    {
        switch (handle->event.btn)
        {

        case EVT_EXT_BTN_LEFT_PRESSED:
        case EVT_EXT_BTN_RIGHT_PRESSED:
        {
            date_time_config_left_right_key_pressed(handle);
            time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
        }
        break;

        case EVT_EXT_BTN_UP_PRESSED:
        case EVT_EXT_BTN_DOWN_PRESSED:
        {
            date_time_config_up_down_key_pressed(handle);
            time_event_start(&handle->event.time.cursor_inact, CURSOR_INACTIVITY_MS);
        }
        break;

        case EVT_EXT_BTN_ENTER_PRESSED:
        {
            date_time_config_enter_key_pressed(handle);
            time_event_start(&handle->event.time.cursor_inact, 2000);
        }
        break;

        default:
            break;
        };

        handle->event.btn = EVT_EXT_BTN_INVALID;
    }

    /* item cursor inactivity timer event */
    if(time_event_is_raised(&handle->event.time.cursor_inact) == true)
    {
        exit_action_date_time_config(handle);
        enter_seq_main_menu(handle);
    }
}



/////////////////////////////////// Drawing Functions  ///////////////////////////////////////////

static void notify_manual_drawer_operation(ui_handle_t handle)
{
    char str[30];
    ui_drawers_config_t *drawer_cfg = &handle->iface.ui.drawers;
    drawer_ctrl_info_t *info = drawer_ctrl_fsm_get_info(drawer_cfg->drawer.no);
    event_t event;

    event.info.name = EVT_EXT_DRAWER_CTRL_INVALID;
    event.info.fsm.src = UI_FSM;
    event.info.fsm.dst = DRAWER_FSM;
    event.info.data_len = sizeof(drawer_ctrl_ev_ext_data_t);

    drawer_ctrl_ev_ext_data_t *event_data = (drawer_ctrl_ev_ext_data_t*)event.data.buff;

    if (info->status.curr == DRAWER_ST_CLOSE || info->status.curr == DRAWER_ST_CLOSING)
    {
        sprintf(str, "Opening Drawer no %d", drawer_cfg->drawer.no + 1);
        ui_fsm_dbg("%s\r\n", str);
        event.info.name = EVT_EXT_DRAWER_CTRL_OPEN;
        event_data->drawer_no = drawer_cfg->drawer.no;
        event_data->request_type = DRAWER_REQUEST_TYPE_MANUAL;
    }

    else if (info->status.curr == DRAWER_ST_OPEN || info->status.curr == DRAWER_ST_OPENING)
    {
        sprintf(str, "Closing Drawer no %d", drawer_cfg->drawer.no + 1);
        ui_fsm_dbg("%s\r\n", str);
        event.info.name = EVT_EXT_DRAWER_CTRL_CLOSE;
        event_data->drawer_no = drawer_cfg->drawer.no;
        event_data->request_type = DRAWER_REQUEST_TYPE_MANUAL;
    }

    if(event.info.name != EVT_EXT_DRAWER_CTRL_INVALID)
    {
        event_manager_write(event_manager_fsm_get(), &event);
    }

    ui_notification_msg_set(&ui_notification, str);
}

static void ui_update_battery(ui_handle_t handle)
{
    // ui_fsm_dbg("update battery status \r\n");

    power_fsm_info_t *info = power_fsm_get_info();
    ui_battery_config_t *ui_config = &handle->iface.ui.battery;
    static uint8_t batt_dummy_val = 0;
    ui_config->set    = BATT_ST_CHARGING;
    ui_config->charge = info->battery.charge_percent;
    ui_battery_set_config(&ui_battery, ui_config);
}

static void ui_update_date_time(ui_handle_t handle)
{
    date_time_t dt_info;
    rtc_get_time(&dt_info);

	ui_date_time_config_t *ui_config = &handle->iface.ui.dt_menu;
    ui_config->dt_config.date.day = dt_info.dayOfMonth;
    ui_config->dt_config.date.month = dt_info.month;
    ui_config->dt_config.time.hour = dt_info.hours;
    ui_config->dt_config.time.minute = dt_info.minutes;
    ui_config->dt_config.time.am_pm = dt_info.am_pm;

    for (int config_idx = DATE_TIME_CNF_HOUR; config_idx < DATE_TIME_CNF_LAST; config_idx++)
    {
        ui_config->set = config_idx;
        ui_date_time_menu_set_config(&ui_date_time_menu, ui_config);
    }
}

static void ui_update_thermostat(ui_handle_t handle)
{
    thermostat_config_info_t *info = temp_ctrl_fsm_get_info();
    ui_thermostat_config_t *ui_config = &handle->iface.ui.therm;
    ui_config->set = THERM_ICON_SET_SENSED_TEMP;
    ui_config->temp.val =  info->sensed.temp;
    ui_config->temp.unit =  info->control.unit;
    ui_thermostat_set_config(&ui_thermostat, ui_config);
}

static void drawer_select_enter_key_pressed(ui_handle_t handle)
{
    ui_fsm_dbg("drawer enter pressed\r\n");
    if(handle->iface.ui.drawers.select.single == UI_ITEM_SELECT)
    {
        enter_seq_feeder_config(handle);
    }
}

static void drawer_select_right_left_key_pressed(ui_handle_t handle)
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

static void main_menu_enter_key_pressed(ui_handle_t handle)
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
        if (handle->iface.ui.petcall.select.main == UI_ITEM_SELECT)
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

static void feeder_config_enter_key_pressed(ui_handle_t handle)
{
    /* Notify Saving Data */
    ui_feeder_menu_show(&ui_feeder_menu, false);
    ui_notification_msg_show(&ui_notification, true);
    ui_notification_msg_set(&ui_notification, "Saving Feeder Config.");

    /* Save information to Feeder FSM */
    event_t event;
    event.info.name = EVT_EXT_feeder_CONFIG_FEEDING_TIME;
    event.info.fsm.src = UI_FSM;
    event.info.fsm.dst = FEEDER_FSM;
    event.info.data_len = sizeof(feeder_ev_ext_data_t);
    feeder_ev_ext_data_t *data = (feeder_ev_ext_data_t *)&event.data;
    data->config_feeding_time.drawer_no = handle->iface.ui.drawers.drawer.no;
    data->config_feeding_time.config = &handle->iface.ui.feeder_menu.feeder;

    event_manager_write(event_manager_fsm_get(), &event);
}


static void feeder_config_right_left_key_pressed(ui_handle_t handle)
{
    drawer_no_t drawer_no = handle->iface.ui.drawers.drawer.no;
    ui_feeder_config_info_t *config = &handle->iface.ui.feeder_menu;
    config->select.single = UI_ITEM_DESELECT;
    ui_feeder_menu_set_config(&ui_feeder_menu, config);

    if(handle->event.btn ==  EVT_EXT_BTN_LEFT_PRESSED)
    {
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
    }
    else if (handle->event.btn ==  EVT_EXT_BTN_RIGHT_PRESSED)
    {
        if (handle->iface.cursor.item <  FEEDER_CNF_DATE_DAILY)
            handle->iface.cursor.item++;
        else 
            handle->iface.cursor.item = FEEDER_CNF_OPEN_TIME_HOUR;

        config->set = handle->iface.cursor.item;
        config->select.single = UI_ITEM_SELECT;
        ui_feeder_menu_set_config(&ui_feeder_menu, config);

        if (handle->iface.cursor.item == FEEDER_CNF_DATE_DAILY)
        {
            if(config->meal < FEEDER_MEAL_SNACK_3)
                config->meal++;
            else
                config->meal = FEEDER_MEAL_BREAKFAST;
        };
    }
}

static void feeder_config_up_down_pressed(ui_handle_t handle)
{
    uint8_t drawer_no = handle->iface.ui.drawers.drawer.no;
    ui_feeder_config_info_t *config = &handle->iface.ui.feeder_menu;
    bool up_pressed = false;

    if (handle->event.btn == EVT_EXT_BTN_UP_PRESSED)
        up_pressed = true;
    else
        up_pressed = false;

    /*increase item value */
    switch (handle->iface.cursor.item)
    {
    case FEEDER_CNF_OPEN_TIME_HOUR:
    {
        if (up_pressed)
            time_config_increase_hour(&config->feeder.config[config->meal].time.open.hour);
        else
            time_config_decrease_hour(&config->feeder.config[config->meal].time.open.hour);
    }
    break;

    case FEEDER_CNF_OPEN_TIME_MIN:
    {
        if (up_pressed)
            time_config_increase_min(&config->feeder.config[config->meal].time.open.minute);
        else
            time_config_decrease_min(&config->feeder.config[config->meal].time.open.minute);
    }
    break;

    case FEEDER_CNF_OPEN_TIME_AM_FM:
    {
        if(config->feeder.config[config->meal].time.open.am_pm == TIME_AM)
            config->feeder.config[config->meal].time.open.am_pm = TIME_PM;
        else
            config->feeder.config[config->meal].time.open.am_pm = TIME_AM;
    }
    break;

    case FEEDER_CNF_CLOSE_TIME_HOUR:
    {
        if (up_pressed)
            time_config_increase_hour(&config->feeder.config[config->meal].time.close.hour);
        else
            time_config_decrease_hour(&config->feeder.config[config->meal].time.close.hour);
    }
    break;

    case FEEDER_CNF_CLOSE_TIME_MIN:
    {
        if (up_pressed)
            time_config_increase_min(&config->feeder.config[config->meal].time.close.minute);
        else
            time_config_decrease_min(&config->feeder.config[config->meal].time.close.minute);
    }
    break;

    case FEEDER_CNF_CLOSE_TIME_AM_FM:
    {
        if(config->feeder.config[config->meal].time.close.am_pm == TIME_AM)
            config->feeder.config[config->meal].time.close.am_pm = TIME_PM;
        else
            config->feeder.config[config->meal].time.close.am_pm = TIME_AM;
    }
    break;

    case FEEDER_CNF_DATE_DAY:
    {
        if (up_pressed)
            date_config_increase_day(&config->feeder.config[config->meal].date.day);
        else
            date_config_increase_day(&config->feeder.config[config->meal].date.day);
    }
    break;

    case FEEDER_CNF_DATE_MONTH:
    {
        if (up_pressed)
            date_config_increase_month(&config->feeder.config[config->meal].date.month);
        else
            date_config_decrease_month(&config->feeder.config[config->meal].date.month);
    }
    break;

    case FEEDER_CNF_DATE_DAILY:
    {
        if(config->feeder.daily_st == FEEDER_DAILY_MEAL_ST_DISABLE)
            config->feeder.daily_st = FEEDER_DAILY_MEAL_ST_ENABLE;
        else
            config->feeder.daily_st = FEEDER_DAILY_MEAL_ST_DISABLE;

    }
    break;

    default: break;
    }

    ui_feeder_menu_set_config(&ui_feeder_menu, config);
}

static void ui_main_menu_update_item_selection(ui_handle_t handle, ui_select_t select)
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
        config->select.main = select;
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
        ui_date_time_menu_set_config(&ui_date_time_menu, config);
    }
    break;

    default:
        break;
    }
}

static void date_time_config_enter_key_pressed(ui_handle_t handle)
{
    /* Notify Saving Data */
    ui_feeder_menu_show(&ui_feeder_menu, false);
    ui_notification_msg_show(&ui_notification, true);
    ui_notification_msg_set(&ui_notification, "Saving Date Time Config.");

    /* Save Data to Feeder RTC */
    ui_date_time_config_t *ui_config = &handle->iface.ui.dt_menu;
    event_t event;
    event.info.name = EVT_EXT_feeder_CONFIG_DATE_TIME;
    event.info.fsm.src = UI_FSM;
    event.info.fsm.dst = FEEDER_FSM;
    event.info.data_len = sizeof(feeder_ev_ext_data_t);
    feeder_ev_ext_data_t *data = (feeder_ev_ext_data_t *)&event.data.buff;
    memcpy((uint8_t*)&data->config_rtc, (uint8_t*)&ui_config->dt_config, sizeof(date_time_config_t));
    event_manager_write(event_manager_fsm_get(), &event);
}

static void date_time_config_left_right_key_pressed(ui_handle_t handle)
{
    drawer_no_t drawer_no = handle->iface.ui.drawers.drawer.no;
    ui_date_time_config_t *ui_config = &handle->iface.ui.dt_menu;

    ui_config->select.single = UI_ITEM_DESELECT;
    ui_date_time_menu_set_config(&ui_date_time_menu, ui_config);

    if(handle->event.btn ==  EVT_EXT_BTN_LEFT_PRESSED)
    {
        if (handle->iface.cursor.item > DATE_TIME_CNF_HOUR)
            handle->iface.cursor.item--;
        else
            handle->iface.cursor.item =  DATE_TIME_CNF_MONTH;
    }

    else if (handle->event.btn ==  EVT_EXT_BTN_RIGHT_PRESSED)
    {
        if(handle->iface.cursor.item < DATE_TIME_CNF_MONTH)
            handle->iface.cursor.item++;
        else
            handle->iface.cursor.item  = DATE_TIME_CNF_HOUR;
    }
    
    ui_config->set = handle->iface.cursor.item;
    ui_config->select.single = UI_ITEM_SELECT;
    ui_date_time_menu_set_config(&ui_date_time_menu, ui_config);
    
    ui_fsm_dbg("date time item update [%d]\r\n", handle->iface.cursor.item);
}

static void date_time_config_up_down_key_pressed(ui_handle_t handle)
{
    uint8_t drawer_no = handle->iface.ui.drawers.drawer.no;
    ui_date_time_config_t *config = &handle->iface.ui.dt_menu;
    bool up_pressed = false;

    if (handle->event.btn == EVT_EXT_BTN_UP_PRESSED)
        up_pressed = true;
    else
        up_pressed = false;

    /*increase item value */
    switch (handle->iface.cursor.item)
    {

    case DATE_TIME_CNF_HOUR:
    {
        if (up_pressed)
            time_config_increase_hour(&config->dt_config.time.hour);
        else
            time_config_decrease_hour(&config->dt_config.time.hour);
    }
    break;

    case DATE_TIME_CNF_MIN:
    {
        if (up_pressed)
            time_config_increase_min(&config->dt_config.time.minute);
        else
            time_config_decrease_min(&config->dt_config.time.minute);
    }
    break;

    case DATE_TIME_CNF_DAY:
    {
        if (up_pressed)
            date_config_increase_day(&config->dt_config.date.day);
        else
            date_config_decrease_day(&config->dt_config.date.day);
    }
    break;

    case DATE_TIME_CNF_AM_PM: 
    {
        if (config->dt_config.time.am_pm == TIME_AM) {
            config->dt_config.time.am_pm = TIME_PM;
        }
        else {
            config->dt_config.time.am_pm = TIME_AM;
        }
    }
    break;

    case DATE_TIME_CNF_MONTH:
    {
        if (up_pressed)
            date_config_increase_month(&config->dt_config.date.month);
        else
            date_config_decrease_month(&config->dt_config.date.month);
    }
    break;

    default:
        break;
    }

    ui_date_time_menu_set_config(&ui_date_time_menu, config);
}


static void therm_menu_right_left_key_pressed(ui_handle_t handle)
{
    ui_thermostat_menu_config_t *config = &handle->iface.ui.therm_menu;
    bool left_pressed = false;

    if (handle->event.btn == EVT_EXT_BTN_LEFT_PRESSED)
        left_pressed = true;
    else
        left_pressed = false;

    /*increase item value */
    switch (handle->iface.cursor.item)
    {
        case THERM_SET_TEMPERATURE: {
            if(left_pressed)
                temperature_increase(&config->temp.val);
            else 
                temperature_decrease(&config->temp.val);
        } break;

        case THERM_SET_UNIT: {
            if (config->temp.unit == TEMP_UNITS_CELSIUS)
            {
                /*convert from celsius to fahrenheit */
                config->temp.val = (int)((config->temp.val * 9 / 5) + 32);
                config->temp.unit = TEMP_UNITS_FAHRENHEIT;
            }
            else
            {
                /*convert from fahrenheit to celsius */
                config->temp.val = (int)((config->temp.val - 32) * 5/9);
                config->temp.unit = TEMP_UNITS_CELSIUS;
            }
        } break;

        case THERM_ENABLE_DISABLE: {
            
            if(config->temp.status == TEMP_CTRL_DISABLE)
                config->temp.status = TEMP_CTRL_ENABLE;
            else
                config->temp.status = TEMP_CTRL_DISABLE;
        } break;

    default: break;
    }

    ui_thermostat_menu_set_config(&ui_therm_menu, config);
}

static void therm_menu_up_down_pressed(ui_handle_t handle)
{
ui_thermostat_menu_config_t *ui_config = &handle->iface.ui.therm_menu;

    ui_config->select.single = UI_ITEM_DESELECT;
    ui_thermostat_menu_set_config(&ui_therm_menu, ui_config);

    if(handle->event.btn ==  EVT_EXT_BTN_DOWN_PRESSED)
    {
        if (handle->iface.cursor.item > THERM_SET_TEMPERATURE)
            handle->iface.cursor.item--;
        else
            handle->iface.cursor.item =  THERM_ENABLE_DISABLE;
    }

    else if (handle->event.btn ==  EVT_EXT_BTN_UP_PRESSED)
    {
        if(handle->iface.cursor.item < THERM_ENABLE_DISABLE)
            handle->iface.cursor.item++;
        else
            handle->iface.cursor.item  = THERM_SET_TEMPERATURE;
    }
    
    ui_config->set = handle->iface.cursor.item;
    ui_config->select.single = UI_ITEM_SELECT;
    ui_thermostat_menu_set_config(&ui_therm_menu, ui_config);
    
    ui_fsm_dbg("therm menu config item update [%d]\r\n", handle->iface.cursor.item);
}

static void therm_menu_enter_key_pressed(ui_handle_t handle)
{
    /* Notify Saving Data */
    ui_thermostat_menu_show(&ui_therm_menu, false);
    ui_notification_msg_show(&ui_notification, true);
    ui_notification_msg_set(&ui_notification, "Saving Therm Config.");

    /* Send Configuration to Temperature Control  FSM */
    ui_thermostat_menu_config_t *config = &handle->iface.ui.therm_menu;

    event_t event;
    event.info.name = EVT_EXT_SET_TEMP_CONFIG;
    event.info.fsm.src = UI_FSM;
    event.info.fsm.dst = TEMP_CTRL_FSM;
    event.info.data_len = sizeof(temp_ctrl_ev_ext_data_t);

    temp_ctrl_ev_ext_data_t *data = (temp_ctrl_ev_ext_data_t *)&event.data;
    data->config.control.status = config->temp.status;
    data->config.control.temp = config->temp.val;
    data->config.control.unit = config->temp.unit;
    
    event_manager_write(event_manager_fsm_get(), &event);
}


static void petcall_menu_right_left_key_pressed(ui_handle_t handle)
{
    ui_petcall_menu_config_t *ui_config = &handle->iface.ui.petcall_menu;

    ui_config->select.single = UI_ITEM_DESELECT;
    ui_petcall_menu_set_config(&ui_petcall_menu, ui_config);

    if(handle->event.btn ==  EVT_EXT_BTN_LEFT_PRESSED)
    {
        if (handle->iface.cursor.item > UI_PETCALL_CNF_ENABLE_DISABLE)
            handle->iface.cursor.item--;
        else
            handle->iface.cursor.item =  UI_PETCALL_CNF_EXIT;
    }

    else if (handle->event.btn ==  EVT_EXT_BTN_RIGHT_PRESSED)
    {
        if(handle->iface.cursor.item < UI_PETCALL_CNF_EXIT)
            handle->iface.cursor.item++;
        else
            handle->iface.cursor.item = UI_PETCALL_CNF_ENABLE_DISABLE;
    }

    ui_config->select.single = UI_ITEM_SELECT;
    ui_config->set = handle->iface.cursor.item;
    ui_petcall_menu_set_config(&ui_petcall_menu, ui_config);
}


static bool petcall_menu_enter_key_pressed(ui_handle_t handle)
{
    bool exit_menu = false;
    ui_petcall_menu_config_t *ui_config = &handle->iface.ui.petcall_menu;
    petcall_config_info_t *info = petcall_fsm_get_info();
    
    event_t event; 
    event.info.fsm.src = UI_FSM;
    event.info.fsm.dst = PETCALL_FSM;
    event.info.name = EVT_EXT_PETCALL_INVALID;

    switch (handle->iface.cursor.item)
    {
        case UI_PETCALL_CNF_ENABLE_DISABLE:   {
            ui_config->en_dis = !ui_config->en_dis;

            if(ui_config->en_dis == PETCALL_ENABLE)
                event.info.name = EVT_EXT_PETCALL_ENABLE;
            else 
                event.info.name = EVT_EXT_PETCALL_DISABLE;       
        } break;

        case UI_PETCALL_CNF_REC_START_STOP:   {
            ui_config->rec_start_stop = !ui_config->rec_start_stop;

            if (ui_config->rec_start_stop == PETCALL_REC_START)
                event.info.name = EVT_EXT_PETCALL_RECORD_START;
            else
                event.info.name = EVT_EXT_PETCALL_RECORD_STOP;
        } break;

        case UI_PETCALL_CNF_SCORE_START_STOP: {
            ui_config->play_stop = !ui_config->play_stop;

            if (ui_config->play_stop == PETCALL_SCORE_PLAY)
                event.info.name = EVT_EXT_PETCALL_SCORE_PLAY;
            else
                event.info.name = EVT_EXT_PETCALL_RECORD_STOP;
        } break;

        case UI_PETCALL_CNF_DELETE_RECORDING: {
            ui_config->delete_file = info->rec_file;

            if(ui_config->delete_file = PETCALL_REC_FILE_AVAILABLE)
                event.info.name = EVT_EXT_PETCALL_DELETE;
            
        } break;

        case UI_PETCALL_CNF_EXIT: {
            exit_menu = true;
        } break;

        default: break;
    }

    ui_petcall_menu_set_config(&ui_petcall_menu, ui_config);

    if(event.info.name != EVT_EXT_PETCALL_INVALID)
    {
        petcall_fsm_write_event(petcall_fsm_get(), &event);
    }

    if(exit_menu)
    {
        ui_petcall_menu_show(&ui_petcall_menu, false);
        ui_notification_msg_show(&ui_notification, true);
        ui_notification_msg_set(&ui_notification, "Saving Petcall Config.");
    }

    return exit_menu;
}

static void petcall_menu_up_down_pressed(ui_handle_t handle)
{
    if (handle->event.btn == EVT_EXT_BTN_UP_PRESSED)
    {
    }
    else if (handle->event.btn == EVT_EXT_BTN_DOWN_PRESSED)
    {
    }
}

static void temperature_increase(uint8_t *temp)
{
    if ((*temp) < 99)
        (*temp)++;
    else
        (*temp) = 0;
}

static void temperature_decrease(uint8_t *temp)
{
    if ((*temp) > 0)
        (*temp)--;
    else
        (*temp) = 99;
}

static void time_config_increase_hour(uint8_t *hour)
{
    if ((*hour) < 12)
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
    if ((*min) < 59)
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


static void save_user_config_in_flash(void)
{

}
