#include "drawer_fsm.h"
#include "printf_dbg.h"

/**@brief Enable/Disable debug messages */
#define DRAWERS_FSM_DBG 1
#define DRAWER_FSM_TAG "drawer ctrl : "

/**@brief uart debug function for server comm operations  */
#if DRAWERS_FSM_DBG
#define drawer_dbg(format, ...)         \
    do                                           \
    {											\
    	printf_dbg_color(E_MAG, DRAWER_FSM_TAG); \
        printf_dbg(format,##__VA_ARGS__ );       \
    } while (0)
#else
#define drawer_dbg(format, ...) \
    do                                 \
    { /* Do nothing */                 \
    } while (0)
#endif

/**
 * @brief State Machine States
 */
typedef enum
{
    ST_DRAWER_CTRL_INVALID = 0,
    ST_DRAWER_CTRL_WATCHER,
    ST_DRAWER_CTRL_OPENING,
    ST_DRAWER_CTRL_CLOSING,
    ST_DRAWER_CTRL_LAST,
}drawer_ctrl_state_t;

/**
 * @brief Internal Events 
 */
typedef enum
{
    EVT_INT_DRAWER_CTRL_INVALID = 0,
    EVT_INT_DRAWER_CTRL_REQ_FAIL = 0,
    EVT_INT_DRAWER_CTRL_LAST,
}drawer_ctrl_ev_int_name_t;

typedef struct 
{
    drawer_ctrl_ev_int_name_t name;
}drawer_ctrl_ev_int_t;

typedef struct
{
    drawer_ctrl_ev_int_t internal; 
    drawer_ctrl_ev_ext_t external; 
}drawer_ctrl_event_t;

typedef struct
{
    drawer_no_t        active_drawer;
    drawer_ctrl_info_t drawers[DRAWERn];
}drawer_ctrl_iface_t;

struct drawer_ctrl_fsm_t
{
    drawer_ctrl_state_t state;
    drawer_ctrl_event_t event;
    drawer_ctrl_iface_t iface;
};

static struct drawer_ctrl_fsm_t drawer_ctrl;         

//------------------ Static State Function Prototypes ---------------------------------------//

drawer_ctrl_handle_t drawer_ctrl_fsm_get(void)
{
    return &drawer_ctrl;
}

drawer_ctrl_info_t *drawer_ctrl_get_info(drawer_no_t drawer_no)
{
    return &drawer_ctrl.iface.drawers[drawer_no];
}


/**
 * @brief Set next state in FSM
 */
static void fsm_set_next_state(drawer_ctrl_handle_t handle, drawer_ctrl_state_t next_state)
{
	handle->state = next_state;
	handle->event.internal.name = EVT_INT_DRAWER_CTRL_INVALID;
	handle->event.external.name = EVT_EXT_DRAWER_CTRL_INVALID;
}

// ---------------- Static State Function Declarations ----------------------------------------//

static void enter_seq_watcher(drawer_ctrl_handle_t handle);
static void entry_action_watcher(drawer_ctrl_handle_t handle);
static void watcher_on_react(drawer_ctrl_handle_t handle);
static void during_action_watcher(drawer_ctrl_handle_t handle);

static void enter_seq_drawer_opening(drawer_ctrl_handle_t handle);
static void entry_action_drawer_opening(drawer_ctrl_handle_t handle);
static void drawer_opening_on_react(drawer_ctrl_handle_t handle);

static void enter_seq_drawer_closing(drawer_ctrl_handle_t handle);
static void entry_action_drawer_closing(drawer_ctrl_handle_t handle);
static void drawer_closing_on_react(drawer_ctrl_handle_t handle);


// ----------------- Miscellaneous Functions Declarations --------------------------------------------------//
static bool is_drawer_closed(drawer_ctrl_handle_t handle);
static bool is_drawer_open(drawer_ctrl_handle_t handle);
static void drawers_status_update(drawer_ctrl_handle_t handle);


//------------------ Static State Function Definition ---------------------------------------//

static void enter_seq_watcher(drawer_ctrl_handle_t handle)
{
    drawer_ctrl_dbg("enter seq\t[ watcher ]\r\n");
    fsm_set_next_state(handle, ST_DRAWER_CTRL_WATCHER);
    entry_action_watcher(handle);
}

static void entry_action_watcher(drawer_ctrl_handle_t handle)
{
    drawers_status_update(handle);    
}

static void watcher_on_react(drawer_ctrl_handle_t handle)
{
    bool did_transition = false;
    if (handle->event.external.name == EVT_EXT_DRAWER_CTRL_OPEN)
    {
        drawer_no_t drawer_no = handle->event.external.data.drawer_no;
        drawer_st_t curr_st = handle->iface.drawers[drawer_no].status.curr;

        if(curr_st == DRAWER_ST_OPEN || curr_st == DRAWER_ST_OPENING)
        {
            drawer_dbg("drawer [%d] already open or opening [%d]...\r\n", drawer_no + 1, curr_st);
            enter_seq_watcher(handle);
            return;
        }

        enter_seq_drawer_opening(handle);
        handle->iface.drawers[drawer_no].status.next = DRAWER_ST_OPEN;

        did_transition = true;
    }
    else if (handle->event.external.name == EVT_EXT_DRAWER_CTRL_CLOSE)
    {
        drawer_no_t drawer_no = handle->event.external.data.drawer_no;
        drawer_st_t curr_st = handle->iface.drawers[drawer_no].status.curr;

        if(curr_st == DRAWER_ST_CLOSE || curr_st == DRAWER_ST_CLOSING)
        {
            drawer_dbg("drawer [%d] already close or closing [%d]...\r\n", drawer_no + 1, curr_st);
            enter_seq_watcher(handle);
            return;
        }

        enter_seq_drawer_opening(handle);
        handle->iface.drawers[drawer_no].status.next = DRAWER_ST_CLOSE;
        did_transition = true;
    }

    if(did_transition == false)
    {
        during_action_watcher(handle);
    }
}

static void during_action_watcher(drawer_ctrl_handle_t handle)
{
    drawers_status_update(handle);
}


static void enter_seq_control_temp(drawer_ctrl_handle_t handle)
{
    drawer_ctrl_dbg("enter seq\t[ control temp ]\r\n");
    fsm_set_next_state(handle, ST_drawer_ctrl_CONTROL);
    entry_action_control_temp(handle);
}

static void entry_action_control_temp(drawer_ctrl_handle_t handle)
{
    thermostat_info_t *info = &handle->iface.therm_info;

    if (info->control.unit == TEMP_UNITS_CELSIUS)
    {
        if (IS_TEMP_CELSIUS_TOO_COLD(info->control.temp, info->sensed.temp))
            handle->event.internal.name = EVT_INT_TEMP_TOO_COLD;

        else if (IS_TEMP_CELSIUS_TOO_HOT(info->control.temp, info->sensed.temp))
            handle->event.internal.name = EVT_INT_TEMP_TOO_HOT;
    }
    else
    {
        if (IS_TEMP_FAHRENHEIT_TOO_COLD(info->control.temp, info->sensed.temp))
            handle->event.internal.name = EVT_INT_TEMP_TOO_COLD;

        else if (IS_TEMP_FAHRENHEIT_TOO_HOT(info->control.temp, info->sensed.temp))
            handle->event.internal.name = EVT_INT_TEMP_TOO_HOT;
    }

    time_event_start(&handle->event.time.sampling_period, TEMP_SAMPLING_PERIOD_MS);

}

static void control_temp_on_react(drawer_ctrl_handle_t handle)
{
    if(handle->event.internal.name == EVT_INT_TEMP_TOO_COLD)
    {
        /*!<TODO : */
        // turn_off_cooler()
        drawer_ctrl_dbg("temperature too high, turn on cooler \r\n");
        handle->event.internal.name = EVT_INT_drawer_ctrl_INVALID;

    }
    else if(handle->event.internal.name == EVT_INT_TEMP_TOO_HOT)
    {
        /*Transition action */
        // turn_on_cooler()
        drawer_ctrl_dbg("temperature too high, turn off cooler \r\n");
        handle->event.internal.name = EVT_INT_drawer_ctrl_INVALID;
    }

    /*sensing time elapsed */
    if(time_event_is_raised(&handle->event.time.sampling_period) == true)
    {
        measure_temperature(handle);

        if(is_temperature_out_of_range(handle) == false)
        {
            enter_seq_watcher(handle);
            time_event_stop(&handle->event.time.sampling_period);
        }
        else
        {
            time_event_start(&handle->event.time.sampling_period, TEMP_SAMPLING_PERIOD_MS);
        }
    }
}

//------------------ FSM generic Functions ---------------------------------------//

drawer_ctrl_handle_t drawer_ctrl_fsm_get(void)
{
    return &drawer_ctrl;
}

thermostat_info_t *drawer_ctrl_get_info(void)
{
    return &drawer_ctrl.iface.therm_info;
}



void drawer_ctrl_fsm_run(drawer_ctrl_handle_t handle)
{
    switch (handle->state)
    {
    case ST_drawer_ctrl_SENSING:  watcher_on_react(handle); break;
    case ST_drawer_ctrl_CONTROL:  control_temp_on_react(handle); break;
    default:
        break;
    }
}

void drawer_ctrl_fsm_init(drawer_ctrl_handle_t handle)
{
    handle->iface.therm_info.control.unit = TEMP_UNITS_CELSIUS;
    handle->iface.therm_info.control.temp = drawer_ctrl_DEFAULT_CELSIUS_VALUE;
    handle->iface.therm_info.control.status = drawer_ctrl_ENABLE;
    
    enter_seq_watcher(handle);
}


void drawer_ctrl_fsm_time_update(drawer_ctrl_handle_t handle)
{
	time_event_t *time_event = (time_event_t *)&handle->event.time;
	for (int tev_idx = 0; tev_idx < sizeof(handle->event.time) / sizeof(time_event_t); tev_idx++)
	{
		time_event_update(time_event);
		time_event++;
	}
}


void drawer_ctrl_fsm_write_event(drawer_ctrl_handle_t handle, event_t *event)
{
    drawer_ctrl_ev_ext_data_t *data = (drawer_ctrl_ev_ext_data_t*)&event->data;
    thermostat_info_t *info = &handle->iface.therm_info;

    if(event->info.name == EVT_EXT_SET_TEMP_CONFIG)
    {
        handle->event.external.name = event->info.name;
        info->control.temp = data->config.control.temp;
        info->control.status = data->config.control.status;
        info->control.unit = data->config.control.unit;
    }
}

drawer_ctrl_info_t *drawer_fsm_get_info(drawer_no_t no)
{
    return &drawer_info[no];
}

void drawer_fsm_set_ext_evt(event_t *event)
{
    if(IS_DRAWER_EV_EXT(event->info.name))
    {
        drawer_no_t drawer_no = ((drawer_ev_ext_data_t*)event->data.buff)->no;

        if(event->info.name == EVT_EXT_DRW_OPEN)
        {
            drawer_info[drawer_no].status.curr = DRAWER_ST_OPEN;
        }
        else if(event->info.name == EVT_EXT_DRW_CLOSE)
        {
            drawer_info[drawer_no].status.curr = DRAWER_ST_CLOSE;
        }
    }
    
}


// ----------------- Miscellaneous Functions Definition  --------------------------------------------------//
static bool is_drawer_closed(drawer_ctrl_handle_t handle)
{

}
static bool is_drawer_open(drawer_ctrl_handle_t handle)
{

}
static void drawers_status_update(drawer_ctrl_handle_t handle)
{

}