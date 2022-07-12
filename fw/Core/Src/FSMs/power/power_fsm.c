#include "power_fsm.h"
#include "printf_dbg.h"
#include "battery.h"

#define BATT_CHARGE_READ_VAL_PERIOD_MS      (20*1000) // 20s
#define BATT_LEVEL_PERCENT_TO_EXIT_LOW_POWER_MODE   (50)

/**@brief Enable/Disable debug messages */
#define PWR_FSM_DEBUG 0
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

/* Static Power Function API declaration */
static battery_st_t pwr_api_get_status(void);
static uint16_t pwr_api_get_voltage(void);
static uint8_t pwr_api_get_voltage_percent(void);
static void pwr_api_get_power_src(void);
static void pwr_api_enter_enter_low_power_mode(void);
static void pwr_api_enter_exit_low_power_mode(void);


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

void power_fsm_time_update(power_handle_t handle)
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
    handle->iface.info.battery.status  =  pwr_api_get_status();
    handle->iface.info.battery.voltage =  pwr_api_get_voltage();
    handle->iface.info.battery.charge_percent  =  pwr_api_get_voltage_percent();

    switch (handle->iface.info.battery.status)
    {
        case BATT_ST_NO_DETECTED: { pwr_fsm_dbg("warning : BATT NOT DETECTED!!!!\r\n"); } break;
        case BATT_ST_CHARGING:    { pwr_fsm_dbg("BATT CHARGING... \r\n"); } break;
        case BATT_ST_DISCHARGING: { pwr_fsm_dbg("BATT DISCHARGING... \r\n"); } break;
        case BATT_ST_LOW_BATTERY: { pwr_fsm_dbg("warning : LOW BATTERY...\r\n");
                                    handle->event.internal.name = EV_INT_LOW_POWER_MODE_ENTER; } break;
    default:
        break;
    }

    /*start time for batt charge reading */
    time_event_start(&handle->event.time.update_info, BATT_CHARGE_READ_VAL_PERIOD_MS);
}

static void transition_action_idle(power_handle_t handle)
{
    pwr_api_enter_enter_low_power_mode();
}

static void idle_on_react(power_handle_t handle)
{
    if(handle->event.internal.name == EV_INT_LOW_POWER_MODE_ENTER)
    {
        transition_action_idle(handle);
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
    handle->iface.info.battery.status  =  pwr_api_get_status();
    handle->iface.info.battery.voltage =  pwr_api_get_voltage();
    handle->iface.info.battery.charge_percent  =  pwr_api_get_voltage_percent();

    switch (handle->iface.info.battery.status)
    {
        case BATT_ST_NO_DETECTED: { pwr_fsm_dbg("warning : BATT NOT DETECTED!!!!\r\n"); } break;
        case BATT_ST_CHARGING:    { pwr_fsm_dbg("BATT CHARGING... \r\n"); } break;
        case BATT_ST_DISCHARGING: { pwr_fsm_dbg("BATT DISCHARGING... \r\n"); } break;
        case BATT_ST_LOW_BATTERY: { pwr_fsm_dbg("warning : LOW BATTERY...\r\n"); } break;
    default:
        break;
    }

    if(handle->iface.info.battery.charge_percent > BATT_LEVEL_PERCENT_TO_EXIT_LOW_POWER_MODE)
    {
        handle->event.internal.name = EV_INT_LOW_POWER_MODE_EXIT;
    }

    /*start time for batt charge reading */
    time_event_start(&handle->event.time.update_info, BATT_CHARGE_READ_VAL_PERIOD_MS);
}

static void transition_action_low_power(power_handle_t handle)
{
    pwr_api_enter_exit_low_power_mode();
}

static void low_power_on_react(power_handle_t handle)
{
    if(handle->event.internal.name == EV_INT_LOW_POWER_MODE_EXIT)
    {
        transition_action_low_power(handle);
        enter_seq_idle(handle);
    }

    if (time_event_is_raised(&handle->event.time.update_info) == true)
    {
        enter_seq_idle(handle);
    }
}


/* Static Power Function API definition */
static battery_st_t pwr_api_get_status(void)
{
    static uint16_t batt_mv_prev = 0;
    uint16_t batt_mv_curr = battery_read_voltage();
    battery_st_t batt_st = BATT_ST_INVALID;

    if(batt_mv_curr >= batt_mv_prev) {
        batt_st = BATT_ST_CHARGING;
    }
    else if(batt_mv_curr < batt_mv_prev) {
        batt_st = BATT_ST_DISCHARGING;
    }

    batt_mv_prev = batt_mv_curr;

    /*Check if battery is too low or if battery is not connected */
    uint8_t batt_percent = battery_read_voltage_percent();
    if(batt_percent < 20) {
        batt_st = BATT_ST_LOW_BATTERY;
    }

    // !< TODO: find the value read by the ADC when no battery is connected and update here. 
    else if(!batt_percent) {
        batt_st = BATT_ST_NO_DETECTED;
    }

    return batt_st;
}

static uint16_t pwr_api_get_voltage(void)
{
    return battery_read_voltage();
}

static uint8_t pwr_api_get_voltage_percent(void)
{
    return battery_read_voltage_percent();
}

static void pwr_api_enter_enter_low_power_mode(void)
{
    pwr_fsm_dbg("entering low power mode ...\r\n");
    /*1. Reduce Clock Speed */

    /*2. Disable Temperature Control */

    /*3. Think about other ones*/
}

static void pwr_api_enter_exit_low_power_mode(void)
{
    pwr_fsm_dbg("exciting low power mode ...\r\n");
    /*1. Restore Clock Speed */

    /*2. Enable Temperature Control */

    /*3. Think about other ones*/
}




