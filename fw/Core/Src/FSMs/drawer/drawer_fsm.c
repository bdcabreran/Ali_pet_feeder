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

// ---------------- Static State Function Declarations ----------------------------------------//

static void enter_seq_watcher(drawer_ctrl_handle_t handle);
static void entry_action_watcher(drawer_ctrl_handle_t handle);
static void watcher_on_react(drawer_ctrl_handle_t handle);
static void during_action_watcher(drawer_ctrl_handle_t handle);


// ----------------- Miscellaneous Functions Declarations --------------------------------------------------//
static void drawer_motor_open(drawer_no_t no);
static void drawer_motor_close(drawer_no_t no);
static void drawer_motor_stop(drawer_no_t no);
static void drawer_ctrl_update_status(drawer_ctrl_handle_t handle);

//------------------ Static State Function Prototypes ---------------------------------------//

drawer_ctrl_handle_t drawer_ctrl_fsm_get(void)
{
    return &drawer_ctrl;
}

drawer_ctrl_info_t *drawer_ctrl_fsm_get_info(drawer_no_t drawer_no)
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

void drawer_ctrl_fsm_init(drawer_ctrl_handle_t handle)
{   
    /*Init Data*/
    handle->iface.active_drawer = DRAWER_NO_1;

    for (drawer_no_t drawer_no = DRAWER_NO_1; drawer_no < DRAWERn; drawer_no++)
    {
        handle->iface.drawers[drawer_no].request_type = DRAWER_REQUEST_TYPE_INVALID;
        handle->iface.drawers[drawer_no].status.next = DRAWER_ST_CLOSE;
    }

    /*default entry sequence */
    enter_seq_watcher(handle);
}

void drawer_ctrl_fsm_run(drawer_ctrl_handle_t handle)
{
    switch (handle->state)
    {
        case ST_DRAWER_CTRL_WATCHER: watcher_on_react(handle); break;
        default: break;
    }
}

void drawer_ctrl_fsm_write_event(drawer_ctrl_handle_t handle, event_t *event)
{
    if(IS_DRAWER_CTRL_EV_EXT(event->info.name))
    {
        handle->event.external.name = event->info.name;
        drawer_ctrl_ev_ext_data_t *data = (drawer_ctrl_ev_ext_data_t*)event->data.buff;
        handle->event.external.data.drawer_no = data->drawer_no;
        handle->event.external.data.request_type = data->request_type;
    }
}


//------------------ Static State Function Definition ---------------------------------------//

static void enter_seq_watcher(drawer_ctrl_handle_t handle)
{
	drawer_dbg("enter seq\t[ watcher ]\r\n");
    fsm_set_next_state(handle, ST_DRAWER_CTRL_WATCHER);
    entry_action_watcher(handle);
}

static void entry_action_watcher(drawer_ctrl_handle_t handle)
{   
}

static void watcher_on_react(drawer_ctrl_handle_t handle)
{
    
    if (handle->event.external.name == EVT_EXT_DRAWER_CTRL_OPEN)
    {
        drawer_no_t drawer_no = handle->event.external.data.drawer_no;
        drawer_st_t curr_st = handle->iface.drawers[drawer_no].status.curr;
        handle->iface.drawers[drawer_no].request_type = handle->event.external.data.request_type; 

        if(curr_st == DRAWER_ST_OPEN || curr_st == DRAWER_ST_OPENING)
        {
            drawer_dbg("drawer [%d] already open or opening [%d]...\r\n", drawer_no + 1, curr_st);
        }
        else
        {
            drawer_dbg("drawer [%d] set to open...\r\n", drawer_no + 1);
            handle->iface.drawers[drawer_no].status.next = DRAWER_ST_OPEN;
        }

        handle->event.external.name = EVT_EXT_DRAWER_CTRL_INVALID;
    }
    else if (handle->event.external.name == EVT_EXT_DRAWER_CTRL_CLOSE)
    {
        drawer_no_t drawer_no = handle->event.external.data.drawer_no;
        drawer_st_t curr_st = handle->iface.drawers[drawer_no].status.curr;

        if(curr_st == DRAWER_ST_CLOSE || curr_st == DRAWER_ST_CLOSING)
        {
            drawer_dbg("drawer [%d] already close or closing [%d]...\r\n", drawer_no + 1, curr_st);
        }
        else
        {
            drawer_dbg("drawer [%d] set to close...\r\n", drawer_no + 1);
            handle->iface.drawers[drawer_no].status.next = DRAWER_ST_CLOSE;
        }

        handle->event.external.name = EVT_EXT_DRAWER_CTRL_INVALID;
    }

    /* always run */
    during_action_watcher(handle);
}

static void during_action_watcher(drawer_ctrl_handle_t handle)
{
    drawer_ctrl_update_status(handle);
}

// ----------------- Miscellaneous Functions Definition  --------------------------------------------------//

static void drawer_ctrl_update_status(drawer_ctrl_handle_t handle)
{
    for(drawer_no_t drawer_no = DRAWER_NO_1; drawer_no < DRAWERn; drawer_no++)
    {
        /*Update Current state of drawer*/
        if ((is_drawer_close(drawer_no) == true) && (is_drawer_open(drawer_no) == false))
        {
            handle->iface.drawers[drawer_no].status.curr = DRAWER_ST_CLOSE;
        }
        else if ((is_drawer_close(drawer_no) == false) && (is_drawer_open(drawer_no) == true))
        {
            handle->iface.drawers[drawer_no].status.curr = DRAWER_ST_OPEN;
        }


        /*check if there is a pending operation ongoing */
        if(handle->iface.drawers[drawer_no].status.next != DRAWER_ST_INVALID)
        {
            if(handle->iface.drawers[drawer_no].status.next == DRAWER_ST_CLOSE)
            {
                // wait until drawer is closed 
                if(handle->iface.drawers[drawer_no].status.curr == DRAWER_ST_CLOSE)
                {
                    handle->iface.drawers[drawer_no].status.next = DRAWER_ST_INVALID;
                    // go to default once it is closed
                     handle->iface.drawers[drawer_no].request_type = DRAWER_REQUEST_TYPE_PROGRAMMED; 
                    drawer_dbg("drawer no [%d] is closed\r\n", drawer_no + 1);
                    drawer_motor_stop(drawer_no);
                }
                else
                {
                    // drawer_dbg("closing drawer no [%d]\r\n", drawer_no + 1);
                    handle->iface.drawers[drawer_no].status.curr = DRAWER_ST_CLOSING;
                    drawer_motor_close(drawer_no);
                }
            }

            else if(handle->iface.drawers[drawer_no].status.next == DRAWER_ST_OPEN)
            {
                // wait until drawer is opened 
                if(handle->iface.drawers[drawer_no].status.curr == DRAWER_ST_OPEN)
                {
                    handle->iface.drawers[drawer_no].status.next = DRAWER_ST_INVALID;
                    drawer_dbg("drawer no [%d] is open\r\n", drawer_no + 1);
                    drawer_motor_stop(drawer_no);
                }
                else
                {
                    // drawer_dbg("opening drawer no [%d]\r\n", drawer_no + 1);
                    handle->iface.drawers[drawer_no].status.curr = DRAWER_ST_OPENING;
                    drawer_motor_open(drawer_no);
                }
            }
        }
    }

    // debug 
    static uint32_t millis_cnt = 0;
    if (HAL_GetTick() - millis_cnt > 2000)
    {
        millis_cnt = HAL_GetTick();
        for (drawer_no_t drawer_no = DRAWER_NO_1; drawer_no < DRAWERn; drawer_no++)
        {
            if(handle->iface.drawers[drawer_no].status.curr == DRAWER_ST_OPENING)
               drawer_dbg("opening drawer no [%d]\r\n", drawer_no + 1);
            else if(handle->iface.drawers[drawer_no].status.curr == DRAWER_ST_CLOSING)
               drawer_dbg("closing drawer no [%d]\r\n", drawer_no + 1);
        }
    }
}

static void drawer_motor_open(drawer_no_t no)
{
    // drawer_dbg("Motor Open,  drawer no [%d]\r\n", no + 1);
}

static void drawer_motor_close(drawer_no_t no)
{
    // drawer_dbg("Motor Close,  drawer no [%d]\r\n", no + 1);
}

static void drawer_motor_stop(drawer_no_t no)
{
    // drawer_dbg("Motor Stop,  drawer no [%d]\r\n", no + 1);
}
