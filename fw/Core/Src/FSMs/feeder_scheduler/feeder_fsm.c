#include "feeder_fsm.h"
#include "event_manager_fsm.h"
#include "printf_dbg.h"
#include "time_event.h"
#include "rct_api.h"

/**@brief Enable/Disable debug messages */
#define feeder_FSM_DEBUG 1
#define feeder_FSM_TAG "feeder : "

/**@brief uart debug function for server comm operations  */
#if feeder_FSM_DEBUG
#define feeder_dbg(format, ...)         \
    do                                           \
    {											\
    	printf_dbg_color(E_YEL, feeder_FSM_TAG); \
        printf_dbg(format,##__VA_ARGS__ );       \
    } while (0)
#else
#define feeder_dbg(format, ...) \
	do                                       \
	{ /* Do nothing */                       \
	} while (0)
#endif

const char *am_pm_str[3] = {"NA","AM", "PM"}; 
const char *meal_name[FEEDER_MEALn] = {
    "FEEDER_MEAL_BREAKFAST",
    "FEEDER_MEAL_LUNCH",
    "FEEDER_MEAL_SNACK_2",
    "FEEDER_MEAL_DINNER",
    "FEEDER_MEAL_SNACK_3",
};


/**
 * @brief State Machine States
 */
typedef enum
{
    ST_feeder_INVALID = 0,
    ST_feeder_WATCHER,
    ST_feeder_DATE_TIME_CONFIG,
    ST_feeder_FEEDING_TIME_CONFIG,
    ST_feeder_LAST,
}feeder_state_t;

typedef enum
{
    EVT_INT_feeder_INVALID = 0,
    EVT_INT_feeder_OPEN_TIME_ELAPSED,
    EVT_INT_feeder_CLOSE_TIME_ELAPSED,
    EVT_INT_feeder_LAST
}feeder_ev_int_name_t;

typedef struct
{
    time_event_t update;
}feeder_ev_time_t;


typedef struct
{
    feeder_ev_int_name_t name;
}feeder_ev_int_t;

typedef struct 
{
    feeder_ev_ext_t  external;
    feeder_ev_int_t  internal;
    feeder_ev_time_t time;
}feeder_event_t;

typedef struct
{
    feeder_config_info_t feeder;
    drawer_no_t   drawer;
    bool          dt_config_done;   // day time configuration
    event_queue_t queue;
}feeder_iface_t;

struct feeder_fsm_t
{
    feeder_state_t state;
    feeder_event_t event;
    feeder_iface_t iface;
};

static struct feeder_fsm_t feeder_fsm;

/////////////////////// Static state function declaration //////////////////////////////////////


static void enter_seq_watcher(feeder_handle_t handle);
static void enter_seq_date_time_config(feeder_handle_t handle);
static void enter_seq_feeding_time_config(feeder_handle_t handle);

static void watcher_on_react(feeder_handle_t handle);
static void date_time_config_on_react(feeder_handle_t handle);
static void feeding_time_config_on_react(feeder_handle_t handle);

static void entry_action_watcher(feeder_handle_t handle);
static void entry_action_date_time_config(feeder_handle_t handle);
static void entry_action_feeding_time_config(feeder_handle_t handle);

/////////////////////////////////// Miscellaneous Function Declaration   ///////////////////////////////////////////
static bool get_user_configuration_from_flash(feeder_handle_t handle);



////////////////////////////// Public function declaration //////////////////////////////////////

feeder_config_info_t *feeder_fsm_get_info(void)
{
    return &feeder_fsm.iface.feeder;
}

feeder_handle_t feeder_fsm_get(void)
{
    return &feeder_fsm;
}

void feeder_fsm_init(feeder_handle_t handle)
{
    /*Init Default Configuration */
    memset((uint8_t *)&handle->iface.feeder, 0, sizeof(feeder_config_info_t));

    /*Load User Configuration */
    get_user_configuration_from_flash(handle);

    handle->iface.drawer = DRAWER_NO_1;
    handle->iface.dt_config_done = false;
    enter_seq_watcher(handle);
}

void feeder_fsm_run(feeder_handle_t handle)
{
    switch (handle->state)
    {
    case ST_feeder_WATCHER:            { watcher_on_react(handle);} break;
    case ST_feeder_DATE_TIME_CONFIG:   { date_time_config_on_react(handle);} break;
    case ST_feeder_FEEDING_TIME_CONFIG:{ feeding_time_config_on_react(handle);} break;
    default:
        break;
    }
}

static void fsm_set_next_state(feeder_handle_t handle, feeder_state_t next_st)
{
    handle->state = next_st;
    handle->event.external.name = EVT_EXT_feeder_INVALID;
    handle->event.internal.name = EVT_INT_feeder_INVALID;
}

void feeder_fsm_time_update(feeder_handle_t handle)
{
	time_event_t *time_event = (time_event_t *)&handle->event.time;
	for (int tev_idx = 0; tev_idx < sizeof(handle->event.time) / sizeof(time_event_t); tev_idx++)
	{
		time_event_update(time_event);
		time_event++;
	}
}

void feeder_fsm_write_event(feeder_handle_t handle, event_t *event)
{
    feeder_ev_ext_data_t *data = (feeder_ev_ext_data_t *)&event->data.buff;

    if(event->info.name == EVT_EXT_feeder_CONFIG_DATE_TIME)
    {
        /*Check if there is any change respect to the previous configuration*/
        if(memcmp((uint8_t *)&handle->event.external.data.config_rtc, (uint8_t*)&data->config_rtc) == 0)
        {
            feeder_dbg("no changes detected in date time configuration\r\n");
        }
        else
        {
            handle->event.external.name = event->info.name;
            memcpy((uint8_t *)&handle->event.external.data.config_rtc, (uint8_t*)event->data.buff, sizeof(feeder_ev_ext_data_t));
        }
    }
    else if(event->info.name == EVT_EXT_feeder_CONFIG_FEEDING_TIME)
    {
        handle->event.external.name = event->info.name;
        handle->event.external.data.config_feeding_time.config = data->config_feeding_time.config;
        handle->event.external.data.config_feeding_time.drawer_no = data->config_feeding_time.drawer_no;

    }
}

////////////////////////////////// State Function Definition ////////////////////////////////////
static void enter_seq_feeding_time_config(feeder_handle_t handle)
{
    feeder_dbg("enter seq: \t[set feeding time]\r\n");
    fsm_set_next_state(handle, ST_feeder_FEEDING_TIME_CONFIG);
    entry_action_feeding_time_config(handle);
}
static void entry_action_feeding_time_config(feeder_handle_t handle)
{
    drawer_no_t drawer_no = handle->event.external.data.config_feeding_time.drawer_no;
    feeder_drawer_data_t *config_ext = handle->event.external.data.config_feeding_time.config; // extern config
    feeder_drawer_data_t *config_int = &handle->iface.feeder.drawer_no[drawer_no];             // current config

    /*Compare if there was any changes*/
    if (memcmp((uint8_t *)config_int, (uint8_t *)config_ext, sizeof(feeder_drawer_data_t)) == 0)
    {
        feeder_dbg("no changes detected in feeder configuration\r\n");
    }
    else
    {
        feeder_dbg("saving new configuration\r\n");
        memcpy((uint8_t *)config_int, (uint8_t *)config_ext, sizeof(feeder_drawer_data_t));
        user_config_set();
    }
}
static void feeding_time_config_on_react(feeder_handle_t handle)
{
    enter_seq_watcher(handle);
}

////////////////////////////////// State Function Definition ////////////////////////////////////
static void enter_seq_date_time_config(feeder_handle_t handle)
{
    feeder_dbg("enter seq: \t[date time config]\r\n");
    fsm_set_next_state(handle, ST_feeder_DATE_TIME_CONFIG);
    entry_action_date_time_config(handle);
}
static void date_time_config_on_react(feeder_handle_t handle)
{
    enter_seq_watcher(handle);
}

static void entry_action_date_time_config(feeder_handle_t handle)
{
    feeder_dbg("saving date time configuration to RTC \r\n");

    handle->iface.dt_config_done = true;

    date_time_t date_time;
    date_time.dayOfMonth = handle->event.external.data.config_rtc.date.day;
    date_time.month = handle->event.external.data.config_rtc.date.month;
    date_time.hours = handle->event.external.data.config_rtc.time.hour;
    date_time.minutes = handle->event.external.data.config_rtc.time.minute;
    date_time.am_pm = handle->event.external.data.config_rtc.time.am_pm;
    date_time.hour_24h_format = false;

    rtc_set_time(date_time); 
    rtc_print_time();
}

////////////////////////////////// State Function Definition ////////////////////////////////////
static void enter_seq_watcher(feeder_handle_t handle)
{
    feeder_dbg("enter seq: \t[ watcher]\r\n");
    fsm_set_next_state(handle, ST_feeder_WATCHER);
    entry_action_watcher(handle);
}
static void entry_action_watcher(feeder_handle_t handle)
{

    time_event_start(&handle->event.time.update, FEEDER_UPDATE_TIME_MS);
}

static bool date_match(date_info_t *date1, date_info_t *date2)
{
    if ((date1->day == date2->day) && (date1->month == date2->month))
        return true;
    else
        return false;
}

static bool time_match(time_info_t *time1, time_info_t *time2)
{
    if (time1->hour == time2->hour && time1->minute == time2->minute && time1->am_pm == time2->am_pm)
        return true;
    else
        return false;
}

static void send_drawer_request(drawer_ctrl_ev_ext_name_t evt_name, drawer_no_t no)
{
    event_t event;
    event.info.name = evt_name;
    event.info.fsm.src = FEEDER_FSM;
    event.info.fsm.dst = DRAWER_FSM;
    event.info.data_len = sizeof(drawer_ctrl_ev_ext_data_t);
    drawer_ctrl_ev_ext_data_t *data = (drawer_ctrl_ev_ext_data_t *)&event.data.buff;
    data->drawer_no = no;
    data->request_type = DRAWER_REQUEST_TYPE_PROGRAMMED;
    event_manager_write(event_manager_fsm_get(), &event);
}


static void check_if_feeding_time_is_elapsed(feeder_handle_t handle)
{
    /*Get Date Time from RTC */
    date_time_t date_time;
    rtc_get_time(&date_time);

    date_info_t rtc_date = {.day = date_time.dayOfMonth, .month = date_time.month};
    time_info_t rtc_time = {.minute = date_time.minutes, .hour = date_time.hours, .am_pm = date_time.am_pm};

    feeder_dbg("RTC Info : Date { DD/MM : %.2d/%.2d }, Time { hh:mm %.2d:%.2d %s }\r\n",
                rtc_date.day, rtc_date.month, rtc_time.hour, rtc_time.minute, am_pm_str[rtc_time.am_pm]);

    feeder_meal_data_t *meal_data;

    for (int drawer_idx = 0; drawer_idx < DRAWERn; drawer_idx++)
    {
        drawer_ctrl_info_t *drawer_info = drawer_ctrl_fsm_get_info(drawer_idx);
        feeder_drawer_data_t *feeder_drawer = &handle->iface.feeder.drawer_no[drawer_idx];

        if (drawer_info->request_type == DRAWER_REQUEST_TYPE_MANUAL)
        {
            feeder_dbg("drawer no [%d] skipped, manually opened [%d]\r\n", drawer_idx + 1, drawer_info->status.curr);
            continue;
        }

        /*check if daily option is disabled */
        for (int meal_idx = 0; meal_idx < FEEDER_MEALn; meal_idx++)
        {
            /*check if date matches*/
            meal_data = &feeder_drawer->config[meal_idx];

            if (feeder_drawer->daily_st == FEEDER_DAILY_MEAL_ST_DISABLE)
            {
                if (date_match(&meal_data->date, &rtc_date) == false)
                {
                    feeder_dbg("drawer no [%d], date mismatch, skip meal [ %s ]\r\n", drawer_idx + 1, meal_name[meal_idx]);
                    continue;
                }
            }

            /* check if open time matches */
            if (time_match(&meal_data->time.open, &rtc_time))
            {

                feeder_dbg("Open drawer no%d - Date { DD/MM : %.2d/%.2d } Time { hh:mm %.2d:%.2d %s } Meal {%s}\r\n",
                           drawer_idx + 1, meal_data->date.day, meal_data->date.month, meal_data->time.open.hour, meal_data->time.open.minute,
                           am_pm_str[meal_data->time.close.am_pm], meal_name[meal_idx]);

                if (drawer_info->status.curr == DRAWER_ST_CLOSE && drawer_info->status.next == DRAWER_ST_INVALID) {
                    send_drawer_request(EVT_EXT_DRAWER_CTRL_OPEN, drawer_idx);
                }
                else {
                    feeder_dbg("WARNING : drawer no [%d] already opened\r\n", drawer_idx + 1);
                }
            }

            /* check if close time matches */
            if (time_match(&meal_data->time.close, &rtc_time))
            {
                feeder_dbg("Close drawer no%d - Date { DD/MM : %.2d/%.2d } Time { hh:mm %.2d:%.2d %s } Meal {%s}\r\n",
                           drawer_idx + 1, meal_data->date.day, meal_data->date.month, meal_data->time.close.hour, meal_data->time.close.minute,
                           am_pm_str[meal_data->time.close.am_pm], meal_name[meal_idx]);

                if (drawer_info->status.curr == DRAWER_ST_OPEN && drawer_info->status.next == DRAWER_ST_INVALID) {
                    send_drawer_request(EVT_EXT_DRAWER_CTRL_CLOSE, drawer_idx);
                }
                else {
                    feeder_dbg("WARNING : drawer no [%d] already closed\r\n", drawer_idx + 1);
                }
            }
        }
    }
}

static void watcher_on_react(feeder_handle_t handle)
{
    if(handle->event.external.name == EVT_EXT_feeder_CONFIG_DATE_TIME)
    {
        enter_seq_date_time_config(handle);
    }
    else if(handle->event.external.name == EVT_EXT_feeder_CONFIG_FEEDING_TIME)
    {
        enter_seq_feeding_time_config(handle);
    }

    /* check time events */
    if(time_event_is_raised(&handle->event.time.update) == true)
    {
        check_if_feeding_time_is_elapsed(handle);
        enter_seq_watcher(handle);
    }
}

/////////////////////////////////// Miscellaneous Function Definition   ///////////////////////////////////////////
static bool is_valid_feeder_config(feeder_config_info_t *info)
{
    for (size_t drawer_idx = 0; drawer_idx < DRAWERn; drawer_idx++)
    {
        feeder_drawer_data_t *drawer_data = &info->drawer_no[drawer_idx];
        if (!IS_VALID_FEEDER_DAILY_ST(drawer_data->daily_st))
            return false;

        for (size_t meal = 0; meal < FEEDER_MEALn; meal++)
        {
            if (!IS_VALID_FEEDER_DATE_DAY(drawer_data->config[meal].date.day))
                return false;
            if (!IS_VALID_FEEDER_DATE_MONTH(drawer_data->config[meal].date.month))
                return false;
            if (!IS_VALID_FEEDER_HOUR(drawer_data->config[meal].time.close.hour))
                return false;
            if (!IS_VALID_FEEDER_HOUR(drawer_data->config[meal].time.open.hour))
                return false;
        }
    }

    return true;
}

static bool get_user_configuration_from_flash(feeder_handle_t handle)
{
    feeder_dbg("checking if there is available info in flash...\r\n");

    user_config_t *user_config = user_config_get();
    feeder_config_info_t *flash_config = &user_config->feeder_info;
    feeder_config_info_t *ram_config = &handle->iface.feeder;

    if(user_config->data_available)
    {
        /*check if there is valid data in flash*/
        if(is_valid_feeder_config(flash_config))
        {
            feeder_dbg("valid feeder configuration found, loading values ...\r\n");
            memcpy((uint8_t *)ram_config, (uint8_t *)flash_config, sizeof(feeder_config_info_t));
        }
        else
        {
            feeder_dbg("Invalid feeder configuration from flash, initializing configuration by default..\r\n");

            for (size_t drawer_idx = 0; drawer_idx < DRAWERn; drawer_idx++)
            {
                feeder_drawer_data_t *drawer_data = &handle->iface.feeder.drawer_no[drawer_idx];
                drawer_data->daily_st = FEEDER_DAILY_MEAL_ST_DISABLE;

                for (size_t meal = 0; meal < FEEDER_MEALn; meal++)
                {
                    drawer_data->config[meal].date.day = 0;
                    drawer_data->config[meal].date.month = 0;
                    drawer_data->config[meal].time.close.hour = 0;
                    drawer_data->config[meal].time.close.minute = 0;
                    drawer_data->config[meal].time.close.am_pm = TIME_PM;
                    drawer_data->config[meal].time.open.hour = 0;
                    drawer_data->config[meal].time.open.minute = 0;
                    drawer_data->config[meal].time.open.am_pm = TIME_PM;
                }
            }

            user_config_set();
        }
    }

    return false;
}
