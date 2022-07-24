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

#include <stdint.h>
#include "event_queue.h"

typedef enum
{
    PETCALL_INVALID,
    PETCALL_ENABLE,
    PETCALL_DISABLE,
    PETCALL_LAST,
}petcall_st;
#define IS_VALID_PETCALL_ST(st) (st > PETCALL_INVALID && st < PETCALL_LAST)

typedef enum
{
    PETCALL_SCORE_ACTION_INVALID,
    PETCALL_SCORE_ACTION_STOP,
    PETCALL_SCORE_ACTION_PLAY,
    PETCALL_SCORE_ACTION_LAST,
}petcall_score_t;
#define IS_VALID_PETCALL_SCORE(score) (score > PETCALL_SCORE_INVALID && score < PETCALL_SCORE_LAST)

typedef enum
{
    PETCALL_REC_ACTION_INVALID,
    PETCALL_REC_ACTION_START,
    PETCALL_REC_ACTION_STOP, 
    PETCALL_REC_ACTION_LAST,
}petcall_recording_t;
#define IS_VALID_PETCALL_REC_ACTION(rec) (rec > PETCALL_REC_INVALID && rec < PETCALL_REC_LAST)

typedef enum
{
    PETCALL_REC_FILE_INVALID,
    PETCALL_REC_FILE_NOT_AVAILABLE,
    PETCALL_REC_FILE_AVAILABLE,
    PETCALL_REC_FILE_LAST,
}petcall_rec_file_t;
#define IS_VALID_PETCALL_REC_FILE(file) (file > PETCALL_REC_FILE_INVALID && file < PETCALL_REC_FILE_LAST)


typedef struct
{
    petcall_score_t     score_action;
    petcall_recording_t rec_action;
    petcall_rec_file_t  rec_file;
    petcall_st          petcall_status;
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
    EVT_EXT_PETCALL_SCORE_PLAY,
    EVT_EXT_PETCALL_SCORE_STOP,
    EVT_EXT_PETCALL_RECORD_START,
    EVT_EXT_PETCALL_RECORD_STOP,
    EVT_EXT_PETCALL_LAST,
}petcall_ev_ext_name_t;

#define IS_PETCALL_EXT_EVT(evt) (evt > EVT_EXT_PETCALL_INVALID && evt < EVT_EXT_PETCALL_LAST)

typedef struct
{
    petcall_config_info_t info;
}petcall_ev_ext_data_t;

typedef struct 
{
    petcall_ev_ext_name_t name;
}petcall_ev_ext_t;


typedef struct petcall_fsm_t* petcall_handle_t;

petcall_handle_t petcall_fsm_get(void);
petcall_config_info_t *petcall_fsm_get_info(void);
void petcall_fsm_init(petcall_handle_t handle);
void petcall_fsm_run(petcall_handle_t handle);
void petcall_fsm_write_event(petcall_handle_t handle, event_t *event);
void petcall_fsm_time_update(petcall_handle_t handle);


#endif
