/**
 * @file ring_buffer.h
 */

#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stdint.h"
#include "assert.h"
#include "stdlib.h"

/**
 * @brief list enumeration for circular buffer state
 * @enum  ring_buffer_st_t
 */
typedef enum
{
	RING_BUFF_OK = 0x00,
	RING_BUFF_FULL,
	RING_BUFF_NOT_ENOUGH_SPACE,

}ring_buffer_st_t;


/*@brief typedef definition for circular buffer struct  */
typedef struct ring_buffer_t ring_buffer_t;

/*@brief pointer typedef to circular buffer struct */
typedef ring_buffer_t* ring_buffer_handle_t;

/**@} */


/**
 * @defgroup Circular_Buffer_Exported_Functions Circular Buffer Exported Functions 
 * @{
 */
ring_buffer_handle_t ring_buffer_init(uint8_t *buffer, size_t size);
void ring_buffer_free(ring_buffer_handle_t rb_handle);
void ring_buffer_reset(ring_buffer_handle_t rb_handle);

uint8_t is_ring_buffer_empty(ring_buffer_handle_t rb_handle);
uint8_t is_ring_buffer_full(ring_buffer_handle_t rb_handle);

size_t ring_buffer_get_free_space(ring_buffer_handle_t rb_handle);
size_t ring_buffer_get_capacity(ring_buffer_handle_t rb_handle);
size_t ring_buffer_get_data_len(ring_buffer_handle_t rb_handle);

uint8_t ring_buffer_get(ring_buffer_handle_t rb_handle, uint8_t *data);
void ring_buffer_put(ring_buffer_handle_t rb_handle, uint8_t data);

ring_buffer_st_t ring_buffer_write(ring_buffer_handle_t rb_handle, uint8_t *data, uint8_t data_len);
uint8_t ring_buffer_read(ring_buffer_handle_t rb_handle, uint8_t *data, size_t data_len);
uint8_t ring_buffer_fetch(ring_buffer_handle_t rb_handle, uint8_t *data, size_t data_len);


#endif