#include "drawer_fsm.h"

drawer_ctrl_info_t drawer_info[DRAWERn] = {
    {.no = DRAWER_NO_1,
     .manually_open = false,
     .status.curr = DRAWER_ST_CLOSE,
     .status.next = DRAWER_ST_INVALID},
    {.no = DRAWER_NO_2,
     .manually_open = false,
     .status.curr = DRAWER_ST_CLOSE,
     .status.next = DRAWER_ST_INVALID},
    {.no = DRAWER_NO_3,
     .manually_open = false,
     .status.curr = DRAWER_ST_CLOSE,
     .status.next = DRAWER_ST_INVALID},
    {.no = DRAWER_NO_4,
     .manually_open = false,
     .status.curr = DRAWER_ST_CLOSE,
     .status.next = DRAWER_ST_INVALID},
};

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
