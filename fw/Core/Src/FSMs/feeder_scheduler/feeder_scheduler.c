#include "feeder_scheduler.h"
#include "printf_dbg.h"
#include "buttons.h"
#include "time_event.h"

/**@brief Enable/Disable debug messages */
#define feeder_scheduler_FSM_DEBUG 0
#define feeder_scheduler_FSM_TAG "ui : "

/**@brief uart debug function for server comm operations  */
#if feeder_scheduler_FSM_DEBUG
#define feeder_scheduler_fsm_dbg(format, ...)         \
    do                                           \
    {											\
    	printf_dbg_color(E_YEL, feeder_scheduler_FSM_TAG); \
        printf_dbg(format,##__VA_ARGS__ );       \
    } while (0)
#else
#define feeder_scheduler_fsm_dbg(format, ...) \
	do                                       \
	{ /* Do nothing */                       \
	} while (0)
#endif


#define UPDATE_Gfeeder_scheduler_MS       (1000)

/**
 * @brief State Machine States
 */
typedef enum
{
    ST_feeder_scheduler_INVALID = 0,
    ST_feeder_scheduler_MAIN_MENU,
    ST_feeder_scheduler_DATE_TIME_CONFIG,
    ST_feeder_scheduler_PET_CALL_CONFIG,
    ST_feeder_scheduler_FEEDER_CONFIG,
    ST_feeder_scheduler_OPEN_DRAWER,
    ST_feeder_scheduler_LAST,
}feeder_scheduler_state_t;

typedef enum
{
    EVT_INT_feeder_scheduler_INVALID = 0,
    EVT_INT_feeder_scheduler_LAST
}feeder_scheduler_ev_int_t;

typedef struct
{
    time_event_t update_gui;
}feeder_scheduler_ev_time_t;

typedef struct 
{
    btn_ev_ext_t btn;
    feeder_scheduler_ev_int_t  internal;
    feeder_scheduler_ev_time_t time;
}feeder_scheduler_event_t;

typedef struct
{
    event_queue_t queue;
}feeder_scheduler_iface_t;

struct feeder_scheduler_fsm_t
{
    feeder_scheduler_state_t state;
    feeder_scheduler_event_t event;
    feeder_scheduler_iface_t iface;
};

static struct feeder_scheduler_fsm_t feeder_scheduler_fsm;

/////////////////////// Static state function declaration //////////////////////////////////////
static void main_menu_enter_seq(feeder_scheduler_handle_t handle);
static void entry_action_main_menu(feeder_scheduler_handle_t handle);
static void main_menu_on_react(feeder_scheduler_handle_t handle);


////////////////////////////// Public function declaration //////////////////////////////////////

feeder_scheduler_handle_t feeder_scheduler_fsm_get(void)
{
    return &feeder_scheduler_fsm;
}

void feeder_scheduler_fsm_init(feeder_scheduler_handle_t handle)
{
    // feeder_scheduler_battery_init(&feeder_scheduler_battery);
    // feeder_scheduler_drawers_init(&feeder_scheduler_drawers);
	// main_menu_enter_seq(handle);
}

void feeder_scheduler_fsm_run(feeder_scheduler_handle_t handle)
{
    switch (handle->state)
    {
        case ST_feeder_scheduler_MAIN_MENU:  /*main_menu_on_react(handle);*/ break;
        case ST_feeder_scheduler_DATE_TIME_CONFIG: break;
        case ST_feeder_scheduler_PET_CALL_CONFIG: break;
        case ST_feeder_scheduler_FEEDER_CONFIG: break;
        case ST_feeder_scheduler_OPEN_DRAWER: break;
    default:
        break;
    }
}

static void fsm_set_next_state(feeder_scheduler_handle_t handle, feeder_scheduler_state_t next_st)
{
    // handle->state = next_st;
    // handle->event.btn = EVT_EXT_BTN_INVALID;
    // handle->event.internal = EVT_INT_feeder_scheduler_INVALID;
}

void feeder_scheduler_fsm_time_update(feeder_scheduler_handle_t handle)
{
	time_event_t *time_event = (time_event_t *)&handle->event;
	for (int tev_idx = 0; tev_idx < sizeof(handle->event.time) / sizeof(time_event_t); tev_idx++)
	{
		time_event_update(time_event);
		time_event++;
	}
}

void feeder_scheduler_fsm_set_ext_event(feeder_scheduler_handle_t handle, event_t *event)
{
    // if(event->header.fsm_src == BTN_FSM)
    // {
    //     feeder_scheduler_fsm_dbg("btn event detected\r\n");
    //     handle->event.btn = event->header.name;
    // }
}



/////////////////////////////////// Main Menu State  ///////////////////////////////////////////

static void main_menu_enter_seq(feeder_scheduler_handle_t handle)
{
    // feeder_scheduler_fsm_dbg("enter seq \t[ main menu ]");
    // fsm_set_next_state(handle, ST_feeder_scheduler_MAIN_MENU);
    // entry_action_main_menu(handle);
}

static void entry_action_main_menu(feeder_scheduler_handle_t handle)
{
    /*1. draw battery */
    /*1. put icons in the main screen with the updated values */
    /*2. put cursor in the first navigation item */
    /*3. start timer to update gui */
    // feeder_scheduler_battery_init(&feeder_scheduler_battery);
    // feeder_scheduler_battery_show(&feeder_scheduler_battery, true);
    // feeder_scheduler_battery_charge(&feeder_scheduler_battery, 99, true);
    // feeder_scheduler_drawers_init(&feeder_scheduler_drawers);
    // // feeder_scheduler_feeder_menu_init(&feeder_scheduler_feeder_menu);
    // feeder_scheduler_date_time_init(&feeder_scheduler_date_time_menu);
    // HAL_Delay(2000);

#if 0
   feeder_config_t config;

   config.set = FEEDER_CNF_OPEN_TIME_AM_FM;
   config.meal = FEEDER_MEAL_BREAKFAST;
   config.time.am_fm = TIME_AM;
   config.select = feeder_scheduler_ITEM_SELECT;

    feeder_scheduler_feeder_menu_set_config(&feeder_scheduler_feeder_menu, &config);
    HAL_Delay(1000);

    config.select = feeder_scheduler_ITEM_DESELECT;
    feeder_scheduler_feeder_menu_set_config(&feeder_scheduler_feeder_menu, &config);
    HAL_Delay(1000);

    config.set = FEEDER_CNF_DATE_DAILY;
   config.meal = 0; //na
   config.date.daily_en = DAILY_MEAL_DISABLE;
   config.select = feeder_scheduler_ITEM_SELECT;
    feeder_scheduler_feeder_menu_set_config(&feeder_scheduler_feeder_menu, &config);
    HAL_Delay(1000);

   config.date.daily_en = DAILY_MEAL_ENABLE;
   config.select = feeder_scheduler_ITEM_SELECT;
    feeder_scheduler_feeder_menu_set_config(&feeder_scheduler_feeder_menu, &config);
    HAL_Delay(1000);

    config.date.daily_en = DAILY_MEAL_ENABLE;
   config.select = feeder_scheduler_ITEM_DESELECT;
    feeder_scheduler_feeder_menu_set_config(&feeder_scheduler_feeder_menu, &config);
    HAL_Delay(1000);
#endif 

    // date_time_config_t config;
    // config.item = DATE_TIME_CNF_HOUR;
    // config.select = feeder_scheduler_ITEM_SELECT;
    // config.time.hour = 5;
    // feeder_scheduler_date_time_set_config(&feeder_scheduler_date_time_menu, &config);
    // HAL_Delay(1000);

    // config.select = feeder_scheduler_ITEM_DESELECT;
    // feeder_scheduler_date_time_set_config(&feeder_scheduler_date_time_menu, &config);
    // HAL_Delay(1000);



    // time_event_start(&handle->event.time.update_gui, UPDATE_Gfeeder_scheduler_MS);
}

static void main_menu_on_react(feeder_scheduler_handle_t handle)
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






