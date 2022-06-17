#include "event_manager_fsm.h"
#include "printf_dbg.h"

/**@brief Enable/Disable debug messages */
#define EVENT_MANAGER_FSM_DBG 0
#define EVENT_MANAGER_TAG "evt mgmt : "

/**@brief uart debug function for server comm operations  */
#if EVENT_MANAGER_FSM_DBG
#define event_manager_dbg(format, ...)         \
    do                                           \
    {											\
    	printf_dbg_color(E_YEL, EVENT_MANAGER_TAG); \
        printf_dbg(format,##__VA_ARGS__ );       \
    } while (0)
#else
#define event_manager_dbg(format, ...) \
    do                                 \
    { /* Do nothing */                 \
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
}event_manager_state_t;

/**
 * @brief Internal Events 
 */
typedef enum
{
    EVT_INT_INVALID = 0,
    EVT_INT_UNREAD_EVENT,
    EVT_INT_NOTIFICATION_DONE,
}event_manager_ev_int_t;

typedef struct 
{
    event_manager_ev_int_t internal;
}event_manager_event_t;

typedef struct
{
    event_t event;
    event_queue_t queue;
}event_manager_iface_t;

struct event_manager_fsm_t
{
    event_manager_state_t state;
    event_manager_event_t event;
    event_manager_iface_t iface;
};

/**
 * @brief There will be a unique instance of Evt Mgmt, we can keep only one
 * 
 */
#define EVT_MANAGER_QUEUE_BUFF_LEN (200)
static uint8_t queue_buffer[EVT_MANAGER_QUEUE_BUFF_LEN];
static struct event_manager_fsm_t event_manager_fsm;                  

//------------------ Static State Function Prototypes ---------------------------------------//
static void enter_seq_wait_event(event_manager_handle_t handle);
static void exit_action_wait_event(event_manager_handle_t handle);
static void during_action_wait_event(event_manager_handle_t handle);
static void wait_event_on_react(event_manager_handle_t handle);

static void enter_seq_notify_event(event_manager_handle_t handle);
static void entry_action_notify_event(event_manager_handle_t handle);
static void notify_event_on_react(event_manager_handle_t handle);


//------------------ FSM generic Functions ---------------------------------------//

event_manager_handle_t event_manager_fsm_get(void)
{
    return &event_manager_fsm;
}

/**
 * @brief Set next state in FSM
 */
static void fsm_set_next_state(event_manager_handle_t handle, event_manager_state_t next_state)
{
	handle->state = next_state;
	handle->event.internal = EVT_INT_INVALID;
}

void event_manager_fsm_run(event_manager_handle_t handle)
{
    switch (handle->state)
    {
    case ST_WAIT_EVENT      : wait_event_on_react(handle); break;
    case ST_NOTIFY_EVENT    : notify_event_on_react(handle); break;    
    default: break;
    }
}

void event_manager_fsm_init(event_manager_handle_t handle)
{
    event_queue_init(&handle->iface.queue, queue_buffer, EVT_MANAGER_QUEUE_BUFF_LEN);
    enter_seq_wait_event(handle);
}

//------------------ Static State Function Definition ---------------------------------------//

static void enter_seq_wait_event(event_manager_handle_t handle)
{
    event_manager_dbg("enter seq \t[ wait event ]\r\n");
    fsm_set_next_state(handle, ST_WAIT_EVENT);
}

static void exit_action_wait_event(event_manager_handle_t handle)
{
    event_queue_read(&handle->iface.queue, &handle->iface.event);
}

static void during_action_wait_event(event_manager_handle_t handle)
{
    if(event_queue_get_pending(&handle->iface.queue))
        handle->event.internal = EVT_INT_UNREAD_EVENT;
}

static void wait_event_on_react(event_manager_handle_t handle)
{
    bool did_transition = true;
    if(handle->event.internal == EVT_INT_UNREAD_EVENT)
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

static void enter_seq_notify_event(event_manager_handle_t handle)
{
    event_manager_dbg("enter seq \t[ notify event ]\r\n");
    fsm_set_next_state(handle, ST_NOTIFY_EVENT);
    entry_action_notify_event(handle);
}

static void entry_action_notify_event(event_manager_handle_t handle)
{
    /*Notify event to dest FSM*/
    event_manager_dbg("\t notify evt = [0x%X], from [0x%X] to [0x%X]\r\n",
                 handle->iface.event.name,
                 handle->iface.event.fsm.src,
                 handle->iface.event.fsm.dst);

    switch (handle->iface.event.fsm.dst)
    {
        case UI_FSM:
        {
            ui_fsm_write_event(ui_fsm_get(), &handle->iface.event);
        }break; 
    
        case DRAWER_FSM:
        {
            drawer_fsm_set_ext_evt(&handle->iface.event);
        }break; 




    default:
        break;
    }

    handle->event.internal = EVT_INT_NOTIFICATION_DONE;
}

static void notify_event_on_react(event_manager_handle_t handle)
{
    if(handle->event.internal == EVT_INT_NOTIFICATION_DONE)
    {
        enter_seq_wait_event(handle);
    }
}

//------------------ Event Manager Function Definition ---------------------------------------//

uint8_t event_manager_write(event_manager_handle_t handle, event_t *event)
{
    return event_queue_write(&handle->iface.queue, event);
}

uint8_t event_manager_fetch(event_manager_handle_t handle, event_t *event)
{
    return event_queue_fetch(&handle->iface.queue, event);
}




