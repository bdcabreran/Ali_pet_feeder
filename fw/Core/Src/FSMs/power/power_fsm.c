#include "power_fsm.h"
#include "printf_dbg.h"

#define BATT_CHARGE_READ_VAL_PERIOD_MS      (20*1000) // 20s

/**@brief Enable/Disable debug messages */
#define PWR_FSM_DEBUG 1
#define PWR_FSM_TAG "pwr : "

/**@brief uart debug function for server comm operations  */
#if PWR_FSM_DEBUG
#define pwr_fsm_dbg(format, ...)         \
    do                                           \
    {											\
    	printf_dbg_color(E_YEL, PWR_FSM_TAG); \
        printf_dbg(format,##__VA_ARGS__ );       \
    } while (0)
#else
#define pwr_fsm_dbg(format, ...) \
	do                                       \
	{ /* Do nothing */                       \
	} while (0)
#endif

typedef enum 
{
    PWR_FSM_ST_INVALID,
    PWR_FSM_ST_IDLE,
    PWR_FSM_ST_LOW_POWER,
    PWR_FSM_ST_LAST,
}power_fsm_state_t;

typedef enum  
{
    EV_INT_PWR_FSM_INVALID, 
    EV_INT_BATT_NOT_DETECTED,
    EV_INT_LOW_POWER_MODE_ENTER,
    EV_INT_LOW_POWER_MODE_EXIT,
    EV_INT_NO_BATT_DETECTED,
    EV_INT_PWR_FSM_LAST, 
}power_fsm_int_evt_name_t;

typedef struct
{
    power_fsm_int_evt_name_t name;
}power_fsm_int_evt_t;

typedef struct
{
    time_event_t update_info;
}power_fsm_time_event_t;

typedef struct
{
    power_fsm_int_evt_t internal;
    power_fsm_time_event_t time;
}power_fsm_event_t;

typedef struct
{
    power_fsm_info_t info;

}power_fsm_iface_t;

struct power_fsm_t
{
    power_fsm_state_t state;
    power_fsm_iface_t iface;
    power_fsm_event_t event;
};

struct power_fsm_t power_fsm;

/* Static State Function Definition */
static void enter_seq_idle(power_handle_t handle);
static void enter_seq_low_power(power_handle_t handle);

static void entry_action_idle(power_handle_t handle);
static void entry_action_low_power(power_handle_t handle);

static void idle_on_react(power_handle_t handle);
static void low_power_on_react(power_handle_t handle);

/* Static function definition */
static void fsm_set_next_state(power_handle_t handle, power_fsm_state_t next_st);


/* Public function definition */

power_fsm_info_t *power_fsm_get_info(void)
{
    return &power_fsm.iface.info;
}

power_handle_t power_fsm_get(void)
{
    return &power_fsm;
}

void power_fsm_init(power_handle_t handle)
{
    enter_seq_idle(handle);
}

void power_fsm_run(power_handle_t handle)
{
    switch (handle->state)
    {
    case PWR_FSM_ST_IDLE:       { idle_on_react(handle);       } break;
    case PWR_FSM_ST_LOW_POWER: {  low_power_on_react(handle); } break;
    
    default:
        break;
    }
}

void power_fsm_time_event_update(power_handle_t handle)
{
	time_event_t *time_event = (time_event_t *)&handle->event.time;
	for (int tev_idx = 0; tev_idx < sizeof(handle->event.time) / sizeof(time_event_t); tev_idx++)
	{
		time_event_update(time_event);
		time_event++;
	}
}

/* Static function Definition */
static void fsm_set_next_state(power_handle_t handle, power_fsm_state_t next_st)
{
    handle->state = next_st;
    handle->event.internal.name = EV_INT_PWR_FSM_INVALID;
}


/* static state function definition */
static void enter_seq_idle(power_handle_t handle)
{
    pwr_fsm_dbg("enter seq/t [ IDLE ]\r\n");
    fsm_set_next_state(handle, PWR_FSM_ST_IDLE);
    entry_action_idle(handle);
}
static void entry_action_idle(power_handle_t handle)
{
    /* Read battery status, Read power supply */
    // handle->iface.info.battery.status =  batt_api_get_status();
    // handle->iface.info.power_src =  batt_api_get_power_src();

    if(handle->iface.info.battery.status == BATT_ST_CHARGING)
    {
        // handle->iface.info.battery.charge =  batt_api_get_charge();
    }

    if(handle->iface.info.battery.status == BATT_ST_NO_DETECTED)
    {
        pwr_fsm_dbg("warning : BATT NOT DETECTED!!!!\r\n");
        handle->event.internal.name = EV_INT_NO_BATT_DETECTED;
    }

    if(handle->iface.info.battery.status == BATT_ST_DISCHARGING)
    {
        handle->event.internal.name = EV_INT_LOW_POWER_MODE_ENTER;
    }

    /*start time for batt charge reading */
    time_event_start(&handle->event.time.update_info, BATT_CHARGE_READ_VAL_PERIOD_MS);
}
static void idle_on_react(power_handle_t handle)
{
    if(handle->event.internal.name == EV_INT_LOW_POWER_MODE_ENTER)
    {
        // pwr_api_enter_enter_low_power_mode()
        enter_seq_low_power(handle);
    }

    if (time_event_is_raised(&handle->event.time.update_info) == true)
    {
        enter_seq_idle(handle);
    }
}

static void enter_seq_low_power(power_handle_t handle)
{
    fsm_set_next_state(handle, PWR_FSM_ST_LOW_POWER);
    entry_action_low_power(handle);
}

static void entry_action_low_power(power_handle_t handle)
{
    // handle->iface.info.power_src =  batt_api_get_power_src();
    // handle->iface.info.battery.status =  batt_api_get_status();

    if(handle->iface.info.battery.status == BATT_ST_CHARGING)
    {
        // handle->iface.info.battery.charge =  batt_api_get_charge();
        handle->event.internal.name = EV_INT_LOW_POWER_MODE_EXIT;
    }

    if(handle->iface.info.battery.status == BATT_ST_NO_DETECTED)
    {
        pwr_fsm_dbg("warning : BATT NOT DETECTED!!!!\r\n");
        handle->event.internal.name = EV_INT_NO_BATT_DETECTED;
    }

    /*start time for batt charge reading */
    time_event_start(&handle->event.time.update_info, BATT_CHARGE_READ_VAL_PERIOD_MS);
}

static void low_power_on_react(power_handle_t handle)
{
    if(handle->event.internal.name == EV_INT_LOW_POWER_MODE_EXIT)
    {
        // pwr_api_enter_exit_low_power_mode()
        enter_seq_idle(handle);
    }

    if (time_event_is_raised(&handle->event.time.update_info) == true)
    {
        enter_seq_idle(handle);
    }
}
