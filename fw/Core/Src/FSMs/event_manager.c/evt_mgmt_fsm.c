#include "evt_mgmt_fsm.h"
#include "printf_dbg.h"

/**@brief Enable/Disable debug messages */
#define EVT_MGMT_DEBUG 0
#define EVT_MGMT_TAG "evt mgmt : "

/**@brief uart debug function for server comm operations  */
#if EVT_MGMT_DEBUG
#define evt_mgmt_dbg(format, ...)         \
    do                                           \
    {											\
    	printf_dbg_color(E_YEL, EVT_MGMT_TAG); \
        printf_dbg(format,##__VA_ARGS__ );       \
    } while (0)
#else
#define evt_mgmt_dbg(format, ...) \
	do                                       \
	{ /* Do nothing */                       \
	} while (0)
#endif

/**
 * @brief State Machine States
 */
typedef enum
{
    ST_INVALID = 0,
    ST_WAIT_EVENT,
    ST_NOTIFY_EVENT,
}evt_mgmt_state_t;

/**
 * @brief Internal Events 
 */
typedef enum
{
    EVT_INT_INVALID = 0,
    EVT_UNREAD_EVENT,
    EVT_NOTIFICATION_DONE,
}evt_mgmt_ev_int_t;

typedef struct 
{
    evt_mgmt_ev_int_t internal;
}evt_mgmt_event_t;

typedef struct
{
    event_t event;
    event_queue_t queue;
}evt_mgmt_iface_t;

struct evt_mgmt_fsm_t
{
    evt_mgmt_state_t state;
    evt_mgmt_event_t event;
    evt_mgmt_iface_t iface;
};

/**
 * @brief There will be a unique instance of Evt Mgmt, we can keep only one
 * 
 */
#define EVT_MANAGER_QUEUE_BUFF_LEN (200)
static uint8_t queue_buffer[EVT_MANAGER_QUEUE_BUFF_LEN];
static struct evt_mgmt_fsm_t evt_mgmt_fsm;                  

//------------------ Static State Function Prototypes ---------------------------------------//
static void enter_seq_wait_event(evt_mgmt_handle_t handle);
static void exit_action_wait_event(evt_mgmt_handle_t handle);
static void during_action_wait_event(evt_mgmt_handle_t handle);
static void wait_event_on_react(evt_mgmt_handle_t handle);

static void enter_seq_notify_event(evt_mgmt_handle_t handle);
static void entry_action_notify_event(evt_mgmt_handle_t handle);
static void notify_event_on_react(evt_mgmt_handle_t handle);


//------------------ FSM generic Functions ---------------------------------------//

evt_mgmt_handle_t evt_mgmt_fsm_get(void)
{
    return &evt_mgmt_fsm;
}

/**
 * @brief Set next state in FSM
 */
static void fsm_set_next_state(evt_mgmt_handle_t handle, evt_mgmt_state_t next_state)
{
	handle->state = next_state;
	handle->event.internal = EVT_INT_INVALID;
}

void evt_mgmt_fsm_run(evt_mgmt_handle_t handle)
{
    switch (handle->state)
    {
    case ST_WAIT_EVENT      : wait_event_on_react(handle); break;
    case ST_NOTIFY_EVENT    : notify_event_on_react(handle); break;    
    default: break;
    }
}

void evt_mgmt_fsm_init(evt_mgmt_handle_t handle)
{
    event_queue_init(&handle->iface.queue, queue_buffer, EVT_MANAGER_QUEUE_BUFF_LEN);
    enter_seq_wait_event(handle);
}

//------------------ Static State Function Definition ---------------------------------------//

static void enter_seq_wait_event(evt_mgmt_handle_t handle)
{
    evt_mgmt_dbg("enter seq \t[ wait event ]\r\n");
    fsm_set_next_state(handle, ST_WAIT_EVENT);
}

static void exit_action_wait_event(evt_mgmt_handle_t handle)
{
    event_queue_read(&handle->iface.queue, &handle->iface.event);
}

static void during_action_wait_event(evt_mgmt_handle_t handle)
{
    if(event_queue_get_pending(&handle->iface.queue))
        handle->event.internal = EVT_UNREAD_EVENT;
}

static void wait_event_on_react(evt_mgmt_handle_t handle)
{
    bool did_transition = true;
    if(handle->event.internal == EVT_UNREAD_EVENT)
    {
        exit_action_wait_event(handle);
        enter_seq_notify_event(handle);
    }
    else
        did_transition = false;

    if(did_transition == false)
    {
        during_action_wait_event(handle);
    }
}

static void enter_seq_notify_event(evt_mgmt_handle_t handle)
{
    evt_mgmt_dbg("enter seq \t[ notify event ]\r\n");
    fsm_set_next_state(handle, ST_NOTIFY_EVENT);
    entry_action_notify_event(handle);
}

static void entry_action_notify_event(evt_mgmt_handle_t handle)
{
    /*Notify event to dest FSM*/
    evt_mgmt_dbg("\t notify evt = [0x%X], from [0x%X] to [0x%X]\r\n",
                 handle->iface.event.header.event,
                 handle->iface.event.header.fsm_src,
                 handle->iface.event.header.fsm_dst);

    switch (handle->iface.event.header.fsm_src)
    {
    /* 
    case FSM_TEMPERATURE_CTRL : 
        temp_ctrl_fsm_write_evt(&temp_fsm, &handle->iface.event);
    break;
    */
    
    default:
        break;
    }

    handle->event.internal = EVT_NOTIFICATION_DONE;
}

static void notify_event_on_react(evt_mgmt_handle_t handle)
{
    if(handle->event.internal == EVT_NOTIFICATION_DONE)
    {
        enter_seq_wait_event(handle);
    }
}

//------------------ Event Manager Function Definition ---------------------------------------//

uint8_t evt_mgmt_write(evt_mgmt_handle_t handle, event_t *event)
{
    return event_queue_write(&handle->iface.queue, event);
}

uint8_t evt_mgmt_read(evt_mgmt_handle_t handle, event_t *event)
{
    return event_queue_read(&handle->iface.queue, event);
}

uint8_t evt_mgmt_fetch(evt_mgmt_handle_t handle, event_t *event)
{
    return event_queue_fetch(&handle->iface.queue, event);
}




