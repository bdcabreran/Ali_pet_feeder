#include "petcall_fsm.h"
#include "printf_dbg.h"

/**@brief Enable/Disable debug messages */
#define PETCALL_FSM_DBG 1
#define PETCALL_TAG "petcall : "

/**@brief uart debug function for server comm operations  */
#if PETCALL_FSM_DBG
#define petcall_dbg(format, ...)         \
    do                                           \
    {											\
    	printf_dbg_color(E_MAG, PETCALL_TAG); \
        printf_dbg(format,##__VA_ARGS__ );       \
    } while (0)
#else
#define petcall_dbg(format, ...) \
    do                                 \
    { /* Do nothing */                 \
    } while (0)
#endif


#define RECORDING_TIMEOUT_MS        (10000)
#define PLAY_PETCALL_TIMEOUT_MS     (20000)
/**
 * @brief State Machine States
 */
typedef enum
{
    ST_PETCALL_INVALID = 0,
    ST_PETCALL_INACTIVE,
    ST_PETCALL_ACTIVE,
    ST_PETCALL_RECORD,
    ST_PETCALL_PLAY,
    ST_PETCALL_LAST,
}petcall_state_t;

/**
 * @brief Internal Events 
 */
typedef enum
{
    EVT_INT_PETCALL_INVALID = 0,
    EVT_INT_PETCALL_PLAY_CPLT,
    EVT_INT_PETCALL_RECORD_OK,
    EVT_INT_PETCALL_LAST,
}petcall_ev_int_name_t;

typedef struct 
{
    petcall_ev_int_name_t name;
}petcall_ev_int_t;

typedef struct
{
    time_event_t play_timeout;
    time_event_t rec_timeout;
    
}petcall_ev_time_t;

typedef struct
{
    petcall_ev_int_t internal; 
    petcall_ev_ext_t external; 
    petcall_ev_time_t time;
}petcall_event_t;

typedef struct
{
    petcall_config_info_t petcall_info;
}petcall_iface_t;

struct petcall_fsm_t
{
    petcall_state_t state;
    petcall_event_t event;
    petcall_iface_t iface;
};

static struct petcall_fsm_t petcall_fsm;

/*********** Static FSM Functions Declaration **********/
static void enter_seq_inactive(petcall_handle_t handle);
static void enter_seq_active(petcall_handle_t handle);
static void enter_seq_record(petcall_handle_t handle);
static void enter_seq_play(petcall_handle_t handle);

static void inactive_on_react(petcall_handle_t handle);
static void active_on_react(petcall_handle_t handle);
static void record_on_react(petcall_handle_t handle);
static void play_on_react(petcall_handle_t handle);

static void entry_action_record(petcall_handle_t handle);
static void entry_action_play(petcall_handle_t handle);
static void entry_action_inactive(petcall_handle_t handle);
static void entry_action_active(petcall_handle_t handle);

/*********** Static Miscellaneous Functions Declaration **********/
static void petcall_remove_rec_file(petcall_handle_t handle);
static void petcall_record_start(petcall_handle_t handle);
static void petcall_record_stop(petcall_handle_t handle);
static void petcall_play_start(petcall_handle_t handle);
static void petcall_play_stop(petcall_handle_t handle);

/************ Public Function definition *************/

petcall_handle_t petcall_fsm_get(void)
{
    return &petcall_fsm;
}

petcall_config_info_t *petcall_fsm_info(void)
{
    return &petcall_fsm.iface.petcall_info;
}

void petcall_fsm_init(petcall_handle_t handle)
{
    /*read from IC if there si a rec file available*/
    /*read from flash petcall config */
    if (handle->iface.petcall_info.status == PETCALL_ENABLE &&
        handle->iface.petcall_info.rec_file_available == true)
    {
        enter_seq_active(handle);
    }
    else
    {
       enter_seq_inactive(handle);
    }
}


void petcall_fsm_run(petcall_handle_t handle)
{
    switch (handle->state)
    {
    case ST_PETCALL_INACTIVE: inactive_on_react(handle); break;
    case ST_PETCALL_ACTIVE:   active_on_react(handle); break;
    case ST_PETCALL_RECORD:   record_on_react(handle); break;
    case ST_PETCALL_PLAY:     play_on_react(handle); break;
    default:
        break;
    }
}

void petcall_fsm_write_event(petcall_handle_t handle, event_t *event)
{
    if(IS_PETCALL_EXT_EVT(event->info.name))
    {
        handle->event.external.name = event->info.name;
    }
}


void petcall_fsm_time_update(petcall_handle_t handle)
{
	time_event_t *time_event = (time_event_t *)&handle->event.time;
	for (int tev_idx = 0; tev_idx < sizeof(handle->event.time) / sizeof(time_event_t); tev_idx++)
	{
		time_event_update(time_event);
		time_event++;
	}
}

/*********** Static FSM Functions Definition **********/
static void fsm_set_next_state(petcall_handle_t handle, petcall_state_t next)
{
    handle->state = next;
    handle->event.external.name = EVT_EXT_PETCALL_INVALID;
    handle->event.internal.name = EVT_INT_PETCALL_INVALID;
}

/* State INACTIVE Related Functions */

static void enter_seq_inactive(petcall_handle_t handle)
{
    petcall_dbg("enter seq\t [ inactive ]\r\n");
    fsm_set_next_state(handle, ST_PETCALL_INACTIVE);
    entry_action_inactive(handle);
}

static void entry_action_inactive(petcall_handle_t handle)
{
    handle->iface.petcall_info.status = PETCALL_DISABLE;
}

static void inactive_on_react(petcall_handle_t handle)
{
    if(handle->event.external.name == EVT_EXT_PETCALL_ENABLE)
    {
        /*check if there is rec file available */
        if(handle->iface.petcall_info.rec_file_available == true)
            enter_seq_active(handle);
        else {
            petcall_dbg("there are no rec file available\r\n");
            enter_seq_inactive(handle);
        }
    }
    else if(handle->event.external.name == EVT_EXT_PETCALL_RECORD_START)
    {
        enter_seq_record(handle);
    }

    else if(handle->event.external.name == EVT_EXT_PETCALL_PLAY_START)
    {
        enter_seq_play(handle);
    }
}

/* State RECORD Related Functions */

static void enter_seq_record(petcall_handle_t handle)
{
    petcall_dbg("enter seq\t [ record ]\r\n");
    fsm_set_next_state(handle, ST_PETCALL_RECORD);
    entry_action_record(handle);
}

static void entry_action_record(petcall_handle_t handle)
{
    if(handle->iface.petcall_info.rec_file_available == true)
        petcall_remove_rec_file(handle);

    petcall_record_start(handle);
    time_event_start(&handle->event.time.rec_timeout, RECORDING_TIMEOUT_MS);
}

static void record_on_react(petcall_handle_t handle)
{
    if (handle->event.external.name == EVT_EXT_PETCALL_RECORD_STOP ||
        time_event_is_raised(&handle->event.time.rec_timeout))
    {
        petcall_record_stop(handle);

        if (handle->iface.petcall_info.status == PETCALL_ENABLE)
            enter_seq_active(handle);
        else
            enter_seq_inactive(handle);
    }
}

/* State PLAY Related Functions */

static void enter_seq_play(petcall_handle_t handle)
{
    petcall_dbg("enter seq\t [ play ]\r\n");
    fsm_set_next_state(handle, ST_PETCALL_PLAY);    
    entry_action_play(handle);
}

static void entry_action_play(petcall_handle_t handle)
{
    petcall_play_start(handle);
    time_event_start(&handle->event.time.play_timeout, PLAY_PETCALL_TIMEOUT_MS);
}

static void play_on_react(petcall_handle_t handle)
{
    if (handle->event.external.name == EVT_EXT_PETCALL_PLAY_STOP ||
        time_event_is_raised(&handle->event.time.play_timeout) == true)
    {
        petcall_play_stop(handle);
        
        if (handle->iface.petcall_info.status == PETCALL_ENABLE)
            enter_seq_active(handle);
        else
            enter_seq_inactive(handle);
    }
}


/* State ACTIVE Related Functions */

static void enter_seq_active(petcall_handle_t handle)
{
    petcall_dbg("enter seq\t [ active ]\r\n");
    fsm_set_next_state(handle, ST_PETCALL_ACTIVE);
    entry_action_active(handle);
}

static void entry_action_active(petcall_handle_t handle)
{
    handle->iface.petcall_info.status = PETCALL_ENABLE;
}

static void active_on_react(petcall_handle_t handle)
{
    if(handle->event.external.name == EVT_EXT_PETCALL_DISABLE)
    {
        /*check if there is rec file available */
        if(handle->iface.petcall_info.rec_file_available == true)
            enter_seq_active(handle);
        else {
            petcall_dbg("there is no rec file available\r\n");
            enter_seq_inactive(handle);
        }
    }
    else if(handle->event.external.name == EVT_EXT_PETCALL_RECORD_START)
    {
        enter_seq_record(handle);
    }

    else if(handle->event.external.name == EVT_EXT_PETCALL_PLAY_START)
    {
        enter_seq_play(handle);
    }
}

/*********** Static Miscellaneous Functions Definition  **********/

static void petcall_remove_rec_file(petcall_handle_t handle)
{
    petcall_dbg("deleting petcall rec file ...\r\n");
    /*!< TODO : implement remove petcall routine */
    handle->iface.petcall_info.rec_file_available = false;
}

static void petcall_record_start(petcall_handle_t handle)
{
    handle->iface.petcall_info.rec_file_available = true;
    /*!<TODO : start petcall record*/

}

static void petcall_record_stop(petcall_handle_t handle)
{   
    /*!<TODO: stop petcall record*/
}

static void petcall_play_start(petcall_handle_t handle)
{
    /*!<TODO : start petcall audio play*/

}
static void petcall_play_stop(petcall_handle_t handle)
{
    /*!<TODO : stop petcall audio play*/
}
