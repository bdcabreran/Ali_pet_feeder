/**
 * @file petcall_fsm.h
 * @author Bayron Cabrera (bdcabreran@unal.edu.com)
 * @brief Petcall configuration for voice record/play processes
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PETCALL_H
#define PETCALL_H

#include "event_manager_fsm.h"


typedef enum
{
    PETCALL_DISABLE,
    PETCALL_ENABLE,
}petcall_st;

typedef struct
{
    bool rec_file_available;
    petcall_st status;

}petcall_config_info_t;


/**
 * @brief Internal Events 
 */
typedef enum
{
    EVT_EXT_PETCALL_INVALID = 0,
    EVT_EXT_PETCALL_DELETE,
    EVT_EXT_PETCALL_ENABLE,
    EVT_EXT_PETCALL_DISABLE,
    EVT_EXT_PETCALL_PLAY_START,
    EVT_EXT_PETCALL_PLAY_STOP,
    EVT_EXT_PETCALL_RECORD_START,
    EVT_EXT_PETCALL_RECORD_STOP,
    EVT_EXT_PETCALL_LAST,
}petcall_ev_ext_name_t;

#define IS_PETCALL_EXT_EVT(evt) (evt > EVT_EXT_PETCALL_INVALID && evt < EVT_EXT_PETCALL_LAST)

typedef struct
{
    uint8_t dummy;
}petcall_ev_ext_data_t;

typedef struct 
{
    petcall_ev_ext_name_t name;
}petcall_ev_ext_t;


typedef struct petcall_fsm_t* petcall_handle_t;

petcall_handle_t petcall_fsm_get(void);
void petcall_fsm_init(petcall_handle_t handle);
void petcall_fsm_run(petcall_handle_t handle);
void petcall_fsm_write_event(petcall_handle_t handle, event_t *event);




#endif
