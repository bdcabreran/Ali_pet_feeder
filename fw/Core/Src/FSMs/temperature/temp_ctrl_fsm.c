#include "event_manager_fsm.h"
#include "time_event.h"
#include "printf_dbg.h"
#include "temp_ctrl_fsm.h"
#include "cooler.h"
#include "ds18b20.h"

/**@brief Enable/Disable debug messages */
#define TEMP_CTRL_FSM_DBG 1
#define TEMP_CTRL_TAG "temp ctrl : "

/**@brief uart debug function for server comm operations  */
#if TEMP_CTRL_FSM_DBG
#define temp_ctrl_dbg(format, ...)         \
    do                                           \
    {											\
    	printf_dbg_color(E_MAG, TEMP_CTRL_TAG); \
        printf_dbg(format,##__VA_ARGS__ );       \
    } while (0)
#else
#define temp_ctrl_dbg(format, ...) \
    do                                 \
    { /* Do nothing */                 \
    } while (0)
#endif

/**
 * @brief State Machine States
 */
typedef enum
{
    ST_TEMP_CTRL_INVALID = 0,
    ST_TEMP_CTRL_SENSING,
    ST_TEMP_CTRL_CONTROL,
    ST_TEMP_CTRL_LAST,
}temp_ctrl_state_t;

/**
 * @brief Internal Events 
 */
typedef enum
{
    EVT_INT_TEMP_CTRL_INVALID = 0,
    EVT_INT_TEMP_OUT_OF_RANGE,
    EVT_INT_TEMP_TOO_COLD,
    EVT_INT_TEMP_TOO_HOT,
    EVT_INT_TEMP_CTRL_LAST,
}temp_ctrl_ev_int_t;

typedef struct 
{
    temp_ctrl_ev_int_t name;
}temp_ctrl_ev_int_name_t;

typedef struct
{
    time_event_t sampling_period;
}temp_ctrl_time_event_t;

typedef struct
{
    temp_ctrl_ev_int_name_t internal; 
    temp_ctrl_ev_ext_name_t external; 
    temp_ctrl_time_event_t  time;  
}temp_ctrl_event_t;

typedef struct
{
    thermostat_config_info_t therm_info;
}temp_ctrl_iface_t;

struct temp_ctrl_fsm_t
{
    temp_ctrl_state_t state;
    temp_ctrl_event_t event;
    temp_ctrl_iface_t iface;
};

static struct temp_ctrl_fsm_t temp_ctrl;         

//------------------ Static State Function Prototypes ---------------------------------------//
static void enter_seq_sensing_temp(temp_ctrl_handle_t handle);
static void entry_action_sensing_temp(temp_ctrl_handle_t handle);
static void sensing_temp_on_react(temp_ctrl_handle_t handle);
static void enter_seq_control_temp(temp_ctrl_handle_t handle);
static void entry_action_control_temp(temp_ctrl_handle_t handle);
static void control_temp_on_react(temp_ctrl_handle_t handle);
static void fsm_set_next_state(temp_ctrl_handle_t handle, temp_ctrl_state_t next_state);

//------------------ Static Miscellaneous Function Prototypes ---------------------------------------//
static bool get_user_configuration_from_flash(temp_ctrl_handle_t handle);
static int  sensor_read_temperature_in_celsius(void);
static int  sensor_read_temperature_in_fahrenheit(void);
static bool is_valid_therm_config(thermostat_config_info_t *info);


static bool is_temperature_out_of_range(temp_ctrl_handle_t handle)
{
    thermostat_config_info_t *info = &handle->iface.therm_info;
    if (info->control.unit == TEMP_UNITS_CELSIUS)
    {
        if(IS_TEMP_CELSIUS_OUT_OF_RANGE(info->control.temp, info->sensed.temp))
            return true;
    }
    else
    {
        if(IS_TEMP_FAHRENHEIT_OUT_OF_RANGE(info->control.temp, info->sensed.temp))
            return true;
    }
    return false;
}

static void measure_temperature(temp_ctrl_handle_t handle)
{
    if (handle->iface.therm_info.control.unit == TEMP_UNITS_CELSIUS)
    {
        handle->iface.therm_info.sensed.temp = sensor_read_temperature_in_celsius();
        temp_ctrl_dbg("measure temperature in celsius [%d C]\r\n", handle->iface.therm_info.sensed.temp);
    }
    else
    {
        handle->iface.therm_info.sensed.temp = sensor_read_temperature_in_fahrenheit();
        temp_ctrl_dbg("measure temperature in fahrenheit [%d F]\r\n", handle->iface.therm_info.sensed.temp);
    }
}
//------------------ Static State Function Definition ---------------------------------------//

static void enter_seq_sensing_temp(temp_ctrl_handle_t handle)
{
    temp_ctrl_dbg("enter seq\t[ sensing temp ]\r\n");
    fsm_set_next_state(handle, ST_TEMP_CTRL_SENSING);
    entry_action_sensing_temp(handle);
}

static void entry_action_sensing_temp(temp_ctrl_handle_t handle)
{
    time_event_start(&handle->event.time.sampling_period, TEMP_SAMPLING_PERIOD_MS);
}

static void sensing_temp_on_react(temp_ctrl_handle_t handle)
{
    if(time_event_is_raised(&handle->event.time.sampling_period) == true)
    {
        measure_temperature(handle);

        if(is_temperature_out_of_range(handle) == true)
        {
            if(handle->iface.therm_info.control.status == TEMP_CTRL_ENABLE)
            {
                time_event_stop(&handle->event.time.sampling_period);
                enter_seq_control_temp(handle);
                temp_ctrl_dbg("Temp out of range, control is enabled\r\n");
            }
            else
            {
                time_event_stop(&handle->event.time.sampling_period);
                enter_seq_sensing_temp(handle);
                temp_ctrl_dbg("Temp out of range, control is disabled\r\n");
            }
        }

        time_event_start(&handle->event.time.sampling_period, TEMP_SAMPLING_PERIOD_MS);
    }

}

static void enter_seq_control_temp(temp_ctrl_handle_t handle)
{
    temp_ctrl_dbg("enter seq\t[ control temp ]\r\n");
    fsm_set_next_state(handle, ST_TEMP_CTRL_CONTROL);
    entry_action_control_temp(handle);
}

static void entry_action_control_temp(temp_ctrl_handle_t handle)
{
    thermostat_config_info_t *info = &handle->iface.therm_info;

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

static void control_temp_on_react(temp_ctrl_handle_t handle)
{
    if(handle->event.internal.name == EVT_INT_TEMP_TOO_COLD)
    {
        turn_off_cooler();
        temp_ctrl_dbg("temperature too low, turn off cooler \r\n");
        handle->event.internal.name = EVT_INT_TEMP_CTRL_INVALID;
    }
    
    else if(handle->event.internal.name == EVT_INT_TEMP_TOO_HOT)
    {
        turn_on_cooler();
        temp_ctrl_dbg("temperature too high, turn on cooler \r\n");
        handle->event.internal.name = EVT_INT_TEMP_CTRL_INVALID;
    }

    /*sensing time elapsed */
    if(time_event_is_raised(&handle->event.time.sampling_period) == true)
    {
        measure_temperature(handle);

        if(is_temperature_out_of_range(handle) == false)
        {
            time_event_stop(&handle->event.time.sampling_period);
            enter_seq_sensing_temp(handle);
        }
        else
        {
            time_event_start(&handle->event.time.sampling_period, TEMP_SAMPLING_PERIOD_MS);
        }
    }
}

//------------------ FSM generic Functions ---------------------------------------//

temp_ctrl_handle_t temp_ctrl_fsm_get(void)
{
    return &temp_ctrl;
}

thermostat_config_info_t *temp_ctrl_fsm_get_info(void)
{
    return &temp_ctrl.iface.therm_info;
}

/**
 * @brief Set next state in FSM
 */
static void fsm_set_next_state(temp_ctrl_handle_t handle, temp_ctrl_state_t next_state)
{
	handle->state = next_state;
	handle->event.internal.name = EVT_INT_TEMP_CTRL_INVALID;
}

void temp_ctrl_fsm_run(temp_ctrl_handle_t handle)
{
    switch (handle->state)
    {
    case ST_TEMP_CTRL_SENSING:  sensing_temp_on_react(handle); break;
    case ST_TEMP_CTRL_CONTROL:  control_temp_on_react(handle); break;
    default:
        break;
    }
}

void temp_ctrl_fsm_init(temp_ctrl_handle_t handle)
{
    /*Read information from flash and make sure the values are valid */
    get_user_configuration_from_flash(handle);
    measure_temperature(handle);
    turn_off_cooler();
    enter_seq_sensing_temp(handle);
}


void temp_ctrl_fsm_time_update(temp_ctrl_handle_t handle)
{
	time_event_t *time_event = (time_event_t *)&handle->event.time;
	for (int tev_idx = 0; tev_idx < sizeof(handle->event.time) / sizeof(time_event_t); tev_idx++)
	{
		time_event_update(time_event);
		time_event++;
	}
}

void temp_ctrl_fsm_write_event(temp_ctrl_handle_t handle, event_t *event)
{
    temp_ctrl_ev_ext_data_t *data = (temp_ctrl_ev_ext_data_t*)&event->data;
    thermostat_config_info_t *info = &handle->iface.therm_info;

    if(event->info.name == EVT_EXT_SET_TEMP_CONFIG)
    {
        if(is_valid_therm_config(&data->config))
        {   
            /*check if configuration values changed */
            if ( info->control.temp == data->config.control.temp &&
                info->control.status == data->config.control.status &&
                info->control.unit == data->config.control.unit)
            {
                temp_ctrl_dbg("no changes detected in thermostat configuration\r\n");
            }
            else
            {
                handle->event.external.name = event->info.name;
                info->control.temp = data->config.control.temp;
                info->control.status = data->config.control.status;
                info->control.unit = data->config.control.unit;
                temp_ctrl_dbg("Saving new configuration\r\n");
                user_config_set();
            }
        }
        else
        {
            printf_dbg_error("Invalid therm configuration\r\n");
        }
    }
}

static bool is_valid_therm_config(thermostat_config_info_t *info)
{
    if (IS_VALID_TEMP_CTRL_ST(info->control.status) &&
        IS_VALID_TEMP_UNIT_ST(info->control.unit))
    {
        return true;
    }

    return false;
}

static int  sensor_read_temperature_in_celsius(void)
{
    float temp_celcius;
    Ds18b20_ManualConvert(&temp_celcius);
    return (int)temp_celcius;
}

static int  sensor_read_temperature_in_fahrenheit(void)
{
    float temp_celcius;
    Ds18b20_ManualConvert(&temp_celcius);
    return (int)((temp_celcius * 9 / 5) + 32);
}

static bool get_user_configuration_from_flash(temp_ctrl_handle_t handle)
{
    temp_ctrl_dbg("checking if there is available info in flash...\r\n");

    user_config_t *user_config = user_config_get();
    thermostat_config_info_t *flash_config = &user_config->thermostat_info;
    thermostat_config_info_t *ram_config = &handle->iface.therm_info;

    if(user_config->data_available)
    {
        /*check if flash info is valid */
        if (is_valid_therm_config(flash_config))
        {
            temp_ctrl_dbg("valid configuration found, loading data from flash...\r\n");
            temp_ctrl_dbg("info : ctrl status [%s], units [%s], ctrl temp [%d]\r\n",
                          flash_config->control.status == TEMP_CTRL_ENABLE ? "enable" : "disable",
                          flash_config->control.unit == TEMP_UNITS_CELSIUS ? "celsius" : "fahrenheit",
                          flash_config->control.temp);

            ram_config->control.status = flash_config->control.status;
            ram_config->control.unit = flash_config->control.unit;
            ram_config->control.temp = flash_config->control.temp;
            return true;
        }
    }

    temp_ctrl_dbg("No valid data available, fill default config...\r\n");
    ram_config->control.status = TEMP_CTRL_ENABLE;
    ram_config->control.unit = TEMP_UNITS_CELSIUS;
    ram_config->control.temp = TEMP_CTRL_DEFAULT_CELSIUS_VALUE;

    user_config_set();

    return false;
}

