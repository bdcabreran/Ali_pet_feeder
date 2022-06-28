#include "feeder_fsm.h"
#include "printf_dbg.h"
#include "time_event.h"
#include "rct_api.h"
#include <stdio.h>
#include <strings.h>

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

const char *am_fm_str[TIMEn] = {"AM", "PM"}; 



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
    feeder_config_t feeder;
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

/////////////////////////////////// State Function Declaration   ///////////////////////////////////////////

static void enter_seq_watcher(feeder_handle_t handle);
static void enter_seq_date_time_config(feeder_handle_t handle);
static void enter_seq_feeding_time_config(feeder_handle_t handle);

static void watcher_on_react(feeder_handle_t handle);
static void date_time_config_on_react(feeder_handle_t handle);
static void feeding_time_config_on_react(feeder_handle_t handle);

static void entry_action_watcher(feeder_handle_t handle);
static void entry_action_date_time_config(feeder_handle_t handle);
static void entry_action_feeding_time_config(feeder_handle_t handle);


////////////////////////////// Public function declaration //////////////////////////////////////

feeder_config_t *feeder_fsm_get_info(void)
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
    memset((uint8_t *)&handle->iface.feeder, 0, sizeof(feeder_config_t));
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
	time_event_t *time_event = (time_event_t *)&handle->event;
	for (int tev_idx = 0; tev_idx < sizeof(handle->event.time) / sizeof(time_event_t); tev_idx++)
	{
		time_event_update(time_event);
		time_event++;
	}
}

void feeder_fsm_write_event(feeder_handle_t handle, event_t *event)
{
    if(event->info.name == EVT_EXT_feeder_CONFIG_DATE_TIME)
    {
        handle->event.external.name = event->info.name;
        memcpy((uint8_t *)&handle->event.external.data.config_rtc, (uint8_t*)event->data.buff, sizeof(feeder_ev_ext_data_t));
    }
    else if(event->info.name == EVT_EXT_feeder_CONFIG_FEEDING_TIME)
    {
        feeder_ev_ext_data_t *data = (feeder_ev_ext_data_t *)&event->data.buff;
        handle->event.external.name = event->info.name;
        handle->event.external.data.config_feeding_time.drawer_no = data->config_feeding_time.drawer_no;
        handle->event.external.data.config_feeding_time.config = data->config_feeding_time.config;
    }
}

////////////////////////////////// State Function Definition ////////////////////////////////////
static void enter_seq_feeding_time_config(feeder_handle_t handle)
{
    feeder_dbg("enter seq: \t[set feeding time]\r\n");
    fsm_set_next_state(handle, ST_feeder_FEEDING_TIME_CONFIG);
    entry_action_feeding_time_config(handle);
}
static void feeding_time_config_on_react(feeder_handle_t handle)
{
    enter_seq_watcher(handle);
}
static void entry_action_feeding_time_config(feeder_handle_t handle)
{
    drawer_no_t drawer_no = handle->event.external.data.config_feeding_time.drawer_no;
    feeder_drawer_data_t *drawer_cfg = &handle->iface.feeder.drawer.no_1;

    for (uint8_t drawer_cnt = 0; drawer_cnt < drawer_no; drawer_cnt++)
        drawer_cfg++;

    /*store new information */
    memcpy((uint8_t *)drawer_cfg, (uint8_t *)handle->event.external.data.config_feeding_time.config, sizeof(feeder_drawer_data_t));
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
    handle->iface.dt_config_done = true;
    /*!<TODO : send date and time values to RTC 
    rtc_set_date_time();
    */
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

static date_info_t rtc_get_date_info(feeder_handle_t handle)
{
    date_info_t info;
    info.day = 25;
    info.month = 30;
    return info;
}

static time_info_t rtc_get_time_info(feeder_handle_t handle)
{
    time_info_t info;
    info.hour = 10;
    info.minute = 25;
    info.am_fm = TIME_AM;
    return info;
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
    if (time1->hour == time2->hour && time1->minute == time2->minute && time1->am_fm == time2->am_fm)
        return true;
    else
        return false;
}

static void check_if_feeding_time_is_elapsed(feeder_handle_t handle)
{
    /*Get Date from RTC */
    date_info_t rtc_date = rtc_get_date_info(handle);
    time_info_t rtc_time = rtc_get_time_info(handle);

    feeder_drawer_data_t *drawer = &handle->iface.feeder.drawer.no_1;
    feeder_meal_data_t *meal_data;

    for (int drawer_idx = 0; drawer_idx < DRAWERn; drawer_idx++)
    {
        drawer_ctrl_info_t *drawer_info = drawer_ctrl_fsm_get_info(drawer_idx);

        if (drawer_info->request_type == DRAWER_REQUEST_TYPE_MANUAL)
        {
            feeder_dbg("drawer no [%d] skipped, manually opened [%d]\r\n", drawer_idx + 1, drawer_info->status.curr);
            continue;
        }

        /*check if daily option is disabled */
        for (int meal_idx = 0; meal_idx < FEEDER_MEALn; meal_idx++)
        {
            /*check if date matches*/
            meal_data = &drawer->config[meal_idx];

            if (drawer->daily_st == FEEDER_DAILY_MEAL_DISABLE)
            {
                if (date_match(&meal_data->date, &rtc_date) == false)
                {
                    feeder_dbg("date mismatch, skip meal [%d]\r\n", meal_idx);
                    continue;
                }
            }

            /* check if open time matches */
            if (time_match(&meal_data->time.open, &rtc_time))
            {
                if (drawer_info->status.curr == DRAWER_ST_CLOSE && drawer_info->status.next == DRAWER_ST_INVALID)
                {
                    feeder_dbg("Open Request : Date { day : [%d], month [%d] }, Tine { hour [%d], min [%d], am_fm [%s]}\r\n",
                               meal_data->date.day, meal_data->date.month, meal_data->time.close.hour, meal_data->time.close.minute,
                               am_fm_str[meal_data->time.close.am_fm]);
                    // write_request_to_drawer_to_open(open drawer );
                }
            }

            /* check if close time matches */
            if (time_match(&meal_data->time.close, &rtc_time))
            {
                if (drawer_info->status.curr == DRAWER_ST_OPEN && drawer_info->status.next == DRAWER_ST_INVALID)
                {
                    feeder_dbg("Close Request : Date { day : [%d], month [%d] }, Tine { hour [%d], min [%d], am_fm [%s]}\r\n",
                               meal_data->date.day, meal_data->date.month, meal_data->time.close.hour, meal_data->time.close.minute,
                               am_fm_str[meal_data->time.close.am_fm]);
                    // write_request_to_drawer_to_open(close drawer );
                }
            }
        }
        drawer++;
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
