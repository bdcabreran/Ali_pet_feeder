/**
 * @file drawer_fsm.h
 * @author Bayron Cabrera (bdcabreran@unal.edu.co)
 * @brief  Drawer Controller FSM 
 * @version 0.1
 * @date 2022-06-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DRAWER_FSM_H
#define DRAWER_FSM_H

#include "stdint.h"
#include "stdbool.h"
#include "event_manager_fsm.h"

typedef enum
{
    DRAWER_NO_1,
    DRAWER_NO_2,
    DRAWER_NO_3,
    DRAWER_NO_4,
    DRAWERn,
}drawer_no_t;

typedef enum
{
    DRAWER_ST_INVALID,
    DRAWER_ST_OPEN,
    DRAWER_ST_OPENING,
    DRAWER_ST_CLOSE,
    DRAWER_ST_CLOSING,
    DRAWER_ST_LAST,
}drawer_st_t;

typedef struct
{
    bool manually_open;
    struct 
    {
        drawer_st_t curr;
        drawer_st_t next;
    }status;

}drawer_ctrl_info_t;

/**
 * @brief Internal Events 
 */
typedef enum
{
    EVT_EXT_DRAWER_CTRL_INVALID = 0,
    EVT_EXT_DRAWER_CTRL_OPEN,
    EVT_EXT_DRAWER_CTRL_CLOSE,
    EVT_EXT_DRAWER_CTRL_LAST
}drawer_ctrl_ev_ext_name_t;

#define IS_DRAWER_CTRL_EV_EXT(evt) (evt > EVT_EXT_DRW_INVALID && evt  < EVT_EXT_DRW_LAST)

typedef struct
{
    drawer_no_t drawer_no;
}drawer_ctrl_ev_ext_data_t;

typedef struct
{
    drawer_ctrl_ev_ext_name_t name;
    drawer_ctrl_ev_ext_data_t data;
}drawer_ctrl_ev_ext_t;

typedef struct drawer_ctrl_fsm_t* drawer_ctrl_handle_t;

/*Exported Public Functions */

drawer_ctrl_handle_t drawer_ctrl_fsm_get(void);
drawer_ctrl_info_t *drawer_ctrl_fsm_get_info(drawer_no_t drawer_no);




#endif 